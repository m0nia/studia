/** @file
 * Interfejs klasy przechowującej funkcje potrzebne do wczytywania danych i poleceń z wejścia w trybie wsadowym
 *
 * @author Monika Michaluk <m.michaluk5@student.uw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 17.05.2020
 */

#ifndef INPUT_H
#define INPUT_H

/**
 * Typ przechowujący poprawne polecenia. Polecenie ma wartością taką, jaką ma znak ASCII wywołujący to polecenie.
 */
enum command_t {
    GAMMA_MOVE = 'm',
    GAMMA_GOLDEN_MOVE = 'g',
    GAMMA_BUSY_FIELDS = 'b',
    GAMMA_FREE_FIELDS = 'f',
    GAMMA_GOLDEN_POSSIBLE = 'q',
    GAMMA_BOARD = 'p',
    GAMMA_BATCH = 'B',
    GAMMA_INTERACTIVE = 'I'
};

/**
 * Typ przechowujący możliwe stany we wczytywanych danych (maszyna stanowa).
 */
enum input_state {
    READ_COMMAND,
    ARGUMENTS,
    INCORRECT_INPUT,
    RUN_COMMAND,
    START,
    NEW_GAME
};

/**
 * Typ przechowujący argumenty do poleceń wczytane z wejścia i ich liczbę.
 */
typedef struct arguments arguments_t;

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include "input.h"
#include "gamma.h"
#include "interactive.h"

/**
 * Inicjalizuje zerami wartości w strukturze typu @p arguements_t.
 * @return Zwraca pustą strukturę.
 */
arguments_t new_argument_struct();

/**
 * Ignoruje znaki z wejścia do momentu wczytania znaku końca lini '\n'
 */
void ignore_line();

/**
 * Sprawdza czy znak @p c jest białym znakiem
 * @param[in] c – kod ASCII znaku, który sprawdzamy.
 * @return Wartość @p true, jeśli @p c jest białym znakiem, wartość @p false w przeciwnym wypadku.
 */
bool is_space(int c);

/**
 * Sprawdza jaki kolejny znak zostanie wczytany ze standardowego wejścia.
 * @return Kod ASCII znaku, który zostanie wczytany następny ze standardowego wejścia.
 */
int peek_next_char();

/**
 * Sprawdza czy w momencie wywołania tej funkcji na wejściu jest wypisywany komentarz, czyli czy kolejny wczytany znak to '#'
 * @return Wartość @p true, jeśli zaczyna się komentarz, wartość @p false w przeciwym wypadku.
 */
bool is_comment();

/**
 * Sprawdza czy wczytywana linia jest pusta, tzn czy jedyny znak w wczytywanej lini to '\n'
 * Powinna być wywoływana na początku wczytywania nowej lini.
 * @return Wartość @p true, jeśli linia jest pusta, wartość @p false w przeciwnym wypadku.
 */
bool is_empty_line();

/**
 * Sprawdza czy doszliśmy do końca pliku.
 * @return Wartość @p true jeśli nie ma już żadnych znaków do wpisania, wartość @p false w przeciwnym wypadku.
 */
bool is_end_of_file();

/**
 * Sprawdza czy znak @p c jest cyfrą.
 * @param[in] c – kod ASCII znaku, który sprawdzamy.
 * @return Wartość @p true, jeśli znak ASCII o wartości @p c jest cyfrą, @p false w przeciwnym wypadku.
 */
bool is_digit(int c);

/**
 * Sprawdza czy w wynik mnożenia dwóch liczb wychodzi poza zakres typu uint32_t
 * @param[in] a – pierwsza z mnożonych liczb,
 * @param[in] b – druga z mnożonych liczb.
 * @return Wartość @p true, jeśli wynik mnożenia wychodzi poza zakres typu, @p false w przeciwnym wypadku
 */
bool multiplication_overflow(uint32_t a, uint32_t b);

/**
 * Sprawdza czy w wynik dodawania dwóch liczb wychodzi poza zakres typu uint32_t
 * @param[in] a – pierwsza z dodawanych liczb,
 * @param[in] b – druga z dodawanych liczb.
 * @return Wartość @p true, jeśli wynik dodawania wychodzi poza zakres typu, @p false w przeciwnym wypadku
 */
bool addition_overflow(uint32_t a, uint32_t b);

