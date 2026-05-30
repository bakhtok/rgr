#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "sortlib.h"
#include "bitstruct.h"

static int compareInts(const void *a, const void *b) {
    return *(const int *)a - *(const int *)b;
}

static int compareAtomsByZ(const void *a, const void *b) {
    return (int)getNumberOfTable((const atom *)a) - (int)getNumberOfTable((const atom *)b);
}

static void printUsage(const char *prog) {
    printf("Usage: %s [-a insertion|heap] [-n size] [-t int|atom] [-o random|sorted|reverse|stdin]\n", prog);
}

int main(int argc, char *argv[]) {
    char *alg = "insertion";
    int n = 10;
    char *type = "int";
    char *order = "random";
    int opt;

    while ((opt = getopt(argc, argv, "a:n:t:o:h")) != -1) {
        switch (opt) {
            case 'a': alg = optarg;       break;
            case 'n': n = atoi(optarg);   break;
            case 't': type = optarg;      break;
            case 'o': order = optarg;     break;
            default:  printUsage(argv[0]); return opt == 'h' ? 0 : 1;
        }
    }

    void (*sortFn)(void *, size_t, size_t, int (*)(const void *, const void *)) = NULL;
    if (strcmp(alg, "insertion") == 0) sortFn = insertionSort;
    else if (strcmp(alg, "heap") == 0) sortFn = heapSort;
    else { fprintf(stderr, "Unknown algorithm: %s\n", alg); return 1; }

    srand((unsigned)time(NULL));

    if (strcmp(type, "int") == 0) {
        int *arr = malloc((size_t)n * sizeof(int));
        if (strcmp(order, "stdin") == 0) {
            printf("Enter %d integers:\n", n);
            for (int i = 0; i < n; i++) {
                if (scanf("%d", &arr[i]) != 1) {
                    fprintf(stderr, "Read error at position %d\n", i);
                    free(arr); return 1;
                }
            }
        } else {
            for (int i = 0; i < n; i++) {
                if (strcmp(order, "sorted") == 0) arr[i] = i;
                else if (strcmp(order, "reverse") == 0) arr[i] = n - i;
                else arr[i] = rand() % 1000;
            }
        }
        printf("Before: ");
        for (int i = 0; i < n; i++) printf("%d ", arr[i]);
        printf("\n");
        sortFn(arr, (size_t)n, sizeof(int), compareInts);
        printf("After:  ");
        for (int i = 0; i < n; i++) printf("%d ", arr[i]);
        printf("\n");
        free(arr);

    } else if (strcmp(type, "atom") == 0) {
        atom *arr = malloc((size_t)n * sizeof(atom));
        int maxZ = n < 118 ? n : 118;
        for (int i = 0; i < n; i++) {
            int z = (rand() % maxZ) + 1;
            atom *a = init(z);
            arr[i] = *a;
            deinit(&a);
        }
        printf("Before:\n");
        for (int i = 0; i < n; i++) printAtomInfo("  ", &arr[i]);
        sortFn(arr, (size_t)n, sizeof(atom), compareAtomsByZ);
        printf("After (sorted by Z):\n");
        for (int i = 0; i < n; i++) printAtomInfo("  ", &arr[i]);
        free(arr);

    } else {
        fprintf(stderr, "Unknown type: %s\n", type);
        return 1;
    }

    return 0;
}
