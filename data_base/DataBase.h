#include <stdio.h>
#include <assert.h>
#include <corecrt_malloc.h>
#include <stdlib.h>
#include <string.h>
#include "Page.h"
#include "hash_table.h"
#include "quick_sort.h"

#define MEMORY_ERROR_MESSAGE "Failing allocating memory in function:"
#define FILE_OPENING_ERROR "Failing opening file in function:"

#ifndef DATA_BASE_DATABASE_H
#define DATA_BASE_DATABASE_H

typedef void (*Sort)(void *arr, unsigned int len_arr,
                     size_t size_elem, int (*cmp)(void *, void *));
typedef int (*cmp)(void *, void *);

typedef struct DataBase {
    char *DataBaseName;
    char *dbMetaDataFile;
    size_t dbSize;
    Sort srtFunc;
    Page *pages;
} DataBase;

void dbInit(DataBase *db, size_t page_amount, char *db_name, char *db_meta_name);
Pointer dbAddPage(DataBase *db, Page *pg);
void dbSort(DataBase *db,char *PageName, cmp cmp);
void dbDestroy(DataBase *db);
Pointer dbAdd(DataBase *db, char *key, Pointer data, const char *PageName);
int dbHas(DataBase *db, char *key);
Pointer dbFind(DataBase *db, char *key);
void dbDelete(DataBase *db, char *key);
void dbDeletePage(DataBase *db, char *PageName);
void saveDb(DataBase *db);
Pointer loadDb(DataBase *db, char *dirname, char *metafile);

#endif //DATA_BASE_DATABASE_H
