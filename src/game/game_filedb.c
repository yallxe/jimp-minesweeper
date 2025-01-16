#include "game_filedb.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

int direxists(const char *path)
{

    DIR *dir = opendir(path);
    if (dir)
    {
        /* Directory exists. */
        closedir(dir);
        return 1;
    }
    else if (ENOENT == errno)
    {
        /* Directory does not exist. */
        return 0;
    }
    else
    {
        /* opendir() failed for some other reason. */
        return -1;
    }
}

DbHandle *open_db_handle()
{
    if (direxists("gamedata") == 0)
    {
        mkdir("gamedata", 0777);
    }

    DbHandle *db = malloc(sizeof(DbHandle));
    db->scores_f = fopen("gamedata/scores.txt", "a+");
    return db;
}

int check_db_integrity(DbHandle *db)
{
    if (db->scores_f == NULL)
    {
        return 0;
    }

    char username[100];
    int seconds;
    int difficulty;
    while (fscanf(db->scores_f, "%s %d %d\n", username, &seconds, &difficulty) != EOF)
    {
        if (seconds < 0 || difficulty < 0 || difficulty > 3)
        {
            return 0;
        }
    }

    rewind(db->scores_f);

    return 1;
}

void save_score(DbHandle *db, char *username, int seconds, GameDifficulty difficulty)
{
    fprintf(db->scores_f, "%s %d %d\n", username, seconds, difficulty);
    fflush(db->scores_f);
}

int compare_scores(const void *a, const void *b)
{
    ScoreRecord *recordA = (ScoreRecord *)a;
    ScoreRecord *recordB = (ScoreRecord *)b;
    return recordA->seconds - recordB->seconds;
}

ScoreRecord *get_top_scores(DbHandle *db, int n)
{
    // First, count the number of records in the file
    int count = 0;
    char username[100];
    int seconds;
    int difficulty;
    rewind(db->scores_f);
    while (fscanf(db->scores_f, "%s %d %d\n", username, &seconds, &difficulty) != EOF)
    {
        count++;
    }

    // Allocate memory for all records
    ScoreRecord *all_records = malloc(sizeof(ScoreRecord) * count);
    for (int i = 0; i < count; i++)
    {
        all_records[i].name = malloc(sizeof(char) * 100);
    }

    // Read all records from the file
    rewind(db->scores_f);
    int i = 0;
    while (fscanf(db->scores_f, "%s %d %d\n", username, &seconds, &difficulty) != EOF)
    {
        strncpy(all_records[i].name, username, 100);
        all_records[i].seconds = seconds;
        // all_records[i].difficulty = difficulty;
        i++;
    }

    // Sort all records
    qsort(all_records, count, sizeof(ScoreRecord), compare_scores);

    // Allocate memory for top n records or fewer if there are not enough records
    int top_count = count < n ? count : n;
    ScoreRecord *top_records = malloc(sizeof(ScoreRecord) * n);
    for (int i = 0; i < top_count; i++)
    {
        top_records[i].name = malloc(sizeof(char) * 100);
        strncpy(top_records[i].name, all_records[i].name, 100);
        top_records[i].seconds = all_records[i].seconds;
        // top_records[i].difficulty = all_records[i].difficulty;
    }

    // Free memory for all records
    for (int i = 0; i < count; i++)
    {
        free(all_records[i].name);
    }
    free(all_records);

    return top_records;
}

void free_score_records(ScoreRecord *records, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(records[i].name);
    }
    free(records);
}