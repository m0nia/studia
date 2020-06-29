#ifndef TREE_H
#define TREE_H

#include "queue.h"

struct node{
	int data;
	List* energy;
	struct node* left_child;
	struct node* right_sibling;
};
typedef struct node Node;

Node* creat_new_node(int state);

Node* find_prev(Node* root, int state);

Node* insert_after_given_node(Node* root, Node* prev, int state);

void add_valid_history(Node* root, Queue* q);

void delete_root(Node* prev, Node* lastNode);

void find_prev_to_dispose(Node* root, Node* lastNode, Node** prev);

void free_root(Node* root);

#endif /* TREE_H */