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
    size_t size; /* next free position in buf */
} SdrStrBuf;

static inline int sdr_strbuf_init_d(SdrStrBuf *sb, unsigned cap_bits) {
    size_t capacity = 1u << cap_bits;
    sb->buf = malloc(capacity);
    if (!sb->buf) return -1;
    sb->capacity = capacity;
    sb->size = 0;
    return 0;
}

static inline int sdr_strbuf_init(SdrStrBuf *sb) {
    return sdr_strbuf_init_d(sb, SDR_SBUF_DFLT_CAP_BITS);
}

static inline int sdr_strbuf_putc(SdrStrBuf *sb, char c) {
    if (sb->size == sb->capacity) {
        size_t new_cap = sb->capacity << 1u;
        void *tmp = realloc(sb->buf, new_cap);
        if (!tmp) return -1;
        sb->buf = tmp;
        sb->capacity = new_cap;
    }

    sb->buf[sb->size++] = c;
    return 0;
}

int sdr_strbuf_putn(SdrStrBuf *sb, const void *src, size_t n);

int sdr_strbuf_putf(SdrStrBuf *sb, char *format, ...) __printflike(2, 3);

static inline int sdr_strbuf_puts(SdrStrBuf *sb, char *str) {
    return sdr_strbuf_putn(sb, str, strlen(str));
}

static inline void sdr_strbuf_destroy(SdrStrBuf *sb) {
    if (sb->buf)
        free(sb->buf);
    sb->buf = NULL;
}

/**
 * sbuf_flush - Flush the underlying raw buffer, which may be longer than needed,
 *              subsequent calls to this sbuf need to be reinitialized first.
 */
static inline char *sdr_strbuf_flush(SdrStrBuf *sb) {
    char *p = sb->buf;
    sb->buf = NULL;
    return p;
}

/**
 * sbuf_str - Building a fitted string from the buffer,
 *            subsequent calls to this sbuf need to be reinitialized first.
 */
char *sdr_strbuf_str(SdrStrBuf *sb);

#endif //SANDRA_STRBUF_H
