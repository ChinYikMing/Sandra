//
// Created by Jason Zheng on 2020/3/17.
//

#include <sandra/strbuf.h>
#include <stdarg.h>
#include <stdio.h>

int sdr_strbuf_init_d(SdrStrBuf *sb, const unsigned cap_bits) {
    size_t capacity = 1u << cap_bits;
    sb->buf = malloc(capacity);
    if (!sb->buf) return -1;
    sb->capacity = capacity;
    sb->size = 0;

    return 0;
}

int sdr_strbuf_putc(SdrStrBuf *sb, const char c) {
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

int sdr_strbuf_putn(SdrStrBuf *sb, const void *src, const size_t n) {
    const size_t capacity = sb->capacity;
    const size_t size = sb->size;
    const size_t balance = capacity - size;
    const size_t extra_need = (n > balance) ? (n - balance) : 0;
    if (extra_need > 0) {
        const size_t total_need = capacity + extra_need;
        size_t new_capacity = capacity;
        do
            new_capacity <<= 1u;
        while (new_capacity < total_need);

        void *tmp = realloc(sb->buf, new_capacity);
        if (!tmp) return -1;
        sb->buf = tmp;
        sb->capacity = new_capacity;
    }

    memcpy(sb->buf + size, src, n);
    sb->size += n;
    return 0;
}

int sdr_strbuf_putf(SdrStrBuf *sb, const char *format, ...) {
    va_list ap;
    va_start(ap, format);

    char *str;
    int vsa_ret = vasprintf(&str, format, ap);
    if (vsa_ret < 0) {
        va_end(ap);
        return -1;
    }

    sdr_strbuf_putn(sb, str, vsa_ret + 1);
    free(str);
    va_end(ap);

    return 0;
}

char *sdr_strbuf_flush(SdrStrBuf *sb, const int fit) {
    size_t size = sb->size;

    if (sb->size == 0 || sb->buf[sb->size - 1] != '\0') {
        sdr_strbuf_putc(sb, '\0');
        size++;
    }

    char *ret = fit ? realloc(sb->buf, size) : sb->buf;
    sb->buf = NULL;
    return ret;
}
