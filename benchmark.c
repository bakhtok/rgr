#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "sortlib.h"

#define N_START  5000
#define N_STEP   5000
#define N_END   50000
#define N_COUNT ((N_END - N_START) / N_STEP + 1)

static int compareInts(const void *a, const void *b) {
    return *(const int *)a - *(const int *)b;
}

static void measure(void (*fn)(void *, size_t, size_t, int (*)(const void *, const void *)),
                    int *src, size_t n, size_t *swaps, size_t *comparisons) {
    int *arr = malloc(n * sizeof(int));
    memcpy(arr, src, n * sizeof(int));
    g_swaps = 0;
    g_comparisons = 0;
    fn(arr, n, sizeof(int), compareInts);
    *swaps = g_swaps;
    *comparisons = g_comparisons;
    free(arr);
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

    FILE *all[] = { fInsRand, fHeapRand, fInsSort, fHeapSort, fInsRev, fHeapRev };
    for (int f = 0; f < 6; f++) fprintf(all[f], "0 0 0\n");

    for (int i = 0; i < N_COUNT; i++) {
        size_t n = (size_t)(N_START + i * N_STEP);
        printf("n = %zu\n", n);
        fflush(stdout);

        int *rnd = malloc(n * sizeof(int));
        int *srt = malloc(n * sizeof(int));
        int *rev = malloc(n * sizeof(int));

        srand(12345);
        for (size_t k = 0; k < n; k++) {
            rnd[k] = rand();
            srt[k] = (int)k;
            rev[k] = (int)(n - 1 - k);
        }

        size_t sw, cmp;

#define RUN(fn, src, file) \
        measure(fn, src, n, &sw, &cmp); \
        fprintf(file, "%zu %zu %zu\n", n, sw, cmp); \
        fflush(file);

        RUN(insertionSort, rnd, fInsRand)
        RUN(heapSort,      rnd, fHeapRand)
        RUN(insertionSort, srt, fInsSort)
        RUN(heapSort,      srt, fHeapSort)
        RUN(insertionSort, rev, fInsRev)
        RUN(heapSort,      rev, fHeapRev)
#undef RUN

        free(rnd); free(srt); free(rev);
    }

    fclose(fInsRand); fclose(fHeapRand);
    fclose(fInsSort); fclose(fHeapSort);
    fclose(fInsRev);  fclose(fHeapRev);

    printf("Done. Run: gnuplot plot.gp\n");
    return 0;
}
