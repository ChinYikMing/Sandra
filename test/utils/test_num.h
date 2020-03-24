//
// Created by Jason Zheng on 2020/3/24.
//

#ifndef SANDRA_TEST_NUM_H
#define SANDRA_TEST_NUM_H

#include <stdio.h>


typedef struct test_numbers {
    int *values;
    size_t capacity;
    size_t size;
} TestNumbers;

int tnums_init(TestNumbers *numbers);

int tnums_gen(TestNumbers *numbers, int range, size_t cnt);

int tnums_read(TestNumbers *numbers,const char *filename, int bin_mode);

int tnums_write(TestNumbers *numbers, const char *filename, int bin_mode);

int tnums_cpy(TestNumbers *numbers, TestNumbers *copy);

int tnums_equal(TestNumbers *n1, TestNumbers *n2);

int tnums_add(TestNumbers *numbers, int num);

int tnums_is_sorted(TestNumbers *numbers, int non_decreasing);

void tnums_sort(TestNumbers *numbers, int non_decreasing);

void tnums_destroy(TestNumbers *numbers);

#define tnums_for(tnums, num) \
    for (size_t m_tf_i = 0; ((num = (tnums)->values[m_tf_i]) || 1) && m_tf_i < (tnums)->size; m_tf_i++)

#endif //SANDRA_TEST_NUM_H
