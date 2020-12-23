#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hash_table.h"
#include "quick_sort.h"

#define MEMORY_ERROR_MESSAGE "Failing allocating memory in function:"
#define FILE_OPENING_ERROR "Failing opening file in function:"

#ifndef DATA_BASE_PAGE_H
#define DATA_BASE_PAGE_H

typedef struct Page {
    char *PageName;
    char *pgMetaDataFile;
    HashTable ht;
    size_t pgSize;
    size_t keySize;
    size_t elemSize;
} Page;

void pgInit(Page *pg, size_t page_size, char *Name, char *MetaFileName, size_t KeySize, size_t DataSize, Destructor dtor);
void pgDestroy(Page *pg);
Pointer pgFromFile(Page *pg, char *filename, char *metafilename);
void pgToFile(Page *pg);

#endif //DATA_BASE_PAGE_H
