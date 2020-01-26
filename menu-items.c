#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include "game.h"
#include "logic.h"
#include "menu-items.h"
#include "rendering.h"


// Root user initialisation
user_t root = {
    .id = "root",
    .password = "root",
};

// Current user
user_t current_user; 

node head = {&root, NULL};
node* list_root = &head;

// User logic

node* user_search(node* head, char* id){
    // Base case 
    if (head == NULL) 
        return NULL;      
    // If id is in node
    if (!strcmp(head->user->id, id))
        return head;  
    // Recur for remaining list 
    return user_search(head->next, id);
}

int compare(node *one , node *two){
    return strcmp(one->user->id, two->user->id);
}

int user_add(node **pp, char* id, char* passwd) {
    node* new = malloc(sizeof(node));
    user_t* user = malloc(sizeof(user_t));
    new->user = user;
    strcpy(new->user->id,id);
    strcpy(new->user->password,passwd);
    for ( ; *pp != NULL; pp = &(*pp)->next) {
        if (!strcmp((*pp)->user->id,id))
            return 1;
    }

    new->next = *pp;
    *pp = new;
    return 0;
}

void user_remove(node** head, char* id){
    int present = 0;
    node* old;
    node** tracer = head;
    
    while((*tracer) && !(present = (strcmp(id,(*tracer)->user->id) == 0 )  ))
        tracer = &(*tracer)->next;

    if(present){
        old = *tracer;
        *tracer = (*tracer)->next;
        free(old->user); // free off space used by adherent struct
        free(old); // free up remainder of node 
    }
}

void fetch_users(node* head){
    FILE* user_file = fopen("records/users", "r");
    char id[20];
    char password[20];
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, user_file)) != -1) {
        sscanf(line,"%s %s",id,password);
        user_add(&head,id,password);
    }

    fclose(user_file);
    if (line)
        free(line);
}

void record_users(node* head){
    node* current = head;
    FILE* user_file = fopen("records/users", "w");
    while(current != NULL){
        if (strcmp(current->user->id,"root"))
            fprintf(user_file, "%s %s\n",current->user->id,current->user->password);
        current = current->next;
    }
    fclose(user_file);
}

void passwd_input(SDL_Renderer* renderer, menu_t* menu, user_t* username, char* id, char* passwd, int* success){
    int done = 0;
    int level = 1;
    SDL_StartTextInput();
    while (!done) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    /* Quit */
                    done = 1;
                    menu->state = QUIT;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new id onto the end of our id */
                    if (strlen(passwd) < 20)
                        strcat(passwd, e.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE:
                        //passwd = calloc(1,31);
                        passwd[0] = 0;
                        done = 1;
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        passwd[strlen(passwd)-1] = 0;
                        break;
                    case SDL_SCANCODE_RETURN:
                        if (!strcmp(username->password, passwd)){
                            menu->type = USER_MENU;
                            strcpy(menu->user_id,username->id);
                            menu->selection = NO_SELECTION;
                            *success = 1;
                            done = 1;
                        }
                        else{
                            passwd[0] = 0;
                        }
                            
                        break;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_text_input(renderer,id,passwd,level);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(1000/60);
    }
}

void game_loop(SDL_Renderer* renderer, game_t* game, menu_t* menu){

    SDL_Event e;
    while (game->state != QUIT_STATE) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                game->state = QUIT_STATE;
                menu->state = QUIT;
                break;

            case SDL_MOUSEBUTTONDOWN:
                click_on_cell(game,
                              e.button.y / CELL_HEIGHT,
                              e.button.x / CELL_WIDTH);
                break;

            default: {}
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_game(renderer, game);
        SDL_RenderPresent(renderer);
    }
    
}

void username_input(SDL_Renderer* renderer, menu_t* menu, char* id, char* passwd){
    fetch_users(&head);
    int done = 0;
    int level = 0;
    int success = 0;
    node* username;
    SDL_StartTextInput();
    while (!done && !success) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    // Quit 
                    done = 1;
                    menu->state = QUIT;
                    break;
                case SDL_TEXTINPUT:
                    // Add new id onto the end of our id 
                    if (strlen(id) < 20)
                        strcat(id, e.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE:
                        done = 1;
                        id = calloc(1,21);
                        menu->selection = NO_SELECTION;
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        id[strlen(id)-1] = 0;
                        break;
                    case SDL_SCANCODE_RETURN:
                        username = user_search(&head,id);
                        if (username == NULL){
                            id = calloc(1,21);
                        }
                        else
                            passwd_input(renderer,menu,username->user,id,passwd,&success);
                        break;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_text_input(renderer,id,passwd,level);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(1000/60);
    }
    menu->selection = NO_SELECTION;
}


