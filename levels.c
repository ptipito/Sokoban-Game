/*
    Contains the code to handle the play screen: levels display and interaction.
    The chosen approach with pointers and the structure Level_state surely is
    over engineered since representing the screen could be simply done with a
    matrix of char based on the constants defined in constants.h. However it
    fully serves its purpose: using and manipulating pointers, as well as practicing
    dynamic (re)allocation.
*/

#include "levels.h"


SDL_Surface* WALL = NULL;
SDL_Surface* BOX_IDLE = NULL;
SDL_Surface* BOX_SUCCESS = NULL;
SDL_Surface* GOAL = NULL;
SDL_Surface* MARIO_UP = NULL;
SDL_Surface* MARIO_DOWN = NULL;
SDL_Surface* MARIO_LEFT = NULL;
SDL_Surface* MARIO_RIGHT = NULL;
SDL_Surface* EMPTY_BLOCK = NULL;

//Used in the initialize method of game.c
void initiate_global_surfaces(){
    if(WALL==NULL)
        WALL = IMG_Load("images/mur.jpg");
    if(BOX_IDLE==NULL)
        BOX_IDLE = IMG_Load("images/caisse.jpg");
    if(BOX_SUCCESS==NULL)
        BOX_SUCCESS = IMG_Load("images/caisse_ok.jpg");
    if(GOAL==NULL)
        GOAL = IMG_Load("images/objectif.png");
    if(MARIO_UP==NULL)
        MARIO_UP = IMG_Load("images/mario_haut.gif");
    if(MARIO_DOWN==NULL)
        MARIO_DOWN = IMG_Load("images/mario_bas.gif");
    if(MARIO_LEFT==NULL)
        MARIO_LEFT = IMG_Load("images/mario_gauche.gif");
    if(MARIO_RIGHT==NULL)
        MARIO_RIGHT = IMG_Load("images/mario_droite.gif");
    if(EMPTY_BLOCK==NULL){
        EMPTY_BLOCK = SDL_CreateRGBSurface(SDL_HWSURFACE,BLOCK_WIDTH,BLOCK_HEIGHT,32,0,0,0,0);
        SDL_FillRect(EMPTY_BLOCK,NULL,SDL_MapRGB(EMPTY_BLOCK->format,255,255,255));
    }
}

void free_global_surfaces(){
    SDL_FreeSurface(WALL);
    SDL_FreeSurface(BOX_IDLE);
    SDL_FreeSurface(BOX_SUCCESS);
    SDL_FreeSurface(GOAL);
    SDL_FreeSurface(MARIO_UP);
    SDL_FreeSurface(MARIO_DOWN);
    SDL_FreeSurface(MARIO_LEFT);
    SDL_FreeSurface(MARIO_RIGHT);
    SDL_FreeSurface(EMPTY_BLOCK);
}

//Free dynamically allocated arrays of a Level_state structure
void free_level_state(Level_state* state){
    if(state->box_positions!=NULL)
        free(state->box_positions);
    if(state->empty_positions!=NULL)
        free(state->empty_positions);
    if(state->goal_positions!=NULL)
        free(state->goal_positions);
    if(state->success_positions!=NULL)
        free(state->success_positions);
    if(state->wall_positions!=NULL)
        free(state->wall_positions);
}

//Function handling the game
Action_result play(SDL_Surface* screen){
    SDL_Event event;
    int continuing=1, cur_level=1;
    Level_state state = load_level(cur_level, screen);
    Action_result result = ACRES_CONTINUE;

    while(continuing){
        int level_nr = state.level_nr;
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                result = ACRES_QUIT;
                continuing=0;
                break;
            case SDL_KEYDOWN:
                result = handle_levels_key_press(event, &state, screen);
                if(result==ACRES_RESTART){
                    state = load_level(level_nr, screen);
                }
                if(result==ACRES_BACK_TO_MENU || result==ACRES_QUIT){
                    continuing=0;
                }
                break;
            default:
                break;
        }

        if(result==ACRES_CONTINUE){
            display_level_state(state,screen);
            if(level_is_complete(state)){
                result = handle_success(screen, state);
                switch(result){
                    case ACRES_QUIT:
                        continuing=0;
                        break;
                    case ACRES_BACK_TO_MENU:
                        continuing=0;
                        break;
                    case ACRES_RESTART:
                        state=load_level(cur_level,screen);
                        break;
                    case ACRES_NEXT:
                        state=load_level(++cur_level,screen);
                        break;
                    default:
                        break;
                }
                SDL_EnableKeyRepeat(100,10);
            }
        }
    }
    free_level_state(&state);
    return result;
}

