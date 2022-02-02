#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quick_sort.h"

int int_cmp(void *x, void *y){
    return *(int*)x - *(int*) y;
}

double double_cmp(void *x, void *y) {
    return *(double*)x - *(double*)y;
}

void swap(void *a, void *b, const size_t size) { // сделать с memcpy
    void * tmp = malloc(size);
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
    free(tmp);
}

void quick_sort(void *arr, unsigned int len_arr,
                size_t size_elem, int (*cmp)(void *, void *)) {
    if (len_arr == 1) {
        return;
    }
    unsigned int i = 0; // индексы левой половины
    unsigned int j = len_arr - 1; // индексы правой половины
    unsigned int median = (len_arr / 2);
    do {
        while (cmp((char *) arr + i * size_elem, (char *) arr + median * size_elem) < 0) { //left < median
            i++;
        }
        while (cmp((char *) arr + j * size_elem, (char *) arr + median * size_elem) > 0) { //right > median
            j--;
        }
        if (i <= j) {  // after while i <= j thereat we must swap left and right
            swap((char *) arr + i * size_elem, (char *) arr + j * size_elem, size_elem);
            i++;
            j--;
        }
    } while (i <= j);
    // recursion
    if (len_arr > i) quick_sort((char *) arr + i * size_elem, median, size_elem, cmp); //right
    if (j > 0) quick_sort((char *) arr, median + 1, size_elem, cmp); // left
}