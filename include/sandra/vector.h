//
// Created by Jason Zheng on 2020/3/12.
//

#ifndef SANDRA_VECTOR_H
#define SANDRA_VECTOR_H

#include <stdlib.h>
#include <string.h>
#include "sandra/basis.h"

#define SDR_VEC_ERR_MALLOC -1
#define SDR_VEC_ERR_EMPTY -2

#ifndef SDR_VEC_DFLT_STACK_CAP_BITS
#define SDR_VEC_DFLT_STACK_CAP_BITS 4u
#endif

#ifndef SDR_VEC_DFLT_HEAP_CAP_BITS
#define SDR_VEC_DFLT_HEAP_CAP_BITS 0u
#endif

#ifndef SDR_VEC_DFLT_FIXED_SIZE
#define SDR_VEC_DFLT_FIXED_SIZE 0u
#endif

#define sdr_vec_pvt_sbuf_init_cap(...) ({ \
    ((__VA_ARGS__ + 0) \
        ? ((__VA_ARGS__ + 0) > 0 ? 1u << (unsigned) (__VA_ARGS__ + 0) : 0) \
        : 1u << SDR_VEC_DFLT_STACK_CAP_BITS); \
})

typedef struct sdr_vec_init_args {
    unsigned heap_cap_bits;
    _Bool fixed_size;
} SdrVecInitArgs;

/**
 *  Vector(type, init_cap_bits)
 *  @type:  vector type
 *  @init_cap_bits: capacity bits for stack buffer
 *                  > 0 -- user specified cap bits
 *                  = 0 -- default cap bits
 *                  < 0 -- no stack buffer
 */
#define SdrVector(type, ...) \
struct { \
    /* raw buffer of elements */ \
    union { \
        /* heap buffer */ \
        type *hbuf; \
        /* stack buffer */ \
        type sbuf[sdr_vec_pvt_sbuf_init_cap(__VA_ARGS__)]; \
    }; \
    /* index of first element */ \
    size_t front; \
    /* index of last element + 1 */ \
    size_t rear; \
    /* currently allocated storage, must be power of two */ \
    size_t capacity; \
    /* number of elements */ \
    size_t size; \
    /* sizeof element */ \
    size_t el_size; \
    /* log2 of capacity */ \
    unsigned cap_bits; \
    /* use heap or stack buffer */ \
    _Bool use_hbuf; \
    /* todo: fixed size */ \
    _Bool fixed_size; \
}


#define sdr_vec_init(vec, ...) ({ \
    SDR_SUPPRESS_WARN("-Winitializer-overrides") \
    SdrVecInitArgs m_init_args = { \
        .heap_cap_bits = SDR_VEC_DFLT_HEAP_CAP_BITS, \
        .fixed_size = SDR_VEC_DFLT_FIXED_SIZE, \
        __VA_ARGS__ \
    }; \
    SDR_SUPPRESS_WARN_END \
    \
    int m_vi_ret = 0; \
    (vec)->front = 0; \
    (vec)->rear = 0; \
    (vec)->size = 0; \
    const size_t m_hbuf_cap_bits = m_init_args.heap_cap_bits; \
    const size_t m_sbuf_cap = SDR_ARRAY_SIZE((vec)->sbuf); \
    const size_t m_sbuf_cap_bits = sdr_lg_po2(m_sbuf_cap); \
    const size_t m_el_size = (vec)->el_size = sizeof((vec)->hbuf[0]); \
    \
    if (m_hbuf_cap_bits > 0) { \
        const size_t m_hbuf_cap = 1u << (unsigned) (m_hbuf_cap_bits); \
        (vec)->hbuf = malloc(m_el_size * m_hbuf_cap); \
        if (!(vec)->hbuf) \
            m_vi_ret = SDR_VEC_ERR_MALLOC; \
        (vec)->use_hbuf = 1; \
        (vec)->capacity = m_hbuf_cap; \
        (vec)->cap_bits = m_hbuf_cap_bits; \
    } else { \
        (vec)->use_hbuf = 0; \
        (vec)->capacity = m_sbuf_cap; \
        (vec)->cap_bits = m_sbuf_cap_bits; \
    } \
    m_vi_ret; \
})

