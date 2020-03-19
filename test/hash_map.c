//
// Created by Jason Zheng on 2020/3/13.
//

#include <criterion/criterion.h>
#include <hash_map.h>

Test(HashMap, init) {
    {
        SDR_DEFINE_HASH_MAP(m);
        cr_assert(m.base.capacity == 1u << SDR_HMAP_DFLT_CAP_BITS);
        cr_assert(m.base.cap_bits == SDR_HMAP_DFLT_CAP_BITS);
        cr_assert(sdr_hash_map_ctx(&m)->cmp == (sdr_fn_cmp) strcmp);
        cr_assert(m.base.hash == NULL);
        cr_assert(m.base.index == NULL);
        cr_assert(m.base.save_hash_val == 1);
        cr_assert(m.max_load == (size_t) (m.base.capacity * SDR_HMAP_DFLT_LOAD_FACTOR));
        cr_assert(m.n_expand == 0);
        cr_assert(m.hashv_list != NULL);
        sdr_hash_map_destroy(&m);
    }

    {
        unsigned cap_bits = 3;
        double load_factor = .6;
        int save_hashv = 0;
        SDR_DEFINE_HASH_MAP(m,
                            .cap_bits = cap_bits,
                            .load_factor = load_factor,
                            .save_hash_val = save_hashv
        );
        cr_assert(m.base.capacity == 1u << cap_bits);
        cr_assert(m.base.cap_bits == 3);
        cr_assert(sdr_hash_map_ctx(&m)->cmp == (sdr_fn_cmp) strcmp);
        cr_assert(m.base.hash == NULL);
        cr_assert(m.base.index == NULL);
        cr_assert(m.base.save_hash_val == save_hashv);
        cr_assert(m.max_load == (size_t) (m.base.capacity * load_factor));
        cr_assert(m.n_expand == 0);
        cr_assert(m.hashv_list == NULL);
        sdr_hash_map_destroy(&m);
    }
}

