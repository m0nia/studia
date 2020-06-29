/** @file
 * Implementacja silnikia gry gamma.
 *
 * Główne funkcje @ref gamma_move i @ref gamma_golden_move umieszczają  pionek gracza na planszy, zmieniając
 * atrybut pola @p owner.
 * Przed każdym ruchem należy sprawdzić czy liczba obszarów zajętych przez gracza (i dodatkowo liczba obszarów
 * zajętych przez gracza, którego pionek jest zbijany, w przypadku złotego ruchu) nie wykroczy po ruchu (w wyniku
 * łączenia / rozdzielania obszarów) poza maksymalną dozwoloną liczbę zajętych obszarów zajętych przez gracza.
 * W tym celu każdy gracz posiada atrybut @p occupied_areas, który jest aktualizowany po każdym ruchu - to o ile
 * zmaleje / wzrośnie liczba tych obszarów oblicza funkcja @ref not_connected_areas, która jest najkosztowniejszą
 * operacją w obecnej implementacji silnika (przechodzenie BFS'em tablicy).
 * Każdy gracz ma atrybut @p occupied_fields, dzięki czemu funkcja @ref gamma_busy_fields wykonywana jest
 * w czasie stałym. Atrybut ten jest aktualizowany po każdym ruchu.
 * W celu optymalizacji funkcji @ref gamma_free_fields struktura @ref gamma_t ma atrybut zliczający wolne pola
 * na planszy - ten atrybut jest zwracany jeśli liczba zajętych obszarów jest ściśle mniejsza niż maksymalna
 * dozwolona liczba zajętych obszarów.
 * Jeśli gracz zajął już maksymalną liczbę obszarów, to jedyne wolne pola, na których może postawić swój pionek, to
 * pola, które nie zwiększą liczby zajętych przez niego obszarów, czyli pola które sąsiadują z jakimś obszarem
 * zajętym przez gracza - te pola zlicza atrybut @p free_fields_on_border w strukturze gracza. Atrybuty te również są
 * aktualizowane po każdym ruchu.
 *
 * @author Monika Michaluk <m.michaluk5@student.uw.edu.pl>
 * @date 17.04.2020
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "gamma.h"
#include "queue.h"

/**
 * Tworzy tablicę struktur @ref player_t o rozmiarze @p players + 1. W rozgrywce gracz o numerze @p player
 * znajduje się pod indeksem o tym samym numerze. Ponieważ poprawny numer gracza jest dodatni, wartość pod
 * indeksem 0 jest ignorowana.
 * Inicjalizuje początkowe wartości parametrów struktur @ref player_t.
 * @param[in] players   – liczba graczy, przekazana z funkcji @ref gamma_new.
 * @return Wskaźnik na zaalokowaną tablicę graczy lub NULL, jeśli nie udało się zaalokować pamięci.
 */
static player_t *create_new_players(uint32_t players) {
    uint64_t players_big = players;
    player_t *new_players = calloc(players_big + 1, sizeof(player_t));

    if (new_players == NULL)
        return NULL;

    for (uint32_t i = 1; i <= players; i++) {
        new_players[i].occupied_fields = 0;
        new_players[i].occupied_areas = 0;
        new_players[i].free_fields_on_border = 0;
        new_players[i].golden_move = false;

    }
    return new_players;
}

/**
 * Tworzy dwuwymiarową tablicę struktur @ref field_t o szerokości @p width i wysokości @p height,
 * w której przechowywan będzie informacja o położeniu pionków graczy.
 * Inicjalizuje początkowe wartości pól.
 * @param[in] width   – szerokość planszy, liczba dodatnia,
 * @param[in] height  – wysokość planszy, liczba dodatnia.
 * @return Wskaźnik na zaalokowaną tablicę pól lub NULL, jeśli nie udało się zaalokować pamięci.
 */
