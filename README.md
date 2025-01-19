# Minesweeper in C

Jest to implementacja gry "Saper" w języku C realizowana w ramach przedmiotu "Języki i metody programowania 1" na Politechnice Warszawskiej. Poniżej znajduje się sprawozdanie z realizacji projektu.

## Podręcznik użytkowania

Program kompiluje się za pomocą polecenia `make`. W celu uruchomienia gry należy wykonać polecenie `./bin/minesweeper`. Aby uruchomić testy jednostkowe, należy wykonać polecenie `make test`.

Zasady gry są identyczne jak w oryginalnej grze "Saper". Gracz ma za zadanie odkryć wszystkie pola, które nie zawierają min. W przypadku odkrycia pola z miną, gracz przegrywa. Gracz wygrywa, gdy odkryje wszystkie pola, które nie zawierają min.

Do odkrycia pola służy komenda `r [row] [column]`, gdzie `[row]` to numer wiersza, a `[column]` to numer kolumny. Przykładowo, komenda `r 1 2` odkryje pole w pierwszym wierszu i drugiej kolumnie.

Do oflagowania pola służy komenda `f [row] [column]`, gdzie `[row]` to numer wiersza, a `[column]` to numer kolumny. Przykładowo, komenda `f 1 2` oznaczy pole w pierwszym wierszu i drugiej kolumnie jako pole z miną.

### Dostępne opcje programu

* `-h` - wyświetla pomoc
* `-c` - włącza tryb cheat, pokazujący miny na planszy
* `-m [moves.txt]` - wczytuje ruchy z pliku
* `-b [board.txt]` - wczytuje planszę z pliku

Format pliku `moves.txt` (po prostu komendy):

```text
r 1 2
f 3 4
[...]
```

Format pliku z planszą `board.txt`:

```text
10 10 3 10       [rows] [columns] [mines] [moves]
1 1  [bombRow] [bombColumn]
1 2
1 3
```

## Implementacja gry

### Podział na moduły

Cały kod źródłowy programu znajduje się w folderze `src`. Program został podzielony na dwie części: `game` oraz `cli`. W `game` znajduje się logika gry, natomiast w `cli` znajduje się interfejs użytkownika (terminalowy). Punkt wejścia programu znajduje się w pliku `src/main.c`.

Podział `game`:

* `game_config.h` - konfiguracja gry
* `game_logic.h` - logika gry (+ .c implementacja)
* `game_filedb.h` - baza danych wszystkich gier (+ .c implementacja)

Podział `cli`:

* `start.h` - start gry, wczytywanie konfiguracji od uzytkownika (+ .c implementacja)
* `game.h` - obsługa gry (+ .c implementacja)
* `utils.h` - pomocnicze funkcje (+ .c implementacja)

### Kluczowe funkcje

Gra:

* `GameState *create_game(GameConfig *config);` - tworzenie nowej gry
* `void generate_mines(GameState *game, int excludeRow, int excludeCol);` - generowanie min
* `int reveal_field(GameState *game, int row, int col);` - odkrywanie pola
* `void flag_field(GameState *game, int row, int col);` - oznaczanie pola jako pole z miną
* `int check_mine_at(GameState *game, int row, int col);` - sprawdzanie, czy na polu znajduje się mina

CLI:

* `GameCliState *init_cli_state(GameState *game, int showBombs)` - inicjalizacja stanu gry w interfejsie użytkownika
* `int render_game(GameState *game, GameCliState *cliState, DbHandle *db);` - renderowanie stanu gry

### Kluczowe struktury

Struktura stanu silnika renderującego grę w terminalu:

```c
typedef struct
{
    char **displayMap;
    int showBombs;
    char *message;
} GameCliState;
```

Przechowuje ona: mapę do wyświetlenia, informację o tym, czy pokazywać miny oraz wiadomość do wyświetlenia.

Struktura stanu gry:

```c
typedef struct
{
    UserMapField **userMap;
    int rows;
    int cols;
    GameDifficulty difficulty;

    int unrevealedFields;
    int minesCount;
    Location *minesLocations;

    int startTime; // unix timestamp w sekundach
    int minutes;
} GameState;
```

Przechowuje ona: planszę gracza, wymiary planszy, poziom trudności, liczbę nieodkrytych pól, liczbę min, lokalizacje min, czas rozpoczęcia gry oraz czas gry w minutach.

### Testy

Wszystkie testy jednostkowe znajdują się w folderze `tests`. Testy pokrywają funkcje związane z logiką gry. Wszystkie testy przechodzą poprawnie.

## Podział pracy w zespole

Pracowałem nad projektem samodzielnie.

## Podsumowanie

Projekt pozwolił mi na lepsze zrozumienie języka C oraz nauczenie się podstawowych technik programowania w tym języku. Wszystkie funkcjonalności zaimplementowałem zgodnie z założeniami, a program działa poprawnie. Trudnością było implementacja algorytmu odkrywania pól, który musiał działać rekurencyjnie.
