#ifndef LIST_H
#define LIST_H

struct list{
	int data;
	struct list* next;
};
typedef struct list List;

List* insert_in_circular_list(List* circularList, int energy);

void make_all_equal(List* node, int energy);

void combine_circular_lists(List* list1, List* list2, int x);

#endif /* LIST_H */