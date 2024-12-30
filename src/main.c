#include "cli/start.h"
#include "cli/play.h"
#include "game/game_logic.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
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

    render_game(game);
    return 0;
}