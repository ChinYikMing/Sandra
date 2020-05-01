//
// Created by Bobo Sheep on 2020/3/23.
//
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils/test_num.h"

#define DFLT_FILENAME   "test_num.txt"
#define DFLT_TEST_SIZE  1024

int main(int argc, char *argv[]) {
    int bin_mode = 1, verbose = 0;
    int num_range = INT_MAX;
    size_t test_size = DFLT_TEST_SIZE;
    char *filename;

    int opt;
    char *tmp;
    while ((opt = getopt(argc, argv, "vts:r:")) != -1) {
        switch (opt) {
            case 'v':
                verbose = 1;
                break;
            case 't':
                bin_mode = 0;
                break;
            case 's':
                test_size = strtol(optarg, &tmp, 10);
                break;
            case 'r':
                num_range = (int) strtol(optarg, &tmp, 10);
                break;
            default:
                fprintf(stderr, "Usage: %s [options] filename\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc)
        filename = argv[optind];
    else
        filename = DFLT_FILENAME;

    TestNumbers tnums;
    tnums_init(&tnums);

    tnums_gen(&tnums, num_range, test_size);

    if (verbose) {
        printf("generate numbers:\n");
        for (size_t i = 0; i < test_size; i++) {
            printf("%d ", tnums.values[i]);
        }
        printf("\n");
    }

    tnums_write(&tnums, filename, bin_mode);

    return 0;
}
