//
// Created by Bobo Sheep on 2020/3/23.
//
#include <unistd.h>
#include <stdio.h>
#include <sandra/str.h>
#include <stdlib.h>
#include <time.h>
#include "utils/test_num.h"

#define DFLT_FILENAME   "test_num.txt"
#define DFLT_TEST_SIZE  1024

int main(int argc, char *argv[]) {
    int bin_mode = 1, verbose = 0;
    int num_range = UINT16_MAX;
    size_t test_size = DFLT_TEST_SIZE;
    char *filename;

    int opt;
    while ((opt = getopt(argc, argv, "vts:")) != -1) {
        switch (opt) {
            case 'v':
                verbose = 1;
                break;
            case 't':
                bin_mode = 0;
                break;
            case 's':
                sdr_str_to_long(optarg, test_size);
                break;
            case 'r':
                sdr_str_to_long(optarg, num_range);
                break;
            default:
                fprintf(stderr, "Usage: %s [-n cnt] [-t] filename\n", argv[0]);
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
