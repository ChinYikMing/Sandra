//
// Created by Jason Zheng on 2020/3/12.
//

#ifndef SANDRA_HASH_MAP_H
#define SANDRA_HASH_MAP_H

#include "basis.h"
#include "map.h"
#include "hash.h"
#include "abs_hash_map.h"
#include "compiler_attr.h"
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

typedef struct sdr_hash_map_iter {
    SdrHashMap *map;
    SdrMapEntry *const buckets;
    const size_t capacity;
    size_t i;
} SdrHashMapIter;

/**
 * HashMapInitArgs extends from AbsHMapInitArgs:
 * use the same field order to ensure the safe cast operation
 */
SDR_SUPPRESS_WARN("-Wunknown-attributes")
typedef struct sdr_attr_designated_init sdr_hash_map_init_args {
    SdrAbsHMapInitArgsField;
    double load_factor;
} SdrHashMapInitArgs;
SDR_SUPPRESS_WARN_END

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

static inline SdrMapCtx *sdr_hash_map_ctx(SdrHashMap *map) {
    return &map->base.ctx;
}

void sdr_hash_map_destroy(SdrHashMap *map);

static inline SdrHashMapIter sdr_hash_map_iter_create(SdrHashMap *map) {
    return (SdrHashMapIter) {
        .map = map,
        .capacity = map->base.capacity,
        .buckets = map->base.buckets,
        .i = 0
    };
}

static inline SdrHashMapIter sdr_hash_map_iter_create_s(SdrHashMap *map) {
    SdrHashMapIter iter = {
        .map = map,
        .capacity = map->base.capacity,
        .buckets = map->base.buckets,
        .i = 0
    };

    size_t i = 0;
    const size_t capacity = map->base.capacity;
    SdrMapEntry *buckets = map->base.buckets;
    SdrMapEntry *ptr = buckets;
    while (i < capacity && ptr->status != SDR_MAP_ENTRY_ADDED) {
        i++;
        ptr = buckets + i;
    }
    iter.i = i;
    return iter;
}

static inline void sdr_hash_map_iter_next_s(SdrHashMapIter *iter) {
    SdrMapEntry *ptr;
    SdrMapEntry *buckets = iter->buckets;
    const size_t capacity = iter->capacity;
    do {
        iter->i++;
        ptr = buckets + iter->i;
    } while ((iter->i < capacity) && ptr->status != SDR_MAP_ENTRY_ADDED);
}

#define sdr_hash_map_for(m_map, m_ptr) \
    for (SdrHashMapIter m_iter = sdr_hash_map_iter_create(m_map); \
        ((m_ptr) = (m_iter.buckets + m_iter.i)) \
            && m_iter.i < m_iter.capacity; \
        m_iter.i++)

#define sdr_hash_map_for_s(m_map, m_ptr) \
    for (SdrHashMapIter m_iter = sdr_hash_map_iter_create_s(m_map); \
        ((m_ptr) = (m_iter.buckets + m_iter.i)) \
            && m_iter.i < m_iter.capacity; \
        sdr_hash_map_iter_next_s(&m_iter)) \

#endif //SANDRA_HASH_MAP_H