int get_to_level(int i, FILE* levels_file){
    int  curr_level_nb=1;
    char curr_char=EOF;
    if(i==curr_level_nb)
        return i;
    do{
        curr_char = fgetc(levels_file);
        if(curr_char==LEVEL_SEPARATOR_KEY){
            curr_level_nb++;
            if(curr_level_nb==i){
                return i;
            }
        }
    }while(curr_char!=EOF);

    return -1;
}

//function to create the Level_state object corresponding to a level
Level_state create_level(int level_number){
    FILE* level_file = fopen(LEVELS_FILENAME,"r");
    SDL_Rect* wall_positions = NULL;
    SDL_Rect* goal_positions = NULL;
    SDL_Rect* box_positions = NULL;
    SDL_Rect* empty_positions = NULL;
    SDL_Rect mario_position;
    int nb_walls=0, nb_goals=0, nb_boxes=0,nb_empty=0, done=0;

    char curr_char=EOF;
    int curr_line=0, curr_col=0;

    SDL_EnableKeyRepeat(100,10);
    Level_state level_initial_state;
    level_initial_state.level_nr = get_to_level(level_number, level_file);
    level_initial_state.nb_goals=0;
    if(level_initial_state.level_nr != level_number){
        return level_initial_state;
    }
    do{
        if(curr_line >= WINDOW_NB_LINES){
            break;
        }
        curr_char = fgetc(level_file);
        switch(curr_char){
            case LEVEL_SEPARATOR_KEY:
                done=1;
                break;
            case NEWLINE_KEY:
                curr_line++;
                curr_col = -1;//incremented to 0 at end of loop. Avoiding to duplicate increment in each case
                break;
            case MARIO_KEY:
                mario_position = get_position(curr_line,curr_col);
                break;
            case BOX_IDLE_KEY:
                nb_boxes = append_pos(get_position(curr_line,curr_col),&box_positions,nb_boxes);
                break;
            case WALL_KEY:
                nb_walls = append_pos(get_position(curr_line,curr_col),&wall_positions,nb_walls);
                break;
            case GOAL_KEY:
                nb_goals = append_pos(get_position(curr_line,curr_col),&goal_positions,nb_goals);
                break;
            case EMPTY_SLOT_KEY:
                nb_empty = append_pos(get_position(curr_line,curr_col),&empty_positions,nb_empty);
                break;
            case EOF:
                done=1;
                break;
            default:
                fprintf(stderr,"Error in level definition: unknown surface\n");
                break;
        }
        //insert_surface(curr_char,screen,curr_line,curr_col);
        curr_col++;
    } while(!done);

    level_initial_state.mario_pos = mario_position;
    level_initial_state.mario_dir = MOVE_DOWN;
    level_initial_state.wall_positions = wall_positions;
    level_initial_state.nb_walls = nb_walls;
    level_initial_state.goal_positions = goal_positions;
    level_initial_state.nb_goals = nb_goals;
    level_initial_state.box_positions = box_positions;
    level_initial_state.nb_boxes = nb_boxes;
    level_initial_state.empty_positions = empty_positions;
    level_initial_state.nb_empty = nb_empty;
    level_initial_state.success_positions = NULL;
    level_initial_state.nb_success = 0;
    fclose(level_file);
    return level_initial_state;
}

//Prepare the background display of a level (i.e. walls & empty slots) as this background needs to be computed only once
void prepare_background(Level_state state, SDL_Surface* screen){
    int nb_walls= state.nb_walls,
        nb_empty = state.nb_empty;
    int i=0;
    SDL_Rect curr_pos;

    SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
    for(i=0;i<nb_empty;i++){
        curr_pos = state.empty_positions[i];
        SDL_BlitSurface(EMPTY_BLOCK,NULL,screen,&curr_pos);
    }
    for(i=0;i<nb_walls;i++){
        curr_pos = state.wall_positions[i];
        SDL_BlitSurface(WALL,NULL,screen,&curr_pos);
    }
}

