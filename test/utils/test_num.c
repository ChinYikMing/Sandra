//
// Created by Jason Zheng on 2020/3/24.
//

#include "test_num.h"
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>

#define BUF_SIZE 1024

int non_decrease_cmp(const void *a, const void *b) {
    return (*(int *) a) - (*(int *) b);
}

int non_increase_cmp(const void *a, const void *b) {
    return (*(int *) b) - (*(int *) a);
}

int tnums_init(TestNumbers *numbers) {
    numbers->values = NULL;
    numbers->capacity = 0;
    numbers->size = 0;
    return 0;
}

int tnums_gen(TestNumbers *numbers, int range, size_t cnt) {


    if (numbers->capacity < cnt) {
        void *tmp = malloc(sizeof(int) * cnt);
        if (!tmp) return -1;
        if (numbers->values)
            free(numbers->values);
        numbers->values = tmp;
    }

    srand(time(NULL));
    for (size_t i = 0; i < cnt; ++i)
        numbers->values[i] = rand() % range;

    numbers->size = cnt;
    return 0;
}

int tnums_read(TestNumbers *numbers, const char *filename, int bin_mode) {
    if (!filename) return -1;

    /* free previous numbers */
    if (numbers->values) {
        free(numbers->values);
    }

    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;

    if (bin_mode) {
        struct stat st;
        stat(filename, &st);
        off_t file_size = st.st_size;
        char *buf = malloc(file_size);
        size_t n_read = fread(buf, file_size, 1, fp);
        if (!n_read) return -1;
        numbers->values = (int *) buf;
        numbers->capacity = file_size / sizeof(int);
        numbers->size = numbers->capacity;
    } else {
        numbers->values = malloc(sizeof(int) * BUF_SIZE);
        numbers->capacity = BUF_SIZE;
        numbers->size = 0;
        int n;
        char buf[16];
        char *tmp;
        while (fgets(buf, 16, fp)) {
            n = (int) strtol(buf, &tmp, 10);
            tnums_add(numbers, n);
        }
    }
    fclose(fp);

    return 0;
}

int tnums_write(TestNumbers *numbers, const char *filename, const int bin_mode) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return -1;

    if (bin_mode) {
        fwrite(numbers->values, sizeof(int), numbers->size, fp);
    } else {
        for (size_t i = 0; i < numbers->size; ++i)
            fprintf(fp, "%d\n", numbers->values[i]);
    }

    fclose(fp);
    return 0;
}

int tnums_cpy(TestNumbers *numbers, TestNumbers *copy) {
    const size_t cap = copy->capacity = numbers->capacity;
    const size_t size = copy->size = numbers->size;
    if (!(copy->values = malloc(cap)))
        return -1;
    memcpy(copy->values, numbers->values, sizeof(int) * size);
    return 0;
}

int tnums_equal(TestNumbers *n1, TestNumbers *n2) {
    const size_t size = n1->size;
    if (size != n2->size) return 0;
    for (size_t i = 0; i < size; ++i) {
        if (n1->values[i] != n2->values[i])
            return 0;
    }
    return 1;
}

int tnums_add(TestNumbers *numbers, int num) {
    if (numbers->size == numbers->capacity) {
        size_t new_cap = numbers->capacity << 1u;
        void *tmp = realloc(numbers->values, sizeof(int) * new_cap);
        if (!tmp) return -1;
        numbers->values = tmp;
        numbers->capacity = new_cap;
    }
    numbers->values[numbers->size++] = num;
    return 0;
}

int tnums_is_sorted(TestNumbers *numbers, int non_decreasing) {
    const size_t size = numbers->size;
    if (size == 1) return 1;

    for (size_t i = 0; i < size - 1; ++i) {
        if (non_decreasing) {
            if (numbers->values[i] > numbers->values[i + 1])
                return 0;
        } else {
            if (numbers->values[i] < numbers->values[i + 1])
                return 0;
        }
    }
    return 1;
}

void tnums_sort(TestNumbers *numbers, int non_decreasing) {
    qsort(numbers->values, numbers->size, sizeof(int),
          non_decreasing ? non_decrease_cmp : non_increase_cmp);
}

void tnums_destroy(TestNumbers *numbers) {
    free(numbers->values);
}

