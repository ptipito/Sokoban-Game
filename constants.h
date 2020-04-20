/*
    File containing the global constants for the project
*/
#ifndef DEF_CONSTANTES
#define DEF_CONSTANTES
    #define BLOCK_WIDTH 34
    #define BLOCK_HEIGHT 34
    #define WINDOW_NB_LINES 12
    #define WINDOW_NB_COL 14
    #define MENU_WIDTH 408
    #define MENU_HEIGHT 408
    #define MARIO_KEY 'M'
    #define BOX_IDLE_KEY 'b'
    #define WALL_KEY 'w'
    #define GOAL_KEY 'o'
    #define EMPTY_SLOT_KEY ' '
    #define NEWLINE_KEY ';'
    #define LEVEL_SEPARATOR_KEY '\n'
    #define LEVELS_FILENAME "levels.txt"

    typedef enum Directions {MOVE_DOWN,MOVE_UP,MOVE_LEFT,MOVE_RIGHT} Directions;
    typedef enum Animations {ANIM_MARIO, ANIM_BOX} Animations;
    typedef enum Move_type {MOVET_IMPOSSIBLE, MOVET_MARIO, MOVET_BOX_N_MARIO} Move_type;
    typedef enum Action_result {ACRES_QUIT, ACRES_CONTINUE, ACRES_BACK_TO_MENU, ACRES_RESTART, ACRES_LEVEL_SUCCESS, ACRES_PREV, ACRES_NEXT} Action_result;

#endif
