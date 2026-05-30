#include "sortlib.h"
#include <string.h>
#include <stdlib.h>

size_t g_swaps       = 0;
size_t g_comparisons = 0;

static void swapBuf(void *a, void *b, size_t size, char *buf) {
    ++g_swaps;
    memcpy(buf, a, size);
    memcpy(a, b, size);
    memcpy(b, buf, size);
}

void insertionSort(void *base, size_t nmemb, size_t size,
                   int (*compar)(const void *, const void *)) {
    if (nmemb < 2) return;
    char *arr = (char *)base;
    char *key = malloc(size);
    for (size_t i = 1; i < nmemb; i++) {
        memcpy(key, arr + i * size, size);
        long j = (long)i - 1;
        while (j >= 0 && (++g_comparisons, compar(arr + j * size, key)) > 0) {
            memcpy(arr + (j + 1) * size, arr + j * size, size);
            ++g_swaps;
            j--;
        }
        memcpy(arr + (j + 1) * size, key, size);
    }
    free(key);
}

static void siftDown(char *arr, size_t root, size_t end, size_t size,
                     int (*compar)(const void *, const void *), char *buf) {
    while (1) {
        size_t largest = root;
        size_t left  = 2 * root + 1;
        size_t right = 2 * root + 2;
        if (left  < end) { ++g_comparisons; if (compar(arr + left  * size, arr + largest * size) > 0) largest = left;  }
        if (right < end) { ++g_comparisons; if (compar(arr + right * size, arr + largest * size) > 0) largest = right; }
        if (largest == root) break;
        swapBuf(arr + root * size, arr + largest * size, size, buf);
        root = largest;
    }
}

void heapSort(void *base, size_t nmemb, size_t size,
              int (*compar)(const void *, const void *)) {
    if (nmemb < 2) return;
    char *arr = (char *)base;
    char *buf = malloc(size);
    for (long i = (long)(nmemb / 2) - 1; i >= 0; i--)
        siftDown(arr, (size_t)i, nmemb, size, compar, buf);
    for (size_t i = nmemb - 1; i > 0; i--) {
        swapBuf(arr, arr + i * size, size, buf);
        siftDown(arr, 0, i, size, compar, buf);
    }
    free(buf);
}
