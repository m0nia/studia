#include <stdio.h>
#include <string.h>
#include "input.h"

#define NUMBER 5
#define LENGHT 8

static char commands[NUMBER][LENGHT] = {
										"VALID",
										"DECLARE",
										"EQUAL",
										"REMOVE",
										"ENERGY"
										};

static void ignore_line(){
	int c = getchar();
	while(c != '\n' && c != EOF)
		c = getchar();
}

int read_command(){ //
	int ret = -1;
	int c;
	char A[LENGHT];
	for(int i = 0; i < LENGHT; i++)
		A[i] = '\0';

	c = getchar();
	if(c == '#' || c == '\n' || c == EOF){
		if(c != '\n')
			ignore_line();
		return ret;
	}
	for(int i = 0; i < LENGHT && c != '\n' && c != EOF && c != ' '; i++){
		A[i] = c;
		c = getchar();
	}
	// after the loop word in array ends in either ' ' (it's possibly a good command) or with '\n'
	// or with any other sign (it's wrong command, array is of size LENGHT)  
	if(c == ' '){
		for(int i = 0; i < NUMBER; i++){
			if(strcmp(commands[i], A) == 0)
				ret = i;
		}
			//if ret is other than -1 then it's definitly a good command witch ends with spacebar (but arguments may be wrong)
	}
	if(ret == - 1){ //definitly wrong command, I want to ignore rest of the line, but it's possible that c=='\n', then i would ignore the next line
		if(c != '\n')
			ignore_line();
		fprintf(stderr, "ERROR\n");
	}
	return ret;
}