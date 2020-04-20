#include "game.h"

SDL_Surface* MENU = NULL;

//initialize the MENU global surface
void initiate_menu(){
    if(MENU==NULL)
        MENU = IMG_Load("images/menu.jpg");
}

//Erase screen and display menu
void load_menu(SDL_Surface* screen){
    SDL_Rect origin;
    origin.x=0;
    origin.y=0;
    SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
    if(MENU==NULL){
        initiate_menu();
    }
    origin.x = (screen->w - MENU->w)/2;
    SDL_BlitSurface(MENU,NULL,screen,&origin);
    SDL_Flip(screen);
}

SDL_Surface* initialize(){
    //Init SDL
    int SDL_Init_Succes = SDL_Init(SDL_INIT_VIDEO);
    if(SDL_Init_Succes!=0){
        fprintf(stderr,"SDL Init failed\n");
        exit(SDL_Init_Succes);
    }

    init_text_lib();

    int width=BLOCK_WIDTH*WINDOW_NB_COL,
        height=BLOCK_HEIGHT*WINDOW_NB_LINES;

    //Adjust window size to menu size if necessary
    if(width < MENU_WIDTH)
        width = MENU_WIDTH;
    if(height < MENU_HEIGHT)
        height = MENU_HEIGHT;

    SDL_Surface* screen = SDL_SetVideoMode(width,height,32,SDL_HWSURFACE | SDL_DOUBLEBUF);

    initiate_global_surfaces();
    load_menu(screen);
    return screen;
}

void quit_game(){
    SDL_FreeSurface(MENU);
    free_global_surfaces();
    free_editor_surfaces();
    close_text_lib(NULL);
    SDL_Quit();
}

//Determine uf user selected play or editor in menu
Menu_Action type_of_action(SDLKey key){
    Menu_Action type=MENU_CONTINUE;
    switch(key){
        case SDLK_KP1:
            type = MENU_PLAY;
            break;
        case SDLK_1:
            type = MENU_PLAY;
            break;
        case SDLK_KP2:
            type = MENU_EDITOR;
            break;
        case SDLK_2:
            type = MENU_EDITOR;
            break;
        case SDLK_ESCAPE:
            type = MENU_QUIT;
            break;
        default:
            break;
    }
    return type;
}

