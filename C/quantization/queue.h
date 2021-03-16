#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

struct queue{
	List* front;
	List* back;
};
typedef struct queue Queue;

int isEmpty(Queue* q);

void Init(Queue** q);

int isFull();

void Enqueue(Queue* q, int x);

int Dequeue(Queue* q);

void Clear(Queue** q);

#endif /* QUEUE_H */