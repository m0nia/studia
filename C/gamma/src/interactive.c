#include <stdio.h>
#include "gamma.h"
#include "interactive.h"
#include <inttypes.h>

#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/ioctl.h>

struct termios orig_termios;

enum interactive_command {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_RIGHT,
    MOVE_LEFT,
    GAMMA_MOVE,
    GAMMA_GOLDEN_MOVE,
    SKIP,
    END_GAME,
    OTHER = -1
};

#define EOT 4

#define POSITION_CURSOR(x, y) printf("\033[%d;%dH", x, y)

#define CURSOR_BACKWARD(x) printf("\033[%dD", x)

#define CLEAR_LINE printf("\033[2K")

#define CLEAR_SCREEN printf("\033c")

#define SET_COLOR_BLUE printf("\033[34m")

#define SET_COLOR_RED printf("\033[1;31m")

#define SET_COLOR_GREEN printf("\033[1;32m")

#define SET_COLOR_DEFAULT printf("\033[0m")

#define SET_REVERSED printf("\033[47m")

#define HIDE_CURSOR printf("\033[?25l")

#define SHOW_CURSOR printf("\033[?25h")

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

bool is_terminal_big_enough(uint32_t width, uint32_t height) {
    struct winsize window_size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size);
    if (window_size.ws_col < width || window_size.ws_row < height)
        return false;
    return true;

}

uint32_t next_index(uint32_t n, uint32_t current_index) {
    if (current_index + 1 == n + 1)
        return 1;
    return current_index + 1;
}

uint32_t next_player(gamma_t *g, uint32_t current_player) {
    uint32_t next_player = next_index(g->nr_players, current_player);
    while (next_player != current_player) {
        if (gamma_free_fields(g, next_player) > 0 || gamma_golden_possible(g, next_player))
            return next_player;
        next_player = next_index(g->nr_players, next_player);
    }
    if (gamma_free_fields(g, current_player) > 0 || gamma_golden_possible(g, current_player))
        return current_player;
    return 0;
}

void move_up(uint32_t *cursor_position_row, uint32_t how_many) {
    if (*cursor_position_row >= 1 + how_many)
        *cursor_position_row -= how_many;
}

void move_left(uint32_t *cursor_position_column, uint32_t how_many) {
    if (*cursor_position_column >= 1 + how_many)
        *cursor_position_column -= how_many;
}

void move_right(uint32_t *cursor_position_column, uint32_t width, uint32_t how_many) {
    if (*cursor_position_column + how_many <= width)
        *cursor_position_column += how_many;
}

void move_down(uint32_t *cursor_position_row, uint32_t height, uint32_t how_many) {
    if (*cursor_position_row + how_many <= height)
        *cursor_position_row += how_many;
}

void display_player_info(gamma_t *g, uint32_t current_player, uint32_t how_many_rows) {
    POSITION_CURSOR(how_many_rows + 1, 1);
    CLEAR_LINE;

    SET_COLOR_BLUE;
    printf("PLAYER %"PRIu32"\n", current_player);
    SET_COLOR_DEFAULT;
    CLEAR_LINE;
    printf("BUSY FIELDS: %"PRIu64"\n", gamma_busy_fields(g, current_player));
    CLEAR_LINE;
    printf("FREE FIELDS: %"PRIu64"\n", gamma_free_fields(g, current_player));
    CLEAR_LINE;
    printf("AREAS %"PRIu32"/%"PRIu32"\n", g->players[current_player].occupied_areas, g->max_areas);
    CLEAR_LINE;
    if (gamma_golden_possible(g, current_player)) {
        SET_COLOR_GREEN;
        printf("GAMMA GOLDEN MOVE POSSIBLE :)");

    } else {
        SET_COLOR_RED;
        printf("GAMMA GOLDEN MOVE NOT POSSIBLE :(");
    }
    SET_COLOR_DEFAULT;

    HIDE_CURSOR;
}

int read_interactive_command() {
    int c = getchar();
    enum interactive_command command = OTHER;

    if (c == '\x1b') {
        c = getchar();
        if (c == '[') {
            c = getchar();
            switch (c) {
                case 'A':
                    command = MOVE_UP;
                    break;
                case 'B':
                    command = MOVE_DOWN;
                    break;
                case 'C':
                    command = MOVE_RIGHT;
                    break;
                case 'D':
                    command = MOVE_LEFT;
                    break;
                default:
                    ungetc(c, stdin);
                    command = OTHER;
                    break;
            }
        }
    } else if (c == ' ') {
        command = GAMMA_MOVE;
    } else if (c == 'g' || c == 'G')
        command = GAMMA_GOLDEN_MOVE;
    else if (c == 'c' || c == 'C') {
        command = SKIP;
    } else if (c == EOT) {
        command = END_GAME;
    }
    return command;
}

uint32_t convert_to_number(char *char_array, uint64_t starting_index) {
    uint64_t j = starting_index;
    uint32_t result_number = 0;
    while (char_array[j] >= '0' && char_array[j] <= '9') {
        result_number *= 10;
        result_number += char_array[j] - '0';
        j++;
    }
    return result_number;
}


void display_small_board(gamma_t *g, uint32_t current_player) {
    char *board = gamma_board(g);
    uint32_t i = 0;

    POSITION_CURSOR(1, 1);
    CLEAR_SCREEN;

    while (board[i] != '\0') {
        uint32_t player = board[i] - '0';
        if (player == current_player) {
            SET_COLOR_BLUE;
        }
        printf("%c", board[i]);

        SET_COLOR_DEFAULT;
        i++;
    }

    free(board);
}