Test(HashMap, string_int) {
    SDR_DEFINE_HASH_MAP(m);

    /* get empty */
    cr_assert(!sdr_hash_map_get(&m, "AA"));
    cr_assert(!sdr_hash_map_get(&m, "BB"));
    cr_assert(!sdr_hash_map_get(&m, "CC"));
    cr_assert(!sdr_hash_map_get(&m, "QQ"));

    cr_assert(sdr_hash_map_remove(&m, "AA") < 0);
    cr_assert(sdr_hash_map_remove(&m, "BB") < 0);
    cr_assert(sdr_hash_map_remove(&m, "CC") < 0);
    cr_assert(sdr_hash_map_remove(&m, "QQ") < 0);

    /* add */
    cr_assert(!sdr_hash_map_put(&m, "AA", (void *) (86)));
    cr_assert(sdr_hash_map_ctx(&m)->size == 1);

    cr_assert(!sdr_hash_map_put(&m, "BB", (void *) (87)));
    cr_assert(sdr_hash_map_ctx(&m)->size == 2);

    cr_assert(!sdr_hash_map_put(&m, "CC", (void *) (88)));
    cr_assert(sdr_hash_map_ctx(&m)->size == 3);

    /* replace */
    cr_assert(!sdr_hash_map_put(&m, "QQ", (void *) (1)));
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    cr_assert((int) sdr_hash_map_put(&m, "QQ", (void *) (2)) == 1);
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    cr_assert((int) sdr_hash_map_put(&m, "QQ", (void *) (3)) == 2);
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    cr_assert((int) sdr_hash_map_put(&m, "QQ", (void *) (4)) == 3);
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    cr_assert((int) sdr_hash_map_put(&m, "QQ", (void *) (89)) == 4);
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    /* get */
    cr_assert((int) sdr_hash_map_get(&m, "AA") == 86);
    cr_assert((int) sdr_hash_map_get(&m, "BB") == 87);
    cr_assert((int) sdr_hash_map_get(&m, "CC") == 88);
    cr_assert((int) sdr_hash_map_get(&m, "QQ") == 89);

    /* release resource by client  */
    int ret = (int) sdr_hash_map_put(&m, "AA", (void *) (1));
    cr_assert(ret == 86);
    cr_assert((int) sdr_hash_map_get(&m, "AA") == 1);
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    ret = (int) sdr_hash_map_put(&m, "BB", (void *) (2));
    cr_assert(ret == 87);
    cr_assert((int) sdr_hash_map_get(&m, "BB") == 2);
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    ret = (int) sdr_hash_map_put(&m, "CC", (void *) (3));
    cr_assert(ret == 88);
    cr_assert((int) sdr_hash_map_get(&m, "CC") == 3);
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    ret = (int) sdr_hash_map_put(&m, "QQ", (void *) (4));
    cr_assert(ret == 89);
    cr_assert((int) sdr_hash_map_get(&m, "QQ") == 4);
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    /* remove */
    cr_assert(sdr_hash_map_remove(&m, "^_^凸") < 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    cr_assert(sdr_hash_map_remove(&m, "AA") == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == 3);

    cr_assert(sdr_hash_map_remove(&m, "BB") == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == 2);

    cr_assert(sdr_hash_map_remove(&m, "CC") == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == 1);

    cr_assert(sdr_hash_map_remove(&m, "QQ") == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == 0);

    sdr_hash_map_destroy(&m);
}


Test(HashMap, rehash_with_save) {
    SDR_DEFINE_HASH_MAP(m, .cap_bits = 1);
    cr_assert(sdr_hash_map_ctx(&m)->size == 0);
    cr_assert(m.base.capacity == 2);
    cr_assert(m.base.cap_bits == 1);
    cr_assert(m.max_load == 1);
    cr_assert(m.n_expand == 0);

    cr_assert(!sdr_hash_map_put(&m, "a", &(int) {1}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 1);
    cr_assert(m.base.capacity == 4);
    cr_assert(m.base.cap_bits == 2);
    cr_assert(m.max_load == 3);
    cr_assert(m.n_expand == 1);

    cr_assert(!sdr_hash_map_put(&m, "b", &(int) {2}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 2);
    cr_assert(m.base.capacity == 4);
    cr_assert(m.base.cap_bits == 2);
    cr_assert(m.n_expand == 1);

    cr_assert(!sdr_hash_map_put(&m, "c", &(int) {3}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 3);
    cr_assert(m.base.capacity == 8);
    cr_assert(m.base.cap_bits == 3);
    cr_assert(m.max_load == 6);
    cr_assert(m.n_expand == 2);

    cr_assert(!sdr_hash_map_put(&m, "d", &(int) {4}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);
    cr_assert(m.base.capacity == 8);
    cr_assert(m.base.cap_bits == 3);
    cr_assert(m.n_expand == 2);

    cr_assert(!sdr_hash_map_put(&m, "e", &(int) {5}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 5);
    cr_assert(m.base.capacity == 8);
    cr_assert(m.base.cap_bits == 3);
    cr_assert(m.n_expand == 2);

    cr_assert(!sdr_hash_map_put(&m, "f", &(int) {6}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 6);
    cr_assert(m.base.capacity == 16);
    cr_assert(m.base.cap_bits == 4);
    cr_assert(m.max_load == 12);
    cr_assert(m.n_expand == 3);
}

Test(HashMap, rehash_without_save) {
    SDR_DEFINE_HASH_MAP(m, .cap_bits = 1, .save_hash_val = 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == 0);
    cr_assert(m.base.capacity == 2);
    cr_assert(m.base.cap_bits == 1);
    cr_assert(m.max_load == 1);
    cr_assert(m.n_expand == 0);

    cr_assert(!sdr_hash_map_put(&m, "a", &(int) {1}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 1);
    cr_assert(m.base.capacity == 4);
    cr_assert(m.base.cap_bits == 2);
    cr_assert(m.max_load == 3);
    cr_assert(m.n_expand == 1);

    cr_assert(!sdr_hash_map_put(&m, "b", &(int) {2}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 2);
    cr_assert(m.base.capacity == 4);
    cr_assert(m.base.cap_bits == 2);
    cr_assert(m.n_expand == 1);

    cr_assert(!sdr_hash_map_put(&m, "c", &(int) {3}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 3);
    cr_assert(m.base.capacity == 8);
    cr_assert(m.base.cap_bits == 3);
    cr_assert(m.max_load == 6);
    cr_assert(m.n_expand == 2);
    cr_assert(m.hashv_list == NULL);

    cr_assert(!sdr_hash_map_put(&m, "d", &(int) {4}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);
    cr_assert(m.base.capacity == 8);
    cr_assert(m.base.cap_bits == 3);
    cr_assert(m.n_expand == 2);

    cr_assert(!sdr_hash_map_put(&m, "e", &(int) {5}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 5);
    cr_assert(m.base.capacity == 8);
    cr_assert(m.base.cap_bits == 3);
    cr_assert(m.n_expand == 2);

    cr_assert(!sdr_hash_map_put(&m, "f", &(int) {6}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 6);
    cr_assert(m.base.capacity == 16);
    cr_assert(m.base.cap_bits == 4);
    cr_assert(m.max_load == 12);
    cr_assert(m.n_expand == 3);
    cr_assert(m.hashv_list == NULL);
}

void fake_free(void *ptr) {}

Test(HashMap, string_pint) {
    SDR_DEFINE_HASH_MAP(m);

    /* add */
    cr_assert(!sdr_hash_map_put(&m, "AA", &(int) {86}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 1);

    cr_assert(!sdr_hash_map_put(&m, "BB", &(int) {87}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 2);

    cr_assert(!sdr_hash_map_put(&m, "CC", &(int) {88}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 3);

    /* replace */
    sdr_hash_map_ctx(&m)->free_val = fake_free;
    cr_assert(!sdr_hash_map_put(&m, "QQ", &(int) {1}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    cr_assert(!sdr_hash_map_put(&m, "QQ", &(int) {2}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    cr_assert(!sdr_hash_map_put(&m, "QQ", &(int) {3}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    cr_assert(!sdr_hash_map_put(&m, "QQ", &(int) {4}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    cr_assert(!sdr_hash_map_put(&m, "QQ", &(int) {89}));
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    /* get */
    cr_assert(*(int *) sdr_hash_map_get(&m, "AA") == 86);
    cr_assert(*(int *) sdr_hash_map_get(&m, "BB") == 87);
    cr_assert(*(int *) sdr_hash_map_get(&m, "CC") == 88);
    cr_assert(*(int *) sdr_hash_map_get(&m, "QQ") == 89);

    /* release resource by client  */
    sdr_hash_map_ctx(&m)->free_val = NULL;
    int *ret = sdr_hash_map_put(&m, "AA", &(int) {1});
    cr_assert(*ret == 86);
    cr_assert(*(int *) sdr_hash_map_get(&m, "AA") == 1);
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    ret = sdr_hash_map_put(&m, "BB", &(int) {2});
    cr_assert(*ret == 87);
    cr_assert(*(int *) sdr_hash_map_get(&m, "BB") == 2);
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    ret = sdr_hash_map_put(&m, "CC", &(int) {3});
    cr_assert(*ret == 88);
    cr_assert(*(int *) sdr_hash_map_get(&m, "CC") == 3);
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    ret = sdr_hash_map_put(&m, "QQ", &(int) {4});
    cr_assert(*ret == 89);
    cr_assert(*(int *) sdr_hash_map_get(&m, "QQ") == 4);
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    /* remove */
    cr_assert(sdr_hash_map_remove(&m, "^_^凸") < 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == 4);

    cr_assert(sdr_hash_map_remove(&m, "AA") == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == 3);

    cr_assert(sdr_hash_map_remove(&m, "BB") == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == 2);

    cr_assert(sdr_hash_map_remove(&m, "CC") == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == 1);

    cr_assert(sdr_hash_map_remove(&m, "QQ") == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == 0);

    sdr_hash_map_destroy(&m);
}

Test(HashMap, string_string) {
    SDR_DEFINE_HASH_MAP(m);

    /* get empty */
    cr_assert(!sdr_hash_map_get(&m, "「"));
    cr_assert(!sdr_hash_map_get(&m, "("));
    cr_assert(!sdr_hash_map_get(&m, "『"));

    cr_assert(sdr_hash_map_remove(&m, "「") < 0);
    cr_assert(sdr_hash_map_remove(&m, "(") < 0);
    cr_assert(sdr_hash_map_remove(&m, "『") < 0);

    /* add */
    cr_assert(sdr_hash_map_ctx(&m)->size == 0);
    cr_assert(!sdr_hash_map_put(&m, "「", "a"));
    cr_assert(sdr_hash_map_ctx(&m)->size == 1);
    cr_assert(!sdr_hash_map_put(&m, "(", "b"));
    cr_assert(sdr_hash_map_ctx(&m)->size == 2);
    cr_assert(!sdr_hash_map_put(&m, "『", "c"));
    cr_assert(sdr_hash_map_ctx(&m)->size == 3);

    /* replace */
    cr_assert(sdr_hash_map_ctx(&m)->size == 3);
    cr_assert(!strcmp(sdr_hash_map_put(&m, "「", "」"), "a"));
    cr_assert(sdr_hash_map_ctx(&m)->size == 3);
    cr_assert(!strcmp(sdr_hash_map_put(&m, "(", ")"), "b"));
    cr_assert(sdr_hash_map_ctx(&m)->size == 3);
    cr_assert(!strcmp(sdr_hash_map_put(&m, "『", "』"), "c"));
    cr_assert(sdr_hash_map_ctx(&m)->size == 3);

    /* get */
    cr_assert(!strcmp((char *) sdr_hash_map_get(&m, "「"), "」"));
    cr_assert(sdr_hash_map_ctx(&m)->size == 3);
    cr_assert(!strcmp((char *) sdr_hash_map_get(&m, "("), ")"));
    cr_assert(sdr_hash_map_ctx(&m)->size == 3);
    cr_assert(!strcmp((char *) sdr_hash_map_get(&m, "『"), "』"));
    cr_assert(sdr_hash_map_ctx(&m)->size == 3);

    cr_assert(sdr_hash_map_get(&m, "，") == NULL);
    cr_assert(sdr_hash_map_get(&m, "。") == NULL);
    cr_assert(sdr_hash_map_get(&m, "『』") == NULL);

    /* remove */
    cr_assert(sdr_hash_map_remove(&m, "^_^凸") < 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == 3);

    cr_assert(sdr_hash_map_remove(&m, "「") == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == 2);

    cr_assert(sdr_hash_map_remove(&m, "(") == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == 1);

    cr_assert(sdr_hash_map_remove(&m, "『") == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == 0);

    sdr_hash_map_destroy(&m);
}

static inline int char_cmp(const char c1, const char c2) {
    return c1 - c2;
}

static size_t my_int_hash(const void *k) {
    return sdr_int_hash((int) k);
}

Test(HashMap, char_int) {
    SDR_DEFINE_HASH_MAP(m,
                        .hash = my_int_hash,
                        .cmp = (sdr_fn_cmp) char_cmp);
    cr_assert(sdr_hash_map_ctx(&m)->cmp == (sdr_fn_cmp) char_cmp);

    char delimiter[] = {'.', ',', '-'};
    size_t delim_size = SDR_ARRAY_SIZE(delimiter);

    /* add */
    for (int i = 0; i < delim_size; ++i) {
        char key = delimiter[i];
        int val = i;
        sdr_hash_map_put(&m,
                         (void *) (intptr_t) key,
                         (void *) (intptr_t) val);
    }

    /* replace */
    cr_assert(sdr_hash_map_ctx(&m)->size == delim_size);
    cr_assert(sdr_hash_map_put(&m, (void *) '.', (void *) 10) == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == delim_size);
    cr_assert(sdr_hash_map_put(&m, (void *) ',', (void *) 20) == (void *) 1);
    cr_assert(sdr_hash_map_ctx(&m)->size == delim_size);
    cr_assert(sdr_hash_map_put(&m, (void *) '-', (void *) 30) == (void *) 2);
    cr_assert(sdr_hash_map_ctx(&m)->size == delim_size);

    /* get */
    cr_assert(sdr_hash_map_get(&m, (void *) ('a')) == NULL);
    cr_assert(sdr_hash_map_get(&m, (void *) ('b')) == NULL);
    cr_assert(sdr_hash_map_get(&m, (void *) ('c')) == NULL);
    cr_assert(sdr_hash_map_get(&m, (void *) ('.')) == (void *) 10);
    cr_assert(sdr_hash_map_get(&m, (void *) (',')) == (void *) 20);
    cr_assert(sdr_hash_map_get(&m, (void *) ('-')) == (void *) 30);

    /* remove */
    cr_assert(sdr_hash_map_remove(&m, (void *) '@') < 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == delim_size);

    cr_assert(sdr_hash_map_remove(&m, (void *) '.') == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == delim_size - 1);

    cr_assert(sdr_hash_map_remove(&m, (void *) ',') == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == delim_size - 2);

    cr_assert(sdr_hash_map_remove(&m, (void *) '-') == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == delim_size - 3);

    sdr_hash_map_destroy(&m);
}

static inline size_t pchar_hash(const void *k) {
    char c = *(char *) k;
    return sdr_int_hash(c);
}

static inline int pchar_cmp(const char *c1, const char *c2) {
    return *c1 - *c2;
}

Test(HashMap, pchar_key) {
    SDR_DEFINE_HASH_MAP(m,
                        .hash = pchar_hash,
                        .cmp = (sdr_fn_cmp) pchar_cmp);

    char delimiter[] = {'.', ',', '-'};
    int size = 3;
    char *keys[size];

    /* add */
    for (int i = 0; i < size; ++i) {
        char *key = malloc(1);
        *key = delimiter[i];
        keys[i] = key;
        int val = i;
        sdr_hash_map_put(&m, key, (void *) (intptr_t) val);
    }

    /* replace */
    char *test_key = malloc(1);
    *test_key = '.';
    cr_assert(pchar_cmp(delimiter, test_key) == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == size);
    cr_assert(sdr_hash_map_put(&m, test_key, (void *) 10) == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == size);

    *test_key = ',';
    cr_assert(sdr_hash_map_put(&m, test_key, (void *) 20) == (void *) 1);
    cr_assert(sdr_hash_map_ctx(&m)->size == size);

    *test_key = '-';
    cr_assert(sdr_hash_map_put(&m, test_key, (void *) 30) == (void *) 2);
    cr_assert(sdr_hash_map_ctx(&m)->size == size);

    /* get */
    *test_key = 'a';
    cr_assert(sdr_hash_map_get(&m, test_key) == NULL);
    *test_key = 'b';
    cr_assert(sdr_hash_map_get(&m, test_key) == NULL);
    *test_key = 'c';
    cr_assert(sdr_hash_map_get(&m, test_key) == NULL);
    *test_key = '.';
    cr_assert(sdr_hash_map_get(&m, test_key) == (void *) 10);
    *test_key = ',';
    cr_assert(sdr_hash_map_get(&m, test_key) == (void *) 20);
    *test_key = '-';
    cr_assert(sdr_hash_map_get(&m, test_key) == (void *) 30);

    /* remove */
    sdr_hash_map_ctx(&m)->free_key = free;
    *test_key = '@';
    cr_assert(sdr_hash_map_remove(&m, test_key) < 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == size);

    *test_key = '.';
    cr_assert(sdr_hash_map_remove(&m, test_key) == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == size - 1);

    *test_key = ',';
    cr_assert(sdr_hash_map_remove(&m, test_key) == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == size - 2);

    *test_key = '-';
    cr_assert(sdr_hash_map_remove(&m, test_key) == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == size - 3);

    free(test_key);
    sdr_hash_map_ctx(&m)->free_key = free;
    sdr_hash_map_destroy(&m);
}

static inline int int_cmp(const int i1, const int i2) {
    return i1 - i2;
}

Test(HashMap, int_int) {
    int arr[] = {94, 87, 666};
    int size = 3;
    SDR_DEFINE_HASH_MAP(m, .hash = my_int_hash,
                        .cmp = (sdr_fn_cmp) int_cmp);

    /* add */
    for (int i = 0; i < size; ++i) {
        int key = arr[i];
        int val = i;
        sdr_hash_map_put(&m,
                         (void *) (intptr_t) key,
                         (void *) (intptr_t) val);
    }

    /* replace */
    cr_assert(sdr_hash_map_ctx(&m)->size == size);
    cr_assert(sdr_hash_map_put(&m, (void *) 94, (void *) 10) == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == size);
    cr_assert(sdr_hash_map_put(&m, (void *) 87, (void *) 20) == (void *) 1);
    cr_assert(sdr_hash_map_ctx(&m)->size == size);
    cr_assert(sdr_hash_map_put(&m, (void *) 666, (void *) 30) == (void *) 2);
    cr_assert(sdr_hash_map_ctx(&m)->size == size);

    /* get */
    cr_assert(sdr_hash_map_get(&m, (void *) 1) == NULL);
    cr_assert(sdr_hash_map_get(&m, (void *) 2) == NULL);
    cr_assert(sdr_hash_map_get(&m, (void *) 3) == NULL);
    cr_assert(sdr_hash_map_get(&m, (void *) 94) == (void *) 10);
    cr_assert(sdr_hash_map_get(&m, (void *) 87) == (void *) 20);
    cr_assert(sdr_hash_map_get(&m, (void *) 666) == (void *) 30);

    /* remove */
    cr_assert(sdr_hash_map_remove(&m, (void *) 1024) < 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == size);

    cr_assert(sdr_hash_map_remove(&m, (void *) 94) == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == size - 1);

    cr_assert(sdr_hash_map_remove(&m, (void *) 87) == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == size - 2);

    cr_assert(sdr_hash_map_remove(&m, (void *) 666) == 0);
    cr_assert(sdr_hash_map_ctx(&m)->size == size - 3);

    sdr_hash_map_destroy(&m);
}

Test(HashMap, foreach) {
    SDR_DEFINE_HASH_MAP(m);

    sdr_hash_map_put(&m, "Sheep", (void *) 1);
    sdr_hash_map_put(&m, "Jason", (void *) 2);
    sdr_hash_map_put(&m, "Lily", (void *) 3);
    sdr_hash_map_put(&m, "WanWan", (void *) 4);


    SdrMapEntry *ptr;

    sdr_hash_map_for_s(&m, ptr) {
        char *name = ptr->key;
        if (!strcmp(name, "Sheep")) {
            cr_assert((int) ptr->value == 1);
        }
        else if (!strcmp(name, "Jason")) {
            cr_assert((int) ptr->value == 2);
        }
        else if (!strcmp(name, "Lily")) {
            cr_assert((int) ptr->value == 3);
        }
        else if (!strcmp(name, "WanWan")) {
            cr_assert((int) ptr->value == 4);
        } else {
            cr_assert_fail("hash_map_for_s error");
        }
    }
}