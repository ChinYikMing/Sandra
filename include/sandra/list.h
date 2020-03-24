//
// Created by Jason Zheng on 2020/3/19.
//

#ifndef SANDRA_LIST_H
#define SANDRA_LIST_H

#include "embed_list.h"

typedef struct sdr_list {
    void *data;
    SdrEList node;
} SdrList;

typedef int (*sdr_fn_list_cmp)(SdrList *e1, SdrList *e2, void *arg);

typedef struct sdr_list_pvt_cmp_data {
    sdr_fn_list_cmp cmp;
    void *data;
} SdrListPvtCmpData;

static inline int sdr_list_pvt_cmp(SdrEList *e1, SdrEList *e2, void *arg) {
    SdrList *l1 = sdr_elist_data(e1, SdrList, node);
    SdrList *l2 = sdr_elist_data(e2, SdrList, node);
    SdrListPvtCmpData *cmp_data = arg;
    return cmp_data->cmp(l1, l2, cmp_data->data);
}

#define SDR_DEFINE_LIST(m_name) \
    SdrList m_name; \
    sdr_list_init(&m_name)

#define SDR_DEFINE_LIST_NODE(m_name, m_data) \
    SdrList m_name; \
    m_name.data = m_data;


/* ================= PUBLIC METHODS ================= */
static inline SdrList *sdr_list_new_node(void *data) {
    SdrList *tmp = malloc(sizeof(SdrList));
    if (!tmp) return NULL;
    tmp->data = data;
    return tmp;
}

static inline void sdr_list_init(SdrList *list) {
    sdr_elist_init(&list->node);
}

static inline void sdr_list_push_front(SdrList *list, SdrList *new) {
    sdr_elist_push_front(&list->node, &new->node);
}

static inline void sdr_list_push_back(SdrList *list, SdrList *new) {
    sdr_elist_push_back(&list->node, &new->node);
}

static inline void sdr_list_remove_entry(SdrList *entry) {
    sdr_elist_remove_entry(&entry->node);
}

static inline SdrList *sdr_list_pop_front(SdrList *list) {
    SdrEList *ret = sdr_elist_pop_front(&list->node);
    return ret ? sdr_elist_data(ret, SdrList, node) : NULL;
}

static inline SdrList *sdr_list_pop_back(SdrList *list) {
    SdrEList *ret = sdr_elist_pop_back(&list->node);
    return ret ? sdr_elist_data(ret, SdrList, node) : NULL;
}

static inline void sdr_list_replace(SdrList *old, SdrList *new) {
    sdr_elist_replace(&old->node, &new->node);
}

static inline void sdr_list_swap(SdrList *e1, SdrList *e2) {
    sdr_elist_swap(&e1->node, &e2->node);
}

static inline int sdr_list_insert(SdrList *list, SdrList *new, size_t idx) {
    return sdr_elist_insert(&list->node, &new->node, idx);
}

static inline void sdr_list_remove(SdrList *node) {
    sdr_elist_remove_entry(&node->node);
}

static inline void sdr_list_sort(SdrList *list, sdr_fn_list_cmp cmp, void *data) {
    sdr_elist_sort(&list->node, sdr_list_pvt_cmp, &(SdrListPvtCmpData) {
        .cmp = cmp,
        .data = data
    });
}


/* ========= SECURE API ========= */
static inline int sdr_list_push_front_s(SdrList *list, SdrList *new) {
    if (!list || !new) return -1;
    return sdr_elist_push_front_s(&list->node, &new->node);
}

static inline int sdr_list_push_back_s(SdrList *list, SdrList *new) {
    if (!list || !new) return -1;
    return sdr_elist_push_back_s(&list->node, &new->node);
}

static inline void sdr_list_remove_entry_s(SdrList *entry) {
    if (!entry) return;
    sdr_elist_remove_entry_s(&entry->node);
}

static inline SdrList *sdr_list_pop_front_s(SdrList *list) {
    if (!list) return NULL;
    SdrEList *ret = sdr_elist_pop_front_s(&list->node);
    return ret ? sdr_elist_data(ret, SdrList, node) : NULL;
}

