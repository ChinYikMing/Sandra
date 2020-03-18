//
// Created by Jason Zheng on 2020/3/12.
//

#ifndef SANDRA_VECTOR_H
#define SANDRA_VECTOR_H

#include <stdlib.h>
#include <string.h>

#define SDR_VECTOR_DFLT_CAP_BITS 4u

#define SdrVector(type) \
struct { \
    /* raw array of elements */ \
    type *val; \
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

#define sdr_vector_init(vec, ...) ({ \
    const size_t m_bits = (__VA_ARGS__ + 0) ? __VA_ARGS__ : SDR_VECTOR_DFLT_CAP_BITS; \
    const size_t m_capacity = 1u << (unsigned) (m_bits); \
    (vec)->el_size = sizeof((vec)->val[0]); /* NOLINT(bugprone-sizeof-expression) */ \
    (vec)->val = malloc((vec)->el_size * m_capacity); \
    (vec)->front = 0; \
    (vec)->rear = 0; \
    (vec)->size = 0; \
    (vec)->capacity = m_capacity; \
    (vec)->cap_bits = m_bits; \
})

#define sdr_vector_expand(vec, multiple) ({ \
    int m_ve_ret = 0; \
    /* store common variables to avoid double counting */ \
    const size_t m_ex_ori_cap = (vec)->capacity; \
    const size_t m_ex_new_cap = m_ex_ori_cap << (multiple); \
    const size_t m_ex_front = (vec)->front; \
    const size_t m_ex_rear = (vec)->rear; \
    const size_t m_ex_el_size = (vec)->el_size; \
    void *m_ex_val = realloc((vec)->val, m_ex_el_size * m_ex_new_cap); \
    if (!m_ex_val) m_ve_ret = -1; \
    else { \
        (vec)->cap_bits += (multiple); \
        (vec)->capacity = m_ex_new_cap; \
        (vec)->val = m_ex_val; \
        /* move memory only when the old vector is circular */ \
        if ((vec)->size && m_ex_rear <= m_ex_front) { \
            /* find the end with fewer memory moves */ \
            const size_t m_ex_front_move = m_ex_ori_cap - m_ex_front; \
            const size_t m_ex_rear_move = m_ex_rear; \
            if (m_ex_front_move > m_ex_rear_move) { \
                memcpy((vec)->val + m_ex_ori_cap, (vec)->val, \
                       m_ex_el_size * m_ex_rear); \
                (vec)->rear += m_ex_ori_cap; \
            } else { \
                const size_t m_ex_front_offset = m_ex_new_cap - m_ex_ori_cap; \
                const size_t m_ex_new_front = m_ex_front + m_ex_front_offset; \
                memcpy((vec)->val + m_ex_new_front, \
                       (vec)->val + m_ex_front, \
                       m_ex_el_size * (m_ex_ori_cap - m_ex_front)); \
                (vec)->front = m_ex_new_front; \
            } \
        } \
    } \
    m_ve_ret; \
})

#define sdr_vector_push_front(vec, item) ({ \
    int m_vpf_ret = 0; \
    /* expand if necessary */ \
    if ((vec)->size == (vec)->capacity) { \
        m_vpf_ret = sdr_vector_expand(vec, 1u); \
    } \
    if (!m_vpf_ret) { \
        size_t m_front = (vec)->front; \
        m_front = (m_front + (vec)->capacity - 1) & ((vec)->capacity - 1); \
        /* store at the new front */ \
        (vec)->val[m_front] = (item); \
        (vec)->front = m_front; \
        (vec)->size++; \
    } \
    m_vpf_ret; \
})

#define sdr_vector_push_back(vec, item) ({ \
    int m_vpb_ret = 0; \
    /* expand if necessary */ \
    if ((vec)->size == (vec)->capacity) { \
        m_vpb_ret = sdr_vector_expand(vec, 1u); \
    } \
    if (!m_vpb_ret) { \
        /* store at the old rear */ \
        const size_t m_rear = (vec)->rear; \
        (vec)->val[m_rear] = (item); \
        (vec)->rear = (m_rear + 1) & ((vec)->capacity - 1); \
        (vec)->size++; \
    } \
    m_vpb_ret; \
})

#define sdr_vector_pop_front(vec) ({ \
    const size_t m_capacity = (vec)->capacity; \
    const size_t m_front = (vec)->front; \
    (vec)->front = (m_front + 1) & (m_capacity - 1); \
    (vec)->size--; \
    /* use old front */ \
    (vec)->val[m_front]; \
})

