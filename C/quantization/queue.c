#include <stdlib.h>
#include "queue.h"

int isEmpty(Queue* q){
	return (q->front == NULL);	
}

void Init(Queue** q){
	Queue* temp = (Queue*)malloc(sizeof(Queue));
	temp->front = NULL;
	temp->back = NULL;
	*q = temp;
}

int isFull(){
	List* temp = (List*)malloc(sizeof(List));
	if(temp==NULL)
		return 1;
	free(temp);
	return 0;
}

void Enqueue(Queue* q, int x){ // !isFull(q)
	if(q->front == NULL){
		List* temp = (List*)malloc(sizeof(List));
		temp->data = x;
		q->front = temp;
		q->back = temp;
	}
	else{
		List* temp = (List*)malloc(sizeof(List));
		temp->data = x;
		q->back->next = temp;
		q->back = temp;
	}
	
}

int Dequeue(Queue* q){ // !isEmpty(q)
	List* temp = q->front;
	int x = temp->data;
	if(q->front == q->back){
		q->front = NULL;
	}
	else
		q->front = q->front->next;
	free(temp);
	return x;
}

void Clear(Queue** q){
	Queue* temp = *q;
	while(!isEmpty(temp)){
		Dequeue(temp);
	}
	free(temp);
}