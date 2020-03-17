//
// Created by Jason Zheng on 2020/3/15.
//

#ifndef SANDRA_BASIS_H
#define SANDRA_BASIS_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifndef SDR_CHAR_BITS
#define SDR_CHAR_BITS    sizeof(char) * CHAR_BIT
#endif
#ifndef SDR_INT_BITS
#define SDR_INT_BITS     sizeof(int) * CHAR_BIT
#endif
#ifndef SDR_LONG_BITS
#define SDR_LONG_BITS    sizeof(long) * CHAR_BIT
#endif
#ifndef SDR_SIZE_T_BITS
#define SDR_SIZE_T_BITS  sizeof(size_t) * CHAR_BIT
#endif

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

/* ===================== BYTES ====================== */
#define SDR_16_GiB 17179869184
#define SDR_8_GiB 8589934592
#define SDR_4_GiB 4294967296
#define SDR_2_GiB 2147483648
#define SDR_1_GiB 1073741824

#define SDR_512_MiB 536870912
#define SDR_256_MiB 268435456
#define SDR_128_MiB 134217728
#define SDR_64_MiB 67108864
#define SDR_32_MiB 33554432
#define SDR_16_MiB 16777216
#define SDR_8_MiB 8388608
#define SDR_4_MiB 4194304
#define SDR_2_MiB 2097152
#define SDR_1_MiB 1048576

#define SDR_512_KiB 524288
#define SDR_256_KiB 262144
#define SDR_128_KiB 131072
#define SDR_64_KiB 65536
#define SDR_32_KiB 32768
#define SDR_16_KiB 16384
#define SDR_8_KiB 8192
#define SDR_4_KiB 4096
#define SDR_2_KiB 2048
#define SDR_1_KiB 1024

#endif //SANDRA_BASIS_H
