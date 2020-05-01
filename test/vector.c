//
// Created by Jason Zheng on 2020/3/3.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

#include <criterion/criterion.h>
#include <sandra/vector.h>

Test(init_with_defalt_value, default_use_sbuf) {
    SdrVector(int) vec;
    sdr_vec_init(&vec);
    cr_assert(vec.front == 0);
    cr_assert(vec.rear == 0);
    cr_assert(vec.capacity == 1u << SDR_VEC_DFLT_STACK_CAP_BITS);
    cr_assert(vec.capacity == sdr_vec_capacity(&vec));
    cr_assert(sizeof(vec.sbuf) == sizeof(int) * (1u << SDR_VEC_DFLT_STACK_CAP_BITS));
    cr_assert(vec.size == 0);
    cr_assert(vec.el_size == sizeof(int));
    cr_assert(vec.cap_bits == SDR_VEC_DFLT_STACK_CAP_BITS);
    cr_assert(vec.use_hbuf == 0);

    sdr_vec_destroy(&vec);
}

Test(init_with_custom_cap_bits, use_hbuf_if_greater_than_sbuf_cap_bits) {
    const unsigned cap_bits = SDR_VEC_DFLT_STACK_CAP_BITS + 1;
    SdrVector(int) vec;
    sdr_vec_init(&vec, .heap_cap_bits = cap_bits);
    cr_assert(vec.front == 0);
    cr_assert(vec.rear == 0);
    cr_assert(vec.capacity == 1u << cap_bits);
    cr_assert(vec.capacity == sdr_vec_capacity(&vec));
    cr_assert(sizeof(vec.sbuf) == sizeof(int) * (1u << SDR_VEC_DFLT_STACK_CAP_BITS));
    cr_assert(vec.size == 0);
    cr_assert(vec.el_size == sizeof(int));
    cr_assert(vec.cap_bits == cap_bits);
    cr_assert(vec.use_hbuf == 1);

    sdr_vec_destroy(&vec);
}

Test(define_vector_with_custom_sbuf_cap, larger_stack_buf_size) {
    const unsigned sbuf_cap_bits = 10;
    SdrVector(int, sbuf_cap_bits) vec;
    sdr_vec_init(&vec);
    cr_assert(vec.front == 0);
    cr_assert(vec.rear == 0);
    cr_assert(vec.capacity == 1u << sbuf_cap_bits);
    cr_assert(vec.capacity == sdr_vec_capacity(&vec));
    cr_assert(sizeof(vec.sbuf) == sizeof(int) * (1u << sbuf_cap_bits));
    cr_assert(vec.size == 0);
    cr_assert(vec.el_size == sizeof(int));
    cr_assert(vec.cap_bits == sbuf_cap_bits);
    cr_assert(vec.use_hbuf == 0);

    sdr_vec_destroy(&vec);
}

Test(sbuf_expand_with_empty, migrate_to_hbuf) {
    SdrVector(int, 2) vec;
    sdr_vec_init(&vec);
    sdr_vec_expand(&vec, 1);

    cr_assert(vec.use_hbuf == 1);
}


Test(sbuf_expand_without_full, migrate_to_hbuf) {
    SdrVector(int, 2) vec;
    sdr_vec_init(&vec);

    sdr_vec_push_back(&vec, 1);
    sdr_vec_push_back(&vec, 2);

    sdr_vec_expand(&vec, 1);

    cr_assert(sdr_vec_get(&vec, 0) == 1);
    cr_assert(sdr_vec_get(&vec, 1) == 2);
}

Test(sbuf_expand_with_front_movement, migrate_to_hbuf) {
    SdrVector(int, 2) vec;
    sdr_vec_init(&vec);

    sdr_vec_push_back(&vec, 1);
    sdr_vec_push_back(&vec, 2);
    sdr_vec_push_back(&vec, 3);
    sdr_vec_push_back(&vec, 4);
    sdr_vec_pop_front(&vec, NULL);
    sdr_vec_pop_front(&vec, NULL);

    sdr_vec_expand(&vec, 1);

    cr_assert(sdr_vec_get(&vec, 0) == 3);
    cr_assert(sdr_vec_get(&vec, 1) == 4);
}

Test(sbuf_expand_with_rear_movement, migrate_to_hbuf) {
    SdrVector(int, 2) vec;
    sdr_vec_init(&vec);

    sdr_vec_push_back(&vec, 1);
    sdr_vec_push_back(&vec, 2);
    sdr_vec_push_back(&vec, 3);
    sdr_vec_push_back(&vec, 4);
    sdr_vec_pop_front(&vec, NULL);
    sdr_vec_pop_front(&vec, NULL);
    sdr_vec_pop_front(&vec, NULL);
    sdr_vec_push_back(&vec, 3);
    sdr_vec_push_back(&vec, 2);
    sdr_vec_push_back(&vec, 1);

    sdr_vec_expand(&vec, 1);

    cr_assert(sdr_vec_get(&vec, 0) == 4);
    cr_assert(sdr_vec_get(&vec, 1) == 3);
    cr_assert(sdr_vec_get(&vec, 2) == 2);
    cr_assert(sdr_vec_get(&vec, 3) == 1);
}

