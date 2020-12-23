#include "Page.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

//////* Работа с отдельными страницами *//////

///* Создать новую страницу *///
void pgInit(Page *pg, size_t page_size, char *Name, char *MetaFileName, size_t KeySize, size_t DataSize, Destructor dtor) {
    pg->pgSize = page_size;
    pg->elemSize = DataSize;
    pg->keySize = KeySize;
    HashTable ht;
    ht_init(&ht, pg->pgSize, 0, dtor);
    pg->ht = ht;
    pg->PageName = Name;
    pg->pgMetaDataFile = MetaFileName;
}

///* Уничтожить существующую страницу *///
void pgDestroy(Page *pg) {
    ht_destroy(&pg->ht);
    pg->pgSize = -1;
    pg->keySize = -1;
    pg->elemSize = -1;
    pg->PageName = "Destroyed";
    pg->pgMetaDataFile = "Destroyed";
}

///* Загрузить данные страницы из файла *///
Pointer pgFromFile(Page *pg, char *filename, char *metafilename) {
    pg->PageName = filename;
    pg->pgMetaDataFile = metafilename;
    FILE *source_file = fopen(filename, "r");
    FILE *metadata = fopen(metafilename, "r");
    if (source_file == NULL) {
        printf(FILE_OPENING_ERROR"pgFromFile||source_file\n");
        exit(1);
    }
    if (metadata == NULL) {
        printf(FILE_OPENING_ERROR"pgFromFile||metadata\n");
        exit(1);
    }
    fscanf(source_file, "%d", &(pg->pgSize));
    fscanf(metadata, "%d\n%d", &(pg->keySize), &(pg->elemSize));
    char *key = NULL;
    Pointer data = NULL;
    for (int i = 0; i < pg->pgSize; i++) {
        fscanf(source_file, "%s", key);
        fread(data, pg->elemSize, 1, source_file);
        ht_set(&pg->ht, key, data);
        key = NULL;
        data = NULL;
    }
    fclose(source_file);
    fclose(metadata);
    return pg;
}

///* Загрузить страницу в файл *///
void pgToFile(Page *pg) {
    FILE *destination_file = fopen(pg->PageName, "wb+");
    FILE *metadata = fopen(pg->pgMetaDataFile, "wb");
    if (destination_file == NULL) {
        printf(FILE_OPENING_ERROR"pgToFile\n");
        exit(1);
    }
    if (metadata == NULL) {
        printf(FILE_OPENING_ERROR"pgToFile||metadata\n");
        exit(1);
    }
    fprintf(metadata, "%d\n", pg->keySize);
    fprintf(metadata, "%d\n", pg->elemSize);
    for (int i = 0; i < pg->pgSize; i++) {
        if (strcmp(pg->PageName, "Empty") != 0) {
            if (pg->ht.table[i].head != NULL) {
                fwrite(pg->ht.table[i].head->key, pg->keySize, 1, destination_file);
                fprintf(destination_file,"\n");
                if ((pg->ht.table[i].head->data) == NULL) {
                    fprintf(destination_file, "%s", "NULL");
                } else {
                    fwrite(*(pg->ht.table[i].head->data), pg->elemSize, 1, destination_file); //!!!!!!!!!!!!!!!!
                }
            }
        }
    }
    fclose(destination_file);
    fclose(metadata);
}

#pragma clang diagnostic pop