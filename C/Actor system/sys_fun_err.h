#ifndef MUTEX_H
#define MUTEX_H

#include "err.h"

void init_mutex(pthread_mutex_t* m){
    int err;
    if ((err = pthread_mutex_init(m, 0)) != 0) {
        syserr(err, "mutex init failed");
    }
}

void init_cond(pthread_cond_t* c){
    int err;
    if ((err = pthread_cond_init(c, 0)) != 0) {
        syserr (err, "cond init failed");
    }
}

void destroy_mutex(pthread_mutex_t* m){
    int err;
    if ((err = pthread_mutex_destroy(m)) != 0)
        syserr(err, "mutex destroy failed");
}

void destroy_cond(pthread_cond_t* c){
    int err;
    if ((err = pthread_cond_destroy(c)) != 0)
        syserr(err, "cond destroy failed");
}

void thread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*work) (void *), void *arg) {
    int err;
    if ((err = pthread_create(thread, attr, work, arg)) != 0) {
        syserr(err, "pthread create failed");
    }
}

void thread_join(pthread_t thread){
    void *ret = NULL;
    int err;
    if((err = pthread_join(thread, &ret)) != 0)
        syserr(err, "join failed");
}

void wait(pthread_cond_t *cond, pthread_mutex_t *m) {
    int err;
    if ((err = pthread_cond_wait(cond, m)) != 0) {
        syserr(err, "cond wait failed");
    }
}

void signal_thread(pthread_cond_t *cond) {
    int err;
    if ((err = pthread_cond_signal(cond)) != 0) {
        syserr(err, "cond signal failed");
    }
}

void notify_all(pthread_cond_t *cond) {
    int err;
    if ((err = pthread_cond_broadcast(cond)) != 0) {
        syserr(err, "cond broadcast failed");
    }
}

void P(pthread_mutex_t *m) {
    int err;
    if ((err = pthread_mutex_lock(m)) != 0) {
        syserr(err, "lock failed");
    }
}

void V(pthread_mutex_t *m) {
    int err;
    if ((err = pthread_mutex_unlock(m)) != 0) {
        syserr(err, "lock failed");
    }
}

void sigint_wait(){
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    int err, sig;
    if((err = sigwait(&set, &sig)) != 0)
        syserr(err, "sigwait failed");
}

void thread_cancel(pthread_t t){
    int err;
    if((err = pthread_cancel(t)) != 0){
        syserr(err, "pthread cancel failed");
    }
}

#endif //MUTEX_H
