#include "game_logic.h"
#include <stdlib.h>
#include <time.h>

GameState *create_game(GameConfig *config)
{
    GameState *game = (GameState *)malloc(sizeof(GameState));
    game->rows = config->rows;
    game->cols = config->cols;

    // Inicjalizacja mapy użytkownika
    game->userMap = (UserMapField **)malloc(sizeof(UserMapField *) * game->rows);
    for (int i = 0; i < game->rows; i++)
    {
        game->userMap[i] = (UserMapField *)malloc(sizeof(UserMapField) * game->cols);
        for (int j = 0; j < game->cols; j++)
        {
            game->userMap[i][j] = -1;
        }
    }

    // Losowo rozmieszczamy miny (15% całej planszy)
    game->minesCount = (int)(game->rows * game->cols * 0.15);
    game->minesLocations = (Location *)malloc(sizeof(Location) * game->minesCount);
    for (int i = 0; i < game->minesCount; i++)
    {
        int row, col;
        do
        {
            row = rand() % game->rows;
            col = rand() % game->cols;
        } while (game->userMap[row][col] == -2);

        game->userMap[row][col] = -2;
        game->minesLocations[i].row = row;
        game->minesLocations[i].col = col;
    }

    game->startTime = time(NULL);
    game->minutes = config->minutes;
    return game;
}