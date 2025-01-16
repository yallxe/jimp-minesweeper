#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

typedef enum
{
    GD_CUSTOM = 0,
    GD_EASY = 1,
    GD_MEDIUM = 2,
    GD_HARD = 3
} GameDifficulty;

typedef struct
{
    int rows;
    int cols;
    int minutes;
    GameDifficulty difficulty;

    char *custom_board_file;
} GameConfig;

#endif // GAME_CONFIG_H