#define sdr_vec_expand(vec, multiple) ({ \
    int m_ve_ret = 0; \
    \
    /* src and dest */\
    typeof((vec)->hbuf) const m_ex_raw = sdr_vec_raw(vec); \
    typeof((vec)->hbuf) m_ex_new_hbuf; \
    \
    /* store common variables to avoid double counting */ \
    const size_t m_ex_ori_cap = (vec)->capacity; \
    const size_t m_ex_new_cap = m_ex_ori_cap << (multiple); \
    const size_t m_ex_front = (vec)->front; \
    const size_t m_ex_rear = (vec)->rear; \
    const size_t m_ex_el_size = (vec)->el_size; \
    const size_t m_ex_size = (vec)->size; \
    const _Bool m_ex_ori_use_hbuf = (vec)->use_hbuf; \
    const size_t m_ex_alloc_size = m_ex_el_size * m_ex_new_cap; \
    \
    if (m_ex_ori_use_hbuf) { \
        m_ex_new_hbuf = realloc((vec)->hbuf, m_ex_alloc_size); \
    } else { \
        m_ex_new_hbuf = malloc(m_ex_alloc_size); \
    } \
    \
    if (!m_ex_new_hbuf) \
        m_ve_ret = SDR_VEC_ERR_MALLOC; \
    else { \
        (vec)->cap_bits += (multiple); \
        (vec)->capacity = m_ex_new_cap; \
        (vec)->use_hbuf = 1; \
        if (m_ex_size) { \
            /* move memory only when the old vector is circular */ \
            if ((m_ex_rear < m_ex_front) \
                || (m_ex_front == m_ex_rear && m_ex_rear != 0)) { \
                /* find the end with fewer memory moves */ \
                const size_t m_ex_front_move = m_ex_ori_cap - m_ex_front; \
                const size_t m_ex_rear_move = m_ex_rear; \
                if (m_ex_front_move > m_ex_rear_move) { \
                    if (!m_ex_ori_use_hbuf) { \
                        /* migrate "sbuf_front ~ sbuf_end" to "hbuf_front" */ \
                        memcpy(m_ex_new_hbuf + m_ex_front, m_ex_raw + m_ex_front, \
                            m_ex_el_size * m_ex_front_move); \
                    } \
                    memcpy(m_ex_new_hbuf + m_ex_ori_cap, m_ex_raw, \
                        m_ex_el_size * m_ex_rear_move); \
                    (vec)->rear += m_ex_ori_cap; \
                } else { \
                    const size_t m_ex_front_offset = m_ex_new_cap - m_ex_ori_cap; \
                    const size_t m_ex_new_front = m_ex_front + m_ex_front_offset; \
                    if (!m_ex_ori_use_hbuf) { \
                        /* migrate "sbuf_start ~ sbuf_rear" to "hbuf_start" */ \
                        memcpy(m_ex_new_hbuf, m_ex_raw, \
                            m_ex_el_size * m_ex_rear_move); \
                    } \
                    memcpy(m_ex_new_hbuf + m_ex_new_front, \
                           m_ex_raw + m_ex_front, \
                           m_ex_el_size * m_ex_front_move); \
                    (vec)->front = m_ex_new_front; \
                } \
            } else  { \
                if (!m_ex_ori_use_hbuf) \
                    memcpy(m_ex_new_hbuf, m_ex_raw + m_ex_front, m_ex_el_size * m_ex_size); \
                /* update the rear if m_ex_front == m_ex_rear == 0 */ \
                if (m_ex_front == m_ex_rear) \
                    (vec)->rear = m_ex_ori_cap; \
            } \
        } \
        /* update hbuf at the end to prevent impact on the union */ \
        (vec)->hbuf = m_ex_new_hbuf; \
    } \
    m_ve_ret; \
})

