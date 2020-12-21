#include <stdio.h>
#include <corecrt_malloc.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "quick_sort.h"

#define MEMORY_ERROR_MESSAGE "Failing allocating memory in function:"
#define FILE_OPENING_ERROR "Failing opening file in function:"

typedef void (*Sort)(void *arr, unsigned int len_arr,
                     size_t size_elem, int (*cmp)(void *, void *));

typedef struct Page {
    char *PageName;
    HashTable *ht;
    size_t pgSize;
    Pointer elem;
    struct Page *relative;
} Page;

typedef struct DataBase {
    char *DataBaseName;
    size_t dbSize;
    Sort srtFunc;
    struct Page *pages;
} DataBase;

//////* Работа с отдельными страницами *//////

///* Создать новую страницу *///
void pgInit(Page *pg, size_t page_size, char *Name, Pointer elem, Destructor dtor) {
    pg->pgSize = page_size;
    ht_init(pg->ht, pg->pgSize, 0, dtor);
    pg->elem = elem;
    pg->PageName = Name;
    pg->relative = NULL;
}

///* Уничтожить существующую страницу *///
void pgDestroy(Page *pg) {
    ht_destroy(pg->ht);
    pg->pgSize = -1;
    pg->PageName = "Destroyed";
    pg->relative = NULL;
    pg->elem = NULL;
}

///* Добавить к странице связь с другой страницей *///
void pgAddRelation(Page *pg, Page *rel) {
    pg->relative = rel;
}

///* Удалить связь с другой страницей *///
void pgDeleteRelation(Page *pg) {
    pg->relative = NULL;
}

///* Загрузить данные страницы из файла *///
void pgFromFile(Page *pg, char *filename) {
    FILE *source_file = fopen(filename, "r");
    if (source_file == NULL) {
        printf(FILE_OPENING_ERROR"pgToFile\n");
        exit(1);
    }
    fread(pg, sizeof(&pg), 1, source_file);
    fclose(source_file);
}

///* Загрузить страницу в файл *///
void pgToFile(Page *pg) {
    FILE *destination_file = fopen(pg->PageName, "w+");
    if (destination_file == NULL) {
        printf(FILE_OPENING_ERROR"pgToFile\n");
        exit(1);
    }
    fwrite(pg, sizeof(&pg), 1, destination_file);
    fclose(destination_file);
}

//////* Работа с базой данных *//////

///* Создать новую базу данных *///
void dbInit(DataBase *db, size_t page_amount, Sort srt_func, char *db_name) {
    db->DataBaseName = db_name;
    db->dbSize = page_amount;
    if(srt_func == 0) {
        db->srtFunc = quick_sort;
    } else {
        db->srtFunc = srt_func;
    }
    db->pages = calloc(db->dbSize, sizeof(struct Page));
    if (db->pages == NULL) {
        printf(MEMORY_ERROR_MESSAGE"dbInit\n");
        exit(1);
    }
    for (int i = 0; i < db->dbSize; i++) {
        db->pages[i].PageName = "Empty";
    }
}

///* Добавить страницу в базу данных *///
Pointer dbAddPage(DataBase *db, Page pg) {
    int i = 0;
    while (strcmp(db->pages[i].PageName, "Empty") != 0) {
        i++;
    }
    db->pages[i] = pg;
    return db;
}

///* Уничтожить базу данных *///
void dbDestroy(DataBase *db) {
    for (int i = 0; i < db->dbSize; i++) {
        pgDestroy(&(db->pages[i]));
    }
    free(db->pages);
    db->DataBaseName = "Destroyed";
    db->dbSize = -1;
    db->srtFunc = NULL;
}

///* Добавить данные в базу данных на заданную страницу *///
Pointer dbAdd(DataBase *db, char *key, Pointer data, const char *PageName) {
    int i = 0;
    while (db->pages[i].PageName != PageName) {
        i++;
    }
    int cur_num = i;
    ht_set(db->pages[cur_num].ht, key, data);
    return db;
}

///* Проверить наличие данных в базе *///
int dbHas(DataBase *db, char *key) {
    int i = 0;
    while (ht_has(db->pages[i].ht, key) == 0) {
        i++;
    }
    return ht_has(db->pages[i].ht, key);
}

///* Найти данные в базе *///
Pointer dbFind(DataBase *db, char *key) {
    int i = 0;
    while (ht_get(db->pages[i].ht, key) != 0) {
        i++;
    }
    return ht_get(db->pages[i].ht, key);
}

///* Удалить данные из базы *///
void dbDelete(DataBase *db, char *key) {
    if (dbHas(db, key) == 0) {
        exit(0);
    } else {
        int i = 0;
        while (ht_has(db->pages[i].ht, key) == 0) {
            i++;
        }
        ht_delete(db->pages[i].ht, key);
    }
}

///* Удалить страницу из базы данных *///
void dbDeletePage(DataBase *db, char *PageName) {
    int i = 0;
    while (strcmp(db->pages[i].PageName, PageName) != 0) {
        i++;
    }
    pgDestroy(&(db->pages[i]));
}

///* Сохранить базу данных в файл *///
void saveDb(DataBase *db) {
    FILE *destination_file = fopen(db->DataBaseName, "w+");
    if (destination_file == NULL) {
            printf(FILE_OPENING_ERROR"saveDb\n");
            exit(1);
    }
    for (int i = 0; i < db->dbSize; i++) {
        pgToFile(&(db->pages[i]));
    }
    fwrite(db->pages, sizeof(Page), db->dbSize, destination_file);
    fwrite((const void *) db->dbSize, sizeof(db->dbSize), 1, destination_file);
    fwrite(db->srtFunc, sizeof(db->srtFunc), 1, destination_file);
    fclose(destination_file);
}

///* Загрузить базу данных из файла *///
Pointer loadDb(DataBase *db, char *filename) {
    FILE *source_file = fopen(filename, "r");
    if (source_file == NULL) {
        printf(FILE_OPENING_ERROR"loadDb\n");
        exit(1);
    }
    for (int i = 0; i < db->dbSize; i++) {
        pgFromFile(&(db->pages[i]), db->pages[i].PageName);
    }
    fread(db->pages, sizeof(db->pages), 1, source_file);
    fread((void *) db->dbSize, sizeof(db->dbSize), 1, source_file);
    fread(db->srtFunc, sizeof(db->srtFunc), 1, source_file);
    fclose(source_file);
    return db;
}

int main() {
    return 0;
}
