#include <stdio.h>
#include <corecrt_malloc.h>
#include <stdlib.h>
#include "hash_table.h"
#include "quick_sort.h"
typedef void (*Sort)(Pointer);

typedef struct DataBase {
    Pointer storage;
    size_t size_of_db;
    Sort sort_func;
} DataBase;

///* This function initializes the database with the given parameters *///
void db_init(DataBase *db, size_t sizeofdb, Sort srt_fnc, Pointer elem) {
    db->size_of_db = sizeofdb;
    if (srt_fnc == 0) {
        db->sort_func = (Sort) quick_sort;
    }else {
        db->sort_func = srt_fnc;
    }
    db->storage = calloc(db->size_of_db, sizeof(elem));
    if (db->storage == NULL) {
        printf("Memory allocating error inside init_data_base()\n");
        exit(1);
    }
}

void db_destroy(DataBase *db) {
    for (int i = 0; i < db->size_of_db; i++) {

    }
}

FILE read_from_file();

Pointer db_add(DataBase *db, char *key, Pointer data);

Pointer db_delete();

void db_output();


int main() {
    DataBase test_db;

    return 0;
}
