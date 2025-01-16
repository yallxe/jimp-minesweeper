#ifndef GAME_FILEDB_H
#define GAME_FILEDB_H
#include "game_config.h"
#include <stdio.h>

typedef struct
{
    FILE *scores_f;
} DbHandle;

DbHandle *open_db_handle();

// sprawdza czy baza danych nie jest uszkodzona
int check_db_integrity(DbHandle *db);

typedef struct
{
    char *name;
    int seconds;
} ScoreRecord;

void save_score(DbHandle *db, char *username, int seconds, GameDifficulty difficulty);
ScoreRecord *get_top_scores(DbHandle *db, int n);
void free_score_records(ScoreRecord *records, int n);

#endif // GAME_FILEDB_H
