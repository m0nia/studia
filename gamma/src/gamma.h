/** @file
 * Interfejs klasy przechowującej stan gry gamma
 *
 * @author Marcin Peczarski <marpe@mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 18.03.2020
 */

#ifndef GAMMA_H
#define GAMMA_H

#include <stdbool.h>
#include <stdint.h>


/**
 *
 * Struktura zawierająca informacje o graczu biorącym udział w rozgrywce.
 */
typedef struct player {
    uint64_t occupied_fields; ///< Liczba zajętych pól przez gracza.
    uint32_t occupied_areas; ///< Liczba zajętych obszarów przez gracza.
    uint64_t free_fields_on_border; ///< Liczba wolnych pól sąsiadujących z jakimś obszarem należącym do gracza.
    bool golden_move; ///< @p true jeśli gracz wykonał złoty ruch, @p false w przeciwnym wypadku.
} player_t;

/**
 *
 * Struktura przechowująca informacje o polu znajdującym się na planszy.
 */
typedef struct field {
    uint32_t owner; ///< Numer gracza, którego pionek znajduje się na danym polu, @p 0 jeśli pole jest puste.
    bool visited; ///< @p true, jeśli pole zostało odwiedzone, @p false w przeciwnym wypadku.
} field_t;

/**
 *
 * Struktura przechowująca stan gry.
 */
typedef struct gamma {
    field_t **board; ///< Dwuwymiarowa tablica pól zawierająca informacje o położeniu pionków graczy.
    uint32_t width; ///< Szerokość planszy.
    uint32_t height; ///< Wysokość planszy.
    uint32_t max_areas; ///< Maksymalna liczba obszarów, jakie może zająć jeden gracz.
    uint32_t nr_players; ///< Liczba graczy.
    uint64_t free_fields; ///< Liczba wolnych pól na planszy.
    player_t *players; ///< Tablica struktur @ref player_t zawierająca wszystkich graczy.
} gamma_t;


/** @brief Tworzy strukturę przechowującą stan gry.
 * Alokuje pamięć na nową strukturę przechowującą stan gry.
 * Inicjuje tę strukturę tak, aby reprezentowała początkowy stan gry.
 * @param[in] width   – szerokość planszy, liczba dodatnia,
 * @param[in] height  – wysokość planszy, liczba dodatnia,
 * @param[in] players – liczba graczy, liczba dodatnia,
 * @param[in] areas   – maksymalna liczba obszarów,
 *                      jakie może zająć jeden gracz.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci lub któryś z parametrów jest niepoprawny.
 */
gamma_t *gamma_new(uint32_t width, uint32_t height, uint32_t players, uint32_t areas);

/** @brief Usuwa strukturę przechowującą stan gry.
 * Usuwa z pamięci strukturę wskazywaną przez @p g.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] g       – wskaźnik na usuwaną strukturę.
 */
void gamma_delete(gamma_t *g);

/** @brief Wykonuje ruch.
 * Ustawia pionek gracza @p player na polu (@p x, @p y).
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Wartość @p true, jeśli ruch został wykonany, a @p false,
 * gdy ruch jest nielegalny lub któryś z parametrów jest niepoprawny.
 */
bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Wykonuje złoty ruch.
 * Ustawia pionek gracza @p player na polu (@p x, @p y) zajętym przez innego
 * gracza, usuwając pionek innego gracza.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Wartość @p true, jeśli ruch został wykonany, a @p false,
 * gdy gracz wykorzystał już swój złoty ruch, ruch jest nielegalny
 * lub któryś z parametrów jest niepoprawny.
 */
bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Podaje liczbę pól zajętych przez gracza.
 * Podaje liczbę pól zajętych przez gracza @p player.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczba pól zajętych przez gracza lub zero,
 * jeśli któryś z parametrów jest niepoprawny.
 */
uint64_t gamma_busy_fields(gamma_t *g, uint32_t player);

/** @brief Podaje liczbę pól, jakie jeszcze gracz może zająć.
 * Podaje liczbę wolnych pól, na których w danym stanie gry gracz @p player może
 * postawić swój pionek w następnym ruchu.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczba pól, jakie jeszcze może zająć gracz lub zero,
 * jeśli któryś z parametrów jest niepoprawny.
 */
uint64_t gamma_free_fields(gamma_t *g, uint32_t player);

/** @brief Sprawdza, czy gracz może wykonać złoty ruch.
 * Sprawdza, czy gracz @p player jeszcze nie wykonał w tej rozgrywce złotego
 * ruchu i jest przynajmniej jedno pole zajęte przez innego gracza. Sprawdza każde pole na planszy,
 * które jest niepuste i nienależy do gracza @p player czy po wykonaniu ruchu na dane pole ilość obszarów u gracza
 * lub przeciwnika nie zmieni się ponad dozwoloną.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Wartość @p false, jeśli podane argumenty są niepoprawne lub jeśli gracz wykonał już złoty ruch lub nie ma
 * na planszy żadnego pola zajętego przez innego gracza lub jeśli złoty ruch nie może być wykonany na żadnym z pól
 * na planszy (bo zwiększyłby liczbę zajętych obszarów ponad maksymalnie dozwoloną).
 * Wartość @p true, jeśli istnieje pole na planszy, na którym gracz @p player może wykonać złoty ruch.
 */
bool gamma_golden_possible(gamma_t *g, uint32_t player);

/** @brief Daje napis opisujący stan planszy.
 * Alokuje w pamięci bufor, w którym umieszcza napis zawierający tekstowy
 * opis aktualnego stanu planszy. Przykład znajduje się w pliku gamma_test.c.
 * Funkcja wywołująca musi zwolnić ten bufor.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry.
 * @return Wskaźnik na zaalokowany bufor zawierający napis opisujący stan
 * planszy lub NULL, jeśli nie udało się zaalokować pamięci.
 */
char *gamma_board(gamma_t *g);

#endif /* GAMMA_H */