//Display controls help on level intro screen
void display_controls(SDL_Surface* screen, SDL_Rect start_pos, SDL_Color text_color){
    int padding=4;
    TTF_Font* police = get_police();
    SDL_Surface* move_down = IMG_Load("images/arrow down.png");
    SDL_Surface* move_right = IMG_Load("images/arrow right.png");
    SDL_Surface* move_up = IMG_Load("images/arrow up.png");
    SDL_Surface* move_left = IMG_Load("images/arrow left.png");
    SDL_Surface* menu_control = TTF_RenderText_Blended(police,"M",text_color);
    SDL_Surface* restart_control = TTF_RenderText_Blended(police,"R",text_color);
    SDL_Surface* text_move_control = TTF_RenderText_Blended(police,":     Move",text_color);
    SDL_Surface* text_menu_control = TTF_RenderText_Blended(police,":     Back to start menu",text_color);
    SDL_Surface* text_restart_control = TTF_RenderText_Blended(police,":     Restart level",text_color);
    //Computations to center controls keys in regards to each other and to center the controls instructions in the screen
    int move_controls_width = padding + move_left->w + padding + move_down->w + padding + move_right->w;
    int move_controls_height = start_pos.y + padding + move_up->h + padding + move_down->h;
    int highest_width, highest_control_width;
    highest_width = move_controls_width + padding + text_move_control->w;
    if(highest_width < menu_control->w + padding + text_menu_control->w)
        highest_width = menu_control->w + padding + text_menu_control->w;
    if(highest_width < restart_control->w + padding + text_restart_control->w)
        highest_width = restart_control->w + padding + text_restart_control->w;
    highest_control_width = move_controls_width;
    SDL_Rect pos=start_pos;

    //Display move instructions
    pos.y = start_pos.y + padding;
    pos.x = start_pos.x + (screen->w - start_pos.x - highest_width)/2 + move_left->w + padding;
    SDL_BlitSurface(move_up,NULL,screen,&pos);
    pos.y += (move_up->w + padding);
    SDL_BlitSurface(move_down,NULL,screen,&pos);
    pos.x = start_pos.x + (screen->w - start_pos.x - highest_width)/2;
    SDL_BlitSurface(move_left,NULL,screen,&pos);
    pos.x += (move_left->w + padding + move_down->w +padding);
    SDL_BlitSurface(move_right,NULL,screen,&pos);
    pos.x += (padding + move_right->w + padding);
    pos.y = start_pos.y + (padding + move_up->h + padding + move_down->h)/2 - text_move_control->h/2;
    SDL_BlitSurface(text_move_control,NULL,screen,&pos);

    //display menu control
    pos.x = start_pos.x + (screen->w - start_pos.x - highest_width)/2 + (highest_control_width - menu_control->w)/2;
    pos.y = move_controls_height + padding;
    SDL_BlitSurface(menu_control,NULL,screen,&pos);
    pos.x += ((highest_control_width - menu_control->w)/2 + menu_control->w + padding);
    SDL_BlitSurface(text_menu_control,NULL,screen,&pos);

    //display restart control
    pos.x = start_pos.x + (screen->w - start_pos.x - highest_width)/2 + (highest_control_width - restart_control->w)/2;
    pos.y += (menu_control->h + padding);
    SDL_BlitSurface(restart_control,NULL,screen,&pos);
    pos.x += ((highest_control_width - restart_control->w)/2 + restart_control->w + padding);
    SDL_BlitSurface(text_restart_control,NULL,screen,&pos);

    TTF_CloseFont(police);
    SDL_FreeSurface(move_down);
    SDL_FreeSurface(move_right);
    SDL_FreeSurface(move_left);
    SDL_FreeSurface(move_up);
    SDL_FreeSurface(menu_control);
    SDL_FreeSurface(restart_control);
    SDL_FreeSurface(text_move_control);
    SDL_FreeSurface(text_menu_control);
    SDL_FreeSurface(text_restart_control);
}

void display_level_intro_screen(SDL_Surface* screen){
    Sint16 text_area_top = 150;
    SDL_Surface* background = SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w,MENU->h-text_area_top,32,0,0,0,0);
    SDL_FillRect(background,NULL,SDL_MapRGB(background->format,0,0,0));
    SDL_Color text_color = {255,255,255};
    char* rules_text = "Help Mario to put the boxes on the right spots!";
    TTF_Font* police = get_police();
    SDL_Rect rules_pos, background_pos, controls_pos;

    //Erase menu display
    background_pos.x=0;
    background_pos.y=text_area_top;

    //Display rules
    SDL_Surface* rules_surface = TTF_RenderText_Blended(police,rules_text,text_color);
    rules_pos.x=background->w/2-rules_surface->w/2; rules_pos.y=0;
    SDL_BlitSurface(rules_surface,NULL,background,&rules_pos);

    controls_pos.x=(screen->w - MENU->w)/2;
    if((screen->w - MENU->w)/2<0){
        controls_pos.x=0;
    }
    controls_pos.y = rules_surface->h;
    display_controls(background,controls_pos,text_color);

    SDL_BlitSurface(background,NULL,screen,&background_pos);

    SDL_Flip(screen);
    TTF_CloseFont(police);
    SDL_FreeSurface(background);
}

