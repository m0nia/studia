#include "cacti.h"
#include <stdlib.h>
#include <stdio.h>

#define CALCULATE_FACTORIAL 1
#define INIT_CALCULATION 1
#define CHILD_ID 2

typedef unsigned long long int num_t;

typedef struct actor_state {
    actor_id_t id;
    num_t k_factorial;
    num_t k;
    num_t n;
} actor_state_t;

void msg_calculate_factorial(void** stateptr, size_t nbytes, void* data);
void msg_child_id(void** stateptr, size_t nbytes, void* data);
void msg_hello(void** stateptr, size_t nbytes, void* data);
void init_calculation(void** stateptr, size_t nbytes, void* data);
void msg_hello_first(void** stateptr, size_t nbytes, void* data);


static const int ROLES_NR = 3;
static const act_t roles_arr[] = {msg_hello, msg_calculate_factorial, msg_child_id};
role_t role = {.prompts = roles_arr, .nprompts = ROLES_NR};

message_t godie(){
    message_t m;
    m.nbytes = 0,
    m.data = NULL,
    m.message_type = MSG_GODIE;
    return m;
}

void msg_calculate_factorial(void** stateptr, size_t nbytes, void* data){
	(void)nbytes;
	(void)data;
    actor_state_t * state = *stateptr;
    state->k += 1;
    state->k_factorial *= state->k;
    if(state->k != state->n){
        send_message(actor_id_self(), (message_t){
                .message_type = MSG_SPAWN,
                .data = &role,
                .nbytes = sizeof(role_t)
        });
    } else {
        printf("%llu\n", state->k_factorial);
        free(state);
        send_message(actor_id_self(), godie());
    }
}

// w data jest stan dziecka - zapisuje mu co ma liczyc i wysylam wiadomosc CALCULATE FACTORIAL
void msg_child_id(void** stateptr, size_t nbytes, void* data){
	(void)nbytes;
    actor_state_t* child_state = data;
    actor_state_t* my_state = *stateptr;
    child_state->k = my_state->k;
    child_state->k_factorial = my_state->k_factorial;
    child_state->n = my_state->n;
    send_message(child_state->id, (message_t){
        .message_type = CALCULATE_FACTORIAL,
        .data = NULL,
        .nbytes = 0
    });
    free(*stateptr);
    send_message(actor_id_self(),godie());
}


void msg_hello(void** stateptr, size_t nbytes, void* data){
	(void)nbytes;
    actor_state_t* state = malloc(sizeof(*state));
    state->id = actor_id_self();
    message_t message;
    message.message_type = CHILD_ID;
    message.nbytes = sizeof(actor_state_t);
    message.data = state;
    *stateptr = state;
    send_message((actor_id_t)data, message);
}

void init_calculation(void** stateptr, size_t nbytes, void* data){
	(void)nbytes;
    actor_state_t* state = *stateptr;
    state->n = *(num_t*)data;
    if(state->k != state->n && state->n != 0){
        send_message(actor_id_self(), (message_t){
                .message_type = MSG_SPAWN,
                .data = &role,
                .nbytes = sizeof(role_t)
        });
    } else {
        printf("%llu\n", state->n);
        free(state);
        send_message(actor_id_self(), godie());
    }
}

void msg_hello_first(void** stateptr, size_t nbytes, void* data){
	(void)nbytes;
	(void)data;
    actor_state_t* state = malloc(sizeof(*state));
    state->id = actor_id_self();
    state->k_factorial = 1;
    state->k = 1;
    *stateptr = state;
}

int main(){
    actor_id_t supervisor;
    int err;
    int n = 5;
    scanf("%d", &n);
    if(n < 0) printf("Can't calculate factorial of negative number\n");
    act_t first_fun_array[] = {msg_hello_first, init_calculation, msg_child_id};
    role_t first_role = {.nprompts = ROLES_NR, .prompts = first_fun_array};
    if((err = actor_system_create(&supervisor, &first_role)) != 0) {
        fprintf(stderr, "actor system create failed, return code: %d\n", err);
        return 1;
    }

    send_message(supervisor, (message_t){
            .message_type = INIT_CALCULATION,
            .data = &n,
            .nbytes = sizeof(int)
    });
    actor_system_join(supervisor);
    return 0;
}
