#ifndef RENDERING_H_
#define RENDERING_H_

#define DEFAULT_FONT "resources/bahnschrift.ttf"

#define MENU_BG "resources/menu.png"
#define RUNNING_BG "resources/BG.png"
#define CRED_SCREEN "resources/login.png"

void render_game(SDL_Renderer *renderer, game_t *game);
void render_menu(SDL_Renderer* renderer, menu_t *menu);
void render_stats(SDL_Renderer* renderer, stats* global_stats);
void render_text_input(SDL_Renderer* renderer, char* text, char* hidden, int level);
void render_answer(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name, const SDL_Color color);
void render_confirm_input(SDL_Renderer* renderer, char* text, char* confirm, int level);
void render_question(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name);
void render_instructions(SDL_Renderer* renderer);

#endif  // RENDERING_H_
