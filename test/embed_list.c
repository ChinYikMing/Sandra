//
// Created by Jason Zheng on 2020/3/19.
//

#include <criterion/criterion.h>
#include <embed_list.h>

Test(EmbedList, init) {
    SDR_DEFINE_EMBED_LIST(list);
    cr_assert(list.next == &list);
    cr_assert(list.prev == &list);
}

Test(EmbedList, push_front) {
    SDR_DEFINE_EMBED_LIST(list);
    SdrEmbedList nodes[3];
    sdr_elist_push_front(&list, nodes);
    cr_assert(sdr_elist_size(&list) == 1);
    sdr_elist_push_front(&list, nodes + 1);
    cr_assert(sdr_elist_size(&list) == 2);
    sdr_elist_push_front(&list, nodes + 2);
    cr_assert(sdr_elist_size(&list) == 3);

    cr_assert(list.next == nodes + 2);
    cr_assert(nodes[0].next == &list);
    cr_assert(nodes[1].next == nodes);
    cr_assert(nodes[2].next == nodes + 1);
    cr_assert(list.prev == nodes);
    cr_assert(nodes[2].prev == &list);
    cr_assert(nodes[1].prev == nodes + 2);
    cr_assert(nodes[0].prev == nodes + 1);
}

Test(EmbedList, push_back) {
    SDR_DEFINE_EMBED_LIST(list);
    SdrEmbedList nodes[3];
    sdr_elist_push_back(&list, nodes);
    cr_assert(sdr_elist_size(&list) == 1);
    sdr_elist_push_back(&list, nodes + 1);
    cr_assert(sdr_elist_size(&list) == 2);
    sdr_elist_push_back(&list, nodes + 2);
    cr_assert(sdr_elist_size(&list) == 3);

    cr_assert(list.next == nodes);
    cr_assert(nodes[0].next == nodes + 1);
    cr_assert(nodes[1].next == nodes + 2);
    cr_assert(nodes[2].next == &list);
    cr_assert(list.prev == nodes + 2);
    cr_assert(nodes[2].prev == nodes + 1);
    cr_assert(nodes[1].prev == nodes);
    cr_assert(nodes[0].prev == &list);
}

Test(EmbedList, insert) {
    SDR_DEFINE_EMBED_LIST(list);
    SdrEmbedList nodes[5];
    sdr_elist_push_back(&list, nodes);
    sdr_elist_push_back(&list, nodes + 1);
    sdr_elist_push_back(&list, nodes + 2);
    sdr_elist_push_back(&list, nodes + 3);

    int ret = sdr_elist_insert(&list, nodes + 4, 2);
    cr_assert(ret == 0);
    cr_assert(sdr_elist_entry(&list, 1) == nodes + 1);
    cr_assert(sdr_elist_entry(&list, 2) == nodes + 4);
    cr_assert(sdr_elist_entry(&list, 3) == nodes + 2);

}

Test(EmbedList, get) {
    typedef struct user {
        int id;
        char *name;
        SdrEmbedList node;
    } User;

    SDR_DEFINE_EMBED_LIST(list);
    User users[] = {
        [0] = {87, "Frankyu"},
        [1] = {66, "Jason"},
        [2] = {69, "Sheep"},
    };

    for (int i = 0; i < sizeof(users) / sizeof(users[0]); ++i) {
        User *u = users + i;
        sdr_elist_push_back(&list, &u->node);
    }

    cr_assert(sdr_elist_get(&list, 0, User, node) == users);
    cr_assert(sdr_elist_get(&list, 1, User, node) == users + 1);
    cr_assert(sdr_elist_get(&list, 2, User, node) == users + 2);
}
