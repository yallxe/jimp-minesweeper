#include "start.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

GameConfig *ask_user_for_game_config()
{
    clearterm();

    printf("Wybierz poziom trudności gry:\n");
    printf("1. Łatwy (9x9, 10 min)\n");
    printf("2. Średni (16x16, 40 min)\n");
    printf("3. Trudny (16x30, 99 min)\n");
    printf("4. Własny\n");
    printf("Twój wybór: ");

    int choice;
    while (scanf("%d", &choice) != 1 || choice < 1 || choice > 4)
    {
        printf("Nieprawidłowy wybór. Spróbuj ponownie: ");
        while (getchar() != '\n')
            ;
    }

    GameConfig *config = (GameConfig *)malloc(sizeof(GameConfig));
    switch (choice)
    {
    case 1:
        config->rows = 9;
        config->cols = 9;
        config->minutes = 10;
        config->difficulty = GD_EASY;
        break;
    case 2:
        config->rows = 16;
        config->cols = 16;
        config->minutes = 40;
        config->difficulty = GD_MEDIUM;
        break;
    case 3:
        config->rows = 16;
        config->cols = 30;
        config->minutes = 99;
        config->difficulty = GD_HARD;
        break;
    case 4:
        config->difficulty = GD_CUSTOM;
        printf("Podaj liczbę wierszy: ");
        while (scanf("%d", &config->rows) != 1 || config->rows < 1 || config->rows > 100)
        {
            printf("Nieprawidłowa liczba wierszy. Spróbuj ponownie: ");
            while (getchar() != '\n')
                ;
        }

        printf("Podaj liczbę kolumn: ");
        while (scanf("%d", &config->cols) != 1 || config->cols < 1 || config->cols > 100)
        {
            printf("Nieprawidłowa liczba kolumn. Spróbuj ponownie: ");
            while (getchar() != '\n')
                ;
        }
        printf("Podaj liczbę minut: ");
        while (scanf("%d", &config->minutes) != 1 || config->minutes < 1)
        {
            printf("Nieprawidłowa liczba minut. Spróbuj ponownie: ");
            while (getchar() != '\n')
                ;
        }
        break;
    }

    return config;
}