#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "commands.h"
#include "input.h"

bool read_line(Node* root){
	int command;
	command = read_command();
	switch(command){
		case 0:
			is_valid(root);
			break;
		case 1:
			declare(root);
			break;
		case 2:
			equal(root);
			break;
		case 3:
			remove_node(root);
			break;
		case 4:
			energy(root);
			break;
	}
	int c = getchar();
	if(c == EOF)
		return false;
	ungetc(c, stdin);
	return true;
}

int main(void){
	Node* root = (Node*)malloc(sizeof(Node));
	root->left_child = NULL;
	root->right_sibling = NULL;
	root->energy = NULL;

	bool dummy = read_line(root);
	while(dummy)
		dummy = read_line(root);

	free_root(root);

	return 0;
}