#define sdr_vec_raw(vec) ({ \
    (vec)->use_hbuf ? (vec)->hbuf : (vec)->sbuf; \
})

#define sdr_vec_push_front(vec, item) ({ \
    int m_vpf_ret = 0; \
    /* expand if necessary */ \
    if ((vec)->size == (vec)->capacity) { \
        m_vpf_ret = sdr_vec_expand(vec, 1u); \
    } \
    if (!m_vpf_ret) { \
        const size_t m_front \
            = ((vec)->front + (vec)->capacity - 1) & ((vec)->capacity - 1); \
        /* store at the new front */ \
        sdr_vec_raw(vec)[m_front] = (item); \
        (vec)->front = m_front; \
        (vec)->size++; \
    } \
    m_vpf_ret; \
})

#define sdr_vec_push_back(vec, item) ({ \
    int m_vpb_ret = 0; \
    /* expand if necessary */ \
    if ((vec)->size == (vec)->capacity) { \
        m_vpb_ret = sdr_vec_expand(vec, 1u); \
    } \
    if (!m_vpb_ret) { \
        /* store at the old rear */ \
        const size_t m_rear = (vec)->rear; \
        sdr_vec_raw(vec)[m_rear] = (item); \
        (vec)->rear = (m_rear + 1) & ((vec)->capacity - 1); \
        (vec)->size++; \
    } \
    m_vpb_ret; \
})

#define sdr_vec_pop_front(vec, dest) ({ \
    int m_vpf_ret = 0; \
    if ((vec)->size) { \
        typeof((vec)->hbuf) const m_dest = (dest); \
        const size_t m_capacity = (vec)->capacity; \
        const size_t m_front = (vec)->front; \
        (vec)->front = (m_front + 1) & (m_capacity - 1); \
        (vec)->size--; \
        /* use old front */ \
        if (m_dest) \
            *m_dest = sdr_vec_raw(vec)[m_front]; \
    } else { \
        m_vpf_ret = SDR_VEC_ERR_EMPTY; \
    } \
    m_vpf_ret; \
})

#define sdr_vec_pop_back(vec, dest) ({ \
    int m_vpb_ret = 0; \
    if ((vec)->size) { \
        typeof((vec)->hbuf) const m_dest = (dest); \
        const size_t m_capacity = (vec)->capacity; \
        const size_t m_rear = (vec)->rear \
            = ((vec)->rear + (m_capacity - 1)) & (m_capacity - 1); \
        (vec)->size--; \
        /* use new rear */ \
        if (m_dest) \
            *m_dest = sdr_vec_raw(vec)[m_rear]; \
    } else { \
        m_vpb_ret = SDR_VEC_ERR_EMPTY; \
    } \
    m_vpb_ret; \
})

#define sdr_vec_bulk_push_back(vec, items, num) ({ \
    int m_vpab_ret = 0; \
    const size_t m_el_size = (vec)->el_size; \
    const size_t m_size = (vec)->size; \
    const size_t m_needed = (num); \
    /* expand if necessary */ \
    if (m_needed > (vec)->capacity - m_size) { \
        /* The simplest expansion strategy for space–time trade-off */ \
        unsigned multiple = (m_needed >> (vec)->cap_bits) + (m_size ? 1u : 0u); \
        m_vpab_ret = sdr_vec_expand(vec, multiple); \
    } \
    if (!m_vpab_ret) { \
        /* capacity shall be enough now */ \
        const size_t m_cap = (vec)->capacity; \
        const size_t m_front = (vec)->front; \
        const size_t m_rear = (vec)->rear; \
        const size_t m_end_size = m_cap - m_rear; \
        typeof((vec)->hbuf) const m_raw = sdr_vec_raw(vec); \
        if (m_front < m_rear) { \
            if (m_end_size >= m_needed) { \
                memcpy(m_raw + m_rear, (items), m_el_size * m_needed); \
            } else { \
                memcpy(m_raw + m_rear, (items), m_el_size * m_end_size); \
                memcpy(m_raw, (items) + m_end_size, m_el_size * (m_needed - m_end_size)); \
            } \
        } else { /* rear <= front */ \
            memcpy(m_raw + m_rear, (items), m_el_size * m_needed); \
        } \
        (vec)->rear = (m_rear + m_needed) & (m_cap - 1); \
        (vec)->size += m_needed; \
    } \
    m_vpab_ret; \
})

