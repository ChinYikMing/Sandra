//
// Created by Jason Zheng on 2020/3/21.
//
#include <embed_list.h>

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
