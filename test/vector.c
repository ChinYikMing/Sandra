//
// Created by Jason Zheng on 2020/3/3.
//

#include <criterion/criterion.h>
#include "vector.h"

Test(IntVector, push_back) {
    SdrVector(int) vec;
    sdr_vector_init(&vec);

    cr_assert(vec.capacity == 1u << SDR_VECTOR_DFLT_CAP_BITS);
    cr_assert(vec.capacity == sdr_vector_capacity(&vec));
    cr_assert(vec.front == 0);
    cr_assert(vec.rear == 0);

    int i;
    int test_size = 1024;
    for (i = 0; i < test_size; i++) {
        int ret = sdr_vector_push_back(&vec, i * 10);
        cr_assert(ret == 0);
    }
    cr_assert(i == test_size);
    cr_assert(vec.size == test_size);
    cr_assert(vec.size == sdr_vector_size(&vec));
    cr_assert(vec.capacity >= test_size);
    cr_assert(vec.capacity == sdr_vector_capacity(&vec));

    size_t index;
    int item;
    sdr_vector_for_each(&vec, index, item) {
        cr_assert(index * 10 == item);
        index++;
    }
    cr_assert(index == test_size);

    for (i = test_size - 1; i >= 0; i--) {
        int num = sdr_vector_pop_back(&vec);
        cr_assert(i * 10 == num);
    }
    cr_assert(i == -1);
    cr_assert(vec.size == 0);
    cr_assert(vec.size == sdr_vector_size(&vec));

    sdr_vector_destroy(&vec);
}

Test(IntVector, push_front) {
    SdrVector(int) vec;
    sdr_vector_init(&vec);

    cr_assert(vec.capacity == 1u << SDR_VECTOR_DFLT_CAP_BITS);
    cr_assert(vec.capacity == sdr_vector_capacity(&vec));
    cr_assert(vec.front == 0);
    cr_assert(vec.rear == 0);

    int i;
    int test_size = 1024;
    for (i = 0; i < test_size; i++) {
        int ret = sdr_vector_push_front(&vec, i * 10);
        cr_assert(ret == 0);
    }
    cr_assert(i == test_size);
    cr_assert(vec.size == test_size);
    cr_assert(vec.size == sdr_vector_size(&vec));
    cr_assert(vec.capacity >= test_size);
    cr_assert(vec.capacity == sdr_vector_capacity(&vec));

    size_t index;
    int item;
    sdr_vector_for_each(&vec, index, item) {
        cr_assert((test_size - index - 1) * 10 == item);
        index++;
    }
    cr_assert(index == test_size);

    for (i = test_size - 1; i >= 0; i--) {
        int num = sdr_vector_pop_front(&vec);
        cr_assert(i * 10 == num);
    }
    cr_assert(i == -1);
    cr_assert(vec.size == 0);
    cr_assert(vec.size == sdr_vector_size(&vec));

    sdr_vector_destroy(&vec);
}


Test(IntVector, push_back_expand_coil_mov_front) {
    SdrVector(int) vec;
    unsigned bits = 3;
    int cap = 1u << 3u;
    sdr_vector_init(&vec, bits);
    cr_assert(vec.capacity == 1u << bits);
    cr_assert(vec.capacity == sdr_vector_capacity(&vec));
    cr_assert(vec.front == 0);
    cr_assert(vec.rear == 0);

    int i;
    for (i = 0; i < 5; ++i) {
        sdr_vector_push_back(&vec, i);
        cr_assert(vec.rear == (i + 1));
    }
    for (i = 0; i < 5; ++i) {
        int val = sdr_vector_pop_front(&vec);
        cr_assert(val == i);
        cr_assert(vec.front == (i + 1));
    }
    cr_assert(vec.front == 5);
    cr_assert(vec.rear == 5);

    for (i = 0; i < 8; ++i) {
        sdr_vector_push_back(&vec, i);
        cr_assert(vec.rear == (5 + i + 1) % cap);
    }

    sdr_vector_push_back(&vec, 8);
    cr_assert(vec.cap_bits == bits + 1);
    cr_assert(vec.capacity == 1u << (bits + 1));
    cr_assert(vec.capacity == sdr_vector_capacity(&vec));
    cr_assert(vec.front == 13);
    cr_assert(vec.rear == 6);

    sdr_vector_destroy(&vec);
}

Test(IntVector, push_back_expand_coil_mov_rear) {
    SdrVector(int) vec;
    unsigned bits = 3;
    int cap = 1u << 3u;
    sdr_vector_init(&vec, bits);
    cr_assert(vec.capacity == 1u << bits);
    cr_assert(vec.capacity == sdr_vector_capacity(&vec));
    cr_assert(vec.front == 0);
    cr_assert(vec.rear == 0);

    int i;
    for (i = 0; i < 3; ++i) {
        sdr_vector_push_back(&vec, i);
        cr_assert(vec.rear == (i + 1));
    }
    for (i = 0; i < 3; ++i) {
        int val = sdr_vector_pop_front(&vec);
        cr_assert(val == i);
        cr_assert(vec.front == (i + 1));
    }
    cr_assert(vec.front == 3);
    cr_assert(vec.rear == 3);

    for (i = 0; i < 8; ++i) {
        sdr_vector_push_back(&vec, i);
        cr_assert(vec.rear == (3 + i + 1) % cap);
    }

    sdr_vector_push_back(&vec, 8);
    cr_assert(vec.cap_bits == bits + 1);
    cr_assert(vec.capacity == 1u << (bits + 1));
    cr_assert(vec.capacity == sdr_vector_capacity(&vec));
    cr_assert(vec.front == 3);
    cr_assert(vec.rear == 12);

    sdr_vector_destroy(&vec);

}