#define sdr_vec_pvt_sort(vec, sort_stat_straighten, sort_stat_curve) ({ \
    const size_t m_size = (vec)->size; \
    if (m_size) { \
        typeof((vec)->hbuf) const m_raw = sdr_vec_raw(vec); \
        const size_t m_front = (vec)->front; \
        const size_t m_rear = (vec)->rear; \
        const size_t m_cap = (vec)->capacity; \
        const size_t m_el_size = (vec)->el_size; \
        if (m_rear > m_front) { \
            sort_stat_straighten; \
        } else { \
            if (m_rear < m_front) { \
                /* concatenate the 'front' to the end of 'rear' */ \
                /* use memmove() instead of memcpy() to prevent overlap */ \
                memmove(m_raw + m_rear, m_raw + m_front, \
                    m_el_size * (m_cap - m_front)); \
            } \
            sort_stat_curve; \
            (vec)->front = 0; \
            (vec)->rear = m_size & (m_cap - 1); \
        } \
    } \
})

#define sdr_vec_sort(vec, fn_sort, fn_cmp) ({ \
    sdr_vec_pvt_sort(vec, fn_sort(m_raw + m_front, m_size, m_el_size, fn_cmp), \
        fn_sort(m_raw, m_size, m_el_size, fn_cmp)); \
})

#define sdr_vec_qsort(vec, fn_cmp) ({ \
    sdr_vec_sort(vec, qsort, fn_cmp); \
})

#define sdr_vec_sort_r(vec, fn_sort_r, fn_cmp, cmp_arg) ({ \
    sdr_vec_pvt_sort(vec, fn_sort_r(m_raw + m_front, m_size, m_el_size, fn_cmp, cmp_arg), \
        fn_sort_r(m_raw, m_size, m_el_size, fn_cmp, cmp_arg)); \
})

#define sdr_vec_qsort_r(vec, fn_cmp, cmp_arg) ({ \
    sdr_vec_sort_r(vec, sdr_u_qsort_r, fn_cmp, cmp_arg); \
})

#define sdr_vec_entry(vec, idx) ({ \
    __auto_type m_ve_idx = (idx); \
    sdr_vec_raw(vec) + ( \
        ((m_ve_idx >= 0 ? (vec)->front : (vec)->rear) + m_ve_idx) \
            & ((vec)->capacity - 1) \
    ); \
})

#define sdr_vec_get(vec, idx) ({ \
    *(sdr_vec_entry(vec, idx)); \
})

#define sdr_vec_for_el(vec, idx, item) \
    for (idx = 0, item = sdr_vec_get(vec, idx); \
        idx < (vec)->size && (item = sdr_vec_get(vec, idx), 1); \
        idx++)

#define sdr_vec_for(vec, idx, m_ptr) \
    for (idx = 0, m_ptr = sdr_vec_entry(vec, idx); \
        idx < (vec)->size && (m_ptr = sdr_vec_entry(vec, idx), 1); \
        idx++)

#define sdr_vec_capacity(vec) ({ \
    (vec)->capacity; \
})

#define sdr_vec_size(vec) ({ \
    (vec)->size; \
})

#define sdr_vec_is_empty(vec) ({ \
    sdr_vec_size(vec) == 0; \
})

#define sdr_vec_clear(vec) ({ \
    (vec)->front = 0; \
    (vec)->rear = 0; \
    (vec)->size = 0; \
})

#define sdr_vec_destroy(vec) ({ \
    if ((vec)->use_hbuf && (vec)->hbuf) { \
        free((vec)->hbuf); \
        (vec)->hbuf = NULL; \
    } \
})

#endif //SANDRA_VECTOR_H
