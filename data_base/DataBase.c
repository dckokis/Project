#include "DataBase.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

//////* Работа с базой данных *//////

///* Создать новую базу данных *///
void dbInit(DataBase *db, size_t page_amount, char *db_name, char *db_meta_name) {
    db->DataBaseName = db_name;
    db->dbMetaDataFile = db_meta_name;
    db->dbSize = page_amount;
    db->srtFunc = quick_sort;
    db->pages = calloc(db->dbSize, sizeof(Page));
    if (db->pages == NULL) {
        printf(MEMORY_ERROR_MESSAGE"dbInit\n");
        exit(1);
    }
    for (int i = 0; i < db->dbSize; i++) {
        db->pages[i].PageName = "Empty";
        db->pages[i].pgMetaDataFile = "Empty";
    }
}

///* Добавить страницу в базу данных *///
Pointer dbAddPage(DataBase *db, Page *pg) {
    int i = 0;
    while (strcmp(db->pages[i].PageName, "Empty") != 0) {
        i++;
    }
    db->pages[i] = *pg;
    return db;
}

///* Отсортировать данные по выбранному полю *///
void dbSort(DataBase *db, char *PageName, cmp cmp) {
    int i = 0;
    while (strcmp(db->pages[i].PageName, PageName) != 0) {
        i++;
    }
    db->srtFunc(db->pages[i].ht.table, db->pages[i].ht.size, db->pages[i].elemSize, cmp);
}

///* Уничтожить базу данных *///
void dbDestroy(DataBase *db) {
    for (int i = 0; i < db->dbSize; i++) {
        pgDestroy(&(db->pages[i]));
    }
    free(db->pages);
    db->DataBaseName = "Destroyed";
    db->dbMetaDataFile = "Destroyed";
    db->dbSize = -1;
    db->srtFunc = NULL;
}

///* Добавить данные в базу данных на заданную страницу *///
Pointer dbAdd(DataBase *db, char *key, Pointer data, const char *PageName) {
    int i = 0;
    while (strcmp(db->pages[i].PageName, PageName) != 0) {
        i++;
    }
    int cur_num = i;
    ht_set(&db->pages[cur_num].ht, key, data);
    return db;
}

///* Проверить наличие данных в базе *///
int dbHas(DataBase *db, char *key) {
    for (int i = 0; i < db->dbSize; i++) {
        if (strcmp(db->pages[i].PageName, "Empty") != 0) {
            if (ht_has(&db->pages[i].ht, key) == 1) {
                return 1;
            } else {
                break;
            }
        }
    }
}

///* Найти данные в базе *///
Pointer dbFind(DataBase *db, char *key) {
    if (dbHas(db, key) == 0) {
        return 0;
    } else {
        for (int i = 0; i < db->dbSize; i++) {
            if (strcmp(db->pages[i].PageName, "Empty") != 0) {
                if (ht_has(&db->pages[i].ht, key) == 1) {
                    return ht_get(&db->pages[i].ht, key);
                } else {
                    break;
                }
            }
        }
    }
}

///* Удалить данные из базы *///
void dbDelete(DataBase *db, char *key) {
    if (dbHas(db, key) == 0) {
        exit(0);
    } else {
        int i = 0;
        while (ht_has(&db->pages[i].ht, key) == 0) {
            i++;
        }
        ht_delete(&db->pages[i].ht, key);
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

///* Сохранить базу данных на диск(создать папку с именем базы данных) *///
void saveDb(DataBase *db) {
    for (int i = 0; i < db->dbSize; i++) {
        if (strcmp(db->pages[i].PageName, "Empty") != 0) {
            pgToFile(&(db->pages[i]));
        }
    }
    FILE *metadata = fopen(db->dbMetaDataFile, "wb+");
    if (metadata == NULL) {
        printf(FILE_OPENING_ERROR"saveDb");
        exit(1);
    }
    fprintf(metadata, "%u\n", db->dbSize);
    for (int i = 0; i < db->dbSize; i++) {
        if (strcmp(db->pages[i].PageName, "Empty") != 0) {
            fprintf(metadata, "%s\t\t%s", db->pages[i].PageName, db->pages[i].pgMetaDataFile);
        }
    }
    fclose(metadata);
}

///* Загрузить базу данных с диска(из папки с названием базы данных) *///
Pointer loadDb(DataBase *db, char *dirname, char *metafile) {
    db->DataBaseName = dirname;
    db->dbMetaDataFile = metafile;
    FILE *metadata = fopen(metafile, "rb");
    if (metadata == NULL) {
        printf(FILE_OPENING_ERROR"loadDb");
        exit(1);
    }
    fscanf(metadata, "%d", &(db->dbSize));
    for (int i = 0; i < db->dbSize; i++) {
        fscanf(metadata, "%s\t\t%s", db->pages[i].PageName, db->pages[i].pgMetaDataFile);
    }
    fclose(metadata);
    return db;
}

#pragma clang diagnostic pop