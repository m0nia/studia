#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

bool isSpace(int c);

bool isEnd();

bool isAsterisk(int A[]);

bool isComment();

void ignoreLine();

void ignoreSpaces();

int extendArray(int *A[], int j, int oldSize);

void free2DArray(int *Arg[]);

bool readArguments(int *Arg[]);

int readCommand();

bool checkInput(int nrCommand, int *Arg[]);

#endif /* INPUT_H */