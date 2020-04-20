/*
    Contains the code to handle the level editor.
    Contrary to the level.c file, the simple approach was chosen:
    the state of the screen is simply represented by a char matrix dimensioned
    on the window dimensions and which values in i,j corresponds to the surface
    blit in coordinate (i,j) of the map.
    NB: i,j represents the coordinates of a block relative to the map, not the coordinates
    in pixel. Thus a block in (i,j) is actually blit in (i*BLOCK_WIDTH,j*BLOCK_HEIGHT).
*/

#include "level_editor.h"

SDL_Surface* EDITOR_WALL = NULL;
SDL_Surface* EDITOR_MARIO = NULL;
SDL_Surface* EDITOR_GOAL = NULL;
SDL_Surface* EDITOR_BOX = NULL;
SDL_Surface* EDITOR_EMPTY = NULL;
SDL_Surface* TO_BLIT=NULL;
char to_blit_key=EMPTY_SLOT_KEY;//remember which block the user last selected
int editor_initiated=0;
int nb_mario=0;//Limit to 1 the number of marios on the screen
int MOUSE_POS_X=0;
int MOUSE_POS_Y=0;
int ongoing_left_clic=0;//allow multiple block inserts on the model drag and drop
SDL_Rect mouse_icon_pos;//Position of the icon of the block to insert (following the mouse)
char level_editor_state[WINDOW_NB_LINES][WINDOW_NB_COL];//Map of the screen

//Display next to the mouse the block to be inserted on click
void display_mouse_icon(SDL_Surface* screen){
    SDL_BlitSurface(TO_BLIT,NULL,screen,&mouse_icon_pos);
}

//Display the current state of the editor
void editor_display(SDL_Surface* screen){
    SDL_Rect pos;
    SDL_Surface* surface;
    int i=0,j=0;
    for(i=0;i<WINDOW_NB_LINES;i++){
        for(j=0;j<WINDOW_NB_COL;j++){
            pos = get_position(i,j);
            surface = get_surface(level_editor_state[i][j]);
            if(level_editor_state[i][j]==MARIO_KEY
               || level_editor_state[i][j]==GOAL_KEY)
                SDL_BlitSurface(EDITOR_EMPTY,NULL,screen,&pos);
            SDL_BlitSurface(surface,NULL,screen,&pos);
        }
    }
    display_mouse_icon(screen);
}

char get_char_from_surface(SDL_Surface* screen){
    if(screen==EDITOR_BOX)
        return BOX_IDLE_KEY;
    if(screen==EDITOR_EMPTY)
        return EMPTY_SLOT_KEY;
    if(screen==EDITOR_GOAL)
        return GOAL_KEY;
    if(screen==EDITOR_MARIO)
        return MARIO_KEY;
    if(screen==EDITOR_WALL)
        return WALL_KEY;
    return '$';
}

//Initiate global surfaces for the editor
void initiate_editor_surfaces(SDL_Surface* screen){
    EDITOR_WALL = IMG_Load("images/mur.jpg");
    EDITOR_BOX = IMG_Load("images/caisse.jpg");
    EDITOR_GOAL = IMG_Load("images/objectif.png");
    EDITOR_MARIO = IMG_Load("images/mario_bas.gif");
    EDITOR_EMPTY = SDL_CreateRGBSurface(SDL_HWSURFACE,BLOCK_WIDTH,BLOCK_HEIGHT,32,0,0,0,0);
    TO_BLIT = EDITOR_EMPTY;
    SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
    SDL_FillRect(EDITOR_EMPTY,NULL,SDL_MapRGB(EDITOR_EMPTY->format,255,255,255));
    editor_initiated=1;
}

void free_editor_surfaces(){
    SDL_FreeSurface(EDITOR_WALL);
    SDL_FreeSurface(EDITOR_BOX);
    SDL_FreeSurface(EDITOR_EMPTY);
    SDL_FreeSurface(EDITOR_MARIO);
    SDL_FreeSurface(EDITOR_EMPTY);
    SDL_FreeSurface(TO_BLIT);
    editor_initiated=0;
}

//Initiate the editor when coming from menu
void initiate_editor_screen(SDL_Surface* screen){
    SDL_Rect curr_pos;

    if(!editor_initiated){
        initiate_editor_surfaces(screen);
    }
    SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
    //initiate screen blank state
    int i=0,j=0;
    for(i=0;i<WINDOW_NB_LINES;i++){
        for(j=0;j<WINDOW_NB_COL;j++){
            curr_pos = get_position(i,j);
            SDL_BlitSurface(EDITOR_EMPTY,NULL,screen,&curr_pos);
        }
    }
    //iniate state global
    for(i=0;i<WINDOW_NB_LINES;i++){
        for(j=0;j<WINDOW_NB_COL;j++){
            level_editor_state[i][j]=WALL_KEY;
        }
    }
    SDL_WarpMouse(screen->w/2,screen->h/2);
    mouse_icon_pos.x = screen->w/2+5;
    mouse_icon_pos.y = screen->h/2+10;
    SDL_Flip(screen);
}

