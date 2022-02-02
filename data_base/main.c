#include "DataBase.h"
#include "Page.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#define MEMORY_ERROR_MESSAGE "Failing allocating memory in function:"
#define FILE_OPENING_ERROR "Failing opening file in function:"

typedef struct USERS {
    int age;
    char *Name;
    char *Surname;
} USERS;

void destructor(Pointer data) {
    data = NULL;
}

void destructor1(Pointer data) {
    free(data);
}

int main() {
    DataBase test_db;
    Page users;
    USERS *user1 = malloc(sizeof(USERS));
    if (user1 == NULL) {
        printf("Memory error\n");
        exit(1);
    }
    user1->Name = "Ivan";
    user1->Surname = "Ivanov";
    user1->age = 28;

    char *pagename = "C:\\Project\\data_base\\cmake-build-debug\\CMakeFiles\\test_db\\Users";
    char *pgmeta = "C:\\Project\\data_base\\cmake-build-debug\\CMakeFiles\\test_db\\Users_meta";
    char *db_name = "C:\\Project\\data_base\\cmake-build-debug\\CMakeFiles\\test_db";
    char *dbmeta = "C:\\Project\\data_base\\cmake-build-debug\\CMakeFiles\\test_db\\test_db_meta";

    dbInit(&test_db, 2, db_name, dbmeta);
    pgInit(&users, 5, pagename, pgmeta, sizeof "IVANOV", 76, destructor1);
    dbAddPage(&test_db, &users);
    dbAdd(&test_db, "IVANOV", user1, pagename);

    assert(dbHas(&test_db, "IVANOV") == 1);
    assert(dbHas(&test_db, "PETROV") == 0);

    saveDb(&test_db);
    dbDestroy(&test_db);

    return 0;
}

#pragma clang diagnostic pop