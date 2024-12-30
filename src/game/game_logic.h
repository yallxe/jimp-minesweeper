#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdint.h>
#include "game_config.h"

// Definiuje pole takie, jak na mapie użytkownika
// -2 - oflagowane pole
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

    int minesCount;
    Location *minesLocations;

    int startTime; // unix timestamp w sekundach
    int minutes;
} GameState;

GameState *create_game(GameConfig *config);

#endif // GAME_LOGIC_H