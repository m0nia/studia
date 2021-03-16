#include <stdbool.h>
#include <pthread.h>
#include <signal.h>
#include "cacti.h"
#include "queue.h"
#include "dynamic_array.h"
#include "sys_fun_err.h"

#define ACTOR_DOESNT_EXIST (-2)
#define ACTOR_IS_DEAD (-1)
#define ACTOR_QUEUE_FULL (-3)
#define SPAWN_BLOCKED (-4)
#define ACTORS_LIMIT_REACHED (-5)
#define SYSTEM_FUN_ERR (-6)


typedef struct tpool {
    pthread_t threads[POOL_SIZE];
    queue_t actors;
    pthread_mutex_t lock;
    pthread_cond_t notempty;;
} tpool_t;

typedef struct actor {
    actor_id_t id;
    void *stateptr;
    role_t* role;
    bool is_dead;
    queue_t mailbox;
} actor_t;

typedef struct actor_system {
    dyn_array_t actors;
    tpool_t thread_pool;
    size_t actors_nr;
    size_t dead_actors;
    size_t empty_mailboxes;
    pthread_mutex_t mutex;
    pthread_t sig_thread;
    bool shutdown;
    bool join;
} actor_system_t;


actor_system_t sa = {
        .actors_nr = 0,
        .shutdown = true,
        .join = true,
        .mutex = PTHREAD_MUTEX_INITIALIZER
};
_Thread_local actor_id_t actor_id = -1;

static void new_actor(role_t *const role, actor_t** actor) {
    actor_t *new = malloc(sizeof(actor_t));
    if (new == NULL) exit(1);
    new->id = sa.actors_nr;
    sa.actors_nr += 1;
    sa.empty_mailboxes += 1;
    new->role = role;
    new->stateptr = NULL;
    new->is_dead = false;
    init_queue(&new->mailbox, sizeof(message_t));
    *actor = new;
}

static int msg_spawn_handler(void *data) {
    P(&sa.mutex);
    if(sa.shutdown == true){
        V(&sa.mutex);
        return SPAWN_BLOCKED;
    }
    actor_t* new = NULL;
    new_actor(data, &new);
    if(!push_back(&sa.actors, new)){
        V(&sa.mutex);
        return ACTORS_LIMIT_REACHED;
    }
    V(&sa.mutex);

    actor_id_t my_id = actor_id_self();
    message_t hello;
    hello.message_type = MSG_HELLO;
    hello.nbytes = sizeof(actor_id_t);
    hello.data = (void*)my_id;
    return send_message(new->id, hello);
}

void msg_godie_handler(actor_t* actor) {
    P(&sa.mutex);
    if(!actor->is_dead) {
        actor->is_dead = true;
        sa.dead_actors++;
    }
    V(&sa.mutex);
}

actor_id_t actor_id_self() {
    return actor_id;
}

void add_work(actor_id_t* id){
    P(&sa.thread_pool.lock);
    enqueue(&sa.thread_pool.actors, id);
    signal_thread(&sa.thread_pool.notempty);
    V(&sa.thread_pool.lock);
}

void *work(void *arg) {
    tpool_t *t_pool = arg;
    actor_id_t self;
    message_t m;
    bool dequeued = false;
    while (true) {
        P(&t_pool->lock);
        P(&sa.mutex);
        while ((dequeued = dequeue(&t_pool->actors, &self)) == false
                && !(sa.dead_actors == sa.actors_nr && sa.empty_mailboxes == sa.actors_nr)) {
            V(&sa.mutex);
            wait(&t_pool->notempty, &t_pool->lock);
            P(&sa.mutex);
        };
        V(&sa.mutex);
        V(&t_pool->lock);

        if(!dequeued) break;

        actor_id = self;

        P(&sa.mutex);
        actor_t *actor_info = get(&sa.actors, self);
        peek(&actor_info->mailbox, &m);
        V(&sa.mutex);
        switch (m.message_type) {
            case MSG_SPAWN:
                msg_spawn_handler(m.data);
                break;
            case MSG_GODIE:
                msg_godie_handler(actor_info);
                break;
            default:
                if (actor_info->role->nprompts > m.message_type) {
                    actor_info->role->prompts[m.message_type](&actor_info->stateptr, m.nbytes, m.data);
                }
        }
        P(&sa.mutex);
        dequeue(&actor_info->mailbox, NULL);
        bool empty = is_empty(&actor_info->mailbox);
        V(&sa.mutex);

        if (!empty) {
            add_work(&self);
        } else {
            P(&sa.mutex);
            sa.empty_mailboxes++;
            if(sa.dead_actors == sa.actors_nr && sa.empty_mailboxes == sa.actors_nr)
                notify_all(&t_pool->notempty);
            V(&sa.mutex);
        }
    }
    return 0;
}

