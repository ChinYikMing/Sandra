//
// Created by Jason Zheng on 2020/3/15.
//

#ifndef SANDRA_BASIS_H
#define SANDRA_BASIS_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#if !defined(__printflike) && defined(GCC_VERSION)
#define __printflike(fmtarg, firstvararg) \
            __attribute__((__format__ (__printf__, fmtarg, firstvararg)))
#endif

#ifndef SDR_PRAGMA
#define SDR_PRAGMA(x) _Pragma(#x)
#endif

#ifdef __clang__
#define SDR_SUPPRESS_WARN(WARN_STR) \
    SDR_PRAGMA(clang diagnostic push) \
    SDR_PRAGMA(clang diagnostic ignored WARN_STR)
#else
#define SDR_SUPPRESS_WARN(WARN_STR) \
    SDR_PRAGMA(GCC diagnostic push) \
    SDR_PRAGMA(GCC diagnostic ignored WARN_STR)
#endif

#define SDR_SUPPRESS_WARN_END \
    _Pragma("clang diagnostic pop")

#ifndef sdr_offsetof
#define sdr_offsetof(type, member)  __builtin_offsetof (type, member)
#endif

#ifndef sdr_container_of
#define sdr_container_of(ptr, type, member) ({ \
    void *m_ptr = (void *) (ptr); \
    ((type *)(m_ptr - sdr_offsetof(type, member))); \
})
#endif

#ifndef sdr_max
#define sdr_max(a, b) ({ \
    __auto_type m_max_a = (a); \
    __auto_type m_max_b = (b); \
    m_max_a > m_max_b ? m_max_a : m_max_b; \
})
#endif

#ifndef sdr_min
#define sdr_min(a, b) ({ \
    __auto_type m_min_a = (a); \
    __auto_type m_min_b = (b); \
    m_max_a < m_max_b ? m_max_a : m_max_b; \
})
#endif

#ifdef _SC_PAGESIZE
#define SDR_GET_PAGE_SIZE() sysconf(_SC_PAGESIZE)
#else
#define SDR_GET_PAGE_SIZE() getpagesize()
#endif

#ifndef SDR_BUILD_BUG_ON_ZERO
#define SDR_BUILD_BUG_ON_ZERO(e) \
    ((int)(sizeof(struct { int:(-!!(e)); })))
#endif

#ifndef SDR_IS_SAME_TYPE
#define SDR_IS_SAME_TYPE(a, b) \
    __builtin_types_compatible_p(typeof(a), typeof(b))
#endif

#ifndef SD_IS_STR
#define SDR_IS_STR(a) \
    __builtin_types_compatible_p(typeof(a), char *)
#endif


#ifndef SDR_IS_ARRAY
#define SDR_IS_ARRAY(a) \
    !SDR_IS_SAME_TYPE((a), &(a)[0])
#endif

#ifndef SDR_IS_PTR
#define SDR_IS_PTR(a) \
    SDR_IS_SAME_TYPE((a), &(a)[0])
#endif

#ifndef SDR_ARRAY_SIZE
#define SDR_ARRAY_SIZE(arr) \
    (sizeof(arr) / sizeof((arr)[0]) \
        + SDR_BUILD_BUG_ON_ZERO(SDR_IS_PTR(arr)))
#endif

/**
 * gclz - Returns the number of leading 0-bits in x, starting at the
 *       most significant bit position. If x is 0, the result is -1.
 */
#define sdr_gclz(x) ({ \
    int m_clz_ret = -1; \
    __auto_type m_clz_num = (x); \
    const size_t m_num_size = sizeof(m_clz_num); \
    if (m_clz_num) { \
        if (m_num_size == sizeof(int)) \
            m_clz_ret = __builtin_clz(m_clz_num); \
        else if (m_num_size == sizeof(long)) \
            m_clz_ret = __builtin_clzl(m_clz_num); \
        else if (m_num_size == sizeof(long long)) \
            m_clz_ret = __builtin_clzll(m_clz_num); \
        else if (m_num_size < sizeof(int)) \
            m_clz_ret = (__builtin_clz(m_clz_num) - (int) (sizeof(int) - m_num_size) * CHAR_BIT); \
    } \
    m_clz_ret;\
})

/**
 * gffs - Returns one plus the index of the least significant 1-bit of x,
 *        or if x is zero, returns zero.
 *
 */
#define sdr_gffs(x) ({ \
    int m_ffs_ret = -1; \
    __auto_type m_ffs_num = (x); \
    const size_t m_num_size = sizeof(m_ffs_num); \
    if (m_ffs_num) { \
        if (m_num_size <= sizeof(int)) \
            m_ffs_ret = __builtin_ffs(m_ffs_num); \
        else if (m_num_size == sizeof(long)) \
            m_ffs_ret = __builtin_ffsl(m_ffs_num); \
        else if (m_num_size == sizeof(long long)) \
            m_ffs_ret = __builtin_ffsll(m_ffs_num); \
    } \
    m_ffs_ret;\
})

/**
 * get_bits - get n bits from position p (lsb: 0)
 *            if (n > p + 1), the behavior is undefined.
 */
#define sdr_get_bits(x, p, n) ({ \
    typeof(x) m_gb_ret; \
    typeof(x) m_x = (x); \
    unsigned m_p = (p); \
    unsigned m_n = (n); \
    m_gb_ret = (m_x >> (m_p + 1 - m_n)) & ~(~((typeof(x)) 0) << m_n); \
    m_gb_ret; \
})

/**
 * ursb - unsets the rightmost set bit of x
 */
#define sdr_ursb(x) ({ \
    __auto_type m_num = (x); \
    m_num & (m_num - 1); \
})

#ifndef sdr_is_pow_of_two
#define sdr_is_pow_of_two(x) ({ \
    ursb(x) == 0; \
})
#endif

#endif //SANDRA_BASIS_H
