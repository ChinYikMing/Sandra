//
// Created by Jason Zheng on 2020/3/12.
//

#ifndef SANDRA_VECTOR_H
#define SANDRA_VECTOR_H

#include <stdlib.h>
#include <string.h>

#define SDR_VECTOR_DFLT_CAP_BITS 4u

#define SdrVector(m_type) \
struct { \
    /* raw array of elements */ \
    m_type *val; \
    /* index of first element */ \
    size_t front; \
    /* index of last element + 1 */ \
    size_t rear; \
    /* number of elements */ \
    size_t size; \
    /* currently allocated storage, must be power of two */ \
    size_t capacity; \
    /* sizeof element */ \
    size_t el_size; \
    /* log2 of capacity */ \
    unsigned cap_bits; \
}

#define sdr_vector_init(m_vec, ...) ({ \
    const size_t m_bits = (__VA_ARGS__ + 0) ? __VA_ARGS__ : SDR_VECTOR_DFLT_CAP_BITS; \
    const size_t m_capacity = 1u << (unsigned) (m_bits); \
    (m_vec)->el_size = sizeof((m_vec)->val[0]); /* NOLINT(bugprone-sizeof-expression) */ \
    (m_vec)->val = malloc((m_vec)->el_size * m_capacity); \
    (m_vec)->front = 0; \
    (m_vec)->rear = 0; \
    (m_vec)->size = 0; \
    (m_vec)->capacity = m_capacity; \
    (m_vec)->cap_bits = m_bits; \
})

#define sdr_vector_expand(m_vec, m_multiple) ({ \
    int m_ve_ret = 0; \
    /* store common variables to avoid double counting */ \
    const size_t m_ex_ori_cap = (m_vec)->capacity; \
    const size_t m_ex_new_cap = m_ex_ori_cap << (m_multiple); \
    const size_t m_ex_front = (m_vec)->front; \
    const size_t m_ex_rear = (m_vec)->rear; \
    const size_t m_ex_el_size = (m_vec)->el_size; \
    void *m_ex_val = realloc((m_vec)->val, m_ex_el_size * m_ex_new_cap); \
    if (!m_ex_val) m_ve_ret = -1; \
    else { \
        (m_vec)->cap_bits += (m_multiple); \
        (m_vec)->capacity = m_ex_new_cap; \
        (m_vec)->val = m_ex_val; \
        /* move memory only when the old vector is circular */ \
        if ((m_vec)->size && m_ex_rear <= m_ex_front) { \
            /* find the end with fewer memory moves */ \
            const size_t m_ex_front_move = m_ex_ori_cap - m_ex_front; \
            const size_t m_ex_rear_move = m_ex_rear; \
            if (m_ex_front_move > m_ex_rear_move) { \
                memcpy((m_vec)->val + m_ex_ori_cap, (m_vec)->val, \
                       m_ex_el_size * m_ex_rear); \
                (m_vec)->rear += m_ex_ori_cap; \
            } else { \
                const size_t m_ex_front_offset = m_ex_new_cap - m_ex_ori_cap; \
                const size_t m_ex_new_front = m_ex_front + m_ex_front_offset; \
                memcpy((m_vec)->val + m_ex_new_front, \
                       (m_vec)->val + m_ex_front, \
                       m_ex_el_size * (m_ex_ori_cap - m_ex_front)); \
                (m_vec)->front = m_ex_new_front; \
            } \
        } \
    } \
    m_ve_ret; \
})

#define sdr_vector_push_front(m_vec, m_item) ({ \
    int m_vpf_ret = 0; \
    /* expand if necessary */ \
    if ((m_vec)->size == (m_vec)->capacity) { \
        m_vpf_ret = sdr_vector_expand(m_vec, 1u); \
    } \
    if (!m_vpf_ret) { \
        size_t m_front = (m_vec)->front; \
        m_front = (m_front + (m_vec)->capacity - 1) & ((m_vec)->capacity - 1); \
        /* store at the new front */ \
        (m_vec)->val[m_front] = (m_item); \
        (m_vec)->front = m_front; \
        (m_vec)->size++; \
    } \
    m_vpf_ret; \
})

#define sdr_vector_push_back(m_vec, m_item) ({ \
    int m_vpb_ret = 0; \
    /* expand if necessary */ \
    if ((m_vec)->size == (m_vec)->capacity) { \
        m_vpb_ret = sdr_vector_expand(m_vec, 1u); \
    } \
    if (!m_vpb_ret) { \
        /* store at the old rear */ \
        const size_t m_rear = (m_vec)->rear; \
        (m_vec)->val[m_rear] = (m_item); \
        (m_vec)->rear = (m_rear + 1) & ((m_vec)->capacity - 1); \
        (m_vec)->size++; \
    } \
    m_vpb_ret; \
})

#define sdr_vector_pop_front(m_vec) ({ \
    const size_t m_capacity = (m_vec)->capacity; \
    const size_t m_front = (m_vec)->front; \
    (m_vec)->front = (m_front + 1) & (m_capacity - 1); \
    (m_vec)->size--; \
    /* use old front */ \
    (m_vec)->val[m_front]; \
})

