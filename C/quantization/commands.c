#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "commands.h"

static int check_overflow(int a, int b){
    if(a > INT_MAX/10) 
        return 0; 
    if(10*a > INT_MAX - b)
    	return 0;
    return 1;
} 

static bool ignore_line(){
	int isCorrect = 1;
	int c = getchar();
	while(c != '\n' && c != EOF){
		if(c < '0' || c > '3')
			isCorrect = 0;
		c = getchar();
	}
	return isCorrect;
}

static int get_energy(){
	bool isCorrect = true;
	int digit;
	int num = 0;
	int x = getchar();
	while(x != '\n' && x != EOF && isCorrect == true){
		if(x >= '0' && x <= '9'){
			digit = x - '0';
			isCorrect = check_overflow(num, digit);
			if(isCorrect){
				num = 10*num + x - '0';
				x = getchar();
			}
		}
		else
			isCorrect = false;
	}
	if(!isCorrect){
		if(x != '\n')
			ignore_line();
		return 0;
	}
	return num;
}

static Node* find(Node* root, int x){ //find x child of node root (root!=NULL)
	Node* temp = root->left_child;
	while(temp != NULL && temp->data != x)
			temp = temp->right_sibling;
	return temp;
}

static Node* ending_node_of_valid_history(Node* root, int* isCorrect){ //return pointer to ending node in valid history, 
	//null if given history is not valid
	int c = getchar();
	if(c == '\n' || c == EOF || c == ' ')
		*isCorrect = false;
	while(c != '\n' && c != ' ' && *isCorrect && root){
		if(c >= '0' && c <= '3'){
			root = find(root, c);
			if(root != NULL){
				c = getchar();
				if(c == EOF)
					*isCorrect = false;
			}
		}
		else
			*isCorrect = false;
	}
	ungetc(c, stdin);
	return root;
}

void is_valid(Node* root){
	int c;
	int isCorrect = true;
	Node* lastNode = ending_node_of_valid_history(root, &isCorrect);
	c = getchar(); //ostatni pobrany znak
	if(lastNode && isCorrect && c == '\n')
		printf("YES\n");
	else if(!lastNode && isCorrect){
		if(c != '\n')
			isCorrect = ignore_line();
		if(isCorrect)
			printf("NO\n");
		else
			fprintf(stderr, "ERROR\n");
	}
	else{
		if(c != '\n')
			ignore_line();
		fprintf(stderr, "ERROR\n");
	}
}

void equal(Node* root){
	int x;
	int c;
	int energy;
	int isCorrect = true;
	Node* history2;
	Node* history1 = ending_node_of_valid_history(root, &isCorrect);
	c = getchar();
	if(history1 && isCorrect && c == ' '){
		history2 = ending_node_of_valid_history(root, &isCorrect);
		c = getchar();
		if(history2 && isCorrect && c == '\n'){
			if(history1 != history2){
				if(history1->energy == NULL && history2->energy != NULL){
					energy = history2->energy->data;
					history1->energy = insert_in_circular_list(history2->energy, energy);
					printf("OK\n");
				}
				else if(history2->energy == NULL && history1->energy != NULL){
					energy = history1->energy->data;
					history2->energy = insert_in_circular_list(history1->energy, energy);
					printf("OK\n");	
				}
				else if(history1->energy != NULL && history2->energy != NULL){
					if(history1 != history2){
						x = history1->energy->data/2 + history2->energy->data/2;
						combine_circular_lists(history2->energy, history1->energy, x);
						printf("OK\n");
					}
				}
				else if(history1->energy == NULL && history2->energy == NULL)
					fprintf(stderr, "ERROR\n");
			}
			if(history1 == history2)
				printf("OK\n");
		}
		else{
			if(c != '\n')
				ignore_line();
			fprintf(stderr, "ERROR\n");
		}
	}
	else{
		if(c != '\n')
			ignore_line();
		fprintf(stderr, "ERROR\n");
	}
}

void remove_node(Node* root){
	Node* prev = NULL;
	int c;
	int isCorrect = true;
	Node* lastNode = ending_node_of_valid_history(root, &isCorrect);
	c = getchar();
	if(lastNode && isCorrect && c == '\n'){
		find_prev_to_dispose(root, lastNode, &prev);
		delete_root(prev, lastNode);
	}
	else if(!lastNode && isCorrect)
		isCorrect = ignore_line();

	if(!isCorrect){
		if(c != '\n')
			ignore_line();
		fprintf(stderr, "ERROR\n");
	}
	else
		printf("OK\n");
}


void declare(Node* root){ //JEST OK ALE CZY TRZEBA WYPISYWAĆ ERROR JAK NIE UDAŁO SIĘ ZAALOKOWAĆ PAMIĘCI W ADD_VALID_HISTORY
	int c;
	bool success = true;
	Queue* history;
	Init(&history);
	c = getchar();
	if(c =='\n' || c == EOF)
		success = false;
	while(c != '\n' && c != EOF && success == true){
		if(c >= '0' && c <= '3'){
			if(isFull())
				exit(1);
			Enqueue(history, c);
			c = getchar();
		}
		else
			success = false;
	}
	if(success){
		add_valid_history(root, history);
		printf("OK\n");
	}
	else{
		if(c != '\n')
			ignore_line();
		fprintf(stderr, "ERROR\n");
	}
	Clear(&history);
}

void energy(Node* root){ /////////////////////////////////////////////////////////////////
	int energy;
	int c;
	int isCorrect = true;
	Node* lastNode = ending_node_of_valid_history(root, &isCorrect);
	c = getchar();
	if(c == '\n'){
		if(isCorrect && lastNode && lastNode->energy)
			printf("%d\n", lastNode->energy->data);
		else
			fprintf(stderr, "ERROR\n");
	}
	else if(lastNode && isCorrect && c == ' '){
		energy = get_energy();
		if(energy != 0){
			if(lastNode->energy == NULL){
				lastNode->energy = insert_in_circular_list(lastNode->energy, energy);
			}
			else{
				make_all_equal(lastNode->energy, energy);
			}
			printf("OK\n");
		}
		else
			fprintf(stderr, "ERROR\n");
	}
	else{
		if(c != '\n')
			ignore_line();
		fprintf(stderr, "ERROR\n");
	}
}