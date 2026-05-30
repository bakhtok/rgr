#ifndef sortlib_h
#define sortlib_h

#include <stddef.h>

extern size_t g_swaps;
extern size_t g_comparisons;

void insertionSort(void *base, size_t nmemb, size_t size,
                   int (*compar)(const void *, const void *));

void heapSort(void *base, size_t nmemb, size_t size,
              int (*compar)(const void *, const void *));

#endif