Test(push_back, auto_expand_if_necessary) {
    SdrVector(int) vec;
    sdr_vec_init(&vec);

    int i;
    int stest_size = 1u << SDR_VEC_DFLT_STACK_CAP_BITS;
    for (i = 0; i < stest_size; i++) {
        int ret = sdr_vec_push_back(&vec, i * 10);
        cr_assert(vec.front == 0);
        cr_assert(vec.rear == (i + 1) % stest_size);
        cr_assert(ret == 0);
    }
    cr_assert(i == stest_size);
    cr_assert(vec.front == 0);
    cr_assert(vec.rear == 0);
    cr_assert(vec.size == stest_size);
    cr_assert(vec.size == sdr_vec_size(&vec));
    cr_assert(vec.capacity >= stest_size);
    cr_assert(vec.capacity == sdr_vec_capacity(&vec));

    int total_test_size = stest_size << 1u;
    for (; i < total_test_size; i++) {
        int ret = sdr_vec_push_back(&vec, i * 10);
        cr_assert(vec.front == 0);
        cr_assert(vec.rear == (i + 1) % total_test_size);
        cr_assert(ret == 0);
    }
    cr_assert(i == total_test_size);
    cr_assert(vec.front == 0);
    cr_assert(vec.rear == 0);
    cr_assert(vec.size == total_test_size);
    cr_assert(vec.size == sdr_vec_size(&vec));
    cr_assert(vec.capacity >= total_test_size);
    cr_assert(vec.capacity == sdr_vec_capacity(&vec));

    size_t index;
    int item;
    sdr_vec_for_el(&vec, index, item) {
        cr_assert(index * 10 == item);
    }
    cr_assert(index == total_test_size);

    int j;
    for (j = 0, i = total_test_size - 1; i >= 0; j++, i--) {
        int num = 0;
        int ret = sdr_vec_pop_back(&vec, &num);
        cr_assert(ret == 0);
        cr_assert(i * 10 == num);
        cr_assert(vec.front == 0);
        cr_assert(vec.rear == total_test_size - 1 - j);
        cr_assert(vec.size == i);
    }
    cr_assert(i == -1);
    cr_assert(vec.size == 0);
    cr_assert(vec.size == sdr_vec_size(&vec));

    sdr_vec_destroy(&vec);
}

Test(push_front, auto_expand_if_necessary) {
    SdrVector(int) vec;
    sdr_vec_init(&vec);

    int i;
    int stest_size = 1u << SDR_VEC_DFLT_STACK_CAP_BITS;
    for (i = 0; i < stest_size; i++) {
        int ret = sdr_vec_push_front(&vec, i * 10);
        cr_assert(vec.front == (stest_size - 1 - i));
        cr_assert(ret == 0);
    }
    cr_assert(i == stest_size);
    cr_assert(vec.front == 0);
    cr_assert(vec.rear == 0);
    cr_assert(vec.size == stest_size);
    cr_assert(vec.size == sdr_vec_size(&vec));
    cr_assert(vec.capacity >= stest_size);
    cr_assert(vec.capacity == sdr_vec_capacity(&vec));

    int total_test_size = stest_size << 1u;
    for (int j = 0; i < total_test_size; i++, j++) {
        int ret = sdr_vec_push_front(&vec, i * 10);
        cr_assert(vec.front == (total_test_size - 1 - j));
        cr_assert(vec.rear == stest_size);

        cr_assert(ret == 0);
    }
    cr_assert(i == total_test_size);
    cr_assert(vec.front == stest_size);
    cr_assert(vec.rear == stest_size);
    cr_assert(vec.size == total_test_size);
    cr_assert(vec.size == sdr_vec_size(&vec));
    cr_assert(vec.capacity >= total_test_size);
    cr_assert(vec.capacity == sdr_vec_capacity(&vec));

    size_t index;
    int item;
    sdr_vec_for_el(&vec, index, item) {
        cr_assert((total_test_size - index - 1) * 10 == item);
    }
    cr_assert(index == total_test_size);

    for (i = total_test_size - 1; i >= 0; i--) {
        int num = 0;
        int ret = sdr_vec_pop_front(&vec, &num);
        cr_assert(ret == 0);
        cr_assert(i * 10 == num);
        cr_assert(vec.size == i);
    }
    cr_assert(i == -1);
    cr_assert(vec.front == stest_size);
    cr_assert(vec.rear == stest_size);
    cr_assert(vec.size == 0);
    cr_assert(vec.size == sdr_vec_size(&vec));

    sdr_vec_destroy(&vec);
}

