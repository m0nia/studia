#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "cacti.h"

// dynamiczna tablica wskaźników
typedef struct dyn_arr {
    void* *array;
    size_t size;
    size_t capacity;
    size_t nbytes; // rozmiar elementu trzymanego w tablicy w bajtach
    size_t max_size;

} dyn_array_t;

void init_array(dyn_array_t *A, size_t max_size, size_t nbytes) {
    A->array = NULL;
    A->size = 0; // liczba elementów w tablicy
    A->capacity = 0; // liczba miejsc w tablicy
    A->max_size = max_size;
    A->nbytes = nbytes;
}

void deallocate(dyn_array_t *A) {
    if (A->array == NULL) {
        A->array = malloc(A->nbytes);
        A->capacity = 1;
    } else {
        size_t new_capacity = 2 * A->capacity > A->max_size ? A->max_size : 2 * A->capacity;
        void* *temp = malloc(A->nbytes * new_capacity);
        if(temp == NULL) exit(1);

        for(size_t i = 0; i < A->size; i++)
            temp[i] = A->array[i];

        free(A->array);
        A->array = temp;
        A->capacity = new_capacity;
    }
}

bool push_back(dyn_array_t *A, void* a) {
    if (A->capacity == A->size) {
        if(A->capacity == A->max_size) return false;
        deallocate(A);
    }
    A->array[A->size] = a;
    A->size += 1;
    return true;
}

// zwraca null jesli index jest spoza zakresu
void* get(dyn_array_t * A, size_t index){
    if(index >= A->size) return NULL;
    return A->array[index];
}

void destroy_array(dyn_array_t* A){
    for(size_t i = 0; i < A->size; i++){
        free(A->array[i]);
    }
    free(A->array);
}