//
// Created by Jason Zheng on 2020/3/19.
//

#ifndef SANDRA_EMBED_LIST_H
#define SANDRA_EMBED_LIST_H

#include "basis.h"
#include "vector.h"
#include "compiler_attr.h"

typedef struct sdr_elist {
    struct sdr_elist *prev;
    struct sdr_elist *next;
} SdrEList;

typedef int (*sdr_fn_elist_cmp)(SdrEList *e1, SdrEList *e2, void *arg);

#define SDR_ELIST_HEAD_INIT(name) { &(name), &(name) }

#define SDR_DEFINE_ELIST(name) \
    SdrEList name = SDR_ELIST_HEAD_INIT(name)


/* ================ PRIVATE METHODS ================= */
sdr_attr_always_inline
static inline void sdr_pvt_elist_remove(SdrEList *prev, SdrEList *next) {
    prev->next = next;
    next->prev = prev;
}

sdr_attr_always_inline
static inline void sdr_pvt_elist_remove_s(SdrEList *prev, SdrEList *next) {
    prev->next->prev = next->prev;
    next->prev->next = prev->next;
    sdr_pvt_elist_remove(prev, next);
}

sdr_attr_always_inline
static inline void sdr_pvt_elist_add(SdrEList *prev, SdrEList *next, SdrEList *new) {
    prev->next = new;
    new->prev = prev;
    next->prev = new;
    new->next = next;
}

sdr_attr_always_inline
static inline void sdr_pvt_elist_add_s(SdrEList *prev, SdrEList *next, SdrEList *new) {
    sdr_pvt_elist_remove(new->prev, new->next);
    sdr_pvt_elist_add(prev, next, new);
}

sdr_attr_always_inline
static inline void sdr_pvt_elist_splice(SdrEList *prev, SdrEList *next,
                                        SdrEList *new_head, SdrEList *new_tail) {
    prev->next = new_head;
    new_head->prev = prev;
    new_tail->next = next;
    next->prev = new_tail;
}

sdr_attr_always_inline
static inline void sdr_pvt_elist_splice_s(SdrEList *prev, SdrEList *next,
                                          SdrEList *new_head, SdrEList *new_tail) {
    sdr_pvt_elist_remove(new_head->prev, new_tail->next);
    sdr_pvt_elist_splice(prev, next, new_head, new_tail);
}


/* ================= PUBLIC METHODS ================= */
static inline void sdr_elist_init(SdrEList *list) {
    list->next = list->prev = list;
}

static inline void sdr_elist_remove_entry(SdrEList *entry) {
    sdr_pvt_elist_remove(entry->prev, entry->next);
}

static inline void sdr_elist_remove_entry_s(SdrEList *entry) {
    if (!entry) return;
    sdr_pvt_elist_remove(entry->prev, entry->next);
    entry->prev = entry;
    entry->next = entry;
}

static inline void sdr_elist_push_front(SdrEList *list, SdrEList *new) {
    sdr_pvt_elist_add(list, list->next, new);
}

static inline int sdr_elist_push_front_s(SdrEList *list, SdrEList *new) {
    if (list == new || !new) return -1;
    sdr_pvt_elist_add_s(list, list->next, new);
    return 0;
}

static inline void sdr_elist_push_back(SdrEList *list, SdrEList *new) {
    sdr_pvt_elist_add(list->prev, list, new);
}

static inline int sdr_elist_push_back_s(SdrEList *list, SdrEList *new) {
    if (list == new || !new) return -1;
    sdr_pvt_elist_add_s(list->prev, list, new);
    return 0;
}

static inline SdrEList *sdr_elist_pop_front(SdrEList *list) {
    SdrEList *ret = list->next;
    if (list == ret) return NULL;
    sdr_elist_remove_entry(ret);
    return ret;
}

static inline SdrEList *sdr_elist_pop_front_s(SdrEList *list) {
    SdrEList *ret = list->next;
    if (list == ret) return NULL;
    sdr_elist_remove_entry_s(ret);
    return ret;
}