void display_level_state(Level_state state, SDL_Surface* screen){
    int size = state.nb_empty;
    SDL_Rect curr_pos;
    int i = 0, is_mario_on_a_goal=0;

    size = state.nb_goals;
    for(i=0;i<size;i++){
        curr_pos = state.goal_positions[i];
        if(compare_pos(state.mario_pos,curr_pos)==0){
            is_mario_on_a_goal = 1;
        }
        SDL_BlitSurface(EMPTY_BLOCK,NULL,screen,&curr_pos);
        SDL_BlitSurface(GOAL,NULL,screen,&curr_pos);
    }
    size = state.nb_boxes;
    for(i=0;i<size;i++){
        curr_pos = state.box_positions[i];
        SDL_BlitSurface(BOX_IDLE,NULL,screen,&curr_pos);
    }
    size = state.nb_success;
    for(i=0;i<size;i++){
        curr_pos = state.success_positions[i];
        SDL_BlitSurface(BOX_SUCCESS,NULL,screen,&curr_pos);
    }
    SDL_Surface* mario_s = MARIO_DOWN;
    switch(state.mario_dir){
        case MOVE_UP:
            mario_s = MARIO_UP;
            break;
        case MOVE_DOWN:
            mario_s = MARIO_DOWN;
            break;
        case MOVE_LEFT:
            mario_s = MARIO_LEFT;
            break;
        case MOVE_RIGHT:
            mario_s = MARIO_RIGHT;
            break;
        default:
            break;
    }

    if(!is_mario_on_a_goal){
        //Delete potential residue (e.g. a moved box)
        SDL_BlitSurface(EMPTY_BLOCK,NULL,screen,&(state.mario_pos));
    }
    SDL_BlitSurface(mario_s,NULL,screen,&(state.mario_pos));
    SDL_Flip(screen);
    return;
}

Level_state load_level(int level_nr, SDL_Surface* screen){
    Level_state state = create_level(level_nr);
    prepare_background(state,screen);
    display_level_state(state, screen);
    return state;
}

SDL_Surface* get_surface(char c){
    SDL_Surface *result_surface = NULL;
    switch(c){
        case WALL_KEY:
            result_surface = WALL;
            break;
        case GOAL_KEY:
            result_surface = GOAL;
            break;
        case BOX_IDLE_KEY:
            result_surface = BOX_IDLE;
            break;
        case MARIO_KEY:
            result_surface = MARIO_DOWN;
            break;
        case EMPTY_SLOT_KEY:
            result_surface = EMPTY_BLOCK;
            break;
        default:
            fprintf(stderr,"Error in level definition: unknown surface\n");
            break;
    }
    return result_surface;
}

int can_move_box(SDL_Rect pos, Directions direction, Level_state state){
    SDL_Rect next_box_pos = new_position(pos,direction);
    if(pos_in_list(next_box_pos,state.empty_positions,state.nb_empty)
        || pos_in_list(next_box_pos,state.goal_positions,state.nb_goals)){
        return 1;
    }
    return 0;
}

Move_type can_move_to(Directions direction, Level_state state){
    SDL_Rect pos_next = new_position(state.mario_pos,direction);
    if(pos_in_list(pos_next,state.wall_positions,state.nb_walls)
       || pos_in_list(pos_next,state.goal_positions1,state.nb_goals)){
        return MOVET_IMPOSSIBLE;
    }
    if(pos_in_list(pos_next,state.box_positions,state.nb_boxes)){
        if(can_move_box(pos_next,direction,state)){
            return MOVET_BOX_N_MARIO;
        }
        return MOVET_IMPOSSIBLE;
    }
    return MOVET_MARIO;
}

void move_box(SDL_Rect src, Directions direction, Level_state* state){
    SDL_Rect box_destination = new_position(src, direction);
    if(pos_in_list(box_destination,state->goal_positions,state->nb_goals)){
        SDL_Rect two_before_box = new_position(src,invert_direction(direction));
        state->nb_empty = append_pos(two_before_box, &(state->empty_positions),state->nb_empty);
        state->nb_success = append_pos(box_destination, &(state->success_positions),state->nb_success);
    }
    update(&(state->box_positions),state->nb_boxes,src,box_destination);
    return;
}

