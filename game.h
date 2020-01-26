#ifndef GAME_H_
#define GAME_H_

// Screen dimensions
#define SCREEN_WIDTH 640.0
#define SCREEN_HEIGHT 480.0
#define CELL_WIDTH (SCREEN_WIDTH / N)
#define CELL_HEIGHT (SCREEN_HEIGHT / N)

#define N 3

#define EMPTY 0
#define PLAYER_X 1
#define PLAYER_O 2

// Answer selction possiblities
#define NO_SELECTION 0
#define A_SELECTED 1
#define B_SELECTED 2
#define C_SELECTED 3
#define D_SELECTED 4
#define A_CONFIRMED 5
#define B_CONFIRMED 6
#define C_CONFIRMED 7
#define D_CONFIRMED 8

// Gamestate
#define RUNNING_STATE 0
#define PLAYER_X_WON_STATE 1
#define PLAYER_O_WON_STATE 2
#define TIE_STATE 3
#define QUIT_STATE 4

// Menu states
#define RUNNING 0
#define QUIT 1

// Menus
#define INIT_MENU 0
#define USER_MENU 1

typedef struct {
    char id[20];
    char password[20];
} user_t;

typedef struct _n{
    user_t* user;
    struct _n* next;
} node;

typedef struct {
    int top_score;
    char top_score_holder[20];
    int number_of_users;
} stats;

typedef struct {
    char* m_ans_a;
    char* m_ans_b;
    char* m_ans_c;
    char* m_ans_d;
} m_question;

typedef struct {
    m_question question[4];
    int state;
    int selection;
    int type;
    char user_id[20];
} menu_t;

typedef struct {
    int board[N * N];
    int player;
    int state;
} game_t;

#endif  // GAME_H_