static field_t **create_new_board(uint32_t width, uint32_t height) {
    field_t **board = calloc(height, sizeof(field_t *));

    if (board == NULL)
        return NULL;

    for (uint32_t i = 0; i < height; i++) {
        board[i] = calloc(width, sizeof(field_t));

        if (board[i] == NULL) {
            for (uint32_t j = 0; j < i; j++)
                free(board[i]);
            free(board);
            return NULL;
        }
    }

    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            board[i][j].owner = 0;
            board[i][j].visited = false;
        }
    }

    return board;
}

gamma_t *gamma_new(uint32_t width, uint32_t height, uint32_t players, uint32_t areas) {

    if (width == 0 || height == 0 || players == 0 || areas == 0)
        return NULL;

    gamma_t *gamma_new = calloc(1, sizeof(gamma_t));
    if (gamma_new == NULL)
        return NULL;

    gamma_new->board = create_new_board(width, height);
    if (gamma_new->board == NULL) {
        free(gamma_new);
        return NULL;
    }

    gamma_new->players = create_new_players(players);
    if (gamma_new->players == NULL) {
        free(gamma_new);
        free(gamma_new->board);
        return NULL;
    }
    gamma_new->free_fields = width * height;
    gamma_new->max_areas = areas;
    gamma_new->width = width;
    gamma_new->height = height;
    gamma_new->nr_players = players;

    return gamma_new;
}

void gamma_delete(gamma_t *g) {
    if (g == NULL)
        return;
    free(g->players);
    for (uint32_t i = 0; i < g->height; i++) {
        free(g->board[i]);
    }
    free(g->board);
    free(g);
}

/**
 * Inicjalizuje strukturę pod indeksem @p n w tablicy @p neighbours na współrzędne punktu (@p x, @p y),
 * zwiększa o jeden liczbę elementów w tablicy neighbours.
 * @param[in] x                – numer kolumny, liczba nieujemna mniejsza od wartości
 *                               @p width z funkcji @ref gamma_new,
 * @param[in] y                – numer wiersza, liczba nieujemna mniejsza od wartości
 *                               @p height z funkcji @ref gamma_new.
 * @param[in,out] n            – wskaźnik na aktualną liczbę sąsiadującyh pól w tablicy @p neighbours
 * @param[in,out] neighbours   – tablica pól sąsiadujących do pola (@p x, @p y)
 */
static void set_coordinates(uint32_t x, uint32_t y, uint32_t *n, point_t neighbours[4]) {
    neighbours[*n].x = x;
    neighbours[*n].y = y;
    neighbours[*n].visited = false;
    *n += 1;
}

/**
 * Inicjalizuje tablicę @p neighbours pól sąsiadujących do pola (@p x, @p y)
 *
 * @param[in] g                 – struktura przechowująca stan gry,
 * @param[in] x                 – numer kolumny, liczba nieujemna mniejsza od wartości
 *                                @p width z funkcji @ref gamma_new,
 * @param[in] y                 – numer wiersza, liczba nieujemna mniejsza od wartości
 *                                @p height z funkcji @ref gamma_new,
 * @param[in,out] neighbours    – tablica pól sąsiadujących do pola (@p x, @p y).
 * @return Rzeczywisty rozmiar tablicy @p neighbours (liczba sąsiadujących pól).
 */
static uint32_t set_neighbouring_fields(gamma_t g, uint32_t x, uint32_t y, point_t neighbours[4]) {
    uint32_t n = 0;

    if (x > 0)
        set_coordinates(x - 1, y, &n, neighbours);

    if (y > 0)
        set_coordinates(x, y - 1, &n, neighbours);

    if (x + 1 < g.width)
        set_coordinates(x + 1, y, &n, neighbours);

    if (y + 1 < g.height)
        set_coordinates(x, y + 1, &n, neighbours);

    assert(n <= 4);
    return n;
}

/**
 * Porównuje współrzędne dwóch punktów.
 *
 * @param[in] p1    – struktura zawierająca współrzędne jednego punktu.
 * @param[in] p2    – struktura zawierająca współrzędne drugiego punktu.
 * @return Wartość @p true jeśli punkty mają takie same współrzędne, a @p false w przeciwnym wypadku.
 */
static bool equals(point_t p1, point_t p2) {
    if (p1.x == p2.x && p1.y == p2.y)
        return true;
    return false;
}

