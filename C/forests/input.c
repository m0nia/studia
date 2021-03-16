#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "input.h"
#include "def.h"


bool isSpace(int c) { ///' ', '\t', '\v', '\f', '\r'
    if (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r')
        return true;
    return false;
}

bool isAsterisk(int A[]) {
    if (A != NULL && A[0] == '*' && A[1] == '\0')
        return true;
    return false;
}

bool isEnd() {
    int c = getchar();
    if (c == EOF)
        return true;
    ungetc(c, stdin);
    return false;
}

bool isComment() {
    int c = getchar();
    if (c == '#')
        return true;
    ungetc(c, stdin);
    return false;
}

void ignoreLine() {
    int c = getchar();
    while (c != '\n' && c != EOF)
        c = getchar();
}

void ignoreSpaces() {
    int c = getchar();
    while (isSpace(c) && c != EOF) {
        c = getchar();
    }
    ungetc(c, stdin);
}

/*Powiększa dynamicznie tablicę, wypełnia nowe miejsca zerami
Zwraca newSize-1 (statnie miejsce zostawiamy na znak końca napisu '\0') */
int extendArray(int *A[], int j, int oldSize) {
    int newSize = 2 * oldSize + 2;
    A[j] = (int *) realloc(A[j], newSize * sizeof(int));
    if (A[j] == NULL)
        exit(1);
    for (int i = oldSize; i < newSize; i++)
        A[j][i] = 0;
    return newSize - 1;
}

/* Czyści tablicę wskaźników */
void free2DArray(int *Arg[]) {
    int *tmp;
    int i = 0;
    while (i < MAX_ARG) {
        tmp = Arg[i];
        free(tmp);
        Arg[i] = NULL;
        i++;
    }
}

/* Porównuje słowo wczytane z inputu znajdujące się w tablicy A z poleceniem znajdującym się w tablicy B. */
static bool isCorrectCommand(int A[], const char B[]) {
    int i = 0;
    while (B[i] != '\0' && A[i] != '\0' && A[i] == B[i])
        i++;
    if (B[i] == '\0' && A[i] == '\0')
        return true;
    return false;
}

/* Wczytuje argumenty w danej lini i zapisuje je w tablicy.
Jednocześnie sprawdza, czy znaki w podanym argumencie są z odpowiedniego zakresu (33 <= && >= 255 w ASCI), czy dana linia kończy się
znakiem nowej lini i czy użytkownik nie podał za dużo argumentów - jeśli nie to wypisuje błąd, ignoruje resztę lini i zwalnia pamięć po (błędnych) argumentach*/
bool readArguments(int *Arg[]) {
    int j = 0;
    int i = 0;
    int size;
    int c = getchar();
    int isCorrect = true;
    while (j < MAX_ARG && c != '\n' && isCorrect) {
        size = 0;
        i = 0;
        while (!isSpace(c) && c != '\n' && isCorrect) {
            if (c >= 33 && c <= 255) {
                if (size == i)
                    size = extendArray(Arg, j, size);
                Arg[j][i] = c;
                c = getchar();
                i++;
            } else
                isCorrect = false;
        }
        if (c != '\n') {
            ignoreSpaces();
            c = getchar();
        }
        j++;
    }
    isCorrect = isCorrect && c == '\n';
    if (!isCorrect) {
        free2DArray(Arg);
        fprintf(stderr, "ERROR\n");
        ignoreLine();
        return false;
    }
    return true;
}

/* Wczytuje początkowe znaki wiersza do tablicy A, które mogą być poprawnym poleceniem. Następnie porównuje wczytane znaki z kolejnymi
poleceniami i zwraca odpowiedni numer polecenia. Jeśli wczytano błędne polecenie, ignoruje resztę lini i zwraca -1 */
int readCommand() {
    int i = 0;
    int commandNr = -1;
    int A[MAX_LENGHT + 1] = {'\0'};
    int c = getchar();
    while (i < MAX_LENGHT + 1 && !isSpace(c) && c != '\n') {
        A[i] = c;
        i++;
        c = getchar();
    }
    ungetc(c,
           stdin); //dla ułatwienia implementacji, dzięki tamu nie trzeba sprawdzać w funkcji readLine czy ostatni wczytany tutaj znak to '\n'
    for (int i = 0; i < NR_COMANDS; i++) {
        if (isCorrectCommand(A, commands[i])) {
            commandNr = i;
        }
    }
    if (commandNr == -1) {
        if (c != '\n')
            ignoreLine();
        fprintf(stderr, "ERROR\n");
    }
    return commandNr;
}

/* sprawdza czy podane argumenty spełniają wymagania poprawności, tzn. czy została podana ich odpowiednia ilość 
do poszczególnych poleceń oraz czy końcowy argument polecenia CHECK nie jest gwiazdką*/
bool checkInput(int nrCommand, int *Arg[]) {
    bool correct = false;
    int i = 0; //iterator i licznik argumentów
    while (i < MAX_ARG && Arg[i] != NULL)
        i++;
    switch (nrCommand) {
        case ADD:
            if (i >= ADD_MIN_ARG && i <= ADD_MAX_ARG)
                correct = true;
            break;
        case DEL:
            if (i >= DEL_MIN_ARG && i <= DEL_MAX_ARG)
                correct = true;
            break;
        case PRINT:
            if (i >= PRINT_MIN_ARG && i <= PRINT_MAX_ARG)
                correct = true;
            break;
        case CHECK:
            if (i >= CHECK_MIN_ARG && i <= CHECK_MAX_ARG && !isAsterisk(Arg[i - 1]))
                correct = true;
            break;
    }
    if (correct)
        return true;
    else {
        free2DArray(Arg);
        fprintf(stderr, "ERROR\n");
        return false;
    }
}
