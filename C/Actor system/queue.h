#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct node {
    void *data;
    struct node *next;
} node_t;

typedef struct queue {
    size_t elements;
    size_t nbytes;
    node_t *head;
    node_t *tail;
} queue_t;

// nbytes - rozmiar w bajtach elementów przechowywanych w strukturze
void init_queue(queue_t *q, size_t nbytes){
    q->head = NULL;
    q->tail = NULL;
    q->elements = 0;
    q->nbytes = nbytes;
}

bool is_empty(queue_t* q){
    return q->head == NULL;
}

void enqueue(queue_t* q, void* x) {
    node_t* node = malloc(sizeof(*node));
    //if(node == NULL) exit(1);
    node->data = malloc(q->nbytes);
    //if(node->data == NULL) exit(1);
    node->next = NULL;
    memcpy(node->data, x, q->nbytes);
    if(q->head == NULL){
        q->head = node;
        q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
    q->elements++;
}

// jesli x == null, to tylko zdejmuje element z kolejki i nie zapisuje go nigdzie
bool dequeue(queue_t* q, void* x){
    if(is_empty(q)) return false;
    node_t* temp = q->head;
    if(x != NULL) memcpy(x, temp->data, q->nbytes);
    q->head = temp->next;
    q->elements--;
    free(temp->data);
    free(temp);
    return true;
}

// x musi być valid miejscem w pamieci, tzn x != NULL
bool peek(queue_t* q, void* x){
    if(is_empty(q)) return false;
    memcpy(x, q->head->data, q->nbytes);
    return true;
}

void clear_queue(queue_t* q) {
    void* temp = NULL;
    while(!is_empty(q))
        dequeue(q, temp);
}

size_t queue_size(queue_t* q){
    return q->elements;
}

#endif // QUEUE_H
