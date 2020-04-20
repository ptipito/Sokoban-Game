#ifndef DEF_POSITIONS
#define DEF_POSITIONS
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "constants.h"

int append_pos(const SDL_Rect, SDL_Rect**, int);
SDL_Rect get_position(const int, const int);
int compare_pos(SDL_Rect, SDL_Rect);
void update(SDL_Rect**, int, SDL_Rect, SDL_Rect);
SDL_Rect new_position(SDL_Rect, Directions);
int pos_in_list(SDL_Rect, SDL_Rect*, int);
Directions invert_direction(Directions);
SDL_Rect block_left_corner_from_coord(int, int);
void get_coordinates_from_pos(SDL_Rect, int*, int*);
#endif // DEF_POSITIONS
