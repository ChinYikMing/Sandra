//
// Created by Jason Zheng on 2020/3/17.
//

#ifndef SANDRA_STRBUF_H
#define SANDRA_STRBUF_H

#include "basis.h"
#include <stdlib.h>
#include <string.h>

#define SDR_SBUF_DFLT_CAP_BITS 4u

typedef struct sdr_strbuf {
    char *buf;
    size_t capacity;
    size_t size;
} SdrStrBuf;

int sdr_strbuf_init_d(SdrStrBuf *sb, unsigned cap_bits);

static inline int sdr_strbuf_init(SdrStrBuf *sb) {
    return sdr_strbuf_init_d(sb, SDR_SBUF_DFLT_CAP_BITS);
}

int sdr_strbuf_putc(SdrStrBuf *sb, char c);

int sdr_strbuf_putn(SdrStrBuf *sb, const void *src, size_t n);

int sdr_strbuf_putf(SdrStrBuf *sb, const char *format, ...) __printflike(2, 3);

static inline int sdr_strbuf_puts(SdrStrBuf *sb, const char *str) {
    return sdr_strbuf_putn(sb, str, strlen(str));
}

static inline void sdr_strbuf_destroy(SdrStrBuf *sb) {
    if (sb->buf) free(sb->buf);
    sb->buf = NULL;
}

/**
 * sbuf_flush - Flush the underlying raw buffer, which may be longer than needed,
 *              subsequent calls to this sbuf need to be reinitialized first.
 */
char *sdr_strbuf_flush(SdrStrBuf *sb, int fit);

#endif //SANDRA_STRBUF_H