//Blit current block and update the editor state array
void insert_block(SDL_Surface* screen){
    int line_nb=0,
        col_nb=0;
    SDL_Rect curr_pos;
    curr_pos = block_left_corner_from_coord(MOUSE_POS_X,MOUSE_POS_Y);
    get_coordinates_from_pos(curr_pos, &line_nb, &col_nb);
    ongoing_left_clic=1;
    switch(to_blit_key){
        case MARIO_KEY:
            if(nb_mario==0){
                SDL_BlitSurface(EDITOR_EMPTY,NULL,screen,&curr_pos);
                SDL_BlitSurface(TO_BLIT,NULL,screen,&curr_pos);
                level_editor_state[line_nb][col_nb] = to_blit_key;
                nb_mario=1;
            }
            break;
        case GOAL_KEY:
            SDL_BlitSurface(EDITOR_EMPTY,NULL,screen,&curr_pos);
            SDL_BlitSurface(TO_BLIT,NULL,screen,&curr_pos);
            if(level_editor_state[line_nb][col_nb])
                nb_mario = 0;
            level_editor_state[line_nb][col_nb] = to_blit_key;
            break;
        default:
            SDL_BlitSurface(TO_BLIT,NULL,screen,&curr_pos);
            if(level_editor_state[line_nb][col_nb])
                nb_mario = 0;
            level_editor_state[line_nb][col_nb] = to_blit_key;
            break;
    }
}

int handle_editor_events(SDL_Event event, SDL_Surface* screen){
    int result=1;
    return result;
}

int handle_editor_keyboard_events(SDL_Event event, SDL_Surface* screen){
    int line_nb=0, col_nb=0;
    int result=ACRES_CONTINUE;
    SDL_Rect curr_pos = block_left_corner_from_coord(MOUSE_POS_X,MOUSE_POS_Y);
    get_coordinates_from_pos(curr_pos, &line_nb, &col_nb);
    switch(event.key.keysym.sym){
        case SDLK_ESCAPE:
            result=ACRES_QUIT;
            break;
        case SDLK_w:
            to_blit_key = WALL_KEY;
            TO_BLIT = EDITOR_WALL;
            break;
        case SDLK_e:
            to_blit_key= EMPTY_SLOT_KEY;
            TO_BLIT = EDITOR_EMPTY;
            break;
        case SDLK_b:
            to_blit_key = BOX_IDLE_KEY;
            TO_BLIT = EDITOR_BOX;
            break;
        case SDLK_g:
            to_blit_key = GOAL_KEY;
            TO_BLIT = EDITOR_GOAL;
            break;
        case SDLK_c:
            to_blit_key = MARIO_KEY;
            TO_BLIT = EDITOR_MARIO;
            break;
        case SDLK_s:
            save_level("levels/user_level.txt");
            break;
        case SDLK_m:
            result=ACRES_BACK_TO_MENU;
            break;
        default:
            break;
    }
    return result;
}

void save_level(char* filename){
    FILE* save_file = fopen(filename,"a");
    int i=0, j=0;

    fputc(LEVEL_SEPARATOR_KEY,save_file);
    for(i=0;i<WINDOW_NB_LINES;i++){
        for(j=0;j<WINDOW_NB_COL;j++){
            fprintf(stdout,"%c",level_editor_state[i][j]);
            fputc(level_editor_state[i][j], save_file);
        }
        fputc(NEWLINE_KEY, save_file);
    }

    fclose(save_file);
}

//editor controller
Action_result editor_run(SDL_Surface* screen){
    SDL_Event event;
    int continuing=1;
    Action_result result=ACRES_CONTINUE;

    initiate_editor_screen(screen);
    while(continuing){
        SDL_WaitEvent(&event);
         switch(event.type){
            case SDL_QUIT:
                result = ACRES_QUIT;
                continuing = 0;
                break;
            case SDL_KEYDOWN:
                result = handle_editor_keyboard_events(event, screen);
                if(result==ACRES_BACK_TO_MENU || result==ACRES_QUIT){
                    continuing = 0;
                }
                break;
            case SDL_MOUSEMOTION:
                MOUSE_POS_X = event.motion.x;
                MOUSE_POS_Y = event.motion.y;
                mouse_icon_pos.x = event.motion.x+5;mouse_icon_pos.y = event.motion.y+10;
                if(ongoing_left_clic){
                    insert_block(screen);
                }
                editor_display(screen);
                SDL_BlitSurface(TO_BLIT,NULL,screen,&mouse_icon_pos);
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch(event.button.button){
                    case SDL_BUTTON_LEFT:
                        insert_block(screen);
                       break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                 if(event.button.button == SDL_BUTTON_LEFT)
                    ongoing_left_clic=0;
            default:
                break;
        }
        editor_display(screen);
        SDL_Flip(screen);
    }
    return result;
}