/**
 * Oznacza pola w tablicy pól sąsiadujących jako nieodwiedzone.
 *
 * @param[in,out] neighbours    – tablica struktur, w których znajdują się współrzędne pól sąsiadujących
 *                                do pola (@p x, @p y) z funkcji @ref gamma_move lub @ref gamma_golden_move,
 * @param[in] n                 – liczba sąsiadujących pól w tablicy @p neighbours.
 */
static void unmark_neighbours(point_t neighbours[4], uint32_t n) {
    for (uint32_t i = 0; i < n; i++) {
        neighbours[i].visited = false;
    }
}

/**
 * Oznacza pola odwiedzone jako nieodwiedzone.
 * Wywoływana po wykonaniu funkcji @ref find_path "czyści" przebytą ścieżkę w tablicy, zaczynając od punktu @p start.
 * Przechodzi przy pomocy kolejki planszę, znajdującą się w strukturze @p g, przez pola oznaczone jako odwiedzone,
 * oznaczając je jako nieodwiedzone.
 * Powinna być wywoływana po każdym wywołaniu funkcji @ref find_path i z tym samym argumentem startowym.
 *
 * @param[in] g         – struktura przechowująca stan gry,
 * @param[in] start     – struktura przechowująca współrzędne punktu startowego.
 */
static void unmark_path(gamma_t g, point_t start) {
    point_t top;
    point_t neighbours[4];
    uint32_t n;
    Queue *q;
    Init(&q);
    Enqueue(q, start);
    while (!isEmpty(q)) {
        top = Dequeue(q);
        if (g.board[top.y][top.x].visited) {
            n = set_neighbouring_fields(g, top.x, top.y, neighbours);
            g.board[top.y][top.x].visited = false;
            for (uint32_t i = 0; i < n; i++)
                Enqueue(q, neighbours[i]);
        }
    }
    Clear(&q);
}

/** @brief Szuka ścieżki pomiędzy dwoma punktami.
 * Szuka ścieżki z pola @p start do pola @p end na planszy znajdującej się w strukturze @p g przechowującej stan gry.
 * Przeszukuje przy pomocy kolejki tablicę, przechodząc tylko przez pola, na których stoi pionek
 * gracza @p player i nie przechodząc przez pole @p center.
 *
 * @param[in] g         – struktura przechowująca stan gry,
 * @param[in] player    – numer gracza, liczba dodatnia niewiększa od wartości
 *                        @p players z funkcji @ref gamma_new,
 * @param[in] center    – struktura przechowująca współrzędne punktu (@p x, @p y) z funkcji @ref gamma_move lub
 *                        @ref gamma_golden_move, na którym stawiany jest pionek gracza,
 * @param[in] start     – struktura przechowująca współrzędne punktu startowego,
 * @param[in] end       – struktura przechowująca współrzędne punktu końcowego.
 * @return Wartość @p true jeśli istnieje ścieżka pomiędzy punktem @p start a punktem @p end, a @p false w przeciwnym wypadku.
 */
static bool find_path(gamma_t g, uint32_t player, point_t center, point_t start, point_t end) {
    point_t top;
    point_t neighbours[4];
    uint32_t n;
    bool found = false;

    Queue *q;
    Init(&q);
    Enqueue(q, start);
    while (!isEmpty(q) && !found) {
        top = Dequeue(q);
        found = equals(top, end);
        if (g.board[top.y][top.x].owner == player && !g.board[top.y][top.x].visited && !equals(top, center)) {
            n = set_neighbouring_fields(g, top.x, top.y, neighbours);
            g.board[top.y][top.x].visited = true;
            for (uint32_t i = 0; i < n; i++)
                Enqueue(q, neighbours[i]);
        }
    }
    Clear(&q);
    return found;
}

