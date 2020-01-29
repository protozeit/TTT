#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include "game.h"
#include "logic.h"
#include "menu-items.h"
#include "rendering.h"

int box_a_x = 65;
int box_a_y = 325;
int box_b_x = 350;
int box_b_y = 325;
int box_c_x = 65;
int box_c_y = 395;
int box_d_x = 350;
int box_d_y = 395;
int box_w = 215;
int box_h = 55;

// Returns a pointer to stats fetched from the records file
stats* fetch_stats(){
    stats* global_stats = malloc(sizeof(stats));
    FILE* stat_file = fopen("records/stats", "r");
    char top_score[20];
    char top_score_holder[20];
    char number_of_users[20];
    
    fscanf(stat_file,"%s",top_score);
    fscanf(stat_file,"%s",top_score_holder);
    fscanf(stat_file,"%s",number_of_users);

    global_stats->top_score = atoi(top_score);
    strcpy(global_stats->top_score_holder, top_score_holder);
    global_stats->number_of_users = atoi(number_of_users);

    fclose(stat_file);
    return global_stats;
}

game_t* game_init()
{
    game_t* game = malloc(sizeof(game_t));

    game->player = PLAYER_X;
    game->state = RUNNING_STATE;
    for (int i = 0; i < N * N; ++i) {
        game->board[i] = EMPTY;
    }
    return game;
}

void switch_player(game_t *game)
{
    if (game->player == PLAYER_X) {
        game->player = PLAYER_O;
    } else if (game->player == PLAYER_O) {
        game->player = PLAYER_X;
    }
}

int check_player_won(game_t *game, int player)
{
    int row_count = 0;
    int column_count = 0;
    int diag1_count = 0;
    int diag2_count = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (game->board[i * N + j] == player) {
                row_count++;
            }

            if (game->board[j * N + i] == player) {
                column_count++;
            }
        }

        if (row_count >= N || column_count >= N) {
            return 1;
        }

        row_count = 0;
        column_count = 0;

        if (game->board[i * N + i] == player) {
            diag1_count++;
        }

        if (game->board[i * N + N - i - 1] == player) {
            diag2_count++;
        }
    }

    return diag1_count >= N || diag2_count >= N;
}

int count_cells(const int *board, int cell)
{
    int count = 0;

    for (int i = 0; i < N * N; ++i) {
        if (board[i] == cell) {
            count++;
        }
    }

    return count;
}

void game_over_condition(game_t *game)
{
    if (check_player_won(game, PLAYER_X)) {
        game->state = PLAYER_X_WON_STATE;
    } else if (check_player_won(game, PLAYER_O)) {
        game->state = PLAYER_O_WON_STATE;
    } else if (count_cells(game->board, EMPTY) == 0) {
        game->state = TIE_STATE;
    }
}

void player_turn(game_t *game, int row, int column)
{
    if (game->board[row * N + column] == EMPTY) {
        game->board[row * N + column] = game->player;
        switch_player(game);
        game_over_condition(game);
    }
}

void reset_game(game_t *game)
{
    game->player = PLAYER_X;
    game->state = RUNNING_STATE;
    for (int i = 0; i < N * N; ++i) {
        game->board[i] = EMPTY;
    }
}

void click_on_cell(game_t *game, int row, int column, menu_t* menu)
{
    if (game->state == RUNNING_STATE) {
        player_turn(game, row, column);
    } else if (game->state == QUIT_STATE) {
        menu->state = RUNNING;
        menu->type = INIT_MENU;
        menu->selection = NO_SELECTION;      
    } else {
        reset_game(game);
    }
}
// Menu logic

int m_box_a_x = 65;
int m_box_a_y = 295;
int m_box_b_x = 350;
int m_box_b_y = 295;
int m_box_c_x = 65;
int m_box_c_y = 365;
int m_box_d_x = 350;
int m_box_d_y = 365;
int m_box_w = 215;
int m_box_h = 55;

