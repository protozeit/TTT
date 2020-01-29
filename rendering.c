#include <stdio.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "game.h"
#include "rendering.h"
#include "logic.h"

const SDL_Color GRID_COLOR = { .r = 255, .g = 255, .b = 255 };
const SDL_Color PLAYER_X_COLOR = { .r = 255, .g = 50, .b = 50 };
const SDL_Color PLAYER_O_COLOR = { .r = 50, .g = 100, .b = 255 };
const SDL_Color TIE_COLOR = { .r = 100, .g = 100, .b = 100 };
const SDL_Rect rect_a = { .x = 0, .y = 300, .w = 320, .h = 90};
const SDL_Rect rect_b = { .x = 320, .y = 300, .w = 320, .h = 90};
const SDL_Rect rect_c = { .x = 0, .y = 390, .w = 320, .h = 90};
const SDL_Rect rect_d = { .x = 320, .y = 390, .w = 320, .h = 90};

int lifeline_x = 267;
int lifeline_y = 5;
int lifeline_w = 35;
int question_x = 100;
int question_y = 80;
int ans_a_x = 65;
int ans_a_y = 337;
int ans_b_x = 347;
int ans_b_y = 337;
int ans_c_x = 65;
int ans_c_y = 406;
int ans_d_x = 347;
int ans_d_y = 406;
int timer_x = 303;
int timer_y = 290;

const SDL_Color green = { .r = 34, .g = 170, .b = 34};
const SDL_Color red = { .r = 230, .g = 0, .b = 0};
const SDL_Color yellow = { .r = 250, .g = 154, .b = 20};
const SDL_Color grey = { .r = 100, .g = 100, .b = 100};
const SDL_Color white = { .r = 255, .g = 255, .b = 255};
const SDL_Color orange = { .r = 255, .g = 72, .b = 0};


SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path){
    //The final texture
    SDL_Texture* new_texture = NULL;

    //Load image at specified path
    SDL_Surface* loaded_surface = IMG_Load(path);
    if(loaded_surface == NULL){
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    } else {
        //Create texture from surface pixels
        new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
        if(new_texture == NULL){
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loaded_surface);
    }

    return new_texture;
}

void render_screen(SDL_Renderer* renderer, const char* path){
    // simply show the chosen end screen
	SDL_Texture* texture = load_texture(renderer, path);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_DestroyTexture(texture);

}

void render_rect(SDL_Renderer* renderer, const SDL_Rect* rect, const SDL_Color* color){
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);
    SDL_RenderFillRect(renderer, rect);
}

void render_question(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name){
    TTF_Init();
    TTF_Font * font = TTF_OpenFont(font_name, 25);
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    Uint32 wrap = 445;

    SDL_Surface * surface = TTF_RenderText_Blended_Wrapped(font, text, white, wrap);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
    TTF_Quit();
}

void render_answer(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name, const SDL_Color color){
    TTF_Init();
    TTF_Font * font;
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    if(strlen(text) < 16){
        font = TTF_OpenFont(font_name, 25);
    }
    else if (strlen(text) < 22){
        font = TTF_OpenFont(font_name, 20);
        dest.y += 4;
    }
    else if (strlen(text) < 26){
        font = TTF_OpenFont(font_name, 17);
        dest.y += 4;
    }
    else{
        font = TTF_OpenFont(font_name, 15);
        dest.y += 4;
    }

    SDL_Surface * surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
    TTF_Quit();
}

void render_money(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name){
    TTF_Init();
    TTF_Font * font = TTF_OpenFont(font_name, 17);

    
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    SDL_Surface * surface = TTF_RenderText_Blended(font, text, white);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
    TTF_Quit();
}

void render_25(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name){
    TTF_Init();
    TTF_Font * font = TTF_OpenFont(font_name, 25);

    
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    SDL_Surface * surface = TTF_RenderText_Blended(font, text, white);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
    TTF_Quit();
}

void render_20(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name){
    TTF_Init();
    TTF_Font * font = TTF_OpenFont(font_name, 25);

    
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    SDL_Surface * surface = TTF_RenderText_Blended(font, text, white);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
    TTF_Quit();
}


void render_25_centered(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name){
    TTF_Init();
    TTF_Font * font = TTF_OpenFont(font_name, 25);

    SDL_Surface * surface = TTF_RenderText_Blended(font, text, white);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);    
    SDL_Rect dest;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    dest.x = x - dest.w/2;
    dest.y = y;

    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
    TTF_Quit();
}

void render_grid(SDL_Renderer *renderer, const SDL_Color *color)
{
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

    for (int i = 1; i < N; ++i) {
        SDL_RenderDrawLine(renderer,
                           i * CELL_WIDTH, 0,
                           i * CELL_WIDTH, SCREEN_HEIGHT);
        SDL_RenderDrawLine(renderer,
                           0,            i * CELL_HEIGHT,
                           SCREEN_WIDTH, i * CELL_HEIGHT);
    }
}

