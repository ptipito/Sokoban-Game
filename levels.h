#ifndef DEF_LEVELS
    #define DEF_LEVELS
    #include <stdlib.h>
    #include <stdio.h>
    #include <SDL/SDL.h>
    #include <SDL/SDL_image.h>
    #include "constants.h"
    #include "positions.h"
    #include "text.h"

    typedef struct Level_state{
        SDL_Rect mario_pos;
        Directions mario_dir;
        SDL_Rect* wall_positions;
        int nb_walls;
        SDL_Rect* box_positions;
        int nb_boxes;
        SDL_Rect* goal_positions;
        int nb_goals;
        SDL_Rect* empty_positions;
        int nb_empty;
        SDL_Rect* success_positions;
        int nb_success;
        int level_nr;
    } Level_state;

    void initiate_global_surfaces();
    void free_global_surfaces();
    void free_level_state(Level_state*);
    int get_to_level(int, FILE*);
    Level_state create_level(int);
    void prepare_background(Level_state, SDL_Surface*);
    void display_level_state(Level_state, SDL_Surface*);
    SDL_Surface* get_surface(const char);
    Level_state load_level(int, SDL_Surface*);
    Move_type can_move_to(Directions, Level_state);
    void move_box(SDL_Rect, Directions, Level_state*);
    void animation_move(Animations, SDL_Rect, SDL_Rect, Level_state, SDL_Surface*);
    void move_mario(Level_state*, Directions, SDL_Surface*);
    Action_result handle_levels_key_press(const SDL_Event, Level_state*, SDL_Surface*);
    int level_is_complete(Level_state);
    int is_last_level(Level_state);
    int last_level_nr();
    Action_result play(SDL_Surface*);
    Action_result handle_success(SDL_Surface*, Level_state);
    void display_success(SDL_Surface*);
    void display_end_screen(SDL_Surface*);
    Action_result handle_success_screen_events();
    Action_result handle_end_screen_events();
#endif // DEF_LEVELS