#define sdr_vector_pop_back(vec) ({ \
    const size_t m_capacity = (vec)->capacity; \
    size_t m_rear = (vec)->rear; \
    m_rear = (vec)->rear = (m_rear + m_capacity - 1) & (m_capacity - 1); \
    (vec)->size--; \
    /* use new rear */ \
    (vec)->val[m_rear]; \
})

#define sdr_vector_push_all_back(vec, items, num) ({ \
    int m_vpab_ret = 0; \
    const size_t m_el_size = (vec)->el_size; \
    const size_t m_size = (vec)->size; \
    const size_t m_needed = (num); \
    /* expand if necessary */ \
    if (m_needed > (vec)->capacity - m_size) { \
        /* The simplest expansion strategy for space–time trade-off */ \
        unsigned multiple = (m_needed >> (vec)->cap_bits) + (m_size ? 1u : 0u); \
        m_vpab_ret = sdr_vector_expand(vec, multiple); \
    } \
    if (!m_vpab_ret) { \
        /* capacity shall be enough now */ \
        const size_t m_cap = (vec)->capacity; \
        const size_t m_front = (vec)->front; \
        const size_t m_rear = (vec)->rear; \
        const size_t m_end_size = m_cap - m_rear; \
        if (m_front < m_rear) { \
            if (m_end_size >= m_needed) { \
                memcpy((vec)->val + m_rear, (items), m_el_size * m_needed); \
            } else { \
                memcpy((vec)->val + m_rear, (items), m_el_size * m_end_size); \
                memcpy((vec)->val, (items) + m_end_size, m_el_size * (m_needed - m_end_size)); \
            } \
        } else { /* rear <= front */ \
            memcpy((vec)->val + m_rear, (items), m_el_size * m_needed); \
        } \
        (vec)->rear = (m_rear + m_needed) & (m_cap - 1); \
        (vec)->size += m_needed; \
    } \
    m_vpab_ret; \
})

#define sdr_vector_straighten(vec) ({ \
    int m_vs_ret = 0; \
    if ((vec)->size > 0 && (vec)->rear <= (vec)->front) { \
        typeof((vec)->val) new_val = malloc((vec)->el_size * (vec)->capacity); \
        if (!new_val) m_vs_ret = -1; \
        else { \
            size_t m_vs_count = (vec)->capacity - (vec)->front; \
            memcpy(new_val, (vec)->val + (vec)->front, m_vs_count * (vec)->el_size); \
            memcpy(new_val + m_vs_count, (vec)->val, (vec)->rear * (vec)->el_size); \
            (vec)->front = 0; \
            (vec)->rear += m_vs_count; \
            free((vec)->val); \
            (vec)->val = new_val; \
        } \
    } \
    m_vs_ret; \
})

#define sdr_vector_sort(vec, fn_cmp) ({ \
    int m_vsort_ret = 0; \
    int m_vs_straighten_ret = sdr_vector_straighten(vec); \
    if (m_vs_straighten_ret < 0) \
        m_vsort_ret = m_vs_straighten_ret; \
    else \
        qsort(((vec)->val + (vec)->front), (vec)->size, (vec)->el_size, fn_cmp); \
    m_vsort_ret; \
})

#define sdr_vector_entry(vec, idx) ({ \
    (vec)->val + (((vec)->front + idx) & ((vec)->capacity - 1)); \
})

#define sdr_vector_get(vec, idx) ({ \
    *(sdr_vector_entry(vec, idx)); \
})

#define sdr_vector_raw(vec) ({ \
    typeof((vec)->val) m_vr_ret = NULL; \
    if (!sdr_vector_straighten(vec)) m_vr_ret = (vec)->val; \
    m_vr_ret; \
})

#define sdr_vector_for_each(vec, idx, item) \
    for (idx = 0, item = sdr_vector_get(vec, idx); \
        idx < (vec)->size && (item = sdr_vector_get(vec, idx), 1); \
        idx++)

#define sdr_vector_for_each_p(vec, idx, m_ptr) \
    for (idx = 0, m_ptr = sdr_vector_entry(vec, idx); \
        idx < (vec)->size && (m_ptr = sdr_vector_entry(vec, idx), 1); \
        idx++)

#define sdr_vector_size(vec) ({ \
    (vec)->size; \
})

#define sdr_vector_capacity(vec) ({ \
    (vec)->capacity; \
})

#define sdr_vector_destroy(vec) ({ \
    free((vec)->val); \
})

#endif //SANDRA_VECTOR_H
