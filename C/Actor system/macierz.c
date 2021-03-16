#include "cacti.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>


typedef struct supervisor_state {
    actor_id_t* actors; // pod i-tym indeksem znajduje się id aktora liczącego i-tą kolumnę
    int actors_nr; // liczba dotychczas stworzonych aktorów
    int ready_actors; // ile aktorów jest już gotowych do liczenia sum
    int summed_rows; // ile zostało zsumowanych wierszy dotychczas
    int* row_sum; // pod i tym indeksem znajduje się dotychczas policzona suma w i-tym wierszu

} supervisor_state_t ;

typedef struct data {
    int row_nr;
    int sum;
    int columns_left;
} data_t;

typedef struct actor_state {
    actor_id_t id;
    actor_id_t* right;
    actor_id_t father;
    int column_nr;
} actor_state_t ;

void msg_row_sum(void** stateptr, size_t nbytes, void* data);
void msg_actor_ready(void** stateptr, size_t nbytes, void* data);
void msg_compute_row(void** stateptr, size_t nbytes, void* data);
void msg_child_id(void** stateptr, size_t nbytes, void* data);
void msg_child_state(void** stateptr, size_t nbytes, void* data);
void msg_hello(void** stateptr, size_t nbytes, void* data);
void msg_hello_supervisor(void** stateptr, size_t nbytes, void* data);
void msg_end_supervisor(void** stateptr, size_t nbytes, void* data);
void msg_end_column_actor(void** stateptr, size_t nbytes, void* data);

/* Komunikaty do głównego aktora: */
static const int CHILD_ID = 1;
static const int ACTOR_READY = 2;
static const int ROW_SUM = 3;
static const int END_SUPERVISOR = 4;
static const int SUPERVISOR_ROLES_NR = 5;

/* Komunikaty do aktorów kolumnowych: */
static const int CHILD_STATE = 1;
static const int COMPUTE_SUM = 2;
static const int END_COLUMN_ACTOR = 3;
static const int COLUMN_ACTOR_ROLES_NR = 4;

int** matrix;
int** time;
int rows, columns;
static const act_t column_actor_fun_arr[] = {msg_hello, msg_child_state, msg_compute_row, msg_end_column_actor};
static role_t column_actor_role = {.nprompts = COLUMN_ACTOR_ROLES_NR, .prompts = column_actor_fun_arr};

void msg_end_column_actor(void** stateptr, size_t nbytes, void* data){
	(void)nbytes;
	(void)data;
    free(*stateptr);
    send_message(actor_id_self(), (message_t){
            .message_type = MSG_GODIE,
            .nbytes = 0,
            .data = NULL
    });
}

void msg_end_supervisor(void** stateptr, size_t nbytes, void* data){
	(void)nbytes;
	(void)data;
    supervisor_state_t* state = *stateptr;
    for(int i = 0; i < rows; i++)
        printf("%d\n", state->row_sum[i]);

    for(int i = 0; i < state->actors_nr; i++){
        send_message(state->actors[i], (message_t){
            .message_type = END_COLUMN_ACTOR,
            .data = NULL,
            .nbytes = 0
        });
    }
    free(state->actors);
    free(state->row_sum);
    free(*stateptr);
    send_message(actor_id_self(), (message_t){
        .message_type = MSG_GODIE,
        .nbytes = 0,
        .data = NULL
    });
}

// Obsługa komunikatu ROW_SUM wysyłanego do głównego aktora - w data dostaje nr wiersza i sumę w tym wierszu.
// Aktor dodaje tą sumę do tablicy wyliczonych sum i jeśli wszystko jest już policzone, wypisuje
// sumę wierszy.
void msg_row_sum(void** stateptr, size_t nbytes, void* data){
	(void)nbytes;
    data_t* row_data = data;
    supervisor_state_t* state = *stateptr;
    state->row_sum[row_data->row_nr] = row_data->sum;
    state->summed_rows++;
    free(data);
    if(state->summed_rows == rows){
        send_message(actor_id_self(), (message_t){
            .message_type = END_SUPERVISOR,
            .nbytes = 0,
            .data = NULL
        });
    }
}

