//
//  sortlib.h
//  rgr
//
//  Created by Bakhtovar Akhmedov on 26.05.2026.
//

#ifndef sortlib_h
#define sortlib_h

#include <stddef.h>

void insertionSort(void *base, size_t nmemb, size_t size,
                   int (*compar)(const void *, const void *));

void heapSort(void *base, size_t nmemb, size_t size,
              int (*compar)(const void *, const void *));

#endif
