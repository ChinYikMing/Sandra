//
// Created by Jason Zheng on 2020/3/12.
//

#include <sandra/hash_map.h>
#include <string.h>
#include <assert.h>

typedef struct sdr_rehash_entry {
    SdrMapEntry entry;
    size_t hash_val;
} SdrRehashEntry;


/* ========= PRIVATE METHODS (DECLARATION) ========== */
static int sdr_hash_map_rehash(SdrHashMap *map);

static void *sdr_pvt_hash_map_put(SdrHashMap *map, const void *k, const void *v, size_t hashv);

/**
* @param index       probing starting index
* @param find_empty  if it finds an empty bucket before the target entry,
*                    it returns a pointer to this empty bucket if 'find_empty' is true.
* @return            pointer to MapEntry which key is equal to k (unless find_empty case)
*/
static SdrMapEntry *sdr_hash_map_linear_probing(SdrHashMap *map, const void *key, size_t index, int find_empty);

static SdrMapEntry *sdr_hash_map_quadratic_probing(SdrHashMap *map, const void *k, size_t index, int find_empty);


/* ================= PUBLIC METHODS ================= */
void *sdr_hash_map_put(SdrHashMap *map, const void *k, const void *v) {
    const size_t hashv = sdr_hash_map_hashv(map, k);
    return sdr_pvt_hash_map_put(map, k, v, hashv);
}

void *sdr_hash_map_entry(SdrHashMap *map, const void *k) {
    const size_t hashv = sdr_hash_map_hashv(map, k);
    const size_t index = sdr_hash_map_index(map, hashv);
    SdrMapEntry *entry = sdr_hash_map_linear_probing(map, k, index, 0);
    return entry ? entry : NULL;
}

int sdr_hash_map_remove(SdrHashMap *map, const void *k) {
    SdrAbsHashMap *base = &map->base;
    SdrMapEntry *entry = sdr_hash_map_entry(map, k);
    if (!entry) return -1;
    sdr_fn_free free_k = base->ctx.free_key;
    sdr_fn_free free_v = base->ctx.free_val;
    if (free_k) free_k(entry->key);
    if (free_v) free_v(entry->value);
    entry->key = NULL;
    entry->value = NULL;
    entry->status = SDR_MAP_ENTRY_DELETED;
    base->ctx.size--;

    return 0;
}

void sdr_hash_map_destroy(SdrHashMap *map) {
    sdr_abs_hmap_destroy(&map->base);
    if (map->base.save_hash_val)
        free(map->hashv_list);
}


/* ================ PRIVATE METHODS ================= */
int sdr_pvt_hash_map_init(SdrHashMap *map, SdrHashMapInitArgs *args) {
    int base_init_ret;
    if ((base_init_ret = sdr_abs_hmap_init(&map->base, (SdrAbsHMapInitArgs *) args)) < 0)
        return base_init_ret;

    if (args->save_hash_val) {
        map->hashv_list = calloc(map->base.capacity, sizeof(size_t));
        if (!map->hashv_list) {
            sdr_abs_hmap_destroy(&map->base);
            return SDR_MAP_ERR_MEM_ALLOC;
        }
    } else
        map->hashv_list = NULL;

    map->load_factor = args->load_factor;
    map->max_load = (size_t) (map->base.capacity * args->load_factor);
    map->n_expand = 0;

    return 0;
}

static void *sdr_pvt_hash_map_put(SdrHashMap *map, const void *k, const void *v, size_t hashv) {
    SdrAbsHashMap *base = &map->base;
    size_t index = sdr_hash_map_index(map, hashv);
    SdrMapEntry *entry = sdr_hash_map_linear_probing(map, k, index, 1);
    assert(entry);

    if (entry->status == SDR_MAP_ENTRY_ADDED) {
        /* the bucket has the same key but an older value */
        void *old_val = entry->value;

        sdr_fn_on_equal eq = base->ctx.on_equal;

        /* handle by custom on_equal function */
        if (eq && !eq(entry, k, v))
            return old_val;

        entry->value = (void *) v;

        /* free old value if free_val is provided */
        sdr_fn_free free_val = base->ctx.free_val;
        if (free_val) {
            free_val(old_val);
            return NULL;
        }

        /* free old_val by client */
        return old_val;
    }

    /* the bucket is empty */
    entry->key = (void *) k;
    entry->value = (void *) v;
    entry->status = SDR_MAP_ENTRY_ADDED;
    const size_t size = ++base->ctx.size;

    if (base->save_hash_val)
        map->hashv_list[entry - base->buckets] = hashv;

    /* rehashing */
    if (size >= map->max_load)
        sdr_hash_map_rehash(map);

    return NULL;
}

