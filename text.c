/*
    Interface with the SDL_TTF library
*/

#include "text.h"


void init_text_lib(){
    int ttf_init_success= TTF_Init();
    if(ttf_init_success==-1){
        fprintf(stderr,"Error TTF init: %s\n",TTF_GetError());
        exit(EXIT_FAILURE);
    }
    atexit(SDL_Quit);
}

TTF_Font* get_police(){
    return TTF_OpenFont("arial.ttf",20);
}

void close_text_lib(TTF_Font* police){
    TTF_CloseFont(police);
    TTF_Quit();
}

