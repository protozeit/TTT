#ifndef MENU_ITEMS_H_
#define MENU_ITEMS_H_

void game_loop(SDL_Renderer* renderer, game_t* game, menu_t* menu);
void username_input(SDL_Renderer* renderer, menu_t* menu, char* id, char* passwd);
void signup_input(SDL_Renderer* renderer, menu_t* menu, char* id, char* passwd);
void admin_input(SDL_Renderer* renderer, menu_t* menu, char* id, char* confirm);
void rights_input(SDL_Renderer* renderer, menu_t* menu, char* id, char* confirm);

#endif // MENU_ITEMS_H_
