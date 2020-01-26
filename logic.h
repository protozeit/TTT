#ifndef LOGIC_H_
#define LOGIC_H_

stats* fetch_stats();
game_t* game_init();

void menu_hover_select(menu_t* menu, int x, int y);
void check_menu_selection(SDL_Renderer* renderer,game_t* game, menu_t* menu);
void click_on_cell(game_t *game, int row, int column);

#endif  // LOGIC_H_