static inline SdrEList *sdr_elist_pop_back(SdrEList *list) {
    SdrEList *ret = list->prev;
    if (list == ret) return NULL;
    sdr_elist_remove_entry(ret);
    return ret;
}

static inline SdrEList *sdr_elist_pop_back_s(SdrEList *list) {
    SdrEList *ret = list->prev;
    if (list == ret) return NULL;
    sdr_elist_remove_entry_s(ret);
    return ret;
}

static inline void sdr_elist_replace(SdrEList *old, SdrEList *new) {
    new->prev = old->prev;
    old->prev->next = new;
    new->next = old->next;
    old->next->prev = new;
}

static inline void sdr_elist_replace_s(SdrEList *old, SdrEList *new) {
    if (old == new || !new) return;
    sdr_elist_replace(old, new);
    old->prev = old;
    old->next = old;
}

static inline void sdr_elist_swap(SdrEList *e1, SdrEList *e2) {
    SdrEList *e1_insert_pos = e2->prev;
    sdr_elist_remove_entry(e2);
    sdr_elist_replace(e1, e2);
    if (e1_insert_pos == e1)
        e1_insert_pos = e2;
    sdr_pvt_elist_add(e1_insert_pos, e1_insert_pos->next, e1);
}

int sdr_elist_insert(SdrEList *list, SdrEList *new, long idx);

int sdr_elist_insert_s(SdrEList *list, SdrEList *new, long idx);

int sdr_elist_remove(SdrEList *list, long idx);

int sdr_elist_remove_s(SdrEList *list, long idx);

void sdr_elist_sort(SdrEList *list, sdr_fn_elist_cmp cmp, void *data);


/* ========== BULK API ========== */
SdrEList *sdr_elist_splice_d(SdrEList *list, long start, size_t delete_cnt,
                             SdrEList *head, SdrEList *tail);

static inline SdrEList *sdr_elist_splice(SdrEList *list, long start, size_t delete_cnt,
                                         SdrEList *target_list) {
    SdrEList *splice_head = NULL;
    SdrEList *splice_tail = NULL;
    if (target_list) {
        splice_head = target_list->next;
        splice_tail = target_list->prev;
        if (splice_head == target_list)
            return NULL;
    }

    return sdr_elist_splice_d(list, start, delete_cnt, splice_head, splice_tail);
}

static inline void sdr_elist_bulk_push_front(SdrEList *list, SdrEList *head, SdrEList *tail) {
    sdr_pvt_elist_splice(list, list->next, head, tail);
}

static inline void sdr_elist_bulk_push_front_s(SdrEList *list, SdrEList *head, SdrEList *tail) {
    sdr_pvt_elist_splice_s(list, list->next, head, tail);
}

static inline void sdr_elist_bulk_push_back(SdrEList *list, SdrEList *head, SdrEList *tail) {
    sdr_pvt_elist_splice(list->prev, list, head, tail);
}

static inline void sdr_elist_bulk_push_back_s(SdrEList *list, SdrEList *head, SdrEList *tail) {
    sdr_pvt_elist_splice_s(list->prev, list, head, tail);
}

int sdr_elist_bulk_insert(SdrEList *list, SdrEList *first, SdrEList *last, long idx);

int sdr_elist_bulk_insert_s(SdrEList *list, SdrEList *first, SdrEList *last, long idx);


/* ==== FUNCTION-LIKE MACROS ==== */
#define sdr_elist_entry(list, idx) ({ \
    SdrEList *m_ee_curr; \
    size_t m_ee_idx = 0; \
    long long m_ee_target_idx = (idx); \
    if (m_ee_target_idx >= 0) { \
        m_ee_curr = (list)->next; \
        while(m_ee_idx < m_ee_target_idx && m_ee_curr != (list)) \
            m_ee_idx++, m_ee_curr = m_ee_curr->next; \
    } else { \
        m_ee_idx = 1; \
        m_ee_target_idx *= -1; \
        m_ee_curr = (list)->prev; \
        while (m_ee_idx < m_ee_target_idx && m_ee_curr != (list)) \
            m_ee_idx++, m_ee_curr = m_ee_curr->prev; \
    } \
    (m_ee_idx == m_ee_target_idx && m_ee_curr != (list)) \
        ? m_ee_curr : NULL; \
})

