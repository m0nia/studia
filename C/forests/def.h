#ifndef DEF_H
#define DEF_H

#define MAX_ARG 3 /* ile najwięcej argumentów może przyjąć polecenie, które przyjmuje największą liczbą argumentów */
#define NR_COMANDS 4 /* ilość komend */
#define MAX_LENGHT 5 /* długość najdłuższej komendy */

#define ADD 0
#define DEL 1
#define PRINT 2
#define CHECK 3

/* maksymalna i minimalna liczba argumentów w poleceniu ADD */
#define ADD_MIN_ARG 1
#define ADD_MAX_ARG 3

/* maksymalna i minimalna liczba argumentów w poleceniu DEL */
#define DEL_MIN_ARG 0
#define DEL_MAX_ARG 3

/* maksymalna i minimalna liczba argumentów w poleceniu PRINT */
#define PRINT_MIN_ARG 0
#define PRINT_MAX_ARG 2

/* maksymalna i minimalna liczba argumentów w poleceniu CHECK */
#define CHECK_MIN_ARG 1
#define CHECK_MAX_ARG 3


extern const char commands[NR_COMANDS][MAX_LENGHT + 1];

#endif /* DEF_H */