#define sdr_vector_pop_back(m_vec) ({ \
    const size_t m_capacity = (m_vec)->capacity; \
    size_t m_rear = (m_vec)->rear; \
    m_rear = (m_vec)->rear = (m_rear + m_capacity - 1) & (m_capacity - 1); \
    (m_vec)->size--; \
    /* use new rear */ \
    (m_vec)->val[m_rear]; \
})

#define sdr_vector_push_all_back(m_vec, m_items, m_num) ({ \
    int m_vpab_ret = 0; \
    const size_t m_el_size = (m_vec)->el_size; \
    const size_t m_size = (m_vec)->size; \
    const size_t m_needed = (m_num); \
    /* expand if necessary */ \
    if (m_needed > (m_vec)->capacity - m_size) { \
        /* The simplest expansion strategy for space–time trade-off */ \
        unsigned m_multiple = (m_needed >> (m_vec)->cap_bits) + (m_size ? 1u : 0u); \
        m_vpab_ret = sdr_vector_expand(m_vec, m_multiple); \
    } \
    if (!m_vpab_ret) { \
        /* capacity shall be enough now */ \
        const size_t m_cap = (m_vec)->capacity; \
        const size_t m_front = (m_vec)->front; \
        const size_t m_rear = (m_vec)->rear; \
        const size_t m_end_size = m_cap - m_rear; \
        if (m_front < m_rear) { \
            if (m_end_size >= m_needed) { \
                memcpy((m_vec)->val + m_rear, (m_items), m_el_size * m_needed); \
            } else { \
                memcpy((m_vec)->val + m_rear, (m_items), m_el_size * m_end_size); \
                memcpy((m_vec)->val, (m_items) + m_end_size, m_el_size * (m_needed - m_end_size)); \
            } \
        } else { /* rear <= front */ \
            memcpy((m_vec)->val + m_rear, (m_items), m_el_size * m_needed); \
        } \
        (m_vec)->rear = (m_rear + m_needed) & (m_cap - 1); \
        (m_vec)->size += m_needed; \
    } \
    m_vpab_ret; \
})

#define sdr_vector_straighten(m_vec) ({ \
    int m_vs_ret = 0; \
    if ((m_vec)->size > 0 && (m_vec)->rear <= (m_vec)->front) { \
        typeof((m_vec)->val) new_val = malloc((m_vec)->el_size * (m_vec)->capacity); \
        if (!new_val) m_vs_ret = -1; \
        else { \
            size_t m_vs_count = (m_vec)->capacity - (m_vec)->front; \
            memcpy(new_val, (m_vec)->val + (m_vec)->front, m_vs_count * (m_vec)->el_size); \
            memcpy(new_val + m_vs_count, (m_vec)->val, (m_vec)->rear * (m_vec)->el_size); \
            (m_vec)->front = 0; \
            (m_vec)->rear += m_vs_count; \
            free((m_vec)->val); \
            (m_vec)->val = new_val; \
        } \
    } \
    m_vs_ret; \
})

#define sdr_vector_sort(m_vec, m_cmp_fn) ({ \
    int m_vsort_ret = 0; \
    int m_vs_straighten_ret = sdr_vector_straighten(m_vec); \
    if (m_vs_straighten_ret < 0) \
        m_vsort_ret = m_vs_straighten_ret; \
    else \
        qsort(((m_vec)->val + (m_vec)->front), (m_vec)->size, (m_vec)->el_size, m_cmp_fn); \
    m_vsort_ret; \
})

#define sdr_vector_entry(m_vec, m_idx) ({ \
    (m_vec)->val + (((m_vec)->front + m_idx) & ((m_vec)->capacity - 1)); \
})

#define sdr_vector_raw(m_vec) ({ \
    typeof((m_vec)->val) m_vr_ret = NULL; \
    if (!sdr_vector_straighten(m_vec)) m_vr_ret = (m_vec)->val; \
    m_vr_ret; \
})

#define sdr_vector_get(m_vec, m_idx) ({ \
    *(sdr_vector_entry(m_vec, m_idx)); \
})

#define sdr_vector_for_each(m_vec, m_idx, m_item) \
    for (m_idx = 0, m_item = sdr_vector_get(m_vec, m_idx); \
        m_idx < (m_vec)->size && (m_item = sdr_vector_get(m_vec, m_idx), 1); \
        m_idx++)

#define sdr_vector_for_each_p(m_vec, m_idx, m_ptr) \
    for (m_idx = 0, m_ptr = sdr_vector_entry(m_vec, m_idx); \
        m_idx < (m_vec)->size && (m_ptr = sdr_vector_entry(m_vec, m_idx), 1); \
        m_idx++)

#define sdr_vector_size(m_vec) ({ \
    (m_vec)->size; \
})

#define sdr_vector_capacity(m_vec) ({ \
    (m_vec)->capacity; \
})

#define sdr_vector_destroy(m_vec) ({ \
    free((m_vec)->val); \
})

#endif //SANDRA_VECTOR_H