/**
 * Zlicza ile odrębnych obszarów należących do gracza @p player znajduje się w sąsiedztwie pola @p center.
 *
 * Na początku przyjmujemy, że odrębnych obszarów jest maksymalna ilość, czyli tyle, ile jest sąsiadów.
 * Potem wartość odpowiednio zmniejszamy kiedy sąsiadujące pole nie należy do gracza lub kiedy dwa pola
 * znajdują się w tym samym obszarze - w tym celu dla każdej pary pól z tablicy @p neighbours sprawdzam
 * czy między tymi polami istnieje ścieżka (@ref find_path). Jeśli tak, to znaczy, że pola należą do tego
 * samego obszaru.
 * Wywoływana w @ref gamma_move lub @ref gamma_golden_move, aby sprawdzić czy ruch jest poprawny i jeśli tak,
 * w celu zaktualizowania parametru @p occupied_areas gracza @p player.

 * @param[in] g             – struktura przechowująca stan gry,
 * @param[in] player        – numer gracza, liczba dodatnia niewiększa od wartości
 *                            @p players z funkcji @ref gamma_new,
 * @param[in] center        – struktura przechowująca współrzędne punktu (@p x, @p y) z funkcji @ref gamma_move lub
 *                            @ref gamma_golden_move, na którym stawiany jest pionek gracza,
 * @param[in] neighbours    – tablica struktur, w których znajdują się współrzędne pól sąsiadujących
 *                            do pola (@p x, @p y) z funkcji @ref gamma_move lub @ref gamma_golden_move,
 * @param[in] n             – liczba sąsiadujących pól w tablicy @p neighbours.
 * @return Liczba odrębnych obszarów wokół pola @p center.
 */
static uint32_t not_connected_areas(gamma_t g, uint32_t player, point_t center, point_t neighbours[4], uint32_t n) {
    uint32_t how_many_areas = n;
    uint32_t x, y;

    for (uint32_t i = 0; i < n; i++) {
        x = neighbours[i].x;
        y = neighbours[i].y;
        if (g.board[y][x].owner != player) {
            how_many_areas--;
            neighbours[i].visited = true;
        }
    }

    for (uint32_t i = 0; i < n; i++) {

        if (!neighbours[i].visited) {
            for (uint32_t j = i + 1; j < n; j++) {
                if (!neighbours[j].visited && find_path(g, player, center, neighbours[i], neighbours[j])) {
                    how_many_areas--;
                    neighbours[j].visited = true;
                }
                unmark_path(g, neighbours[i]);
            }
        }
        neighbours[i].visited = true;
    }
    unmark_neighbours(neighbours, n);

    assert(how_many_areas <= 4);

    return how_many_areas;
}

/**
 * Sprawdza czy pole @p p sąsiaduje z polem należącym do gracza @p player i innym niż @p center.
 * @param[in] g             – struktura przechowująca stan gry,
 * @param[in] center        – struktura przechowująca współrzędne punktu (@p x, @p y) z funkcji @ref gamma_move lub
 *                            @ref gamma_golden_move, na którym stawiany jest pionek gracza,
 * @param[in] p             – struktura przechowująca współrzędne punktu, którego pola sąsiadujące będziemy sprawdzać
 * @param[in] player        – numer gracza, liczba dodatnia niewiększa od wartości @p players z funkcji @ref gamma_new,
 * @return Wartość @p true, jeśli pole @p p sąsiaduje z polem, na którym stoi pionek gracza @p player
 *         i innym niż @p center, a @p false w przeciwnym wypadku.
 */
static bool has_neighbour(gamma_t g, point_t center, point_t p, uint32_t player) {
    uint32_t x1, y1;
    point_t neighbours[4];
    uint32_t n = set_neighbouring_fields(g, p.x, p.y, neighbours);
    bool has_neighbour = false;
    for (uint32_t i = 0; i < n; i++) {
        x1 = neighbours[i].x;
        y1 = neighbours[i].y;
        if (!equals(neighbours[i], center) && g.board[y1][x1].owner == player)
            has_neighbour = true;
    }
    return has_neighbour;
}

