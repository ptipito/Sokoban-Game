/*
    Contains the code to handle  geographical positions, computations on them
     and to match a relative     position on the map with the absolute position
     in pixels. For instance the position (2,3) on the map is actually in pixels
     (2*BLOCK_WIDTH,3*BLOCK_HEIGHT)
*/
#include "positions.h"


int append_pos(const SDL_Rect pos, SDL_Rect** pos_list, int pos_list_size){
    int size = pos_list_size;
    SDL_Rect* safe_realloc;
    safe_realloc = (SDL_Rect*) realloc(*pos_list,(pos_list_size+1)*sizeof(SDL_Rect));
    if(safe_realloc != NULL){
        *pos_list = safe_realloc;
        (*pos_list)[pos_list_size] = pos;
        size++;
    }
    return size;
}

SDL_Rect get_position(const int line_nb, const int col_nb){
    SDL_Rect position;
    position.x = col_nb*BLOCK_WIDTH;
    position.y = line_nb*BLOCK_HEIGHT;
    return position;
}

int compare_pos(SDL_Rect pos1, SDL_Rect pos2){
    if(pos1.x > pos2.x){
        return 1;
    }
    if(pos1.x == pos2.x){
        if(pos1.y > pos2.y)
            return 1;
        if(pos1.y == pos2.y)
            return 0;
    }
    return -1;
}

void update(SDL_Rect** pos_list, int pos_list_size, SDL_Rect remove_pos, SDL_Rect add_pos){
    int i = 0;
    for(i=0;i<pos_list_size;i++){
        if(compare_pos((*pos_list)[i],remove_pos)==0){
            (*pos_list)[i] = add_pos;
        }
    }
    return;
}

SDL_Rect new_position(SDL_Rect pos, Directions direction){
    switch(direction){
        case MOVE_UP:
            if(pos.y>0){
                pos.y -= BLOCK_HEIGHT;
            }
            break;
        case MOVE_DOWN:
            if(pos.y < BLOCK_HEIGHT*(WINDOW_NB_LINES-1)){
                pos.y += BLOCK_HEIGHT;
            }
            break;
        case MOVE_LEFT:
            if(pos.x > 0){
                pos.x -= BLOCK_WIDTH;
            }
            break;
        case MOVE_RIGHT:
            if(pos.x < BLOCK_WIDTH*(WINDOW_NB_COL-1)){
                pos.x += BLOCK_WIDTH;
            }
            break;
    }
    return pos;
}

int pos_in_list(SDL_Rect pos, SDL_Rect* pos_list, int list_size){
    int i=0, result=0;
    for(i=0;i<list_size;i++){
        if(compare_pos(pos_list[i],pos)==0){
            result = 1;
            break;
        }
    }
    return result;
}

Directions invert_direction(Directions direction){
    switch(direction){
        case MOVE_UP:
            return MOVE_DOWN;
        case MOVE_DOWN:
            return MOVE_UP;
        case MOVE_LEFT:
            return MOVE_RIGHT;
        case MOVE_RIGHT:
            return MOVE_LEFT;
    }
    return MOVE_UP;
}

SDL_Rect block_left_corner_from_coord(int pos_x, int pos_y){
    return get_position(pos_y/BLOCK_HEIGHT,pos_x/BLOCK_WIDTH);
}

void get_coordinates_from_pos(SDL_Rect curr_pos, int* line_nb, int* col_nb){
    *line_nb = curr_pos.y/BLOCK_HEIGHT;
    *col_nb = curr_pos.x/BLOCK_WIDTH;
    return;
}
