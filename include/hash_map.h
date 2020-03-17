//
// Created by Jason Zheng on 2020/3/12.
//

#ifndef SANDRA_HASH_MAP_H
#define SANDRA_HASH_MAP_H

#include "basis.h"
#include "map.h"
#include "hash.h"
#include "abs_hash_map.h"
#include <string.h>

#define SDR_HMAP_DFLT_LOAD_FACTOR 0.75
#define SDR_HMAP_DFLT_CAP_BITS 4u

#define SDR_DEFINE_HASH_MAP(name, ...) \
    SdrHashMap name; \
    sdr_hash_map_init(&name, __VA_ARGS__)

typedef struct sdr_hash_map {
    /* composition over inheritance */
    SdrAbsHashMap base;
    size_t max_load;
    size_t *hashv_list;
    int n_expand;
    double load_factor;
} SdrHashMap;

/**
 * HashMapInitArgs extends from AbsHMapInitArgs:
 * use the same field order to ensure the safe cast operation
 */
typedef struct sdr_hash_map_init_args {
    SdrAbsHMapInitArgsField;
    double load_factor;
} SdrHashMapInitArgs;

#define sdr_hash_map_init(map, ...) ({ \
SDR_SUPPRESS_WARN("-Winitializer-overrides") \
    sdr_pvt_hash_map_init((map), \
        (&((SdrHashMapInitArgs){ \
        .cmp = (sdr_fn_cmp) strcmp, \
        .hash = NULL, \
        .index = NULL, \
        .cap_bits = SDR_HMAP_DFLT_CAP_BITS, \
        .save_hash_val = 1, \
        .load_factor = SDR_HMAP_DFLT_LOAD_FACTOR, \
        __VA_ARGS__ \
    }))); \
SDR_SUPPRESS_WARN_END \
})

#define sdr_hash_map_base(map) \
    (map)->base;

#define sdr_hash_map_hashv(map, key) ({ \
    (map)->base.hash ? (map)->base.hash(key) : SDR_STR_HASH65(key); \
})

#define sdr_hash_map_index(map, hashv) ({ \
    SdrAbsHashMap *m_hmi_base = &sdr_hash_map_base(map); \
    m_hmi_base->index ? m_hmi_base->index(m_hmi_base, (hashv)) \
        : (hashv) & (m_hmi_base->capacity - 1u); \
})

int sdr_pvt_hash_map_init(SdrHashMap *map, SdrHashMapInitArgs *args);

void *sdr_hash_map_put(SdrHashMap *map, const void *k, const void *v);

void *sdr_hash_map_entry(SdrHashMap *map, const void *k);

static inline void *sdr_hash_map_get(SdrHashMap *map, const void *k) {
    SdrMapEntry *entry = sdr_hash_map_entry(map, k);
    return entry ? entry->value : NULL;
}

int sdr_hash_map_remove(SdrHashMap *map, const void *k);

void sdr_hash_map_for_each(SdrHashMap *map, int (*task)(SdrMapEntry *, size_t, int argc, void **argv),
                           int argc, void **argv);

static inline SdrMapCtx *sdr_hash_map_ctx(SdrHashMap *map) {
    return &map->base.ctx;
}

void sdr_hash_map_destroy(SdrHashMap *map);

#endif //SANDRA_HASH_MAP_H
