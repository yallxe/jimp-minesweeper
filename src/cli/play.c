#include "play.h"
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void render_game(GameState *game, int showBombs)
{
    clearterm();

    // Obliczamy ile czasu zostało
    time_t currentTime = time(NULL);
    int elapsedTime = (int)difftime(currentTime, game->startTime);
    int remainingTime = game->minutes * 60 - elapsedTime;
    printf("Czasu zostało: %d minut %d sekund\n", remainingTime / 60, remainingTime % 60);

    // Tworzymy plansze (macierz charów) do wyświetlenia
    char **displayMap = (char **)malloc(sizeof(char *) * game->rows);
    for (int i = 0; i < game->rows; i++)
    {
        displayMap[i] = (char *)malloc(sizeof(char) * game->cols);
        for (int j = 0; j < game->cols; j++)
        {
            if (game->userMap[i][j] == -1)
            {
                displayMap[i][j] = '.';
            }
            else if (game->userMap[i][j] == -2)
            {
                displayMap[i][j] = 'F';
            }
            else if (game->userMap[i][j] == 0)
            {
                displayMap[i][j] = ' ';
            }
            else
            {
                displayMap[i][j] = '0' + game->userMap[i][j];
            }
        }
    }

    // Cheat mode - pokazujemy miny
    if (showBombs)
    {
        for (int i = 0; i < game->minesCount; i++)
        {
            if (game->userMap[game->minesLocations[i].row][game->minesLocations[i].col] != -2)
                displayMap[game->minesLocations[i].row][game->minesLocations[i].col] = 'B';
        }
    }

    // Wyświetlamy planszę
    printf("   ");
    for (int i = 1; i <= game->cols; i++)
    {
        printf("%2d ", i);
    }
    printf("\n");
    for (int i = 0; i < game->rows; i++)
    {
        printf("%2d ", i + 1);
        for (int j = 0; j < game->cols; j++)
        {
            printf("%2c ", displayMap[i][j]);
        }
        printf("\n");
    }
}