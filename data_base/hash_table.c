#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash_table.h"


///* Jenkins Hash Function *///
unsigned jenkins_one_at_a_time_hash(char *key) {
    unsigned hash = 0;

    for (; *key; ++key) {
        hash += *key;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

///* Инициализировать таблицу.
///* size - размер базового массива;
/// * hf   - хеш-функция;
/// * dtor - деструктор. Этой функции будут передаваться data удаляемых элементов
/// * (ht_destroy, ht_delete, а также при перезаписи элементов в ht_set).
/// * Если hf=0, берется стандартная функция (Дженкинс).
/// * Если dtor=0, деструктор отсутствует (не будет вызываться).
void ht_init(HashTable *ht, size_t size, HashFunction hf, Destructor dtor) {
    ht->size = size;
    if (hf == 0) ht->hashfunc = jenkins_one_at_a_time_hash;
    ht->dtor = dtor;
    ht->table = calloc(size, sizeof(struct List));
    if (ht->table == NULL) {
        printf("Memory error\n");
        exit(1);
    }
}

///* Уничтожить таблицу *///
void ht_destroy(HashTable *ht) {
    for (int i = 0; i < ht->size; i++) {
        ListNode **node =&(ht->table[i].head);
        while ((*node) != NULL) {
            ht->dtor((*node)->data);
            //node->data = NULL;
            ListNode **next = &((*node)->next);
            (*node) = *next;
        }
    }
    free(ht->table);
    ht->table = NULL;
    ht->size = -1;
}

///* Записать в таблицу соответствие key -> data. Если key уже существовал,
/// * соотв. поле data будет удалено (dtor) и перезаписано
Pointer ht_set(HashTable *ht, char *key, Pointer data) {
    unsigned hash = ht->hashfunc(key) % ht->size;
    ListNode **node = &(ht->table[hash].head);
    while ((*node) != NULL) {
        if (strcmp((*node)->key, key) == 0) {
            // нужно позвать деструктор
            ht->dtor((*node)->data);
            (*node)->data = data;
        } else {
            (*node) = (*node)->next;
        }
    }
    if ((*node) == NULL) {
        (*node) = calloc(1, sizeof(struct ListNode));
        if (node == NULL) {
            printf("Memory error\n");
            exit(1);
        }
        (*node)->key = key;
        (*node)->data = data;
        (*node)->next = NULL;
    }
    return ht;
}

///* Получить значение по ключу. Если ключа нет в таблице, вернуть 0. *///
Pointer ht_get(HashTable *ht, char *key) {
    unsigned hash = ht->hashfunc(key) % ht->size;
    ListNode **node = &(ht->table[hash].head);
    while ((*node) != NULL) {
        if (strcmp((*node)->key, key) == 0) {
            return (*node)->data;
        } else {
            (*node) = (*node)->next;
        }
    }
    if ((*node) == NULL) {
        return 0;
    }
}

///* Проверка существования ключа key в таблице. 1 - есть, 0 - нет. *///
int ht_has(HashTable *ht, char *key) {
    if(ht_get(ht, key) == 0) return 0;
    else if (ht_get(ht, key) != 0) return 1;
}

///* Удалить элемент с ключом key из таблицы (если он есть) *///
void ht_delete(HashTable *ht, char *key) {
    unsigned hash = ht->hashfunc(key) % ht->size;
    if (ht_has(ht, key) == 0) {
        exit(0);
    }else {
        ListNode **node = &(ht->table[hash].head);
        while (strcmp((*node)->key, key) != 0) {
            (*node) = (*node)->next;
        }
        ht->dtor(ht->table[hash].head->data);
        ht->table[hash].head = NULL;
    }
}

///* Обход таблицы с посещением всех элементов. Функция f будет вызвана для
/// * всех пар (key, data) из таблицы *///
void ht_traverse(HashTable *ht, void (*f)(char *key, Pointer data)) {
    for (int i = 0; i < ht->size; i++) {
        ListNode **node = &(ht->table[i].head);
        if((*node) != NULL) {
            f((*node)->key, (*node)->data);
            ListNode **nodenext = &((*node)->next);
            while ((*nodenext) != NULL) {
                f((*nodenext)->key, (*nodenext)->data);
                (*nodenext) = (*nodenext)->next;
            }
        }
    }
}

///* Изменить размер базового массива *///
void ht_resize(HashTable *ht, size_t new_size) {
    List *newtable = calloc(new_size, sizeof(struct List));
    if (newtable == NULL) {
        printf("Memory error");
        exit(1);
    }
    for (int i = 0; i < ht->size; i++) {
        ListNode **node = &(ht->table[i].head);
        ListNode **newnode = &(newtable[i].head);
        if((*node) != NULL) {
            (*newnode) = calloc(1, sizeof(struct ListNode));
            (*newnode)->data = (*node)->data;
            (*newnode)->key = (*node)->key;
            ListNode **nodenext = &((*node)->next);
            ListNode **newnodenext = &((*newnode)->next);
            while ((*nodenext) != NULL) {
                (*newnodenext) = calloc(1, sizeof(struct ListNode));
                (*newnodenext)->key = (*nodenext)->key;
                (*newnodenext)->data = (*nodenext)->data;
                (*nodenext) = (*nodenext)->next;
                (*newnodenext) = (*newnodenext)->next;
            }
        }
    }
    ht->table = newtable;
    ht->size = new_size;
}