void render_x(SDL_Renderer *renderer,
              int row, int column,
              const SDL_Color *color)
{
    const float half_box_side = fmin(CELL_WIDTH, CELL_HEIGHT) * 0.25;
    const float center_x = CELL_WIDTH * 0.5 + column * CELL_WIDTH;
    const float center_y = CELL_HEIGHT * 0.5 + row * CELL_HEIGHT;

    thickLineRGBA(renderer,
                  center_x - half_box_side,
                  center_y - half_box_side,
                  center_x + half_box_side,
                  center_y + half_box_side,
                  10,
                  color->r,
                  color->g,
                  color->b,
                  255);
    thickLineRGBA(renderer,
                  center_x + half_box_side,
                  center_y - half_box_side,
                  center_x - half_box_side,
                  center_y + half_box_side,
                  10,
                  color->r,
                  color->g,
                  color->b,
                  255);
}

void render_o(SDL_Renderer *renderer,
              int row, int column,
              const SDL_Color *color)
{
    const float half_box_side = fmin(CELL_WIDTH, CELL_HEIGHT) * 0.25;
    const float center_x = CELL_WIDTH * 0.5 + column * CELL_WIDTH;
    const float center_y = CELL_HEIGHT * 0.5 + row * CELL_HEIGHT;

    filledCircleRGBA(renderer,
                     center_x, center_y, half_box_side + 5,
                     color->r, color->g, color->b, 255);
    filledCircleRGBA(renderer,
                     center_x, center_y, half_box_side - 5,
                     0, 0, 0, 255);
}

void render_board(SDL_Renderer *renderer,
                  const int *board,
                  const SDL_Color *player_x_color,
                  const SDL_Color *player_o_color)
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            switch (board[i * N + j]) {
            case PLAYER_X:
                render_x(renderer, i, j, player_x_color);
                break;

            case PLAYER_O:
                render_o(renderer, i, j, player_o_color);
                break;

            default: {}
            }
        }
    }
}

void render_running_state(SDL_Renderer *renderer, const game_t *game)
{
    render_grid(renderer, &GRID_COLOR);
    render_board(renderer,
                 game->board,
                 &PLAYER_X_COLOR,
                 &PLAYER_O_COLOR);
}

void render_game_over_state(SDL_Renderer *renderer,
                            const game_t *game,
                            const SDL_Color *color)
{
    render_grid(renderer, color);
    render_board(renderer,
                 game->board,
                 color,
                 color);
}

void render_game(SDL_Renderer *renderer, game_t *game)
{
    switch (game->state) {
    case RUNNING_STATE:
        render_running_state(renderer, game);
        break;

    case PLAYER_X_WON_STATE:
        render_game_over_state(renderer, game, &PLAYER_X_COLOR);
        break;

    case PLAYER_O_WON_STATE:
        render_game_over_state(renderer, game, &PLAYER_O_COLOR);
        break;

    case TIE_STATE:
        render_game_over_state(renderer, game, &TIE_COLOR);
        break;

    default: {}
    }
}

// Menu rendering

int m_ans_a_x = 65;
int m_ans_a_y = 307;
int m_ans_b_x = 347;
int m_ans_b_y = 307;
int m_ans_c_x = 65;
int m_ans_c_y = 376;
int m_ans_d_x = 347;
int m_ans_d_y = 376;

void render_opts(SDL_Renderer* renderer, menu_t* menu){
    m_question current_q = menu->question[menu->type];
    render_answer(renderer, m_ans_a_x, m_ans_a_y, current_q.m_ans_a, DEFAULT_FONT, white);
    render_answer(renderer, m_ans_b_x, m_ans_b_y, current_q.m_ans_b, DEFAULT_FONT, white);
    render_answer(renderer, m_ans_c_x, m_ans_c_y, current_q.m_ans_c, DEFAULT_FONT, white);
    render_answer(renderer, m_ans_d_x, m_ans_d_y, current_q.m_ans_d, DEFAULT_FONT, white);
}

const SDL_Rect m_rect_a = { .x = 0, .y = 270, .w = 320, .h = 90};
const SDL_Rect m_rect_b = { .x = 320, .y = 270, .w = 320, .h = 90};
const SDL_Rect m_rect_c = { .x = 0, .y = 360, .w = 320, .h = 90};
const SDL_Rect m_rect_d = { .x = 320, .y = 360, .w = 320, .h = 90};

void render_menu(SDL_Renderer* renderer, menu_t* menu){
switch(menu->selection){
        case A_SELECTED:
            render_rect(renderer, &m_rect_a, &grey);
            break;

        case B_SELECTED:
            render_rect(renderer, &m_rect_b, &grey);
            break;

        case C_SELECTED:
            render_rect(renderer, &m_rect_c, &grey);
            break;

        case D_SELECTED:
            render_rect(renderer, &m_rect_d, &grey);
            break;

        default : {}

    }
    render_screen(renderer, MENU_BG);
    render_money(renderer, 10, 5, menu->user_id, DEFAULT_FONT);
    if (menu->type == INIT_MENU)
        render_answer(renderer, 5, 453, "By Saad El Jebbari & Dalia El Aiche", DEFAULT_FONT, white);
    render_opts(renderer, menu);
}

