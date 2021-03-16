#include <stdlib.h>
#include "tree.h"

void free_root(Node* root){
	Node* temp;
	if(root){
		temp = root;
		free_root(temp->left_child);
		free_root(temp->right_sibling);
		free(root->energy);
		free(root);
	}
}

void delete_root(Node* prev, Node* lastNode){
	if(prev->left_child == lastNode){
		prev->left_child = lastNode->right_sibling;
		lastNode->right_sibling = NULL;
		free_root(lastNode);
	}
	else{
		prev->right_sibling = lastNode->right_sibling;
		lastNode->right_sibling = NULL;
		free_root(lastNode);
	}
}

Node* creat_new_node(int state){
	Node* new = (Node*)malloc(sizeof(Node));
	if(!new)
		exit(1);
	new->data = state;
	new->energy = NULL;
	new->left_child = NULL;
	new->right_sibling = NULL;
	return new;
}

Node* find_prev(Node* root, int state){ // root != NULL
	Node* prev = NULL;
	if(root->left_child == NULL || root->left_child->data > state)
		return root;
	root = root->left_child;
	while(root != NULL && state > root->data){
		prev = root;
		root = root->right_sibling;
	}
	if(root != NULL && state == root->data)
		return root; //dany node juz istnieje w drzewie
	else //root == NULL || state < root->data
		return prev;
}

Node* insert_after_given_node(Node* root, Node* prev, int state){
	Node* temp;
	if(root == prev){
		temp = creat_new_node(state);
		temp->right_sibling = prev->left_child;
		temp->left_child = NULL;
		prev->left_child = temp;
		return temp;
	}
	else if(prev->data == state)
		return prev;
	else{
		temp = creat_new_node(state);
		temp->right_sibling = prev->right_sibling;
		prev->right_sibling = temp;
		return temp;
	}
}

void add_valid_history(Node* root, Queue* q){ //++++czy wystarczylo pamieci
	int state;
	Node* prev;
	while(!isEmpty(q)){
		state = Dequeue(q);
		prev = find_prev(root, state);
		root = insert_after_given_node(root, prev, state);
	}
}

void find_prev_to_dispose(Node* root, Node* lastNode, Node** prev){
	if(root){
		if(root->left_child == lastNode || root->right_sibling == lastNode)
			*prev = root;
		else{
			find_prev_to_dispose(root->left_child, lastNode, prev);
			find_prev_to_dispose(root->right_sibling, lastNode, prev);
		}
	}
}