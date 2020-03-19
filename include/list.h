//
// Created by Jason Zheng on 2020/3/19.
//

#ifndef SANDRA_LIST_H
#define SANDRA_LIST_H

#include "embed_list.h"

typedef struct sdr_list {
    void *data;
    SdrEmbedList node;
} SdrList;

#define SDR_DEFINE_LIST(m_name) \
    SdrList m_name; \
    sdr_list_init(&m_name)

#define SDR_DEFINE_LIST_NODE(m_name, m_data) \
    SdrList m_name; \
    m_name.data = m_data;

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

static inline int sdr_list_insert(SdrList *list, SdrList *new, size_t idx) {
    return sdr_elist_insert(&list->node, &new->node, idx);
}

static inline void sdr_list_remove(SdrList *node) {
    sdr_elist_remove(&node->node);
}

static inline SdrList *sdr_list_pop_front(SdrList *list) {
    SdrEmbedList *ret = sdr_elist_pop_front(&list->node);
    return ret ? sdr_elist_data(ret, SdrList, node) : NULL;
}

static inline SdrList *sdr_list_pop_back(SdrList *list) {
    SdrEmbedList *ret = sdr_elist_pop_back(&list->node);
    return ret ? sdr_elist_data(ret, SdrList, node) : NULL;
}

#define sdr_list_entry(list, idx) ({ \
    SdrEmbedList *m_le_curr = sdr_elist_entry(&(list)->node, idx); \
    (m_le_curr) ? sdr_elist_data(m_le_curr, SdrList, node) : NULL; \
})

#define sdr_list_first(list) ({ \
    SdrEmbedList *m_lf_ret = sdr_list_first(&(list)->node); \
    (m_lf_ret) ? sdr_elist_data(m_lf_ret, SdrList, node) : NULL; \
})

#define sdr_list_last(list) ({ \
    SdrEmbedList *m_ll_ret = sdr_list_last(&(list)->node); \
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

#define sdr_list_for(list, ptr) \
    for(SdrList *ptr = sdr_elist_data((list)->node.next, SdrList, node); \
        ptr != (list); ptr = sdr_elist_data(ptr->node.next, SdrList, node))

#define sdr_list_for_s(list, ptr) \
    for(SdrList *ptr = sdr_elist_data((list)->node.next, SdrList, node), \
        *m_lfs_tmp = sdr_elist_data(ptr->node.next, SdrList, node); \
        ptr != (list); ptr = m_lfs_tmp, \
        m_lfs_tmp = sdr_elist_data(m_lfs_tmp->node.next, SdrList, node))

#define sdr_list_size(list) ({ \
    sdr_elist_size(&(list)->node); \
})

#endif //SANDRA_LIST_H