void display_editor_intro_screen(SDL_Surface* screen){
    SDL_Surface *title_surface= NULL, *explanations_part1_surface = NULL, *explanations_part2_surface = NULL,
                *explanations_part3_surface = NULL, *explanations_part4_surface = NULL, *cur_sprite=NULL, *cur_sprite_text_surface=NULL;
    int buffer_size=200;
    int padding_sprites = 10, title_padding=5, sprite_height_start=0;
    char explanations_text1[buffer_size], explanations_text2[buffer_size], explanations_text3[buffer_size],
         explanations_text4[buffer_size], title_text[buffer_size], cur_sprite_text[buffer_size];
    TTF_Font* police=get_police();
    SDL_Color text_color={255,255,255}, title_color={255,0,0};
    SDL_Rect explanations_pos, sprite_pos, sprite_text_pos;
    sprite_pos.x = screen->w/4;
    sprite_text_pos.x = screen->w/4 + BLOCK_WIDTH + 10;

    snprintf(title_text, buffer_size, "Create your own levels!");
    snprintf(explanations_text1, buffer_size, "Left-click to insert a block on the clicked case.");
    snprintf(explanations_text2, buffer_size, "To change the block to insert, press:");
    snprintf(explanations_text3, buffer_size, "Click S to save your level.");
    snprintf(explanations_text4, buffer_size, "It will be added at the end of the default levels.");

    //Display title and first explanations
    title_surface= TTF_RenderText_Blended(police,title_text,title_color);
    explanations_part1_surface = TTF_RenderText_Blended(police,explanations_text1,text_color);
    explanations_part2_surface = TTF_RenderText_Blended(police,explanations_text2,text_color);

    SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
    explanations_pos.x = screen->w/2-title_surface->w/2;
    explanations_pos.y = 20;
    SDL_BlitSurface(title_surface,NULL,screen,&explanations_pos);
    explanations_pos.x = screen->w/2-explanations_part1_surface->w/2;
    explanations_pos.y += title_surface->h + title_padding;
    SDL_BlitSurface(explanations_part1_surface,NULL,screen,&explanations_pos);
    explanations_pos.x = screen->w/2-explanations_part2_surface->w/2;
    explanations_pos.y += explanations_part1_surface->h;
    SDL_BlitSurface(explanations_part2_surface,NULL,screen,&explanations_pos);

    //Display block keys
    cur_sprite = IMG_Load("images/caisse.jpg");
    snprintf(cur_sprite_text,buffer_size,"B for a box");
    cur_sprite_text_surface = TTF_RenderText_Blended(police,cur_sprite_text,text_color);
    sprite_height_start = explanations_pos.y + explanations_part2_surface->h + 2*padding_sprites;
    sprite_pos.y = sprite_height_start;
    sprite_text_pos.y = sprite_height_start + (cur_sprite->h/2-cur_sprite_text_surface->h/2);
    SDL_BlitSurface(cur_sprite,NULL,screen,&sprite_pos);
    SDL_BlitSurface(cur_sprite_text_surface,NULL,screen,&sprite_text_pos);

    sprite_pos.y += cur_sprite->w + padding_sprites;
    cur_sprite = IMG_Load("images/mario_bas.gif");
    snprintf(cur_sprite_text,buffer_size,"C for Mario");
    cur_sprite_text_surface = TTF_RenderText_Blended(police,cur_sprite_text,text_color);
    sprite_text_pos.y = sprite_pos.y + (cur_sprite->h/2-cur_sprite_text_surface->h/2);
    SDL_BlitSurface(cur_sprite,NULL,screen,&sprite_pos);
    SDL_BlitSurface(cur_sprite_text_surface,NULL,screen,&sprite_text_pos);

    sprite_pos.y += cur_sprite->w + padding_sprites;
    cur_sprite = IMG_Load("images/objectif.png");
    snprintf(cur_sprite_text,buffer_size,"G for goal");
    cur_sprite_text_surface = TTF_RenderText_Blended(police,cur_sprite_text,text_color);
    sprite_text_pos.y = sprite_pos.y + (cur_sprite->h/2-cur_sprite_text_surface->h/2);
    SDL_BlitSurface(cur_sprite,NULL,screen,&sprite_pos);
    SDL_BlitSurface(cur_sprite_text_surface,NULL,screen,&sprite_text_pos);

    sprite_pos.y += cur_sprite->w + padding_sprites;
    cur_sprite = IMG_Load("images/mur.jpg");
    snprintf(cur_sprite_text,buffer_size,"W for a wall");
    cur_sprite_text_surface = TTF_RenderText_Blended(police,cur_sprite_text,text_color);
    sprite_text_pos.y = sprite_pos.y + (cur_sprite->h/2-cur_sprite_text_surface->h/2);
    SDL_BlitSurface(cur_sprite,NULL,screen,&sprite_pos);
    SDL_BlitSurface(cur_sprite_text_surface,NULL,screen,&sprite_text_pos);

    sprite_pos.y += cur_sprite->w + padding_sprites;
    cur_sprite = SDL_CreateRGBSurface(SDL_HWSURFACE,BLOCK_WIDTH,BLOCK_HEIGHT,32,0,0,0,0);
    SDL_FillRect(cur_sprite,NULL,SDL_MapRGB(screen->format,255,255,255));
    snprintf(cur_sprite_text,buffer_size,"E for an empty block");
    cur_sprite_text_surface = TTF_RenderText_Blended(police,cur_sprite_text,text_color);
    sprite_text_pos.y = sprite_pos.y + (cur_sprite->h/2-cur_sprite_text_surface->h/2);
    SDL_BlitSurface(cur_sprite,NULL,screen,&sprite_pos);
    SDL_BlitSurface(cur_sprite_text_surface,NULL,screen,&sprite_text_pos);

    //Display end of the explanations
    explanations_part3_surface = TTF_RenderText_Blended(police,explanations_text3,text_color);
    explanations_pos.x = (screen->w - explanations_part3_surface->w)/2;
    explanations_pos.y = sprite_pos.y + cur_sprite->w + padding_sprites;
    SDL_BlitSurface(explanations_part3_surface,NULL,screen,&explanations_pos);
    explanations_part4_surface = TTF_RenderText_Blended(police,explanations_text4,text_color);
    explanations_pos.x = (screen->w - explanations_part4_surface->w)/2;
    explanations_pos.y += explanations_part3_surface->h;
    SDL_BlitSurface(explanations_part4_surface,NULL,screen,&explanations_pos);


    SDL_Flip(screen);
    TTF_CloseFont(police);
    SDL_FreeSurface(title_surface);
    SDL_FreeSurface(explanations_part1_surface);
    SDL_FreeSurface(explanations_part2_surface);
    SDL_FreeSurface(explanations_part3_surface);
    SDL_FreeSurface(explanations_part4_surface);
    SDL_FreeSurface(cur_sprite_text_surface);
    SDL_FreeSurface(cur_sprite);
}