// Obsługa komunikatu COMPUTE_SUM wysyłanego do aktora kolumnowego.
// Aktor dostaje w data nr wiersza i dotychczas policzoną sumę. Liczy swoją wartość, dodaje ją do
// dotychczasowej sumy i jeśli jest już policzony cały wiersz, przekazuje policzoną sumą do głównego
// aktora, wpp. przekazuje dane do aktora liczącego kolejną kolumnę
void msg_compute_row(void** stateptr, size_t nbytes, void* data){
	(void)nbytes;
    data_t* row_data = (data_t*)data;
    actor_state_t* state = *stateptr;
    usleep(1000* time[row_data->row_nr][state->column_nr]);
    row_data->sum += matrix[row_data->row_nr][state->column_nr];
    row_data->columns_left -= 1;
    if(row_data->columns_left == 0){
        send_message(state->father, (message_t){
            .message_type = ROW_SUM,
            .data = row_data,
            .nbytes = sizeof(*row_data)
        });
    } else {
        send_message(*state->right, (message_t){
            .message_type = COMPUTE_SUM,
            .data = row_data,
            .nbytes = sizeof(*row_data)
        });
    }
}

// Obsługa komunikatu ACTOR_READY wysyłanego do głównego aktora.
// Jest to komunikat pusty, służący tylko do przekazania informacji głównemu aktorowi, że dziecko
// odebrało już komunikat o numerze kolumny która ma liczyć. Jeśli wszyscy aktorzy kolumnowi już
// są gotowi, główny aktor wysyła im komunikaty o liczeniu wierszy.
void msg_actor_ready(void** stateptr, size_t nbytes, void* data){
	(void)nbytes;
	(void)data;
    supervisor_state_t* state = *stateptr;
    state->ready_actors++;
    if(state->ready_actors == columns){
        int k = 0;
        for(int i = 0; i < rows; i++) {
            data_t* row_info = malloc(sizeof(*row_info));
            row_info->row_nr = i;
            row_info->sum = 0;
            row_info->columns_left = columns;
            send_message(state->actors[k], (message_t) {
                .message_type = COMPUTE_SUM,
                .data = row_info,
                .nbytes = sizeof(*row_info)
            });
            k = (k + 1) % columns;
        }
    }
}

// Obsługa komunikatu CHILD_ID wysyłanego do głównego aktora. Dostaje on w data id aktora, którego
// stworzył. Zapisuje to id w tablicy aktorów i wysyła do dziecka komunikat z wartością stanu dziecka,
// m.in. numerem kolumny jaką ma liczyć i id aktora, do którego ma wysyłać policzoną sumę.
void msg_child_id(void** stateptr, size_t nbytes, void* data){
	(void)nbytes;
    supervisor_state_t* state = *stateptr;
    state->actors_nr++;
    int column_nr = columns - state->actors_nr; // uzupelnia tablice aktorow od konca
    state->actors[column_nr] = *(actor_id_t*)data;
    free(data);
    actor_id_t* right = column_nr == columns-1 ? &state->actors[0] : &state->actors[column_nr+1];

    actor_state_t* child = malloc(sizeof(*child));
    child->id = state->actors[column_nr];
    child->father = actor_id_self();
    child->column_nr = column_nr;
    child->right = right;
    send_message(state->actors[column_nr], (message_t){
        .message_type = CHILD_STATE,
        .nbytes = sizeof(*child),
        .data = child
    });
    if(state->actors_nr < columns){
        send_message(actor_id_self(), (message_t){
                .message_type = MSG_SPAWN,
                .nbytes = sizeof(role_t),
                .data = &column_actor_role
        });
    }
}

