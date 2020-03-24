//
// Created by Jason Zheng on 2020/3/12.
//

#ifndef SANDRA_ABS_HASH_MAP_H
#define SANDRA_ABS_HASH_MAP_H

#include "basis.h"
#include "size.h"
#include "map.h"
#include "compiler_attr.h"

typedef struct sdr_abs_hash_map SdrAbsHashMap;

typedef size_t (*sdr_fn_hash)(const void *k);

typedef size_t (*sdr_fn_index)(SdrAbsHashMap *map, size_t hashv);

#define SdrAbsHMapInitArgsField \
    sdr_fn_cmp cmp; \
    sdr_fn_hash hash; \
    sdr_fn_index index; \
    unsigned cap_bits; \
    int save_hash_val

struct sdr_abs_hash_map {
    SdrMapCtx ctx;
    sdr_fn_hash hash;
    sdr_fn_index index;
    SdrMapEntry *buckets;
    size_t capacity;
    unsigned cap_bits; /* 2 ^ bits = capacity */
    int save_hash_val;
};

SDR_SUPPRESS_WARN("-Wunknown-attributes")
typedef struct sdr_attr_designated_init sdr_abs_hmap_init_args {
    SdrAbsHMapInitArgsField;
} SdrAbsHMapInitArgs;
SDR_SUPPRESS_WARN_END

int sdr_abs_hmap_init(SdrAbsHashMap *map, SdrAbsHMapInitArgs *args);

void sdr_abs_hmap_destroy(SdrAbsHashMap *map);

static inline size_t sdr_mid_index(SdrAbsHashMap *map, size_t hash) {
    unsigned bits = map->cap_bits;
    if (!hash) return 0;
    unsigned mid = (SDR_SIZE_T_BITS >> 1u);
    if (bits > mid) mid += (bits - mid);
    return sdr_get_bits(hash, mid - 1, bits);
}

#endif //SANDRA_ABS_HASH_MAP_H