//Function added for more fluidity in the movements/transitions
void animation_move(Animations anim_type, SDL_Rect src, SDL_Rect dest, Level_state state, SDL_Surface* screen){
    SDL_Rect potential_box_pos = dest;
    SDL_Surface *mario_surface = NULL, *box_surface=NULL;
    int increment = 1;
    //using pointers to write only one loop independent from the axis of the move
    Sint16* src_ordinate = &(src.x);
    Sint16* box_pos_ordinate = &(potential_box_pos.x);
    int dest_ordinate = dest.x;
    switch(state.mario_dir){
        case MOVE_UP:
            increment = -1;
            src_ordinate = NULL;
            src_ordinate = &(src.y);
            box_pos_ordinate = &(potential_box_pos.y);
            dest_ordinate = dest.y;
            mario_surface = MARIO_UP;
            break;
        case MOVE_DOWN:
            src_ordinate = NULL;
            src_ordinate = &(src.y);
            box_pos_ordinate = &(potential_box_pos.y);
            dest_ordinate = dest.y;
            mario_surface = MARIO_DOWN;
            break;
        case MOVE_LEFT:
            increment = -1;
            mario_surface = MARIO_LEFT;
            break;
        case MOVE_RIGHT:
            mario_surface = MARIO_RIGHT;
            break;
    }
    switch(anim_type){
        case ANIM_BOX:
            box_surface = BOX_IDLE;
            break;
        default:
            break;
    }
    int cur_time=0, prev_time=0,
        is_mario_going_on_goal=pos_in_list(dest,state.goal_positions,state.nb_goals),
        was_mario_on_goal=pos_in_list(state.mario_pos,state.goal_positions,state.nb_goals);//Mario pos corresponds to mario initial pos during the whole animation
    while((*src_ordinate) != dest_ordinate){
        cur_time = SDL_GetTicks();
        if(cur_time-prev_time>5){
            SDL_BlitSurface(EMPTY_BLOCK,NULL,screen,&src);
            if(is_mario_going_on_goal){
                //Prevent goal image to be erased during the move animation
                SDL_BlitSurface(GOAL,NULL,screen,&dest);
            }
            if(was_mario_on_goal){
                //Prevent goal image to be erased during the move animation
                SDL_BlitSurface(GOAL,NULL,screen,&(state.mario_pos));
            }
            (*src_ordinate) += increment;
            (*box_pos_ordinate) += increment;
            SDL_BlitSurface(mario_surface,NULL,screen,&src);
            SDL_BlitSurface(box_surface,NULL,screen,&potential_box_pos);
            SDL_Flip(screen);
            prev_time=cur_time;
        }
    }
}

//Perform the movement animation and update the level state
void move_mario(Level_state* state, Directions direction, SDL_Surface *screen){
    SDL_Rect pos = (*state).mario_pos;
    SDL_Rect dest = new_position(pos,direction);
    state->mario_dir = direction;
    int move_type = can_move_to(direction,*state);
    switch(move_type){
        case MOVET_IMPOSSIBLE:
            break;
        case MOVET_MARIO:
            animation_move(ANIM_MARIO,state->mario_pos,dest,*state,screen);
            (state->mario_pos) = dest;
            update(&(state->empty_positions),state->nb_empty,state->mario_pos,pos);
            break;
        case MOVET_BOX_N_MARIO:
            animation_move(ANIM_BOX,state->mario_pos,dest,*state,screen);
            (state->mario_pos) = dest;
            update(&(state->empty_positions),state->nb_empty,new_position(state->mario_pos,direction),pos);
            move_box(dest,direction,state);
            break;
    }
    return;
}

