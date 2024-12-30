#include "play.h"
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int render_game(GameState *game, int showBombs, char** displayMap)
{
    clearterm();

    // Obliczamy ile czasu zostało
    time_t currentTime = time(NULL);
    int elapsedTime = (int)difftime(currentTime, game->startTime);
    int remainingTime = game->minutes * 60 - elapsedTime;
    if (remainingTime <= 0)
    {
        printf("Czas minął!\n");
        return -1;
    }

    printf("Czasu zostało: %d minut %d sekund\n", remainingTime / 60, remainingTime % 60);

    // Wypełniamy plansze do wyświetlenia
    for (int i = 0; i < game->rows; i++)
    {
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

    // Pytamy użytkownika o ruch
    printf("Wpisz komende ('r/f [x] [y]'): ");

    // Czytamy komendę
    char action;
    scanf(" %c", &action);
    if (action == 'q')
    {
        return -1;
    }

    int x, y;
    scanf("%d %d", &x, &y);
    x--;
    y--;

    if (x < 0 || x >= game->rows || y < 0 || y >= game->cols)
    {
        printf("Nieprawidłowe współrzędne\n");
        return 0;
    }

    if (action == 'r')
    {
        int result = reveal_field(game, x, y);
        if (result == -1)
        {
            printf("Przegrałeś!\n");
            return -1;
        }
    }
    else if (action == 'f')
    {
        flag_field(game, x, y);
    }

    return 0;
}