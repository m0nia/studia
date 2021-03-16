/** @file
 * Interfejs klasy przechowującej stan gry gamma
 *
 * @author Monika Michaluk <m.michaluk5@student.uw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 17.05.2020
 */
#ifndef INTERACTIVE_H
#define INTERACTIVE_H

/**
 * Wyłącza "raw mode" w terminalu.
 * Źródło: https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
 */
void disableRawMode();

/**
 * Włącza "raw mode" w terminalu.
 * Żródło: https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
 */
void enableRawMode();

/**
 * Zwraca następny indeks po @p current_index w cyklicznej tablicy rozmiaru n (numerowanej od 1)
 * @param[in] n – liczba elementów w tablicy,
 * @param[in] current_index – aktualny indeks.
 * @return Zwraca kolejny indeks.
 */
uint32_t next_index(uint32_t n, uint32_t current_index);

/**
 * Zwraca numer następnego gracza w kolejce, który może wykonać ruch.
 * @param[in,out] g – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] current_player – numer aktualnego gracza.
 * @return Zwraca numer następnego gracza w kolejce, który może wykonać ruch lub @p 0 jeśli nikt nie może wykonać ruchu.
 */
uint32_t next_player(gamma_t *g, uint32_t current_player);

/**
 * Przesuwa kursor o @p i jednostek do góry.
 * @param[in,out] cursor_position_row – aktualne położenie kursora.
 * @param[in] i - o ile jednostek przesunąć kursor.
 */
void move_up(uint32_t *cursor_position_row, uint32_t i);

/**
 * Przesuwa kursor o @p i jednostek w lewo.
 * @param[in,out] cursor_position_column – aktualne położenie kursora.
 * @param[in] i - o ile jednostek przesunąć kursor.
 */
void move_left(uint32_t *cursor_position_column, uint32_t i);

/**
 * Przesuwa kursor o @p i jednostek w prawo.
 * @param[in,out] cursor_position_column – aktualne położenie kursora.
 * @param[in] width – szerokość planszy,
 * @param[in] i - o ile jednostek przesunąć kursor.
 */
void move_right(uint32_t *cursor_position_column, uint32_t width, uint32_t i);

/**
 * Przesuwa kursor o @p i jednostek w dół.
 * @param[in,out] cursor_position_row – aktualne położenie kursora,
 * @param[in] height – wysokość planszy,
 * @param[in] i - o ile jednostek przesunąć kursor.
 */
void move_down(uint32_t *cursor_position_row, uint32_t height, uint32_t i);

/**
 * Wyświetla pod planszą informacje, którego gracza jest kolej, ile zajął pól, ile jest wolnych pól, na których
 * może postawić pionek, liczbę zajętych przez niego obszarów (na liczbę dozwolonych) oraz informację,
 * czy gracz może wykonać złoty ruch.
 *
 * @param[in] g – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] current_player  – aktualny gracz,
 * @param[in] how_many_rows – liczba wierszy, jakie zajmuje plansza.
 */
void display_player_info(gamma_t *g, uint32_t current_player, uint32_t how_many_rows);

/**
 * Wczytuje z wejścia polecenie od użytkownika i zwraca odpowiadającą mu wartość
 * @return Kod liczbowy wczytanego polecenia.
 */
int read_interactive_command();

/**
 * Wyświetla podsumowanie gry gamma - planszę, pod planszą listę graczy z informacją ile zajęli pól oraz
 * wyświetla zwycięzcę lub informację o remisie.
 *
 * @param[in] g – wskaźnik na strukturę przechowującą stan gry.
 * @param[in] rows_number – liczba wierszy planszy.
 */
void display_summary(gamma_t *g, uint32_t rows_number);

/**
 * Wyświetla standardową planszę, jeśli w grze bierze udział mniej niż 10 graczy.
 * Pionki na planszy gracza, którego jest kolej wyświetlane są na niebiesko.
 * @param[in] g  – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] current_player - gracz, którego jest kolej.
 */
void display_small_board(gamma_t *g, uint32_t current_player);


/**
 * Wyświetla dużą planszę, jeśli w grze liczba graczy jest >= 10.
 * Pionki na planszy gracza, którego jest kolej wyświetlane są na niebiesko.
 * W tej planszy kursor oznacza całe pole, na którym się znajduje w negatywie. Pojedyncze pole na planszy, na którym można
 * stawiać pionek ma wysokość 1 i szerokość równą liczbie cyfr w numerze gracza o największym numerze.
 * @param[in] g – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] current_player - gracz, którego jest kolej,
 * @param[in] cursor_row - numer wiersza, w którym znajduje się aktualnie kursor,
 * @param[in] cursor_column - numer kolumny, w której znajduje się aktualnie kursor,
 * @param[in] cursor_size - szerokość kursora.
 */
void display_big_board(gamma_t *g, uint32_t current_player, uint32_t cursor_row, uint32_t cursor_column,
                  uint32_t cursor_size);
/**
 * Rozpoczyna grę w trybie interaktywnym.
 * @param[in,out] g – wskaźnik na strukturę przechowującą stan gry.
 * @return Wartość @p 0 jeśli udało się rozpocząć grę, wartość @p 1 jeśli terminal był za mały, aby utworzyć grę.
 */
int start_interactive_mode(gamma_t *g);

#endif /* INTERACTIVE_H */
