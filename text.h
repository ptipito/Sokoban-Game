#ifndef DEF_TEXT
    #define DEF_TEXT
    #include <stdlib.h>
    #include <stdio.h>
    #include <SDL/SDL.h>
    #include <SDL/SDL_ttf.h>

    void init_text_lib();
    TTF_Font* get_police();
    void close_text_lib(TTF_Font* police);

#endif

