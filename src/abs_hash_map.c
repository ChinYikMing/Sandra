//
// Created by Jason Zheng on 2020/3/12.
//

#include <sandra/abs_hash_map.h>
#include <stdlib.h>

int sdr_abs_hmap_init(SdrAbsHashMap *map, SdrAbsHMapInitArgs *args) {
    const unsigned cap_bits = args->cap_bits;
    size_t n_buckets = 1u << cap_bits;
    map->buckets = calloc(n_buckets, sizeof(SdrMapEntry));
    if (!map->buckets) return SDR_MAP_ERR_MEM_ALLOC;
    map->capacity = n_buckets;
    map->cap_bits = cap_bits;

    const sdr_fn_cmp cmp = args->cmp;
    map->ctx.size = 0;
    map->ctx.free_key = NULL;
    map->ctx.free_val = NULL;
    map->ctx.on_equal = NULL;
    map->ctx.cmp = cmp;

    const sdr_fn_hash hash = args->hash;
    const sdr_fn_index index = args->index;
    map->hash = hash;
    map->index = index;
    map->save_hash_val = args->save_hash_val;

    return 0;
}

void sdr_abs_hmap_destroy(SdrAbsHashMap *map) {
    SdrMapEntry *bucket = map->buckets;
    sdr_fn_free free_k = map->ctx.free_key;
    sdr_fn_free free_v = map->ctx.free_val;
    if (free_k || free_v)
        for (size_t i = 0; i < map->capacity; i++) {
            SdrMapEntry *entry = bucket + i;
            if (entry->key) {
                if (free_k)free_k(entry->key);
                if (free_v)free_v(entry->value);
            }
        }
    free(map->buckets);
    map->buckets = NULL;
}
