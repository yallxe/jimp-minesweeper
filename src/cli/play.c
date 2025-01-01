#include "play.h"
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

GameCliState *init_cli_state(GameState *game, int showBombs)
{
    GameCliState *state = (GameCliState *)malloc(sizeof(GameCliState));
    state->displayMap = (char **)malloc(sizeof(char *) * game->rows);
    for (int i = 0; i < game->rows; i++)
    {
        state->displayMap[i] = (char *)malloc(sizeof(char) * game->cols);
    }
    state->showBombs = 0;
    state->message = NULL;
    return state;
}

int render_game(GameState *game, GameCliState *cliState)
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
                cliState->displayMap[i][j] = '.';
            }
            else if (game->userMap[i][j] == -2)
            {
                cliState->displayMap[i][j] = 'F';
            }
            else if (game->userMap[i][j] == 0)
            {
                cliState->displayMap[i][j] = ' ';
            }
            else
            {
                cliState->displayMap[i][j] = '0' + game->userMap[i][j];
            }
        }
    }

    // Cheat mode - pokazujemy miny
    if (cliState->showBombs)
    {
        for (int i = 0; i < game->minesCount; i++)
        {
            if (game->userMap[game->minesLocations[i].row][game->minesLocations[i].col] != -2)
                cliState->displayMap[game->minesLocations[i].row][game->minesLocations[i].col] = 'B';
        }
    }

    // Wyświetlamy planszę
    print_display_map(cliState->displayMap, game->rows, game->cols);

    // Wyświetlamy nowy komunikat
    if (cliState->message != NULL)
    {
        printf("KOMUNIKAT: %s\n", cliState->message);
        cliState->message = NULL;
    }

    // Pytamy użytkownika o ruch
    printf("Wpisz komende ('r/f [x] [y]'): ");

    // Czytamy komendę
    char *input = NULL;
    size_t len = 0;
    if (getline(&input, &len, stdin) == -1)
    {
        cliState->message = "Błąd podczas czytania komendy";
        free(input);
        return 0;
    }

    // Parsujemy komendę
    char action;
    int x, y;
    if (sscanf(input, " %c %d %d", &action, &x, &y) != 3)
    {
        cliState->message = "Nieprawidłowa komenda";
        free(input);
        return 0;
    }

    free(input);

    if (action == 'q')
    {
        return -1;
    }

    x--;
    y--;

    if (x < 0 || x >= game->rows || y < 0 || y >= game->cols)
    {
        cliState->message = "Nieprawidłowe współrzędne";
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
    else
    {
        cliState->message = "Nieprawidłowa komenda";
    }

    return 0;
}