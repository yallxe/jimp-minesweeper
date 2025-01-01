#include "cli/start.h"
#include "cli/play.h"
#include "game/game_logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv)
{
    // Wczytywanie konfiguracji gry
    int show_bombs_cheat = argc > 1 && strcmp(argv[1], "--cheat") == 0;
    GameConfig *config = ask_user_for_game_config();
    if (config == NULL)
    {
        fprintf(stderr, "Nie udało się utworzyć konfiguracji gry\n");
        return 1;
    }

    // Inicjalizacja generatora liczb losowych
    srand(time(NULL));

    // Inicjalizacja stanu gry - część logiki
    GameState *game = create_game(config);
    if (game == NULL)
    {
        fprintf(stderr, "Nie udało się utworzyć gry\n");
        return 1;
    }

    // Inicjalizacja stanu gry - część renderująca
    GameCliState *cliGameState = init_cli_state(game, show_bombs_cheat);

    // Pętla gry
    while (render_game(game, cliGameState) != -1)
        ;

    return 0;
}