Action_result handle_intro_screens_events(){
    SDL_Event event;
    int continuing=1, result=ACRES_NEXT;
    while(continuing){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                continuing=0;
                result=ACRES_QUIT;
                break;
            case SDL_MOUSEBUTTONDOWN:
                continuing=0;
                break;
            case SDL_KEYDOWN:
                continuing=0;
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    result=ACRES_QUIT;
                break;
            default:
                break;
        }
    }
    return result;
}

void run(){
    int continuing = 1;
    SDL_Event event;
    SDL_Surface *screen = initialize();
    Action_result result=ACRES_BACK_TO_MENU;

    while(continuing){
        load_menu(screen);
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                continuing=0;
                break;
            case SDL_KEYDOWN:
                switch(type_of_action(event.key.keysym.sym)){
                    case MENU_QUIT:
                        continuing=0;
                        break;
                    case MENU_PLAY:
                        SDL_EnableKeyRepeat(100,10);
                        display_level_intro_screen(screen);
                        switch(handle_intro_screens_events()){
                            case ACRES_QUIT:
                                continuing=0;
                                break;
                            case ACRES_NEXT:
                                result = play(screen);
                                if(result==ACRES_QUIT){
                                    continuing = 0;
                                }
                                break;
                            default:
                                break;
                        }
                        break;
                    case MENU_EDITOR:
                        display_editor_intro_screen(screen);
                        switch(handle_intro_screens_events()){
                            case ACRES_QUIT:
                                continuing=0;
                                break;
                            case ACRES_NEXT:
                                result = editor_run(screen);
                                if(result==ACRES_QUIT){
                                    continuing = 0;
                                }
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    quit_game();
}

