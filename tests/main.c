#include "../src/game/game_logic.h"
#include <stdlib.h>
#include <stdio.h>

void assert_int(int actual, int expected, char *message)
{
    if (actual != expected)
    {
        printf("Assertion failed: %s\n", message);
        printf(" - Expected: %d\n", expected);
        printf(" - Actual: %d\n", actual);
        exit(1);
    }
}

void assert_bool(int v, char *message)
{
    if (!v)
    {
        printf("Assertion failed: %s\n", message);
        exit(1);
    }
}

int main()
{
    GameConfig cfg = {0, 0, 10, GD_CUSTOM, "board.txt"};
    GameState *game = create_game(&cfg);

    printf("Testuje create_game()\n");
    assert_bool(game != NULL, "create_game() should return non-NULL pointer");
    assert_bool(game->minesCount > 0, "create_game() should init mines");

    printf("Testuje flag_field() + reveal_field() na bombie\n");
    assert_int(reveal_field(game, 5, 5), 0, "reveal_field() should return 0 when field is revealed");
    flag_field(game, 0, 0);
    assert_int(reveal_field(game, 0, 0), 0, "reveal_field() should return -1 when mine is revealed");
    assert_int(check_mine_at(game, 0, 0), 1, "check_mine_at() should return 1 when mine is at 0, 0");

    printf("Testuje reveal_field() na bombie\n");
    assert_int(reveal_field(game, 0, 1), -1, "reveal_field() should return 0 when field is revealed");

    printf("Testy zakończone pomyślnie\n");
    return 0;
}