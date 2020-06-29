/** @file
 * Implementacja kolejki.
 *
 * @author Monika Michaluk <m.michaluk5@student.uw.edu.pl>
 * @date 17.04.2020
 */

#include "queue.h"
#include <stdlib.h>

int isEmpty(Queue *q) {
    return (q->front == NULL);
}

void Init(Queue **q) {
    Queue *temp = (Queue *) malloc(sizeof(Queue));
    if(temp == NULL)
        exit(1);
    temp->front = NULL;
    temp->back = NULL;
    *q = temp;
}

void Enqueue(Queue *q, point_t value) {
    struct list *temp = (struct list *) malloc(sizeof(struct list));
    if(temp == NULL)
        exit(1);
    temp->value = value;
    temp->next = NULL;

    if (q->front == NULL) {
        q->front = temp;
        q->back = temp;
    } else {
        q->back->next = temp;
        q->back = temp;
    }
}


point_t Dequeue(Queue *q) {
    struct list *temp = q->front;

    point_t value = q->front->value;
    q->front = q->front->next;

    free(temp);
    return value;
}

void Clear(Queue **q) {
    if (q == NULL)
        return;
    Queue *temp = *q;
    while (!isEmpty(temp)) {
        Dequeue(temp);
    }
    free(temp);
}
