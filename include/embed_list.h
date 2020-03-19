//
// Created by Jason Zheng on 2020/3/19.
//

#ifndef SANDRA_EMBED_LIST_H
#define SANDRA_EMBED_LIST_H

#include "basis.h"

typedef struct sdr_embed_list {
    struct sdr_embed_list *next;
    struct sdr_embed_list *prev;
} SdrEmbedList;

#define SDR_DEFINE_EMBED_LIST(name) \
    SdrEmbedList name; \
    sdr_elist_init(&name)

static inline void sdr_elist_init(SdrEmbedList *elist) {
    elist->next = elist->prev = elist;
}

static inline void sdr_elist_push_front(SdrEmbedList *elist, SdrEmbedList *new) {
    SdrEmbedList *next = elist->next;
    elist->next = new;
    new->prev = elist;
    new->next = next;
    next->prev = new;
}

static inline void sdr_elist_push_back(SdrEmbedList *elist, SdrEmbedList *new) {
    sdr_elist_push_front(elist->prev, new);
}

static inline int sdr_elist_insert(SdrEmbedList *elist, SdrEmbedList *new, size_t idx) {
    SdrEmbedList *curr = elist->next;
    size_t curr_idx = 0;
    while (curr_idx < idx && curr != (elist))
        curr_idx++, curr = curr->next;
    if (curr_idx != idx || curr == elist) return -1;
    sdr_elist_push_front(curr->prev, new);
    return 0;
}

static inline void sdr_elist_remove(SdrEmbedList *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

static inline SdrEmbedList *sdr_elist_pop_front(SdrEmbedList *elist) {
    SdrEmbedList *ret = elist->next;
    if (elist == ret) return NULL;
    sdr_elist_remove(ret);
    return ret;
}

static inline SdrEmbedList *sdr_elist_pop_back(SdrEmbedList *elist) {
    SdrEmbedList *ret = elist->prev;
    if (elist == ret) return NULL;
    sdr_elist_remove(ret);
    return ret;
}

#define sdr_elist_entry(elist, idx) ({ \
    SdrEmbedList *m_ee_curr = (elist)->next; \
    size_t m_idx = 0; \
    while(m_idx < idx && m_ee_curr != (elist)) { \
        m_idx++, m_ee_curr = m_ee_curr->next; \
    } \
    (m_idx == idx && m_ee_curr != (elist)) ? m_ee_curr : NULL; \
})

#define sdr_elist_first(elist) ({ \
    SdrEmbedList *m_ef_ret = (elist)->next; \
    (m_ef_ret != (elist)) ? m_ef_ret : NULL; \
})

#define sdr_elist_last(elist) ({ \
    SdrEmbedList *m_el_ret = (elist)->prev; \
    (m_el_ret != (elist)) ? m_el_ret : NULL; \
})

#define sdr_elist_get(elist, idx, type, member) ({ \
     SdrEmbedList *m_eg_entry = sdr_elist_entry(elist, idx); \
     (m_eg_entry) ? sdr_elist_data(m_eg_entry, type, member) : NULL; \
})

#define sdr_elist_data(elist, type, member) ({ \
    sdr_container_of(elist, type, member); \
})

#define sdr_elist_is_empty(elist) ({ \
    (elist)->next == (elist); \
})

#define sdr_elist_for(elist, ptr) \
    for(SdrEmbedList *ptr = (elist)->next; ptr != (elist); ptr = ptr->next)

#define sdr_elist_for_s(elist, ptr) \
    for(SdrEmbedList *ptr = (elist)->next, *m_efs_tmp = ptr->next; \
        ptr != (elist); ptr = m_efs_tmp, m_efs_tmp = m_efs_tmp->next)

#define sdr_elist_size(elist) ({ \
    size_t m_es_ret = 0; \
    sdr_elist_for(elist, m_es_ptr) { \
        m_es_ret++; \
    } \
    m_es_ret; \
})

#endif //SANDRA_EMBED_LIST_H