void render_stats(SDL_Renderer* renderer, stats* global_stats){
    render_answer(renderer, 450, 455, "Press any key to go back", DEFAULT_FONT, white);
    FILE* stat_file = fopen("records/stats", "r");
    char top_score[20];
    char top_score_holder[20];
    char number_of_users[20];
    char correct_answers[20];
    char lifeline_50_uses[20];
    char lifeline_25_uses[20];
    char lifeline_switch_uses[20];
    
    fscanf(stat_file,"%s",top_score);
    fscanf(stat_file,"%s",top_score_holder);
    fscanf(stat_file,"%s",number_of_users);
    fscanf(stat_file,"%s",correct_answers);
    fscanf(stat_file,"%s",lifeline_50_uses);
    fscanf(stat_file,"%s",lifeline_25_uses);
    fscanf(stat_file,"%s",lifeline_switch_uses);
    fclose(stat_file);

    render_25(renderer, 150, 100 - 25, "Top Score : ", DEFAULT_FONT);
    render_25(renderer, 400, 100 - 25, top_score, DEFAULT_FONT);

    render_25(renderer, 150, 150 - 25, "Held By : ", DEFAULT_FONT);
    render_25(renderer, 400, 150 - 25, top_score_holder, DEFAULT_FONT);
    
    render_25(renderer, 150, 200 - 25, "Number of users : ", DEFAULT_FONT);
    render_25(renderer, 400, 200 - 25, number_of_users, DEFAULT_FONT);
    
    render_25(renderer, 150, 250 - 25, "Correct answers : ", DEFAULT_FONT);
    render_25(renderer, 400, 250 - 25, correct_answers, DEFAULT_FONT);
    
    render_25(renderer, 150, 300 - 25, "50s used : ", DEFAULT_FONT);
    render_25(renderer, 400, 300 - 25, lifeline_50_uses, DEFAULT_FONT);
    
    render_25(renderer, 150, 350 - 25, "25s used : ", DEFAULT_FONT);
    render_25(renderer, 400, 350 - 25, lifeline_25_uses, DEFAULT_FONT);
    
    render_25(renderer, 150, 400 - 25, "Switches used: ", DEFAULT_FONT);
    render_25(renderer, 400, 400 - 25, lifeline_switch_uses, DEFAULT_FONT);
}

void render_text_input(SDL_Renderer* renderer, char* text, char* hidden, int level){
    char passwd[20] = "";
    // hiding password
    for (int i=0; i<strlen(hidden); i++)
        strcat(passwd,"*");
    render_25_centered(renderer, 320, 270, passwd, DEFAULT_FONT);
    render_25_centered(renderer, 320, 173, text, DEFAULT_FONT);
    render_screen(renderer, CRED_SCREEN);
    render_money(renderer, 275, 130, "Username : ", DEFAULT_FONT);
    render_money(renderer, 275, 225, "Password : ", DEFAULT_FONT);
    // Indicators
    if (level == 0){
        render_25(renderer, 150, 173, "O", DEFAULT_FONT);
        render_25(renderer, 470, 173, "O", DEFAULT_FONT);
    }

    else{
        render_25(renderer, 150, 270, "O", DEFAULT_FONT);
        render_25(renderer, 470, 270, "O", DEFAULT_FONT);
    }
    render_answer(renderer, 355, 455, "Press Enter to confirm and ESC to go back", DEFAULT_FONT, white);

}

void render_confirm_input(SDL_Renderer* renderer, char* text, char* confirm, int level){
    render_25_centered(renderer, 320, 270, confirm, DEFAULT_FONT);
    render_25_centered(renderer, 320, 173, text, DEFAULT_FONT);
    render_screen(renderer, CRED_SCREEN);
    render_money(renderer, 275, 130, "Username : ", DEFAULT_FONT);
    render_money(renderer, 285, 225, "Confirm : ", DEFAULT_FONT);
    // Indicators
    if (level == 0){
        render_25(renderer, 150, 173, "O", DEFAULT_FONT);
        render_25(renderer, 470, 173, "O", DEFAULT_FONT);
    }

    else{
        render_25(renderer, 150, 270, "O", DEFAULT_FONT);
        render_25(renderer, 470, 270, "O", DEFAULT_FONT);
    }
    render_answer(renderer, 355, 455, "Press Enter to confirm and ESC to go back", DEFAULT_FONT, white);

}

void render_instructions(SDL_Renderer* renderer){
    render_screen(renderer,"resources/instructs.png");
}
