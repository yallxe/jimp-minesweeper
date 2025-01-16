#include "play.h"
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

GameCliState *init_cli_state(GameState *game, int showBombs)
{
    GameCliState *state = (GameCliState *)malloc(sizeof(GameCliState));
    state->displayMap = (char **)malloc(sizeof(char *) * game->rows);
    for (int i = 0; i < game->rows; i++)
    {
        state->displayMap[i] = (char *)malloc(sizeof(char) * game->cols);
    }
    state->showBombs = showBombs;
    state->message = NULL;
    return state;
}

void ask_save_score(GameState *game, DbHandle *db)
{
    printf("Podaj swoje imię: ");
    char username[100];
    scanf("%s", username);
    int completed_in = (int)difftime(time(NULL), game->startTime);
    save_score(db, username, completed_in, game->difficulty);
}

void print_top_scores(DbHandle *db)
{
    ScoreRecord *records = get_top_scores(db, 5);
    printf("Top 5 wyników:\n");
    for (int i = 0; i < 5; i++)
    {
        printf("%d. %s - %d sekund\n", i + 1, records[i].name, records[i].seconds);
    }
    // free_score_records(records, 5);
}

void show_game_results(GameState *game, GameCliState *cliState)
{
    clearterm();
    printf("Gra zakończona!\n");

    // Wypełniamy plansze do wyświetlenia
    // Tam gdzie sa miny, ale nie ma flagi, wstawiamy 'B'
    // Tam gdzie są flagi, ale nie ma miny, wstawiamy '-'
    // Tam gdzie są flagi i miny, wstawiamy 'X'
    for (int i = 0; i < game->rows; i++)
    {
        for (int j = 0; j < game->cols; j++)
        {
            if (game->userMap[i][j] == -1)
            {
                if (check_mine_at(game, i, j))
                {
                    cliState->displayMap[i][j] = 'B';
                }
                else
                {
                    cliState->displayMap[i][j] = '.';
                }
            }
            else if (game->userMap[i][j] == -2)
            {
                cliState->displayMap[i][j] = 'X';
            }
            else if (game->userMap[i][j] == -3)
            {
                cliState->displayMap[i][j] = '-';
            }
            else
            {
                if (game->userMap[i][j] == 0)
                {
                    cliState->displayMap[i][j] = ' ';
                }
                else
                {
                    cliState->displayMap[i][j] = '0' + game->userMap[i][j];
                }
            }
        }
    }

    // Wyświetlamy planszę
    print_display_map(cliState->displayMap, game->rows, game->cols);
}

int render_game(GameState *game, GameCliState *cliState, DbHandle *db)
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
            else if (game->userMap[i][j] <= -2)
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
            int fieldState = game->userMap[game->minesLocations[i].row][game->minesLocations[i].col];
            if (fieldState == -2)
                cliState->displayMap[game->minesLocations[i].row][game->minesLocations[i].col] = 'X';
            else
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

    if (input[0] == '\n')
    {
        free(input);
        return 0;
    }

    // Parsujemy komendę
    char action;
    int x, y;
    if (sscanf(input, "%c %d %d", &action, &x, &y) != 3)
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
        if (result != 0)
        {
            show_game_results(game, cliState);
        }

        if (result == 1)
        {
            printf("Wygrałeś!\n");
            ask_save_score(game, db);
            print_top_scores(db);
            return -1;
        }
        else if (result == -1)
        {
            printf("Przegrałeś!\n");
            print_top_scores(db);
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