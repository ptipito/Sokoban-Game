#ifndef DEF_LEVEL_EDITOR
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "constants.h"
#include "positions.h"
#include "levels.h"

void display_mouse_icon(SDL_Surface*);
void editor_display(SDL_Surface*);
char get_char_from_surface(SDL_Surface*);
void initiate_editor_surfaces(SDL_Surface*);
void free_editor_surfaces();
void initiate_editor_screen(SDL_Surface*);
void insert_block(SDL_Surface*);
int handle_editor_events(SDL_Event event, SDL_Surface*);
int handle_editor_keyboard_events(SDL_Event, SDL_Surface*);
void save_level(char*);
Action_result editor_run(SDL_Surface*);
#endif // DEF_LEVEL_EDITOR