// Mouse hover detection
void menu_hover_select(menu_t* menu, int x, int y){
    menu->selection = NO_SELECTION;        
    if (x > m_box_a_x && x < m_box_a_x + m_box_w && y > m_box_a_y && y < m_box_a_y + m_box_h)
        menu->selection = A_SELECTED;
    if (x > m_box_b_x && x < m_box_b_x + m_box_w && y > m_box_b_y && y < m_box_b_y + m_box_h)
        menu->selection = B_SELECTED;
    if (x > m_box_c_x && x < m_box_c_x + m_box_w && y > m_box_c_y && y < m_box_c_y + m_box_h)
        menu->selection = C_SELECTED;
    if (x > m_box_d_x && x < m_box_d_x + m_box_w && y > m_box_d_y && y < m_box_d_y + m_box_h)
        menu->selection = D_SELECTED;
}

// Main Menu
void check_menu_selection(SDL_Renderer* renderer,game_t* game, menu_t* menu){
    // Resetting the game without the questions before the start button is pressed so it's responsive
    
    stats* global_stats = fetch_stats();

    // Variables for text input
    char* id = calloc(1,21); 
    char* passwd = calloc(1,21);
    char* confirm = calloc(1,21);
    char* text = calloc(1,151);

    switch (menu->type){  
        case INIT_MENU:
        switch (menu->selection){
            case A_CONFIRMED: // Login
                username_input(renderer,menu,id,passwd);
                break;
            case B_CONFIRMED: // Signup
                signup_input(renderer,menu,id,passwd);
                break;
            case C_CONFIRMED: // Show instructions
                SDL_RenderClear(renderer);
                render_instructions(renderer);
                SDL_RenderPresent(renderer);
                SDL_Event o;
                do{
                    SDL_WaitEvent(&o);
                } while(o.type != SDL_KEYDOWN && o.type != SDL_MOUSEBUTTONDOWN);
                menu->state = RUNNING;
                menu->selection = NO_SELECTION;
                break;
            case D_CONFIRMED: // Show stats
                SDL_RenderClear(renderer);
                stats* global_stats = fetch_stats();
                render_stats(renderer,global_stats);
                SDL_RenderPresent(renderer);
                SDL_Event e;
                do{
                    SDL_WaitEvent(&e);
                } while(e.type != SDL_KEYDOWN && e.type != SDL_MOUSEBUTTONDOWN);
                menu->state = RUNNING;
                menu->selection = NO_SELECTION;
                break;
            }
            break;

        case USER_MENU:
        switch (menu->selection){
            case A_CONFIRMED: // Start game
                game = game_init();
                game_loop(renderer, game, menu);
                //game = game_init(menu);
                //record_stats(game);
                break;
            case B_CONFIRMED: // Logout
                menu->user_id[0] = 0;
                menu->type = INIT_MENU;
                menu->selection = NO_SELECTION;
                break;
            case C_CONFIRMED: // Show instructions
                SDL_RenderClear(renderer);
                render_instructions(renderer);
                SDL_RenderPresent(renderer);
                SDL_Event o;
                do{
                    SDL_WaitEvent(&o);
                } while(o.type != SDL_KEYDOWN && o.type != SDL_MOUSEBUTTONDOWN);
                menu->state = RUNNING;
                menu->selection = NO_SELECTION;
                break;
            case D_CONFIRMED: // Show stats
                SDL_RenderClear(renderer);
                stats* global_stats = fetch_stats();
                render_stats(renderer,global_stats);
                SDL_RenderPresent(renderer);
                SDL_Event e;
                do{
                    SDL_WaitEvent(&e);
                } while(e.type != SDL_KEYDOWN && e.type != SDL_MOUSEBUTTONDOWN);
                menu->state = RUNNING;
                menu->selection = NO_SELECTION;
                break;
                break;
            }

            break;

        default : {}
    }
}
