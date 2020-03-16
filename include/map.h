//
// Created by Jason Zheng on 2020/3/12.
//

#ifndef SANDRA_MAP_H
#define SANDRA_MAP_H

#include <stdlib.h>

#define SDR_MAP_ENTRY_EMPTY 0
#define SDR_MAP_ENTRY_ADDED 1
#define SDR_MAP_ENTRY_DELETED 2

#define SDR_MAP_ERR_ILLEGAL_ARGS -1
#define SDR_MAP_ERR_MEM_ALLOC -2

#define SDR_IMPL_MAP(type, map) ({ \
    (map)->put = (sdr_fn_map_put) sdr_##type##_put; \
    (map)->get = (sdr_fn_map_get) sdr_##type##_get; \
    (map)->entry = (sdr_fn_map_entry) sdr_##type##_entry; \
    (map)->remove = (sdr_fn_map_remove) sdr_##type##_remove; \
    (map)->for_each = (sdr_fn_map_for_each) sdr_##type##_for_each; \
    (map)->ctx = (sdr_fn_map_ctx) sdr_##type##_ctx; \
    (map)->destroy = (sdr_fn_map_destroy) sdr_##type##_destroy; \
})

#define SDR_INIT_MAP(type, name, ...) ({ \
    (name)->impl = (&(struct sdr_##type) {}); \
    sdr_##type##_init((name)->impl, __VA_ARGS__); \
    SDR_IMPL_MAP(type, (name)); \
})

#define SDR_DEFINE_MAP(type, name, ...) \
    SdrMap name; \
    SDR_INIT_MAP(type, &name, __VA_ARGS__)
    
typedef struct sdr_map SdrMap;
typedef struct sdr_map_entry {
    void *key;
    void *value;
    int status;
} SdrMapEntry;

/* ctx functions */
typedef int (*sdr_fn_cmp)(const void *, const void *);
typedef int (*sdr_fn_on_equal)(SdrMapEntry *entry, const void *k, const void *v);
typedef void (*sdr_fn_free)(void *);

typedef struct sdr_map_ctx {
    sdr_fn_cmp cmp;
    sdr_fn_on_equal on_equal;
    sdr_fn_free free_key;
    sdr_fn_free free_val;
    size_t size;
} SdrMapCtx;

/* methods */
typedef void *(*sdr_fn_map_put)(const void *impl, const void *k, const void *v);
typedef void *(*sdr_fn_map_get)(const void *impl, const void *k);
typedef SdrMapEntry *(*sdr_fn_map_entry)(const void *impl, const void *k);
typedef int (*sdr_fn_map_remove)(const void *impl, const void *k);
typedef void (*sdr_fn_map_for_each)(const void *impl,
                                int (*task)(SdrMapEntry *, size_t, int argc, void **argv),
                                int argc, void **argv);
typedef SdrMapCtx *(*sdr_fn_map_ctx)(const void *impl);
typedef void (*sdr_fn_map_destroy)(void *impl);

struct sdr_map {
    void *impl;

    /* methods */
    sdr_fn_map_put put;
    sdr_fn_map_get get;
    sdr_fn_map_entry entry;
    sdr_fn_map_remove remove;
    sdr_fn_map_for_each for_each;
    sdr_fn_map_ctx ctx;
    sdr_fn_map_destroy destroy;
};

/**
 * Associates the specified value with the specified key in this map.
 * If the map previously contained a mapping for the key, the old value is replaced.
 * @param map  The hash map to put this entry
 * @param k  key with which the specified value is to be associated
 * @param v  value to be associated with the specified key
 * @return   If the free_val function is not provided, the address of the old value
 *           is returned for the client to release it, so that there is no need to
 *           check for the existence of 'MapEntry' before adding it. It avoids the
 *           hassle of maintaining old entries and the twice-hashing performance
 *           issue when adding.
 */
static inline void *sdr_map_put(const SdrMap *map, const void *k, const void *v) {
    return map->put(map->impl, k, v);
}

static inline SdrMapEntry *sdr_map_entry(const SdrMap *map, const void *k) {
    return map->entry(map->impl, k);
}

/**
 * Returns the value to which the specified key is mapped, or NULL if this map contains
 * no mapping for the key.
 *
 * @param map  The hash map to get the value
 * @param k  key whose associated value is to be returned
 * @return  the value to which the specified key is mapped, or NULL if this map contains
 *          no mapping for the key
 */
static inline void *sdr_map_get(const SdrMap *map, const void *k) {
    return map->get(map->impl, k);
}

static inline int sdr_map_remove(const SdrMap *map, const void *k) {
    return map->remove(map->impl, k);
}

static inline void sdr_map_for_each(const SdrMap *map, int (*task)(SdrMapEntry *, size_t, int argc, void **argv),
                                    int argc, void **argv) {
    map->for_each(map->impl, task, argc, argv);
}

static inline SdrMapCtx *sdr_map_ctx(const SdrMap *map) {
    return map->ctx(map->impl);
}

static inline void sdr_map_destroy(SdrMap *map) {
    map->destroy(map->impl);
    map->impl = NULL;
}

#endif //SANDRA_MAP_H