void signup_passwd_input(SDL_Renderer* renderer, menu_t* menu, char* id, char* passwd, int* success){
    int done = 0;
    int level = 1;
    SDL_StartTextInput();
    while (!done) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    /* Quit */
                    done = 1;
                    menu->state = QUIT;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new id onto the end of our id */
                    if (strlen(passwd) < 20)
                        strcat(passwd, e.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE:
                        //passwd = calloc(1,21);
                        passwd[0] = 0;
                        done = 1;
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        passwd[strlen(passwd)-1] = 0;
                        break;
                    case SDL_SCANCODE_RETURN:
                        if (strlen(passwd)){
                            // Successful signup!
                            user_add(&list_root,id,passwd);
                            record_users(&head);
                            menu->selection = NO_SELECTION;
                            *success = 1;
                            done = 1;
                        }
                        else{
                            passwd[0] = 0;
                        }
                            
                        break;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_text_input(renderer,id,passwd,level);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(1000/60);
    }
}

void signup_input(SDL_Renderer* renderer, menu_t* menu, char* id, char* passwd){
    const SDL_Color white = { .r = 255, .g = 255, .b = 255};
    fetch_users(&head);
    int done = 0;
    int level = 0;
    int success = 0;
    node* username;
    SDL_StartTextInput();
    while (!done && !success) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    /* Quit */
                    done = 1;
                    menu->state = QUIT;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new id onto the end of our id */
                    if (strlen(id) < 20)
                        strcat(id, e.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE:
                        done = 1;
                        id = calloc(1,21);
                        menu->selection = NO_SELECTION;
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        id[strlen(id)-1] = 0;
                        break;
                    case SDL_SCANCODE_RETURN:
                        username = user_search(&head,id);
                        if (username != NULL){
                            id = calloc(1,21);
                            render_answer(renderer, 5, 450, "Username Taken :c", DEFAULT_FONT,white);
                            SDL_RenderPresent(renderer);
                            SDL_Delay(500);
                        }
                        else
                            signup_passwd_input(renderer,menu,id,passwd,&success);
                        break;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_text_input(renderer,id,passwd,level);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(1000/60);
    }
    menu->selection = NO_SELECTION;
}

void confirm_admin_input(SDL_Renderer* renderer, menu_t* menu, user_t* username, char* id, char* confirm, int* success){
    int done = 0;
    int level = 1;
    SDL_StartTextInput();
    while (!done) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    /* Quit */
                    done = 1;
                    menu->state = QUIT;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new id onto the end of our id */
                    if (strlen(confirm) < 20)
                        strcat(confirm, e.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE:
                        //confirm = calloc(1,21);
                        confirm[0] = 0;
                        done = 1;
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        confirm[strlen(confirm)-1] = 0;
                        break;
                    case SDL_SCANCODE_RETURN:
                        if (!strcmp(username->id, confirm)){
                            // Successful deletion!
                            user_remove(&list_root,confirm);
                            record_users(&head);
                            menu->selection = NO_SELECTION;
                            *success = 1;
                            done = 1;
                        }
                        else{
                            confirm[0] = 0;
                        }
                            
                        break;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_confirm_input(renderer,id,confirm,level);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(1000/60);
    }
}

void admin_input(SDL_Renderer* renderer, menu_t* menu, char* id, char* confirm){
    fetch_users(&head);
    int done = 0;
    int level = 0;
    int success = 0;
    node* username;
    SDL_StartTextInput();
    while (!done && !success) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    /* Quit */
                    done = 1;
                    menu->state = QUIT;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new id onto the end of our id */
                    if (strlen(id) < 20)
                        strcat(id, e.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE:
                        done = 1;
                        id = calloc(1,21);
                        menu->selection = NO_SELECTION;
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        id[strlen(id)-1] = 0;
                        break;
                    case SDL_SCANCODE_RETURN:
                        username = user_search(&head,id);
                        // can't delete root or your account
                        if (username == NULL || !strcmp(username->user->id,"root") || !strcmp(username->user->id,menu->user_id)){
                            id = calloc(1,21);
                        }
                        else
                            confirm_admin_input(renderer,menu,username->user,id,confirm,&success);
                        break;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_confirm_input(renderer,id,confirm,level);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(1000/60);
    }
    menu->selection = NO_SELECTION;
}
