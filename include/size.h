//
// Created by Jason Zheng on 2020/3/17.
//

#ifndef SANDRA_SIZE_H
#define SANDRA_SIZE_H

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

/* ===================== BYTES ====================== */
#define _16_GiB 17179869184
#define _8_GiB 8589934592
#define _4_GiB 4294967296
#define _2_GiB 2147483648
#define _1_GiB 1073741824

#define _512_MiB 536870912
#define _256_MiB 268435456
#define _128_MiB 134217728
#define _64_MiB 67108864
#define _32_MiB 33554432
#define _16_MiB 16777216
#define _8_MiB 8388608
#define _4_MiB 4194304
#define _2_MiB 2097152
#define _1_MiB 1048576

#define _512_KiB 524288
#define _256_KiB 262144
#define _128_KiB 131072
#define _64_KiB 65536
#define _32_KiB 32768
#define _16_KiB 16384
#define _8_KiB 8192
#define _4_KiB 4096
#define _2_KiB 2048
#define _1_KiB 1024

#endif //SANDRA_SIZE_H