void
display_big_board(gamma_t *g, uint32_t current_player, uint32_t cursor_row, uint32_t cursor_column,
                  uint32_t cursor_size) {
    char *board = gamma_board(g);
    uint64_t i = 0;
    POSITION_CURSOR(1, 1);
    CLEAR_SCREEN;
    uint32_t current_row = 1;
    uint32_t current_column = 1;
    uint32_t cursor_start = 0;
    bool printing_cursor = false;

    while (board[i] != '\0') {
        if (current_row == cursor_row && current_column == cursor_column) {
            SET_REVERSED;
            cursor_start = 0;
            printing_cursor = true;
        }

        if (board[i] >= '0' && board[i] <= '9') {
            uint32_t player = convert_to_number(board, i);
            if (player == current_player) {
                SET_COLOR_BLUE;
            }
            while (board[i] >= '0' && board[i] <= '9') {
                printf("%c", board[i++]);
                current_column++;
                if (printing_cursor)
                    cursor_start++;
            }
            if (!printing_cursor)
                SET_COLOR_DEFAULT;
        } else {
            if (board[i] == '\n') {
                current_row += 1;
                current_column = 1;

            } else {
                current_column++;
            }
            printf("%c", board[i]);
            i++;
            if (printing_cursor)
                cursor_start++;
        }

        if (cursor_start == cursor_size) {
            SET_COLOR_DEFAULT;
            printing_cursor = false;
        }
    }

    free(board);
}

void display_summary(gamma_t *g, uint32_t rows_number) {

    if (g->nr_players >= 10)
        display_big_board(g, 0, 0, 0, 0);
    else
        display_small_board(g, 0);
    uint64_t max = 0;
    uint32_t best_player = 1;
    bool draw = false;

    POSITION_CURSOR(rows_number + 1, 1);
    for (uint32_t i = 1; i <= g->nr_players; i++) {
        uint64_t occupied_fields = gamma_busy_fields(g, i);
        printf("PLAYER %"PRIu32": %"PRIu64" occupied fields\n", i, occupied_fields);
        if (occupied_fields == max) {
            draw = true;
        } else if (occupied_fields > max) {
            best_player = i;
            max = occupied_fields;
            draw = false;
        }
    }
    HIDE_CURSOR;
    if (draw == false) {
        int j = 0;
        SET_COLOR_RED;
        printf("PLAYER %"PRIu32" WON !!\n", best_player);
        while (j < 2) {
            SET_COLOR_RED;
            CURSOR_BACKWARD(16);
            CLEAR_LINE;

            printf("CONGRATULATIONS!");
            fflush(stdout);
            sleep(1);

            SET_COLOR_BLUE;
            CURSOR_BACKWARD(16);
            CLEAR_LINE;

            printf("CONGRATULATIONS!");
            fflush(stdout);
            sleep(1);

            SET_COLOR_GREEN;
            CURSOR_BACKWARD(16);
            CLEAR_LINE;

            printf("CONGRATULATIONS!");
            fflush(stdout);
            sleep(1);

            j++;
        }
        printf("\n");
    } else {
        printf("Game ended with draw\n");
    }
    SET_COLOR_DEFAULT;
    SHOW_CURSOR;
}

int start_interactive_mode(gamma_t *g) {

    char *board = gamma_board(g);
    if (board == NULL)
        return 1;

    uint64_t i = 0;
    uint32_t new_line_count = 0;
    uint32_t columns_count = 0;
    while (board[i] != '\0') {
        if (board[i] == '\n') {
            new_line_count++;
            if (columns_count == 0)
                columns_count = i;
        }
        i++;
    }
    if (!is_terminal_big_enough(columns_count, new_line_count)) {
        free(board);
        return 1;
    }

    uint32_t cell_width = columns_count / g->width;
    uint32_t cell_height = g->nr_players >= 10 ? 2 : 1;

    uint32_t cursor_position_column = 1 + ((g->width - 1) / 2) * cell_width + 1;
    if (columns_count == 1) cursor_position_column = 1;
    uint32_t cursor_position_row = ((g->height - 1) / 2) * cell_height + 1;

    enableRawMode();

    bool end = false;
    uint32_t current_player = 1;
    enum interactive_command command;
    bool player_moved;
    uint32_t board_column_index, board_row_index;

    printf("\033[=3h");
    while (!end) {
        if (g->nr_players >= 10) {
            display_big_board(g, current_player, cursor_position_row, cursor_position_column, cell_width - 1);
            display_player_info(g, current_player, new_line_count);

        } else {
            display_small_board(g, current_player);
            display_player_info(g, current_player, new_line_count);
            SHOW_CURSOR;
            POSITION_CURSOR(cursor_position_row, cursor_position_column);
        }

        board_column_index = (cursor_position_column - 1) / cell_width;
        board_row_index = (cell_height * g->height - cursor_position_row) / cell_height;

        command = read_interactive_command();
        player_moved = false;

        switch (command) {
            case MOVE_UP:
                move_up(&cursor_position_row, cell_height);
                break;
            case MOVE_DOWN:
                move_down(&cursor_position_row, cell_height * g->height, cell_height);
                break;
            case MOVE_RIGHT:
                move_right(&cursor_position_column, cell_width * g->width, cell_width);
                break;
            case MOVE_LEFT:
                move_left(&cursor_position_column, cell_width);
                break;
            case GAMMA_MOVE:
                player_moved = gamma_move(g, current_player, board_column_index, board_row_index);
                break;
            case GAMMA_GOLDEN_MOVE:
                player_moved = gamma_golden_move(g, current_player, board_column_index, board_row_index);
                break;
            case SKIP:
                current_player = next_player(g, current_player);
                break;
            case END_GAME:
                end = true;
                break;
            default:
                break;
        }
        if (player_moved) {
            current_player = next_player(g, current_player);
            if (current_player == 0)
                end = true;
        }
    }

    display_summary(g, new_line_count);
    disableRawMode();
    return 0;

}