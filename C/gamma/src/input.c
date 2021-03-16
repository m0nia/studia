#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include "input.h"
#include "gamma.h"
#include "interactive.h"

#define MAX_NR_ARGUMENTS 4

struct arguments {
    uint32_t arg_array[MAX_NR_ARGUMENTS]; ///< tablica przechowująca argumenty wczytane z wejścia do odpowiednich poleceń gry gamma
    int nr_of_arguments; ///< liczba rzeczywiście wczytanych argumentów
};

arguments_t new_argument_struct() {
    arguments_t empty_arg;
    empty_arg.nr_of_arguments = 0;
    for (int i = 0; i < MAX_NR_ARGUMENTS; i++)
        empty_arg.arg_array[i] = 0;
    return empty_arg;
}

void ignore_line() {
    int c = getchar();
    while (c != '\n' && c != EOF)
        c = getchar();
}

bool is_space(int c) {
    if (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r')
        return true;
    return false;
}

int peek_next_char() {
    int c = getchar();
    ungetc(c, stdin);
    return c;
}

bool is_comment() {
    int c = peek_next_char();
    return c == '#';
}

bool is_empty_line() {
    int c = peek_next_char();
    return c == '\n';
}

bool is_end_of_file() {
    int c = getchar();
    if (c == EOF)
        return true;
    ungetc(c, stdin);
    return false;
}

bool is_digit(int c) {
    if (c >= '0' && c <= '9')
        return true;
    return false;
}

bool multiplication_overflow(uint32_t a, uint32_t b) {
    if (b == 0)
        return false;
    if (a > UINT32_MAX / b)
        return true;
    return false;
}

bool addition_overflow(uint32_t a, uint32_t b) {
    if (a > UINT32_MAX - b)
        return true;
    return false;
}

bool is_acceptable_command(int command, bool game_started) {
    bool is_acceptable_command;
    if (game_started) {
        switch (command) {
            case GAMMA_MOVE:
                is_acceptable_command = true;
                break;
            case GAMMA_GOLDEN_MOVE:
                is_acceptable_command = true;
                break;
            case GAMMA_BUSY_FIELDS:
                is_acceptable_command = true;
                break;
            case GAMMA_FREE_FIELDS:
                is_acceptable_command = true;
                break;
            case GAMMA_GOLDEN_POSSIBLE:
                is_acceptable_command = true;
                break;
            case GAMMA_BOARD:
                is_acceptable_command = true;
                break;
            default:
                is_acceptable_command = false;
                break;
        }
    } else {
        switch (command) {
            case GAMMA_BATCH:
                is_acceptable_command = true;
                break;
            case GAMMA_INTERACTIVE:
                is_acceptable_command = true;
                break;
            default:
                is_acceptable_command = false;
                break;
        }
    }
    return is_acceptable_command;
}

int required_nr_arguments(int command) {
    int args_nr = -1;
    switch (command) {
        case GAMMA_MOVE:
            args_nr = 3;
            break;
        case GAMMA_GOLDEN_MOVE:
            args_nr = 3;
            break;
        case GAMMA_BUSY_FIELDS:
            args_nr = 1;
            break;
        case GAMMA_FREE_FIELDS:
            args_nr = 1;
            break;
        case GAMMA_GOLDEN_POSSIBLE:
            args_nr = 1;
            break;
        case GAMMA_BOARD:
            args_nr = 0;
            break;
        case GAMMA_BATCH:
            args_nr = 4;
            break;
        case GAMMA_INTERACTIVE:
            args_nr = 4;
            break;
    }
    assert(args_nr != -1);
    return args_nr;
}

uint32_t read_number(int *correct_input) {
    assert(*correct_input == true);
    uint32_t value = 0; //wartość wczytaj liczby
    uint32_t factor = 10;

    int c = peek_next_char();
    if (c == '0') { //przyjmuję, że liczby nie mogą zaczynać się od początkowych zer
        getchar();
        c = peek_next_char();
        if (c == '\n' || is_space(c))
            return 0;
        else
            *correct_input = false;
    }

    assert(!is_space(c) && c != '\n' && c != EOF);
    while (!is_space(c) && c != '\n' && c != EOF && *correct_input) {

        if (is_digit(c)) {
            getchar();

            if (multiplication_overflow(value, factor) || addition_overflow(value * factor, c - '0')) {
                *correct_input = false;
            } else {
                value = value * factor + c - '0';
                c = peek_next_char();
            }
        } else {
            *correct_input = false;
        }
    }
    if (c == EOF)
        *correct_input = false;
    if (*correct_input)
        return value;
    return 0;
}

arguments_t read_arguments(int *correct_input, int command) {
    assert(*correct_input == true);
    arguments_t input_arg = new_argument_struct();
    int i;
    uint32_t arg;
    int c = '\0';
    bool end = false;
    while (!end && *correct_input) { //c != '\n' && c != EOF && *correct_input
        c = peek_next_char();
        if (is_digit(c)) {
            arg = read_number(correct_input); // pobiera z wejścia same cyfry, kolejny char to biały znak, '\n' lub eof (jeśli input poprawny)
            i = input_arg.nr_of_arguments;
            input_arg.arg_array[i] = arg;
            input_arg.nr_of_arguments += 1;
        } else if (c == '\n') {
            getchar();
            end = true;
        } else if (is_space(c)) {
            getchar();
        } else {
            *correct_input = false;
        }
    }

    if (*correct_input == false) {
        input_arg = new_argument_struct();
    }

    if (required_nr_arguments(command) != input_arg.nr_of_arguments) {
        ungetc(c, stdin);
        *correct_input = false;
    }

    return input_arg;
}

int read_command(int *correct_input, bool game_started) {
    int command = getchar();
    int next_char = peek_next_char();
    if ((!is_space(next_char) && next_char != '\n') || !is_acceptable_command(command, game_started)) {
        *correct_input = false;
        return 0;
    }
    return command;
}

void run(int command, arguments_t arguments, gamma_t *g) {
    uint32_t *args = arguments.arg_array;
    char *board;
    switch (command) {
        case GAMMA_MOVE:
            printf("%d\n", gamma_move(g, args[0], args[1], args[2]));
            break;
        case GAMMA_GOLDEN_MOVE:
            printf("%d\n", gamma_golden_move(g, args[0], args[1], args[2]));
            break;
        case GAMMA_BUSY_FIELDS:
            printf("%"PRIu64"\n", gamma_busy_fields(g, args[0]));
            break;
        case GAMMA_FREE_FIELDS:
            printf("%"PRIu64"\n", gamma_free_fields(g, args[0]));
            break;
        case GAMMA_GOLDEN_POSSIBLE:
            printf("%d\n", gamma_golden_possible(g, args[0]));
            break;
        case GAMMA_BOARD:
            board = gamma_board(g);
            printf("%s", board);
            free(board);
    }
}

void start(){
    int line_nr = 1;
    int command;
    int correct_input;
    uint32_t *args;
    gamma_t *g = NULL;
    arguments_t input_arguments;
    enum input_state state = START;
    bool end = false;
    bool game_started = false;
    while (!end) {
        switch (state) {
            case START:
                correct_input = true;
                if (is_end_of_file()) {
                    end = true;
                } else if (is_comment() || is_empty_line()) {
                    ignore_line();
                    line_nr++;
                } else {
                    state = READ_COMMAND;
                }
                break;
            case READ_COMMAND:
                command = read_command(&correct_input, game_started);
                state = correct_input == true ? ARGUMENTS : INCORRECT_INPUT;
                break;
            case ARGUMENTS:
                input_arguments = read_arguments(&correct_input, command);
                if (correct_input) {
                    if (game_started)
                        state = RUN_COMMAND;
                    else
                        state = NEW_GAME;
                } else {
                    state = INCORRECT_INPUT;
                }
                break;
            case NEW_GAME:
                args = input_arguments.arg_array;
                g = gamma_new(args[0], args[1], args[2], args[3]);
                if (g != NULL) {
                    if (command == GAMMA_INTERACTIVE) {
                        int exit_code = start_interactive_mode(g);
                        if(exit_code == 0){
                            end = true;
                        } else {
                            printf("Nie można stworzyć gry, terminal jest za mały.\n");
                            line_nr++;
                            state = START;
                            gamma_delete(g);
                            g = NULL;
                        }
                    } else {
                        game_started = true;

                        printf("OK %d\n", line_nr);
                        line_nr++;
                        state = START;
                    }
                } else {
                    ungetc('\n', stdin);
                    state = INCORRECT_INPUT;
                }
                break;
            case RUN_COMMAND:
                run(command, input_arguments, g);
                line_nr++;
                state = START;
                break;
            case INCORRECT_INPUT:
                ignore_line();
                fprintf(stderr, "ERROR %d\n", line_nr);
                state = START;
                line_nr++;
                break;
        }
    }
    gamma_delete(g);
}