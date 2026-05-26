//
//  benchmark.c
//  rgr
//
//  Created by Bakhtovar Akhmedov on 26.05.2026.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "sortlib.h"

#define MAX_SECONDS 900.0
#define RUNS        5

static int compareInts(const void *a, const void *b) {
    return *(const int *)a - *(const int *)b;
}

static double measure(void (*fn)(void *, size_t, size_t, int (*)(const void *, const void *)),
                      int *src, size_t n) {
    double total = 0.0;
    for (int r = 0; r < RUNS; r++) {
        int *arr = malloc(n * sizeof(int));
        memcpy(arr, src, n * sizeof(int));

        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        fn(arr, n, sizeof(int), compareInts);
        clock_gettime(CLOCK_MONOTONIC, &t1);

        free(arr);
        double t = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;
        total += t;
        if (t > MAX_SECONDS) return total / (r + 1);
    }
    return total / RUNS;
}

int main(void) {
    mkdir("data",  0755);
    mkdir("plots", 0755);

    FILE *fInsRand  = fopen("data/insertion_random.dat",  "w");
    FILE *fHeapRand = fopen("data/heap_random.dat",       "w");
    FILE *fInsSort  = fopen("data/insertion_sorted.dat",  "w");
    FILE *fHeapSort = fopen("data/heap_sorted.dat",       "w");
    FILE *fInsRev   = fopen("data/insertion_reverse.dat", "w");
    FILE *fHeapRev  = fopen("data/heap_reverse.dat",      "w");

    int stopIns[3]  = {0, 0, 0};
    int stopHeap[3] = {0, 0, 0};

    for (int p = 8; p <= 15; p++) {
        size_t n = (size_t)1 << p;
        printf("n = %zu\n", n);
        fflush(stdout);

        int *rnd = malloc(n * sizeof(int));
        int *srt = malloc(n * sizeof(int));
        int *rev = malloc(n * sizeof(int));

        srand(12345);
        for (size_t i = 0; i < n; i++) {
            rnd[i] = rand();
            srt[i] = (int)i;
            rev[i] = (int)(n - 1 - i);
        }

        if (!stopIns[0])  { double t = measure(insertionSort, rnd, n); fprintf(fInsRand,  "%zu %.9f\n", n, t); if (t > MAX_SECONDS) stopIns[0]  = 1; }
        if (!stopHeap[0]) { double t = measure(heapSort,      rnd, n); fprintf(fHeapRand, "%zu %.9f\n", n, t); if (t > MAX_SECONDS) stopHeap[0] = 1; }

        if (!stopIns[1])  { double t = measure(insertionSort, srt, n); fprintf(fInsSort,  "%zu %.9f\n", n, t); if (t > MAX_SECONDS) stopIns[1]  = 1; }
        if (!stopHeap[1]) { double t = measure(heapSort,      srt, n); fprintf(fHeapSort, "%zu %.9f\n", n, t); if (t > MAX_SECONDS) stopHeap[1] = 1; }

        if (!stopIns[2])  { double t = measure(insertionSort, rev, n); fprintf(fInsRev,   "%zu %.9f\n", n, t); if (t > MAX_SECONDS) stopIns[2]  = 1; }
        if (!stopHeap[2]) { double t = measure(heapSort,      rev, n); fprintf(fHeapRev,  "%zu %.9f\n", n, t); if (t > MAX_SECONDS) stopHeap[2] = 1; }

        free(rnd); free(srt); free(rev);
    }

    fclose(fInsRand); fclose(fHeapRand);
    fclose(fInsSort); fclose(fHeapSort);
    fclose(fInsRev);  fclose(fHeapRev);

    printf("Done. Run: gnuplot plot.gp\n");
    return 0;
}