static inline SdrList *sdr_list_pop_back_s(SdrList *list) {
    if (!list) return NULL;
    SdrEList *ret = sdr_elist_pop_back_s(&list->node);
    return ret ? sdr_elist_data(ret, SdrList, node) : NULL;
}

int sdr_list_remove_s(SdrList *list, long idx) {
    return sdr_elist_remove_s(&list->node, idx);
}

static inline void sdr_list_replace_s(SdrList *old, SdrList *new) {
    if (old == new) return;
    sdr_elist_replace(&old->node, &new->node);
}


/* ========== BULK API ========== */
SdrList *sdr_list_splice_d(SdrList *list, long start, size_t delete_cnt,
                           SdrList *head, SdrList *tail) {
    if (!list) return NULL;
    SdrEList *ret = sdr_elist_splice_d(&list->node, start, delete_cnt,
                                       &head->node, &tail->node);
    return ret ? sdr_elist_data(ret, SdrList, node) : NULL;
}

static inline SdrList *sdr_list_splice(SdrList *list, long start, size_t delete_cnt,
                                       SdrList *target_list) {
    if (!list || !target_list) return NULL;
    SdrEList *ret = sdr_elist_splice(&list->node, start, delete_cnt, &target_list->node);
    return ret ? sdr_elist_data(ret, SdrList, node) : NULL;
}

static inline void sdr_list_bulk_push_front(SdrList *list, SdrList *head, SdrList *tail) {
    sdr_elist_bulk_push_front(&list->node, &head->node, &tail->node);
}

static inline void sdr_list_bulk_push_back(SdrList *list, SdrList *head, SdrList *tail) {
    sdr_elist_bulk_push_back(&list->node, &head->node, &tail->node);
}

int sdr_list_bulk_insert(SdrList *list, SdrList *first, SdrList *last, long idx) {
    return sdr_elist_bulk_insert(&list->node, &first->node, &last->node, idx);
}


#define sdr_list_entry(list, idx) ({ \
    SdrEList *m_le_curr = sdr_elist_entry(&(list)->node, idx); \
    (m_le_curr) ? sdr_elist_data(m_le_curr, SdrList, node) : NULL; \
})

#define sdr_list_first(list) ({ \
    SdrEList *m_lf_ret = sdr_list_first(&(list)->node); \
    (m_lf_ret) ? sdr_elist_data(m_lf_ret, SdrList, node) : NULL; \
})

#define sdr_list_last(list) ({ \
    SdrEList *m_ll_ret = sdr_list_last(&(list)->node); \
    (m_ll_ret) ? sdr_elist_data(m_ll_ret, SdrList, node) : NULL; \
})

#define sdr_list_get(list, idx) ({ \
    SdrList *m_lg_entry = sdr_list_entry(list, idx); \
    (m_lg_entry) ? sdr_list_data(m_lg_entry) : NULL; \
})

#define sdr_list_data(list) ({ \
    (list)->data; \
})

#define sdr_list_is_empty(list) ({ \
    sdr_elist_is_empty(&(list)->node); \
})

#define sdr_list_is_singular(list) ({ \
    sdr_elist_is_singular(&(list)->node); \
})

#define sdr_list_for(list, ptr) \
    for(ptr = sdr_elist_data((list)->node.next, SdrList, node); \
        ptr != (list); ptr = sdr_elist_data(ptr->node.next, SdrList, node))

#define sdr_list_for_s(list, ptr) \
    for(ptr = sdr_elist_data((list)->node.next, SdrList, node), \
        *m_lfs_tmp = sdr_elist_data(ptr->node.next, SdrList, node); \
        ptr != (list); ptr = m_lfs_tmp, \
        m_lfs_tmp = sdr_elist_data(m_lfs_tmp->node.next, SdrList, node))

#define sdr_list_size(list) ({ \
    sdr_elist_size(&(list)->node); \
})

#endif //SANDRA_LIST_H