//Handle keyboard actions during a level
Action_result handle_levels_key_press(const SDL_Event event, Level_state* state, SDL_Surface* screen){
    Action_result result=ACRES_CONTINUE;
    switch(event.key.keysym.sym){
        case SDLK_ESCAPE://quit game
            result=ACRES_QUIT;
            break;
        case SDLK_UP:
            move_mario(state, MOVE_UP,screen);
            break;
        case SDLK_DOWN:
            move_mario(state, MOVE_DOWN,screen);
            break;
        case SDLK_RIGHT:
            move_mario(state, MOVE_RIGHT,screen);
            break;
        case SDLK_LEFT:
            move_mario(state, MOVE_LEFT,screen);
            break;
        case SDLK_r://restart level
            fprintf(stdout,"pressed r\n");
            result = ACRES_RESTART;
            break;
        case SDLK_m://back to menu
            result = ACRES_BACK_TO_MENU;
            break;
        default:
            break;
    }
    return result;
}

int level_is_complete(Level_state state){
    return (state.nb_success == state.nb_goals);
}

int is_last_level(Level_state level_state){
    return (level_state.level_nr == last_level_nr());
}

int last_level_nr(){

    FILE* levels_file = fopen(LEVELS_FILENAME,"r");
    int last_level = 0;
    char cur_char=EOF;

    do{
        cur_char=fgetc(levels_file);
        if(cur_char==LEVEL_SEPARATOR_KEY)
            last_level++;
    }while(cur_char!=EOF);
    return last_level;
}

//Handle the end of level when the level is complete
Action_result handle_success(SDL_Surface* screen, Level_state state){
    SDL_EnableKeyRepeat(0,0);
    Action_result result=ACRES_NEXT;
    if(is_last_level(state)){
        display_end_screen(screen);
        result = handle_end_screen_events();
    }
    else{
        display_success(screen);
        result = handle_success_screen_events();
    }
    return result;
}

void display_success(SDL_Surface* screen){
    char* text = "Success!";
    SDL_Color text_color = {255,255,255}, back_color={0,0,0};
    TTF_Font* police = get_police();
    SDL_Surface* text_surface = TTF_RenderText_Shaded(police,text,text_color,back_color);
    int padding_height = 4,
        padding_width = 4;
    SDL_Surface* container_of_text_surface = SDL_CreateRGBSurface(SDL_HWSURFACE,text_surface->w+padding_width,
                                                                  text_surface->h+padding_height,32,0,0,0,0);
    SDL_FillRect(container_of_text_surface,NULL,SDL_MapRGB(container_of_text_surface->format,255,0,0));
    SDL_Rect text_pos, container_pos;
    text_pos.x=padding_width/2;text_pos.y=padding_height/2;
    container_pos.x=screen->w/2-container_of_text_surface->w/2;container_pos.y=screen->h/2-container_of_text_surface->h/2;
    SDL_BlitSurface(text_surface,NULL,container_of_text_surface,&text_pos);
    SDL_BlitSurface(container_of_text_surface,NULL,screen,&container_pos);
    fprintf(stdout,"blit success\n");
    SDL_Flip(screen);
    TTF_CloseFont(police);
}

Action_result handle_success_screen_events(){
    SDL_Event event;
    int continuing=1;
    Action_result result=ACRES_CONTINUE;
    fprintf(stdout,"in handle_success_screen_events\n");
    while(continuing){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                result = ACRES_QUIT;
                continuing=0;
                break;
            case SDL_KEYDOWN:
                result=ACRES_NEXT;
                continuing=0;
                switch(event.key.keysym.sym){
                    case SDLK_m:
                        result=ACRES_BACK_TO_MENU;
                        break;
                    case SDLK_r:
                        result=ACRES_RESTART;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
    return result;
}

//End screen when the user completed all levels
void display_end_screen(SDL_Surface* screen){
    SDL_Surface* end_screen = IMG_Load("images/end_screen.jpg");
    SDL_Rect pos;
    pos.x=(screen->w-end_screen->w)/2;
    pos.y=(screen->h-end_screen->h)/2;
    SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
    SDL_BlitSurface(end_screen,NULL,screen,&pos);
    SDL_Flip(screen);
    SDL_FreeSurface(end_screen);
}

Action_result handle_end_screen_events(){
    SDL_Event event;
    int continuing=1;
    Action_result result=ACRES_BACK_TO_MENU;
    while(continuing){
            SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                continuing = 0;
                result = ACRES_QUIT;
            case SDL_KEYDOWN:
                continuing=0;
                result = ACRES_BACK_TO_MENU;
                break;
            default:
                break;
        }
    }
    return result;
}
