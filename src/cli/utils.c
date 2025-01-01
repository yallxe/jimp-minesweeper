#include "utils.h"
#include <unistd.h>
#include <stdio.h>

void clearterm()
{
    printf("\n");
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

void print_display_map(char **displayMap, int rows, int cols)
{
    printf("   ");
    for (int i = 1; i <= cols; i++)
    {
        printf("%2d ", i);
    }
    printf("\n");
    for (int i = 0; i < rows; i++)
    {
        printf("%2d ", i + 1);
        for (int j = 0; j < cols; j++)
        {
            printf("%2c ", displayMap[i][j]);
        }
        printf("\n");
    }
}