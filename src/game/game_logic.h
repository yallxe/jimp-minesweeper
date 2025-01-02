#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdint.h>
#include "game_config.h"

// Definiuje pole takie, jak na mapie użytkownika
// Możliwe wartości:
// -3 - oflagowane pole nieprawidłowo - nie ma miny
// -2 - oflagowane pole prawidłowo - jest mina
// -1 - nieodkryte pole
// 0 - odkryte pole i brak min w sąsiedztwie
// 1-... - odkryte pole i liczba min w sąsiedztwie
typedef int8_t UserMapField;

// definiuje dowolny punkt na planszy według wiersza i kolumny
typedef struct Location
{
    int row;
    int col;
} Location;

// definiuje stan gry
typedef struct
{
    UserMapField **userMap;
    int rows;
    int cols;

    int unrevealedFields;
    int minesCount;
    Location *minesLocations;

    int startTime; // unix timestamp w sekundach
    int minutes;
} GameState;

GameState *create_game(GameConfig *config);

void generate_mines(GameState *game, int excludeRow, int excludeCol);

// zwraca:
//  0 - pole odkryte, nic się nie stało
// -1 - przegrana
// 1 - wygrana, nie ma już nieodkrytych pól
int reveal_field(GameState *game, int row, int col);

void flag_field(GameState *game, int row, int col);
int check_mine_at(GameState *game, int row, int col);

#endif // GAME_LOGIC_H