Test(push_back_with_coil, mov_front_to_end) {
    SdrVector(int) vec;
    sdr_vec_init(&vec);

    unsigned cap_bits = SDR_VEC_DFLT_STACK_CAP_BITS;
    size_t capacity = 1u << SDR_VEC_DFLT_STACK_CAP_BITS;

    unsigned seed = (capacity >> 1u) + 1;
    vec.front = vec.rear = seed;
    vec.size = capacity;

    unsigned expect_front = vec.front + capacity;
    sdr_vec_push_back(&vec, 87);

    cr_assert(vec.cap_bits == cap_bits + 1);
    cr_assert(vec.capacity == 1u << (cap_bits + 1));
    cr_assert(vec.capacity == sdr_vec_capacity(&vec));
    cr_assert(vec.front == expect_front);
    cr_assert(vec.rear == seed + 1);

    sdr_vec_destroy(&vec);
}

Test(push_back_with_coil, mov_rear_to_new_space) {
    SdrVector(int) vec;
    sdr_vec_init(&vec);

    unsigned cap_bits = SDR_VEC_DFLT_STACK_CAP_BITS;
    size_t capacity = 1u << SDR_VEC_DFLT_STACK_CAP_BITS;

    unsigned seed = (capacity >> 1u) - 1;
    vec.front = vec.rear = seed;
    vec.size = capacity;

    sdr_vec_push_back(&vec, 87);

    cr_assert(vec.cap_bits == cap_bits + 1);
    cr_assert(vec.capacity == 1u << (cap_bits + 1));
    cr_assert(vec.capacity == sdr_vec_capacity(&vec));
    cr_assert(vec.front == seed);
    cr_assert(vec.rear == seed + capacity + 1);

    sdr_vec_destroy(&vec);
}

Test(push_front_with_coil, mov_front_to_end) {
    SdrVector(int) vec;
    sdr_vec_init(&vec);

    unsigned cap_bits = SDR_VEC_DFLT_STACK_CAP_BITS;
    size_t capacity = 1u << SDR_VEC_DFLT_STACK_CAP_BITS;

    unsigned seed = (capacity >> 1u) + 1;
    vec.front = vec.rear = seed;
    vec.size = capacity;

    unsigned expect_front = vec.front + capacity - 1;
    sdr_vec_push_front(&vec, 87);

    cr_assert(vec.cap_bits == cap_bits + 1);
    cr_assert(vec.capacity == 1u << (cap_bits + 1));
    cr_assert(vec.capacity == sdr_vec_capacity(&vec));
    cr_assert(vec.front == expect_front);
    cr_assert(vec.rear == seed);

    sdr_vec_destroy(&vec);
}

Test(push_front_with_coil, mov_rear_to_new_space) {
    SdrVector(int) vec;
    sdr_vec_init(&vec);

    unsigned cap_bits = SDR_VEC_DFLT_STACK_CAP_BITS;
    size_t capacity = 1u << SDR_VEC_DFLT_STACK_CAP_BITS;

    unsigned seed = (capacity >> 1u) - 1;
    vec.front = vec.rear = seed;
    vec.size = capacity;

    sdr_vec_push_front(&vec, 87);

    cr_assert(vec.cap_bits == cap_bits + 1);
    cr_assert(vec.capacity == 1u << (cap_bits + 1));
    cr_assert(vec.capacity == sdr_vec_capacity(&vec));
    cr_assert(vec.front == seed - 1);
    cr_assert(vec.rear == seed + capacity);

    sdr_vec_destroy(&vec);
}

Test(push_all_back, auto_expand_if_necessary) {
    SdrVector(int) vec;
    sdr_vec_init(&vec);

    const size_t s = 250;
    int list[s];
    int ret = sdr_vec_bulk_push_back(&vec, list, s);
    cr_assert(ret == 0);
    cr_assert(vec.cap_bits >= 8);
    cr_assert(vec.capacity >= 256);
    cr_assert(vec.capacity == sdr_vec_capacity(&vec));
    cr_assert(vec.size == s);
    cr_assert(vec.rear == s);

    sdr_vec_destroy(&vec);
}

int cmp(const void *a, const void *b) {
    return *(char *) a - *(char *) b;
}

Test(IntVector, sort) {
    SdrVector(char) vec;
    sdr_vec_init(&vec);

    sdr_vec_push_back(&vec, 'c');
    sdr_vec_push_back(&vec, 'a');
    sdr_vec_push_back(&vec, 'b');
    sdr_vec_push_back(&vec, 'd');

    sdr_vec_qsort(&vec, cmp);

    cr_assert(sdr_vec_get(&vec, 0) == 'a');
    cr_assert(sdr_vec_get(&vec, 1) == 'b');
    cr_assert(sdr_vec_get(&vec, 2) == 'c');
    cr_assert(sdr_vec_get(&vec, 3) == 'd');
}


#pragma clang diagnostic pop