Test(IntVector, push_front_expand_coil_mov_front) {
    SdrVector(int) vec;
    unsigned bits = 3;
    int cap = 1u << 3u;
    sdr_vector_init(&vec, bits);
    cr_assert(vec.capacity == 1u << bits);
    cr_assert(vec.capacity == sdr_vector_capacity(&vec));
    cr_assert(vec.front == 0);
    cr_assert(vec.rear == 0);

    int i;
    for (i = 0; i < 5; ++i) {
        sdr_vector_push_front(&vec, i);
        cr_assert(vec.front == (8 - i - 1));
    }
    for (i = 0; i < 5; ++i) {
        int val = sdr_vector_pop_back(&vec);
        cr_assert(val == i);
        cr_assert(vec.rear == (8 - i - 1));
    }
    cr_assert(vec.front == 3);
    cr_assert(vec.rear == 3);

    for (i = 0; i < 8; ++i) {
        sdr_vector_push_front(&vec, i);
        cr_assert(vec.front == (cap + 3 - i - 1) % cap);
    }

    sdr_vector_push_front(&vec, 8);
    cr_assert(vec.cap_bits == bits + 1);
    cr_assert(vec.capacity == 1u << (bits + 1));
    cr_assert(vec.capacity == sdr_vector_capacity(&vec));
    cr_assert(vec.front == 2);
    cr_assert(vec.rear == 11);

    sdr_vector_destroy(&vec);
}

Test(IntVector, push_all_back) {
    SdrVector(int) vec;
    int bits = 3;
    int cap = 1u << 3u;
    sdr_vector_init(&vec, bits);
    for (int i = 0; i < 5; i++)
        sdr_vector_push_back(&vec, i);
    cr_assert(vec.cap_bits == bits);
    cr_assert(vec.capacity == cap);
    cr_assert(vec.capacity == sdr_vector_capacity(&vec));
    cr_assert(vec.rear == 5);

    int list[15];
    sdr_vector_push_all_back(&vec, list, 15);
    cr_assert(vec.cap_bits == 5);
    cr_assert(vec.capacity == 32);
    cr_assert(vec.capacity == sdr_vector_capacity(&vec));
    cr_assert(vec.rear == 20);
    cr_assert(vec.size == 20);

}

Test(IntVector, push_all_back_with_empty_size) {
    SdrVector(int) vec;
    int bits = 3;
    sdr_vector_init(&vec, bits);

    int list[16];
    sdr_vector_push_all_back(&vec, list, 16);
    cr_assert(vec.cap_bits == 5);
    cr_assert(vec.capacity == 32);
    cr_assert(vec.capacity == sdr_vector_capacity(&vec));
    cr_assert(vec.rear == 16);
    cr_assert(vec.size == 16);
    cr_assert(vec.size == sdr_vector_size(&vec));

}

Test(IntVector, straighten) {
    SdrVector(int) vec;
    int bits = 2;
    sdr_vector_init(&vec, bits);

    sdr_vector_push_back(&vec, 'x');
    sdr_vector_push_back(&vec, 'y');
    sdr_vector_push_back(&vec, 'a');
    sdr_vector_push_back(&vec, 'b');

    sdr_vector_pop_front(&vec);
    sdr_vector_pop_front(&vec);

    sdr_vector_push_back(&vec, 'c');
    sdr_vector_push_back(&vec, 'd');

    int *raw = sdr_vector_raw(&vec);
    cr_assert(vec.front == 0);
    cr_assert(vec.rear == 4);
    cr_assert(vec.val == raw);
    cr_assert(vec.val[0] == 'a');
    cr_assert(vec.val[1] == 'b');
    cr_assert(vec.val[2] == 'c');
    cr_assert(vec.val[3] == 'd');
}

int cmp(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
}

Test(IntVector, sort) {
    SdrVector(int) vec;
    int bits = 2;
    sdr_vector_init(&vec, bits);

    sdr_vector_push_back(&vec, 'c');
    sdr_vector_push_back(&vec, 'a');
    sdr_vector_push_back(&vec, 'b');
    sdr_vector_push_back(&vec, 'd');

    sdr_vector_sort(&vec, cmp);

    cr_assert(vec.val[0] == 'a');
    cr_assert(vec.val[1] == 'b');
    cr_assert(vec.val[2] == 'c');
    cr_assert(vec.val[3] == 'd');
}