/**
 * Przechodzi przez tablicę sąsiadów pola @p center i zlicza puste pola, które nie sąsiadują z żadnym polem, na którym
 * stoi pionek gracza @p player, nie wliczając pola @p center.
 * Wywoływana w celu zaktualizowania atrybutu @p free_fields_on_border gracza @p player.
 *
 * W funkcji @ref gamma_move gracz @p player stawia pionek na pustym polu, co może powodować zwiększenie
 * @p p.free_fields_on_border - musimy uwzględnić te puste pola, których nie uwzględniliśmy wcześniej,
 * czyli te, które nie sąsiadowały wcześniej z polem należącym do gracza @p player.
 *
 * W funkcji @ref gamma_golden_move funkcja wywoływana jest dwa razy, raz dla gracza wykonującego ruch i raz
 * dla przeciwnika.
 * Jeśli gracz @p player stawia swój pionek na polu @p center, to tak jak w zwykłym ruchu
 * może to powodować zwiększenie @p p.free_fields_on_border.
 * Jeśli pionek gracza @p player jest zdejmowany z planszy, to może to spowodować zmniejszenie
 * parametru p.free_fields_on_border - puste pola, które przed "zbiciem" nie sąsiadowały z żadnym innym polem
 * gracza @p player (tylko z polem @p center) po zbiciu nie będą sąsiadować z żadnym polem gracza @p player,
 * musimy więc odpowiednio zmniejszyć atrybut @p p.free_fields_on_border.
 *
 * @param[in] g             – struktura przechowująca stan gry,
 * @param[in] center        – struktura przechowująca współrzędne punktu (@p x, @p y) z funkcji @ref gamma_move lub
 *                            @ref gamma_golden_move, na którym stawiany jest pionek gracza,
 * @param[in] neighbours    – tablica struktur, w których znajdują się współrzędne pól sąsiadujących
 *                            do pola (@p x, @p y) z funkcji @ref gamma_move lub @ref gamma_golden_move,
 * @param[in] n             – liczba sąsiadujących pól w tablicy @p neighbours,
 * @param[in] player        – numer gracza, liczba dodatnia niewiększa od wartości
 *                            @p players z funkcji @ref gamma_new.
 * @return Liczba nieujemna, zmiana jaką należy wprowadzić w atrybucie @p free_fields_on_border gracza
 *         @p player po poprawnie wykonanym ruchu.
 */
static uint32_t count_change(gamma_t g, point_t center, point_t *neighbours, uint32_t n, uint32_t player) {
    uint32_t x1, y1, count = 0;
    for (uint32_t i = 0; i < n; i++) {
        x1 = neighbours[i].x;
        y1 = neighbours[i].y;
        if (g.board[y1][x1].owner == 0 && !has_neighbour(g, center, neighbours[i], player))
            count++;
    }
    assert(count <= 4);
    return count;
}

/**
 * Zmniejsza o 1 parametr @p free_fields_on_border graczy znajdujących się w tablicy @p neighbours.
 * Wywoływana w funkcji gamma_move po poprawnie wykonanym ruchu.
 * Przed wykonaniem ruchu pole (@p x, @p y) z funkcji @ref gamma_move było puste, zatem jeśli z tym
 * polem sąsiaduje pole, na którym stoi pionek jakiegoś gracza, to po wykonaniu ruchu liczba pól sąsiadujących
 * z obszarami tego gracza zmniejsza się o 1.

 * @param[in] g             – struktura przechowująca stan gry
 * @param[in] neighbours    – tablica zawierająca sąsiadujące pola do pola (@p x, @p y) z funkcji @ref gamma_move
 * @param[in] n             – liczba sąsiadujących pól w tablicy @p neighbours.
 */
static void decrease_border_fields_of_neighbours(gamma_t g, point_t *neighbours, uint32_t n) {
    uint32_t x1, y1, owner;

    for (uint32_t i = 0; i < n; i++) {
        x1 = neighbours[i].x;
        y1 = neighbours[i].y;
        owner = g.board[y1][x1].owner;
        if (owner != 0 && !neighbours[i].visited) { // Jeśli pole nie jest puste i nie zmniejszyliśmy jeszcze
            // parametru free_fields_on_border dla danego gracza w tej funkcji.

            assert(g.players[owner].free_fields_on_border > 0);
            g.players[owner].free_fields_on_border -= 1;
            for (uint32_t j = i + 1; j < n; j++) {
                x1 = neighbours[j].x;
                y1 = neighbours[j].y;
                if (owner == g.board[y1][x1].owner)
                    neighbours[j].visited = true; // Oznaczamy pole jako odwiedzone - dla każdego gracza zmniejszamy
                // ten parametr tylko raz w danym wywołaniu funkcji.
            }
        }
    }
    unmark_neighbours(neighbours, n);
}

bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (g == NULL || x >= g->width || y >= g->height || player == 0 || player > g->nr_players ||
        g->board[y][x].owner != 0)
        return false;

    point_t center;
    center.x = x;
    center.y = y;
    point_t neighbours[4];
    uint32_t n = set_neighbouring_fields(*g, x, y, neighbours);

    uint32_t areas = not_connected_areas(*g, player, center, neighbours, n);

    assert(g->players[player].occupied_areas >= areas);
    if (g->players[player].occupied_areas - areas + 1 > g->max_areas)
        return false;

    g->board[y][x].owner = player;
    g->players[player].occupied_fields += 1;
    g->players[player].occupied_areas -= areas;
    g->players[player].occupied_areas += 1;

    uint32_t new_fields = count_change(*g, center, neighbours, n, player);
    g->players[player].free_fields_on_border += new_fields;
    decrease_border_fields_of_neighbours(*g, neighbours, n);

    assert(g->free_fields > 0);
    g->free_fields -= 1;

    return true;
}

uint64_t gamma_busy_fields(gamma_t *g, uint32_t player) {
    if (g == NULL || player == 0 || player > g->nr_players)
        return 0;
    return g->players[player].occupied_fields;
}

uint64_t gamma_free_fields(gamma_t *g, uint32_t player) {
    if (g == NULL || player == 0 || player > g->nr_players)
        return 0;
    if (g->players[player].occupied_areas == g->max_areas)
        return g->players[player].free_fields_on_border;
    return g->free_fields;
}

bool gamma_golden_possible(gamma_t *g, uint32_t player) {
    if (g == NULL || player == 0 || player > g->nr_players)
        return false;
    uint64_t fields_of_opponents_nr = g->height * g->width - g->free_fields - g->players[player].occupied_fields;
    if (g->players[player].golden_move == true || fields_of_opponents_nr == 0)
        return false;

    // dla każdego niepustego i nienależącego do danego gracza pola sprawdzam, czy można na nim wykonać złoty ruch
    for(uint32_t i = 0; i < g->height; i++){
        for(uint32_t j = 0; j < g->width; j++){
            if(g->board[i][j].owner != 0 && g->board[i][j].owner != player){
                bool golden_possible = true;
                uint32_t opponent = g->board[i][j].owner;
                point_t neighbours[4];
                uint32_t n = set_neighbouring_fields(*g, j, i, neighbours);
                point_t center;
                center.x = j;
                center.y = i;

                uint32_t areas_of_player = not_connected_areas(*g, player, center, neighbours, n);
                uint32_t areas_of_opponent = not_connected_areas(*g, opponent, center, neighbours, n);

                if (g->players[player].occupied_areas - areas_of_player + 1 > g->max_areas)
                    golden_possible = false;

                if (g->players[opponent].occupied_areas - 1 + areas_of_opponent > g->max_areas)
                    golden_possible = false;

                if(golden_possible == true)
                    return true;
            }
        }
    }
    return false;
}

bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (g == NULL || player == 0 || player > g->nr_players || x >= g->width || y >= g->height)
        return false;

    uint32_t opponent = g->board[y][x].owner;
    if (g->players[player].golden_move == true || opponent == 0 || opponent == player)
        return false;

    point_t center;
    center.x = x;
    center.y = y;
    point_t neighbours[4];
    uint32_t n = set_neighbouring_fields(*g, x, y, neighbours);

    uint32_t areas_of_player = not_connected_areas(*g, player, center, neighbours, n);
    uint32_t areas_of_opponent = not_connected_areas(*g, opponent, center, neighbours, n);

    assert(g->players[player].occupied_areas >= areas_of_player);
    if (g->players[player].occupied_areas - areas_of_player + 1 > g->max_areas)
        return false;

    assert(g->players[opponent].occupied_areas > 0);
    if (g->players[opponent].occupied_areas - 1 + areas_of_opponent > g->max_areas)
        return false;

    g->board[y][x].owner = player;

    g->players[player].golden_move = true;
    g->players[player].occupied_fields += 1;
    g->players[player].occupied_areas -= areas_of_player;
    g->players[player].occupied_areas += 1;
    g->players[player].free_fields_on_border += count_change(*g, center, neighbours, n, player);

    g->players[opponent].occupied_fields -= 1;
    g->players[opponent].occupied_areas += areas_of_opponent;
    g->players[opponent].occupied_areas -= 1;
    g->players[opponent].free_fields_on_border -= count_change(*g, center, neighbours, n, opponent);

    return true;
}

/**
 * Oblicza ile cyfrowa jest liczba @p number
 * @param[in] number    – liczba dodatnia.
 * @return Liczba cyfr w liczbie @p number.
 */
static uint32_t count_digits(uint32_t number) {
    uint32_t digits = 0;
    while (number != 0) {
        digits++;
        number /= 10;
    }
    return digits;
}

/**
 * Daje napis opisujący stan planszy, jeśli w grze bierze udział więcej niż 9 graczy.
 * Każda wypisywana komórka ma tą samą szerokość równą ilości cyfr w liczbie graczy biorących udział w grze.
 * Każda komórka w tym samym wierszu jest oddzielona od siebie spacją, a wiersze z wypisywanymi komórkami
 * oddzielone są od siebie pustymi wierszami, w których znajdują się same spacje.
 *
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry.
 * @return Wskaźnik na zaalokowany bufor zawierający napis opisujący stan
 * planszy lub NULL, jeśli nie udało się zaalokować pamięci.
 */
static char *board_with_players_greater_than_9(gamma_t *g) {
    uint32_t max = g->nr_players;
    uint32_t digits = count_digits(max);
    uint32_t cell_width = digits + 1;
    uint64_t board_width = g->width * cell_width + 1;
    uint64_t board_height = g->height * 2;
    char *board = malloc(sizeof(char) * (board_width * board_height + 1));
    if (board == NULL)
        return NULL;

    char *buffer = malloc(sizeof(char) * digits + 1);
    if (buffer == NULL) {
        free(board);
        return NULL;
    }
    uint64_t start_column;
    uint64_t j;
    uint64_t k = 0;
    uint64_t n;
    for (uint64_t i = g->height - 1; i != (uint64_t) -1; i--) {
        j = 0;
        start_column = 1;
        while (j < board_width - 1) {
            if (j == start_column) {
                n = 0;
                sprintf(buffer, "%d", g->board[i][start_column / cell_width].owner);
                if (buffer[n] == '0') {
                    board[k++] = '.';
                    n++;
                }
                while (buffer[n] != '\0') {
                    board[k++] = buffer[n++];
                }
                start_column += cell_width;
                j += n;
            } else {
                board[k++] = ' ';
                j++;
            }
        }
        board[k++] = '\n';
        if (i != 0) {
            for (uint64_t l = 0; l < board_width - 1; l++)
                board[k++] = ' ';
            board[k++] = '\n';
        }
    }
    free(buffer);
    board[k] = '\0';

    return board;
}

char *gamma_board(gamma_t *g) {
    if (g == NULL)
        return NULL;
    if (g->nr_players > 9)
        return board_with_players_greater_than_9(g);
    uint64_t size = (g->width + 1) * g->height + 1;
    char *board = malloc(size * sizeof(char));
    if (board == NULL)
        return NULL;
    uint32_t k = 0;
    for (uint32_t i = g->height - 1; i != (uint32_t) -1; i--) {
        for (uint32_t j = 0; j < g->width; j++) {
            if (g->board[i][j].owner == 0)
                board[k] = '.';
            else
                board[k] = g->board[i][j].owner + '0';
            k++;
        }
        board[k] = '\n';
        k++;
    }
    board[k] = '\0';
    return board;
}