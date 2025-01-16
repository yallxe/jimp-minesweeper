#include "cli/start.h"
#include "cli/play.h"
#include "game/game_logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void print_usage()
{
    printf("Tutorial: \n");
    printf("-c - pokazuje miny na planszy (default false)\n");
    printf("-b <plik.txt> - plik z planszą do gry (opcjonalnie)\n");
    printf("-m <plik.txt> - plik z ruchami (opcjonalnie)\n");
    printf("-h - pokazuje pomoc\n");
}

int main(int argc, char **argv)
{
    // TODO: Wczytywanie argumentow CLI
    int opt;
    int cheat_mode = 0;
    char *board_file = NULL;
    char *moves_file = NULL;

    while ((opt = getopt(argc, argv, "cb:m:h")) != -1)
    {
        switch (opt)
        {
        case 'c': // Cheat flag
            cheat_mode = 1;
            break;
        case 'b': // Board file
            board_file = optarg;
            break;
        case 'm': // Moves file
            moves_file = optarg;
            break;
        case 'h': // Help
            print_usage();
            return 0;
        default: // Invalid option
            print_usage();
            return 1;
        }
    }

    // Wczytywanie konfiguracji gry
    GameConfig *config = NULL;
    if (board_file != NULL)
    {
        config = (GameConfig *)malloc(sizeof(GameConfig));
        config->custom_board_file = board_file;
    }
    else
    {
        GameConfig *config = ask_user_for_game_config();
        if (config == NULL)
        {
            fprintf(stderr, "Nie udało się utworzyć konfiguracji gry\n");
            return 1;
        }
    }

    // Inicjalizacja generatora liczb losowych
    srand(time(NULL));

    // Inicjalizacja bazy danych
    DbHandle *db = open_db_handle();

    // Inicjalizacja stanu gry - część logiki
    GameState *game = create_game(config);
    if (game == NULL)
    {
        fprintf(stderr, "Nie udało się utworzyć gry\n");
        return 1;
    }

    if (moves_file != NULL)
    {
        simulate_game_from_file(game, moves_file);
        return 0;
    }

    // Inicjalizacja stanu gry - część renderująca
    GameCliState *cliGameState = init_cli_state(game, cheat_mode);

    // Pętla gry
    while (render_game(game, cliGameState, db) != -1)
        ;

    return 0;
}