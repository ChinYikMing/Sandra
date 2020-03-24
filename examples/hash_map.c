//
// Created by Jason Zheng on 2020/3/17.
//
#include <assert.h>
#include <sandra/hash_map.h>

static inline int on_key_equal(SdrMapEntry *entry, const void *k, const void *v) {
    printf("insert: %d failure, key already exist.\n", *(int *) v);
    return 0;
}

static inline size_t my_hash_fn(const void *str) {
    return SDR_DJB2(9487, 10u, str);
}

int main() {
    SdrHashMap map;
    sdr_hash_map_init(&map, .cap_bits = 5, .hash = my_hash_fn);

    sdr_hash_map_put(&map, "apple", &(int) {87});
    int val = *(int *) sdr_hash_map_get(&map, "apple");
    assert(val == 87);

    int *prev = sdr_hash_map_put(&map, "apple", &(int) {66});
    assert(*prev == 87);

    sdr_hash_map_ctx(&map)->on_equal = on_key_equal;

    sdr_hash_map_put(&map, "apple", &(int) {99});
    val = *(int *) sdr_hash_map_get(&map, "apple");
    assert(val == 66);
}
