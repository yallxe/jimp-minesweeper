#ifndef PLAY_H
#define PLAY_H
#include "../game/game_logic.h"

typedef struct
{
    char **displayMap;
    int showBombs;
    char *message;
} GameCliState;

GameCliState *init_cli_state(GameState *game, int showBombs);
int render_game(GameState *game, GameCliState *cliState);

#endif // PLAY_H