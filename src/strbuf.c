//
// Created by Jason Zheng on 2020/3/17.
//

#include "strbuf.h"
#include <stdarg.h>
#include <stdio.h>

int sdr_sbuf_putn(SdrStringBuf *sb, const void *src, const size_t n) {
    const size_t capacity = sb->capacity;
    const size_t size = sb->size;
    const ssize_t extra_need = size + n - capacity;
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

char *sdr_sbuf_str(SdrStringBuf *sb) {
    size_t len;
    if (sb->size == 0) {
        len = 0;
        sdr_sbuf_putc(sb, '\0');
    } else if (sb->buf[sb->size - 1] != '\0') {
        len = sb->size;
        sdr_sbuf_putc(sb, '\0');
    } else
        len = sb->size - 1;

    char *ret = realloc(sb->buf, len + 1);
    sb->buf = NULL;

    return ret;
}

int sdr_sbuf_putf(SdrStringBuf *sb, char *format, ...) {
    va_list ap;
    va_start(ap, format);

    char *str;
    int vsa_ret = vasprintf(&str, format, ap);
    if (vsa_ret < 0) {
        va_end(ap);
        return -1;
    }

    sdr_sbuf_putn(sb, str, vsa_ret + 1);
    free(str);
    va_end(ap);

    return 0;
}
