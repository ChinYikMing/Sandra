//
// Created by Jason Zheng on 2020/3/21.
//
#include <sandra/embed_list.h>

#define SDR_ELIST_MERGE_ARR_SIZE (CHAR_BIT * sizeof(size_t))

int sdr_elist_insert(SdrEList *list, SdrEList *new, long idx) {
    if (idx == 0) {
        sdr_elist_push_front(list, new);
        return 0;
    }

    SdrEList *entry = sdr_elist_entry(list, idx);
    if (!entry) return -1;
    sdr_pvt_elist_add(entry->prev, entry, new);
    return 0;
}

int sdr_elist_bulk_insert(SdrEList *list, SdrEList *first, SdrEList *last, long idx) {
    if (idx == 0) {
        sdr_elist_bulk_push_front(list, first, last);
        return 0;
    }

    SdrEList *entry = sdr_elist_entry(list, idx);
    if (!entry) return -1;
    sdr_pvt_elist_splice(entry->prev, entry, first, last);
    return 0;
}

int sdr_elist_remove(SdrEList *list, long idx) {
    SdrEList *entry = sdr_elist_entry(list, idx);
    if (!entry) return -1;
    sdr_elist_remove_entry(entry);
    return 0;
}

int sdr_elist_remove_s(SdrEList *list, long idx) {
    SdrEList *entry = sdr_elist_entry(list, idx);
    if (!entry) return -1;
    sdr_elist_remove_entry_s(entry);
    return 0;
}

SdrEList *sdr_elist_splice_d(SdrEList *list, long start, size_t delete_cnt,
                             SdrEList *head, SdrEList *tail) {
    SdrEList *target = NULL;
    SdrEList *deleted_start = NULL;
    SdrEList *deleted_end = NULL;
    SdrEList *splice_prev = NULL;
    SdrEList *splice_next = NULL;

    target = sdr_elist_entry(list, start);
    if (!target) return NULL;

    if (delete_cnt) {
        deleted_start = target;
        deleted_end = target;
        size_t curr_idx = 1;
        while (curr_idx < delete_cnt && deleted_end != (list))
            curr_idx++, deleted_end = deleted_end->next;
        splice_prev = deleted_start->prev;
        splice_next = deleted_end->next;
        sdr_pvt_elist_bulk_remove_s(splice_prev, splice_next);
    } else {
        splice_prev = target->prev;
        splice_next = target;
    }

    if (head && tail)
        sdr_pvt_elist_splice(splice_prev, splice_next, head, tail);

    return deleted_start;
}

void sdr_elist_sort(SdrEList *list, sdr_fn_elist_cmp cmp, void *data) {
    SdrEList *list_head = list;
    list = list->next;
    /* no need to sort */
    if (list_head->prev == list) return;

    /* convert to null-terminated list */
    list_head->prev->next = NULL;

    /* save sorted lists in k, each list length is 2^k */
    SdrEList *merged[SDR_ELIST_MERGE_ARR_SIZE] = {0};

    /* simple bottom-up merge sort implementation */
    size_t level = 0, max_level = 0;
    SdrEList *a, *b = NULL, *next = list;
    while (next) {
        b = next;
        next = b->next;
        b->next = NULL;

        level = 0;
        while ((a = merged[level])) {
            SdrEList *head, **tail = &head;
            for (;;) {
                if (cmp(a, b, data) <= 0) {
                    *tail = a;
                    tail = &a->next;
                    a = a->next;
                    if (!a) {
                        *tail = b;
                        break;
                    }
                } else {
                    *tail = b;
                    tail = &b->next;
                    b = b->next;
                    if (!b) {
                        *tail = a;
                        break;
                    }
                }
            }
            merged[level] = NULL;
            b = head;
            if (++level > max_level)
                max_level = level;
        }
        merged[level] = b;
    }

    /* merge the remaining nodes */
    if (level + 1 <= max_level) {
        do {
            level++;
        } while (!merged[level]);

        while (level <= max_level) {
            if ((a = merged[level])) {
                SdrEList *head, **tail = &head;
                for (;;) {
                    if (cmp(a, b, data) <= 0) {
                        *tail = a;
                        tail = &a->next;
                        a = a->next;
                        if (!a) {
                            *tail = b;
                            break;
                        }
                    } else {
                        *tail = b;
                        tail = &b->next;
                        b = b->next;
                        if (!b) {
                            *tail = a;
                            break;
                        }
                    }
                }
                merged[level] = NULL;
                b = head;
            }
            /* no update max_level */
            level++;
        }
        merged[level] = b;
    }

    /* convert to original doubly linked list */
    SdrEList *curr = list_head;
    next = merged[level];
    list_head->next = next;
    while (next) {
        next->prev = curr;
        curr = next;
        next = next->next;
    }
    curr->next = list_head;
    list_head->prev = curr;
}