// dziecko dostaje tą wiadomość - w data ma numer kolumny ktora bedzie liczyc, aktora do ktorego bedzie wysylac sume
// i liczbę kolumn do policzenia - zapisuje sobie to info w state_ptr
void msg_child_state(void** stateptr, size_t nbytes, void* data){
	(void)nbytes;
    actor_state_t* state = data;
    *stateptr = state;
    send_message(state->father, (message_t){
        .message_type = ACTOR_READY,
        .data = NULL,
        .nbytes = 0
    });
}


void msg_hello(void** stateptr, size_t nbytes, void* data){
	(void)nbytes;
	(void)stateptr;
    actor_id_t* my_id = malloc(sizeof(actor_id_t));
    *my_id = actor_id_self();
    message_t child_id;
    child_id.message_type = CHILD_ID;
    child_id.nbytes = sizeof(*my_id);
    child_id.data = my_id;
    assert(send_message((actor_id_t)data, child_id) == 0);
}


void msg_hello_supervisor(void** stateptr, size_t nbytes, void* data){
	(void)nbytes;
	(void)data;
    supervisor_state_t* state = malloc(sizeof(supervisor_state_t));
    state->summed_rows = 0;
    state->ready_actors = 0;
    state->actors_nr = 0;
    state->actors = malloc(sizeof(actor_id_t) * columns);
    state->row_sum = malloc(sizeof(int)* rows);
    *stateptr = state;
        send_message(actor_id_self(), (message_t){
                .message_type = MSG_SPAWN,
                .nbytes = sizeof(role_t),
                .data = &column_actor_role
        });
}

/*
 * Pierwszy aktor (supervisor) tworzy columns aktorów kolumnowych. Każdy z tych aktorów odsyła supervisorowi
 * swoje id. Supervisor to id zapisuje i wysyła dziecku dodatkowe informacje, w tym numer kolumny którą ma liczyć.
 * Dziecko jak dostanie informacje, wysyła rodzicowi info, że jest gotowe.
 * Rodzic zlicza ile jest gotowych aktorów i jak wszyscy już będą gotowi, to w pętli
 * wysyła do 1 licz wiersz 1, wysyla do 2 licz wiersz 2 itd.
 *
 * Każdy z kolejnych aktorów kolumnowych liczy swoją wartość i przekazuje dalej, chyba że już
 * wszystkie kolumny w danym wierszu są policzone - wtedy wysyła dane (nr wiersza, suma) do rodzica.
 *
 * Rodzic zbiera te sumy i zapisuje sobie w tablicy, a jak już wszystkie wiersze będą policzone to wypisuje
 * sumy wierszy i wysyła do stworzonych aktorów MSG_GODIE.
 */

int main(){
    scanf("%d", &rows); // liczba wierszy
    scanf("%d", &columns); // liczba kolumn
    matrix = malloc(sizeof(int*) * rows);
    for(int i = 0; i < rows; i++)
        matrix[i] = malloc(sizeof(int) * columns);

    time = malloc(sizeof(int*) * rows);
    for(int i = 0; i < rows; i++)
        time[i] = malloc(sizeof(int) * columns);

    for(int i = 0; i < rows * columns; i++){
        int row = i / columns;
        int column = i % columns;
        scanf("%d", &matrix[row][column]);
        scanf("%d", &time[row][column]);
    }

    actor_id_t supervisor;
    act_t supervisor_fun_arr[] = {msg_hello_supervisor, msg_child_id, msg_actor_ready, msg_row_sum, msg_end_supervisor};
    role_t supervisor_role = (role_t){
        .nprompts = SUPERVISOR_ROLES_NR,
        .prompts = supervisor_fun_arr
    };

    int err;
    if((err = actor_system_create(&supervisor, &supervisor_role)) != 0) {
        fprintf(stderr, "actor system create failed, return code: %d\n", err);
        return 1;
    }
    actor_system_join(supervisor);

    for(int i = 0; i < rows; i++) {
        free(matrix[i]);
        free(time[i]);
    }

    free(matrix);
    free(time);
    return 0;

}
