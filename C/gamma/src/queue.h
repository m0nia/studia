/** @file
 * Interfejs kolejki.
 *
 * @author Monika Michaluk <m.michaluk5@student.uw.edu.pl>
 * @date 17.04.2020
 */

#ifndef GAMMA_QUEUE_H
#define GAMMA_QUEUE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Struktura przechowująca współrzędne pola.
 */
typedef struct point {
    uint32_t x; ///< Numer kolumny.
    uint32_t y; ///< Numer wiersza.
    bool visited; ///< @p true, jeśli pole zostało odwiedzone, @p false w przeciwnym wypadku.
} point_t;

/**
 * Struktura implementująca listę jednokierunkową
 */
struct list {
    point_t value; ///< Przechowywana wartość.
    struct list *next; ///< Wskaźnik na następny element w liście.
};

/**
 * Struktura implementująca kolejkę.
 */
typedef struct queue {
    struct list *front; ///< Wskaźnik na pierwszy element w kolejce.
    struct list *back; ///< Wskaźnik na ostatni element w kolejce.
} Queue;

/**
 * Sprawdza czy kolejka jest pusta.
 * @param q
 * @return
 */
int isEmpty(Queue *q);

/**
 *
 * Inicjalizuje nową kolejkę.
 *
 * @param q
 */
void Init(Queue **q);

/**
 * Umieszcza element w kolejce.
 * @param q
 * @param value
 */
void Enqueue(Queue *q, point_t value);

/**
 * Zdejmuje i zwraca pierwszy element w kolejce.
 * @param q
 * @return
 */
point_t Dequeue(Queue *q);

/**
 * Opróżnia kolejkę.
 * @param q
 */
void Clear(Queue **q);

#endif //GAMMA_QUEUE_H