#define sdr_elist_first(list) ({ \
    SdrEList *m_ef_ret = (list)->next; \
    (m_ef_ret != (list)) ? m_ef_ret : NULL; \
})

#define sdr_elist_last(list) ({ \
    SdrEList *m_el_ret = (list)->prev; \
    (m_el_ret != (list)) ? m_el_ret : NULL; \
})

#define sdr_elist_get(list, idx, type, member) ({ \
     SdrEList *m_eg_entry = sdr_elist_entry(list, idx); \
     (m_eg_entry) ? sdr_elist_data(m_eg_entry, type, member) : NULL; \
})

#define sdr_elist_data(list, type, member) ({ \
    sdr_container_of(list, type, member); \
})

#define sdr_elist_is_empty(list) ({ \
    (list)->next == (list); \
})

#define sdr_elist_is_singular(list) ({ \
    !sdr_elist_is_empty(list) && ((list)->next == (list)->prev); \
})

#define sdr_elist_for(list, ptr) \
    for(ptr = (list)->next; ptr != (list); ptr = ptr->next)

#define sdr_elist_for_s(list, ptr, tmp) \
    for(ptr = (list)->next, tmp = ptr->next; \
        ptr != (list); ptr = tmp, tmp = tmp->next)

#define sdr_elist_for_data(list, ptr, type, member) \
    for(SdrEList *m_efd_ptr = (list)->next; \
    m_efd_ptr != (list) && ((ptr) = sdr_elist_data(m_efd_ptr, type, member)); \
    m_efd_ptr = m_efd_ptr->next)

#define sdr_elist_find(list, fn_test, data, type, member) ({ \
        void *m_ef_ret = NULL; \
        SdrEList *m_ef_ptr; \
        sdr_elist_for(list, m_ef_ptr) { \
            void *m_ef_tmp = sdr_elist_data(m_ef_ptr, type, member); \
            if ((fn_test(m_ef_tmp, (data)))) { \
                m_ef_ret = m_ef_tmp; \
                break; \
            } \
        } \
        m_ef_ret; \
})

/*
 * This macro is destructive
 */
#define sdr_elist_filter(list, filtered_list, fn_test, data, type, member) ({ \
        sdr_elist_init((filtered_list)); \
        SdrEList *m_ef_ptr, *m_ef_tmp; \
        sdr_elist_for_s(list, m_ef_ptr, m_ef_tmp) { \
            void *m_ef_data = sdr_elist_data(m_ef_ptr, type, member); \
            if ((fn_test(m_ef_data, (data)))) { \
                sdr_elist_push_back_s((filtered_list), m_ef_ptr); \
            } \
        } \
})

#define sdr_elist_filter_s(list, vec, fn_test, data, type, member) ({ \
        sdr_vector_init((vec)); \
        SdrEList *m_ef_ptr; \
        sdr_elist_for(list, m_ef_ptr) { \
            void *m_ef_tmp = sdr_elist_data(m_ef_ptr, type, member); \
            if ((fn_test(m_ef_tmp, (data)))) { \
                sdr_vector_push_back((vec), m_ef_tmp); \
            } \
        } \
})

#define sdr_elist_size(list) ({ \
    size_t m_es_ret = 0; \
    SdrEList *m_es_ptr; \
    sdr_elist_for(list, m_es_ptr) { \
        m_es_ret++; \
    } \
    m_es_ret; \
})

#endif //SANDRA_EMBED_LIST_H
