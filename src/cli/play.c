#include "play.h"
#include "utils.h"
#include <stdio.h>
#include <time.h>

void render_game(GameState *game)
{
    clearterm();

    // Obliczamy ile czasu zostało
    time_t currentTime = time(NULL);
    int elapsedTime = (int)difftime(currentTime, game->startTime);
    int remainingTime = game->minutes * 60 - elapsedTime;
    printf("Czasu zostało: %d minut %d sekund\n", remainingTime / 60, remainingTime % 60);

    // Rysujemy planszę
    for (int i = 0; i < game->rows; i++)
    {
        for (int j = 0; j < game->cols; j++)
        {
            UserMapField field = game->userMap[i][j];
            if (field == -2)
            {
                printf("F "); // Oflagowane pole
            }
            else if (field == -1)
            {
                printf(". "); // Nieodkryte pole
            }
            else
            {
                printf("%d ", field); // Odkryte pole z liczbą min w sąsiedztwie
            }
        }
        printf("\n");
    }
}