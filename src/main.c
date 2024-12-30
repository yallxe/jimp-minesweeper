#include "cli/start.h"
#include "cli/play.h"
#include "game/game_logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    int show_bombs_cheat = argc > 1 && strcmp(argv[1], "--cheat") == 0;
    GameConfig *config = ask_user_for_game_config();
    if (config == NULL)
    {
        fprintf(stderr, "Nie udało się utworzyć konfiguracji gry\n");
        return 1;
    }

    GameState *game = create_game(config);
    if (game == NULL)
    {
        fprintf(stderr, "Nie udało się utworzyć gry\n");
        return 1;
    }

    render_game(game, show_bombs_cheat);
    return 0;
}