/**
 * Sprawdza czy w danym momencie gry polecenie @p command jest poprawne.
 * @param[in] command – wartość liczbowa sprawdzanego polecenia,
 * @param[in] game_started  – wartość @p true, jeśli gra się rozpoczęła (wykonano z powodzeniem funkcję @ref gamma_new),
 *                            @p false, jeśli nie rozpoczęto jeszcze gry.
 * @return Wartość @p true, jeśli polecenie jest poprawne, @p false w przeciwnym wypadku.
 */
bool is_acceptable_command(int command, bool game_started);

/**
 * Dla danego polecenia @p command, zwraca ile powinno być podanych argumentów w standardowym wejsciu.
 * @param[in] command – polecenie, dla którego sprawdzamy liczbę wymaganych argumentów.
 * @return Liczba potrzebnych argumentów lub -1 jeśli polecenie nie pasuje do żadnego z wymienionych w typie enum command_t
 */
int required_nr_arguments(int command);

/**
 * Wczytuje z wejścia kolejne cyfry i oblicza wartość wczytywanej liczby. Wczytuje znaki dopóki nie trafi na
 * biały znak, znak końca lini lub koniec pliku. Wczytywane liczby nie mogą mieć wiodących zer. Jeśli wczytana liczba
 * przekracza zakres typu uint32_t, wczytany znak nie jest cyfrą lub nastąpił inny błąd przypisuje zmiennej
 * @p correct_input wartość @p false. Wczytuje same cyfry (w szczególności nie wczytuje znaku końca linii).
 *
 * @param[in,out] correct_input – zmienna przechowująca informacje czy dotychczas wczytane dane w wierszu są poprawne
 * @return Wartość wczytanej liczby lub 0 jeśli podczas wczytywania nastąpił błąd.
 */
uint32_t read_number(int *correct_input);

/**
 * Funkcja czytająca z wejścia odzielone białymi znakami argumenty typu uint32_t dopóki nie natrafi na znak końca linii
 * lub koniec pliku. Jeśli dane są niepoprawne przypisuje wartość @p false do zmiennej @p correct_input i przerywa
 * działanie.
 *
 * @param[in, out] correct_input – wskaźnik do zmiennej o wartści @p true, jeśli dane w danym wierszu są poprawne
 *                                 i @p false, jeśli niepoprawne.
 * @param[in] command – numer polecenia, dla którego mają zostać wczytane argumenty.
 * @return Zwraca strukturę zawierającą te argumenty oraz liczbę wczytanych argumentów, jeśli dane były poprawne.
 *          Jeśli dane były niepoprawme, struktura ta jest zainicjalizowana zerami.
 */
arguments_t read_arguments(int *correct_input, int command);

/**
 * Wczytuje jednoznakowe polecenie i sprawdza czy jest poprawne w danym momencie gry. Jeśli polecenie nie jest poprawne
 * przypisuje zmiennej @p correct_input wartość @p false.
 * @param[in,out] correct_input – zmienna przechowująca informacje czy dotychczas wczytane dane w wierszu są poprawne
 * @param[in] game_started  – @p true, jeśli gra się rozpoczęła, @p false w przeciwnym wypadku
 * @return
 */
int read_command(int *correct_input, bool game_started);

/**
 * Na podstawie polecenia @p command wykonuje odpowiednią funkcje w trakcie gry w trybie wsadowym
 * z argumentami znajdującymi się w strukturze @p arguments.
 *
 * @param[in] command – polecenie, na podstawie którego wybierana jest odpowiednia funkcja,
 * @param[in] arguments – argumenty do wywołania tej funkcji,
 * @param[in,out] g – wskaźnik na strukturę przechowującą stan gry.
 */
void run(int command, arguments_t arguments, gamma_t *g);

/**
 * Funkcja rozpoczynająca wczytywanie danych ze standardowego wejścia. Wczytuje dane dopóki nie natrafi na polecenie
 * rozpoczęcia gry w trybie wsadowym lub interaktywnym - jeśli jest to tryb interaktywny, wywołuje funkcję, rozpoczynającą
 * ten tryb w terminalu. Jeśli funkcja ta zakończy się kodem 1 (co oznacza, że nie udało się rozpocząć gry)
 * wczytuje nadal dane, dopóki nie natrafi na poprawne polecenie rozpoczęcia gry.
 * Z wykorzystaniem maszyny stanowej wczytuje kolejne znaki i sprawdza na bieżąco poprawność wczytywanych danych.
 */
void start();

#endif /* INPUT_H */