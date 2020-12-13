#ifndef ADVANCED_QCK_SRT_QUICK_SORT_H
#define ADVANCED_QCK_SRT_QUICK_SORT_H

int int_cmp(void *x, void *y);
double double_cmp(void *x, void *y);
void swap(void *a, void *b, size_t size);
void quick_sort(void *arr, unsigned int len_arr,
                size_t size_elem, int (*cmp)(void *, void *));

#endif 