_Noreturn static void* sig_thread(void* arg){
    int err;
    if((err = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) != 0)
        syserr(err, "set cancel state failed");
    if((err = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) != 0)
        syserr(err, "set cancel type failed");
    while(true) {
        sigint_wait();
        P(&sa.mutex);
        if (sa.shutdown == false) {
            sa.shutdown = true;
            for (size_t i = 0; i < sa.actors_nr; i++) {
                actor_t *a = get(&sa.actors, i);
                if (!a->is_dead) {
                    a->is_dead = true;
                    sa.dead_actors++;
                }
            }
        }
        notify_all(&sa.thread_pool.notempty);
        V(&sa.mutex);
    }
}


static void tpool_create(tpool_t *t_pool, size_t size) {
    init_queue(&t_pool->actors, sizeof(actor_id_t));
    init_mutex(&t_pool->lock);
    init_cond(&t_pool->notempty);
    for (size_t i = 0; i < size; i++) {
        thread_create(&t_pool->threads[i], NULL, work, t_pool);
    }
}

static void system_destroy(){
    sa.shutdown = true;
    thread_cancel(sa.sig_thread);
    thread_join(sa.sig_thread);
    destroy_array(&sa.actors);
    destroy_mutex(&sa.thread_pool.lock);
    destroy_cond(&sa.thread_pool.notempty);
}

int actor_system_create(actor_id_t *actor, role_t *const role) {
    if(CAST_LIMIT < 1) return ACTORS_LIMIT_REACHED;
    sa.shutdown = false;
    sa.empty_mailboxes = 0;
    sa.actors_nr = 0;
    sa.dead_actors = 0;
    sa.join = false;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    if(pthread_sigmask(SIG_BLOCK, &set, NULL) != 0) return SYSTEM_FUN_ERR;
    init_array(&sa.actors, CAST_LIMIT, sizeof(actor_t*));
    actor_t* supervisor = NULL;
    new_actor(role, &supervisor);
    push_back(&sa.actors, supervisor);
    *actor = 0;

    if(pthread_create(&sa.sig_thread, NULL, sig_thread, &set) != 0) {
        destroy_array(&sa.actors);
        return SYSTEM_FUN_ERR;
    }
    tpool_create(&sa.thread_pool, POOL_SIZE);

    int err;
    if((err = send_message(supervisor->id, (message_t){
        .message_type = MSG_HELLO,
        .data = NULL,
        .nbytes = 0
    })) != 0){
        system_destroy();
        return err;
    }
    return 0;
}

int send_message(actor_id_t actor, message_t message) {
    P(&sa.mutex); // trzeba mieć mutex na cały czas trwania tej funkcji w razie gdyby został wysłany sigint
    if(sa.shutdown == true){
        V(&sa.mutex);
        return -4;
    }
    actor_t *receiver = get(&sa.actors, actor);
    if (receiver == NULL) {
        V(&sa.mutex);
        return ACTOR_DOESNT_EXIST;
    }
    if (receiver->is_dead) {
        V(&sa.mutex);
        return ACTOR_IS_DEAD;
    }
    size_t mailbox_size = queue_size(&receiver->mailbox);
    if (mailbox_size == ACTOR_QUEUE_LIMIT) {
        V(&sa.mutex);
        return ACTOR_QUEUE_FULL;
    }
    enqueue(&receiver->mailbox, &message);
    if (mailbox_size == 0) {
        sa.empty_mailboxes--;
        V(&sa.mutex);
        add_work(&receiver->id);
    } else {
        V(&sa.mutex);
    }
    return 0;
}

void actor_system_join(actor_id_t actor) {
    P(&sa.mutex);
    if(actor >= sa.actors_nr || sa.join) {
        V(&sa.mutex);
        return; // danego aktora nie ma w systemie
    } else {
        sa.join = true;
        V(&sa.mutex);
    }
    for (int i = 0; i < POOL_SIZE; i++)
        thread_join(sa.thread_pool.threads[i]);
    system_destroy();
}