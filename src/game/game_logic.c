#include "game_logic.h"
#include <stdlib.h>
#include <time.h>

int check_mine_at(GameState *game, int row, int col)
{
    for (int i = 0; i < game->minesCount; i++)
    {
        if (game->minesLocations[i].row == row && game->minesLocations[i].col == col)
        {
            return 1;
        }
    }
    return 0;
}

GameState *create_game(GameConfig *config)
{
    GameState *game = (GameState *)malloc(sizeof(GameState));
    game->rows = config->rows;
    game->cols = config->cols;
    game->difficulty = config->difficulty;

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

    // Inicjalizacja licznika nieodkrytych pól
    game->unrevealedFields = game->rows * game->cols;

    // Miny generują się dopiero w momencie odkrycia pierwszego pola
    game->minesCount = 0;
    game->minesLocations = NULL;

    game->startTime = time(NULL);
    game->minutes = config->minutes;
    return game;
}

void generate_mines(GameState *game, int excludeRow, int excludeCol)
{
    game->minesCount = (int)(game->rows * game->cols * 0.15);
    game->minesLocations = (Location *)malloc(sizeof(Location) * game->minesCount);
    int placedMines = 0;
    while (placedMines < game->minesCount)
    {
        int row = rand() % game->rows;
        int col = rand() % game->cols;

        // Sprawdzamy, czy na tym polu już jest mina lub czy to pole musi być wyłączone z generowania,
        // jeśli tak, to losujemy ponownie
        if (check_mine_at(game, row, col) || (row == excludeRow && col == excludeCol))
        {
            continue;
        }

        game->minesLocations[placedMines].row = row;
        game->minesLocations[placedMines].col = col;
        placedMines++;
    }
}

int reveal_field(GameState *game, int row, int col)
{
    if (game->minesCount == 0)
    {
        generate_mines(game, row, col);
    }

    if (game->userMap[row][col] != -1)
    {
        return 0;
    }

    if (check_mine_at(game, row, col))
    {
        return -1;
    }

    int minesCount = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (row + i < 0 || row + i >= game->rows || col + j < 0 || col + j >= game->cols)
            {
                continue;
            }
            if (check_mine_at(game, row + i, col + j))
            {
                minesCount++;
            }
        }
    }

    game->userMap[row][col] = minesCount;
    game->unrevealedFields--;

    if (minesCount == 0)
    {
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (row + i < 0 || row + i >= game->rows || col + j < 0 || col + j >= game->cols)
                {
                    continue;
                }
                reveal_field(game, row + i, col + j);
            }
        }
    }

    if (game->unrevealedFields == game->minesCount)
    {
        return 1;
    }

    return 0;
}

void flag_field(GameState *game, int row, int col)
{
    if (game->userMap[row][col] == -1)
    {
        if (check_mine_at(game, row, col))
        {
            game->userMap[row][col] = -2;
        }
        else
        {
            game->userMap[row][col] = -3;
        }
    }
    else if (game->userMap[row][col] <= -2)
    {
        game->userMap[row][col] = -1;
    }
}