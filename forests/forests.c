#include <stdlib.h>
#include <stdio.h>
#include "def.h"
#include "bst.h"
#include "commands.h"
#include "input.h"

const char commands[NR_COMANDS][MAX_LENGHT + 1] = {
        "ADD",
        "DEL",
        "PRINT",
        "CHECK",
};

/* Wczytuje wiersz z wejścia i jednocześnie sprawdza czy wprowadzone dane są poprawne - jeśli nie, kończy pracę funkcji.
Obsługa błędów znajduje się we wnętrzu odpowiednich funkcji */
void readLine(Tree *t) {
    int *Arg[MAX_ARG] = {NULL};
    if (isComment()) {
        ignoreLine();
        return;
    }
    ignoreSpaces();
    int c = getchar();
    if (c == '\n' || c == EOF)
        return;
    ungetc(c, stdin);
    int nrCommand = readCommand();
    if (nrCommand == -1) {
        return;
    }
    ignoreSpaces();
    if (!readArguments(Arg)) {
        return;
    }
    if (!checkInput(nrCommand, Arg)) {
        return;
    }

    switch (nrCommand) {
        case ADD:
            add(t, Arg);
            break;
        case DEL:
            del(t, Arg);
            break;
        case PRINT:
            print(*t, Arg);
            break;
        case CHECK:
            check(*t, Arg);
            break;
    }
    if (nrCommand != ADD)
        free2DArray(Arg);
}

int main() {
    Tree t = NULL;
    while (!isEnd())
        readLine(&t);
    removeAll(t);
    return 0;
}