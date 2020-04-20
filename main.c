#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "levels.h"
#include "constants.h"
#include "game.h"


typedef struct coord {
    Sint16 x;
    Sint16 y;
} coord;


int main(int argc, char* argv[]){

    run();

    return EXIT_SUCCESS;
}

