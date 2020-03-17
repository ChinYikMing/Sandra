//
// Created by Jason Zheng on 2020/3/17.
//

#ifndef SANDRA_STRBUF_H
#define SANDRA_STRBUF_H

#include <stdlib.h>
#include <string.h>

#define SDR_SBUF_DFLT_CAP_BITS 4u

typedef struct sdr_string_buf {
    char *buf;
    size_t capacity;
    size_t size; /* next free position in buf */
} SdrStringBuf;

static inline int sdr_sbuf_init_d(SdrStringBuf *sb, unsigned cap_bits) {
    size_t capacity = 1u << cap_bits;
    sb->buf = malloc(capacity);
    if (!sb->buf) return -1;
    sb->capacity = capacity;
    sb->size = 0;
    return 0;
}

static inline int sdr_sbuf_init(SdrStringBuf *sb) {
    return sdr_sbuf_init_d(sb, SDR_SBUF_DFLT_CAP_BITS);
}

static inline int sdr_sbuf_putc(SdrStringBuf *sb, char c) {
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

int sdr_sbuf_putn(SdrStringBuf *sb, const void *src, size_t n);

int sdr_sbuf_putf(SdrStringBuf *sb, char *format, ...) __printflike(2, 3);

static inline int sdr_sbuf_puts(SdrStringBuf *sb, char *str) {
    return sdr_sbuf_putn(sb, str, strlen(str));
}

static inline void sdr_sbuf_destroy(SdrStringBuf *sb) {
    if (sb->buf)
        free(sb->buf);
    sb->buf = NULL;
}

/**
 * sbuf_flush - Flush the underlying raw buffer, which may be longer than needed,
 *              subsequent calls to this sbuf need to be reinitialized first.
 */
static inline char *sdr_sbuf_flush(SdrStringBuf *sb) {
    char *p = sb->buf;
    sb->buf = NULL;
    return p;
}

/**
 * sbuf_str - Building a fitted string from the buffer,
 *            subsequent calls to this sbuf need to be reinitialized first.
 */
char *sdr_sbuf_str(SdrStringBuf *sb);

#endif //SANDRA_STRBUF_H