static int sdr_hash_map_rehash(SdrHashMap *map) {
    SdrAbsHashMap *base = &map->base;
    const size_t old_size = base->capacity;
    const size_t new_size = old_size << 1u;

    void *tmp = realloc(base->buckets, new_size * sizeof(SdrMapEntry));
    if (!tmp) return SDR_MAP_ERR_MEM_ALLOC;
    base->buckets = tmp;
    memset(base->buckets + old_size, 0, old_size * sizeof(SdrMapEntry));

    const int save_hashv = base->save_hash_val;
    if (save_hashv) {
        tmp = realloc(map->hashv_list, new_size * sizeof(size_t));
        if (!tmp) return SDR_MAP_ERR_MEM_ALLOC;
        map->hashv_list = tmp;
    }

    SdrRehashEntry *old_entries = malloc(base->ctx.size * sizeof(SdrRehashEntry));
    if (!old_entries) return SDR_MAP_ERR_MEM_ALLOC;

    map->base.capacity = new_size;
    map->base.cap_bits++;
    map->n_expand++;
    map->max_load = old_size > 2 ? map->max_load << 1u : new_size * map->load_factor;

    size_t esp = 0;
    for (size_t i = 0; i < old_size; i++) {
        SdrMapEntry *curr = base->buckets + i;
        if (curr->status == SDR_MAP_ENTRY_ADDED) {
            old_entries[esp].entry = *curr;
            old_entries[esp].hash_val = save_hashv
                                        ? map->hashv_list[i]
                                        : sdr_hash_map_hashv(map, curr->key);
            memset(curr, 0, sizeof(SdrMapEntry));
            base->ctx.size--;
            esp++;
        } else {
            curr->status = SDR_MAP_ENTRY_EMPTY;
        }
    }

    for (size_t i = 0; i < esp; i++) {
        SdrRehashEntry *e = old_entries + i;
        sdr_pvt_hash_map_put(map, e->entry.key, e->entry.value, e->hash_val);
    }
    free(old_entries);

    return 0;
}

SdrMapEntry *sdr_hash_map_linear_probing(SdrHashMap *map, const void *key, size_t index, int find_empty) {
    SdrAbsHashMap *base = &map->base;
    SdrMapEntry *start = map->base.buckets + index;
    SdrMapEntry *curr = start;
    SdrMapEntry *entry_bound = base->buckets + base->capacity;
    void *curr_key;

    for (int i = 0; i < 2; ++i) {
        for (; curr < entry_bound; curr++) {
            switch (curr->status) {
                case SDR_MAP_ENTRY_EMPTY:
                    return find_empty ? curr : NULL;
                case SDR_MAP_ENTRY_DELETED:
                    continue;
                case SDR_MAP_ENTRY_ADDED:
                    curr_key = curr->key;
                    if (curr_key == key || !base->ctx.cmp(curr_key, key))
                        return curr;
                    break;
            }
        }
        /* find bucket circularly */
        curr = base->buckets;
        entry_bound = start;
    }

    return NULL;
}

SdrMapEntry *sdr_hash_map_quadratic_probing(SdrHashMap *map, const void *k, size_t index, int find_empty) {
    SdrAbsHashMap *base = &map->base;
    SdrMapEntry *curr = base->buckets + index;
    const size_t n_buckets = base->capacity;
    void *curr_key;
    size_t b = index;
    size_t off = 1;

    for (int i = 0; i < n_buckets;) {
        switch (curr->status) {
            case SDR_MAP_ENTRY_EMPTY:
                return find_empty ? curr : NULL;
            case SDR_MAP_ENTRY_DELETED:
                break;
            case SDR_MAP_ENTRY_ADDED:
                curr_key = curr->key;
                if (curr_key == k || !base->ctx.cmp(curr_key, k))
                    return curr;
                break;
        }

        i++;
        /* quadratic function: (h + (0.5 * i) + (0.5 * i^2)) mode m */
        curr = base->buckets + ((size_t) (b = (b + off++)) & (n_buckets - 1));
    }

    return NULL;
}
