#include <stdlib.h>
#include "list.h"

List* insert_in_circular_list(List* circularList, int energy){
	List* temp;
	List* node = (List*)malloc(sizeof(List));
	if(!node)
		exit(1);
	node->data = energy;
	if(circularList == NULL)
		node->next = node;
	else{
		temp = circularList->next;
		circularList->next = node;
		node->next = temp;
	}
	return node;
}

void make_all_equal(List* node, int energy){
	List* first = node;
	while(node->next != first){
		node->data = energy;
		node = node->next;
	}
	node->data = energy;
}

void combine_circular_lists(List* list1, List* list2, int x){
	List* first2 = list2;
	List* first1 = list1->next;
	list1->next = first2;

	while(list2->next != first2){
		list2 = list2->next;
	}
	list2->next = first1;
	make_all_equal(first1, x);
}