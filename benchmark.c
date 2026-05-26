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
#include <math.h>
#include <sys/stat.h>
#include "sortlib.h"

#define MAX_SECONDS 900.0
#define RUNS        10
#define P_MIN        8
#define P_MAX       15
#define P_COUNT     (P_MAX - P_MIN + 1)

static int compareInts(const void *a, const void *b) {
    return *(const int *)a - *(const int *)b;
}

static double measure(void (*fn)(void *, size_t, size_t, int (*)(const void *, const void *)),
                      int *src, size_t n) {
    // Warmup
    int *tmp = malloc(n * sizeof(int));
    memcpy(tmp, src, n * sizeof(int));
    fn(tmp, n, sizeof(int), compareInts);
    free(tmp);

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

static void writeIdeal(const char *path, double *times,
                       double (*theory)(double)) {
    int calP = -1;
    double tCal = 0.0;
    for (int i = P_COUNT - 1; i >= 0; i--) {
        if (times[i] > 0.0) { calP = P_MIN + i; tCal = times[i]; break; }
    }
    if (calP < 0) return;

    double nCal = (double)((size_t)1 << calP);
    double C = tCal / theory(nCal);

    FILE *f = fopen(path, "w");
    if (!f) return;
    for (int p = P_MIN; p <= P_MAX; p++) {
        double n = (double)((size_t)1 << p);
        fprintf(f, "%.0f %.9f\n", n, C * theory(n));
    }
    fclose(f);
}

static double quadratic(double n)    { return n * n; }
static double linearithmic(double n) { return n * log2(n); }
static double linear(double n)       { return n; }

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

    double tInsRand[P_COUNT]  = {0};
    double tHeapRand[P_COUNT] = {0};
    double tInsSort[P_COUNT]  = {0};
    double tHeapSort[P_COUNT] = {0};
    double tInsRev[P_COUNT]   = {0};
    double tHeapRev[P_COUNT]  = {0};

    for (int p = P_MIN; p <= P_MAX; p++) {
        size_t n = (size_t)1 << p;
        int idx = p - P_MIN;
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

#define MRUN(stop, fn, src, file, arr) \
        if (!stop) { \
            double t = measure(fn, src, n); \
            arr[idx] = t; \
            fprintf(file, "%zu %.9f\n", n, t); \
            fflush(file); \
            if (t > MAX_SECONDS) stop = 1; \
        }

        MRUN(stopIns[0],  insertionSort, rnd, fInsRand,  tInsRand)
        MRUN(stopHeap[0], heapSort,      rnd, fHeapRand, tHeapRand)
        MRUN(stopIns[1],  insertionSort, srt, fInsSort,  tInsSort)
        MRUN(stopHeap[1], heapSort,      srt, fHeapSort, tHeapSort)
        MRUN(stopIns[2],  insertionSort, rev, fInsRev,   tInsRev)
        MRUN(stopHeap[2], heapSort,      rev, fHeapRev,  tHeapRev)
#undef MRUN

        free(rnd); free(srt); free(rev);
    }

    fclose(fInsRand); fclose(fHeapRand);
    fclose(fInsSort); fclose(fHeapSort);
    fclose(fInsRev);  fclose(fHeapRev);

    writeIdeal("data/ideal_insertion_random.dat",  tInsRand,  quadratic);
    writeIdeal("data/ideal_heap_random.dat",        tHeapRand, linearithmic);
    writeIdeal("data/ideal_insertion_sorted.dat",   tInsSort,  linear);
    writeIdeal("data/ideal_heap_sorted.dat",        tHeapSort, linearithmic);
    writeIdeal("data/ideal_insertion_reverse.dat",  tInsRev,   quadratic);
    writeIdeal("data/ideal_heap_reverse.dat",       tHeapRev,  linearithmic);

    printf("Done. Run: gnuplot plot.gp\n");
    return 0;
}
