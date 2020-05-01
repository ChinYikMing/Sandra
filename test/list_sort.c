//
// Created by Jason Zheng on 2020/3/23.
//
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sandra/embed_list.h>
#include <assert.h>
#include <sandra/string.h>
#include "utils/test_num.h"

#define DFLT_FILENAME   "test_num.txt"
#define DFLT_TEST_SIZE  1024

typedef struct user {
    int id;
    SdrEList list;
} User;

int user_cmp(SdrEList *a, SdrEList *b, void *data) {
    const User *ua = sdr_elist_data(a, User, list);
    const User *ub = sdr_elist_data(b, User, list);
    return ua->id - ub->id;
}

int main(int argc, char *argv[]) {
    int verbose = 0;
    int bin_mode = 1;
    int progressive_test = 0;
    int file_mode = 0;
    int num_range = INT_MAX;
    size_t test_size = DFLT_TEST_SIZE, test_cnt = 1;
    char *filename = NULL;

    int opt;
    char *tmp;
    while ((opt = getopt(argc, argv, "vtps:f:r:c:")) != -1) {
        switch (opt) {
            case 'v':
                verbose = 1;
                break;
            case 't':
                bin_mode = 0;
                break;
            case 'p':
                progressive_test = 1;
                break;
            case 's':
                test_size = strtol(optarg, &tmp, 10);
                break;
            case 'f':
                file_mode = 1;
                filename = optarg;
                break;
            case 'r':
                num_range = (int) strtol(optarg, &tmp, 10);
                break;
            case 'c':
                test_cnt = strtol(optarg, &tmp, 10);
                break;
            default:
                fprintf(stderr, "Usage: %s [options]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    for (size_t cnt = 0; cnt < test_cnt; cnt++) {
        TestNumbers tnums;

        if (tnums_init(&tnums) < 0) {
            fprintf(stderr, "test_numbers init failure\n");
            exit(EXIT_FAILURE);
        }

        if (file_mode) {
            tnums_read(&tnums, filename, bin_mode);
        } else {
            size_t tsize = test_size;
            if (progressive_test) {
                tsize = tsize - (test_cnt - cnt - 1) * (test_size / (test_cnt));
            }
            tnums_gen(&tnums, num_range, tsize);
        }

        printf("[list_sort] test %zu numbers\n", tnums.size);
        if (verbose) {
            printf("%*s", 12, "");
            printf("b: ");
            for (size_t i = 0; i < tnums.size; ++i)
                printf("%d ", tnums.values[i]);
            printf("\n");
        }

        /* list init */
        SDR_DEFINE_ELIST(list);
        User *users = malloc(sizeof(User) * (tnums.size));
        int i = 0, num;
        tnums_for(&tnums, num) {
            users[i].id = num;
            sdr_elist_push_back(&list, &users[i].list);
            i++;
        }

        tnums_sort(&tnums, 1);

        sdr_elist_sort(&list, user_cmp, NULL);
        if (verbose) {
            printf("%*s", 12, "");
            printf("a: ");
        }

        i = 0;
        SdrEList *eptr = list.next;
        while ((eptr != &list)) {
            User *eu = sdr_elist_data(eptr, User, list);
            num = tnums.values[i];
            assert(eu->id == num);
            if (verbose)
                printf("%d ", num);
            i++;
            eptr = eptr->next;
        }
        if (verbose)
            printf("\n");

        tnums_destroy(&tnums);
    }

    printf("[list_sort] test pass\n");
    return 0;
}


