#ifndef DEF_GAME
    #define DEF_GAME
    #include <stdlib.h>
    #include <stdio.h>
    #include <SDL/SDL.h>
    #include <SDL/SDL_image.h>
    #include <SDL/SDL_ttf.h>
    #include "constants.h"
    #include "positions.h"
    #include "levels.h"
    #include "level_editor.h"
    #include "text.h"

    typedef enum Menu_Action {MENU_QUIT, MENU_PLAY, MENU_EDITOR, MENU_CONTINUE} Menu_Action;

    void initiate_menu();
    void load_menu(SDL_Surface*);
    SDL_Surface* initialize();
    void quit_game();
    Menu_Action type_of_action(SDLKey);
    void display_controls(SDL_Surface*, SDL_Rect, SDL_Color);
    void display_level_intro_screen(SDL_Surface*);
    void display_editor_intro_screen(SDL_Surface*);
    Action_result handle_intro_screens_events();
    void run();
#endif
