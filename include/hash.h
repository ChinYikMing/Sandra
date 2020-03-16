//
// Created by Jason Zheng on 2020/3/12.
//

#ifndef SANDRA_HASH_H
#define SANDRA_HASH_H

#include <stdio.h>
#include <stdlib.h>

#define SDR_STR_HASH33_SEED 5381
#define SDR_STR_HASH65_SEED 9887
#define SDR_INT_HASH_SEED 0xB11924E1
#define SDR_LONG_HASH_SEED 0x16069317E428CA9ull

#define SDR_DJB2(seed, shift, key) ({ \
    const char *mPvtDjb2Str = (const char *) (key); \
    size_t mHashv = (seed); \
    while(*mPvtDjb2Str) \
        mHashv = (mHashv << (shift)) + mHashv + *(mPvtDjb2Str)++; \
    mHashv; \
})

static inline size_t sdr_djb2(const unsigned seed, const unsigned shift, const char *str) {
    return SDR_DJB2(seed, shift, str);
}

#define SDR_STR_HASH33(str) ({ \
    SDR_DJB2(SDR_STR_HASH33_SEED, 5u, str); \
})

static inline size_t sdr_str_hash33(const char *str) {
    return SDR_STR_HASH33(str);
}

#define SDR_STR_HASH65(str) ({ \
    SDR_DJB2(SDR_STR_HASH65_SEED, 6u, str); \
})

static inline size_t sdr_str_hash65(const char *str) {
    return SDR_STR_HASH65(str);
}

#define SDR_INT_HASH(num) ({ \
   num * SDR_INT_HASH_SEED; \
})

static inline size_t sdr_int_hash(const int num) {
    return SDR_INT_HASH(num);
}

#define SDR_LONG_HASH(num) ({ \
   num * SDR_LONG_HASH_SEED; \
})

static inline size_t sdr_long_hash(const long num) {
    return SDR_LONG_HASH(num);
}

#endif //SANDRA_HASH_H
