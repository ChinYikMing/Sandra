//
// Created by Jason Zheng on 2020/3/19.
//

#include <criterion/criterion.h>
#include <sandra/list.h>

#define D(x) \
    sdr_elist_data(x, SdrList, node)

Test(EmbedList, init) {
    SDR_DEFINE_LIST(list);
    cr_assert(list.node.next == &list.node);
    cr_assert(list.node.prev == &list.node);
}

Test(EmbedList, push_front) {
    SDR_DEFINE_LIST(list);
    SdrList nodes[3];
    sdr_list_push_front(&list, nodes);
    cr_assert(sdr_list_size(&list) == 1);
    sdr_list_push_front(&list, nodes + 1);
    cr_assert(sdr_list_size(&list) == 2);
    sdr_list_push_front(&list, nodes + 2);
    cr_assert(sdr_list_size(&list) == 3);


    cr_assert(D(list.node.next) == nodes + 2);
    cr_assert(D(nodes[0].node.next) == &list);
    cr_assert(D(nodes[1].node.next) == nodes);
    cr_assert(D(nodes[2].node.next) == nodes + 1);
    cr_assert(D(list.node.prev) == nodes);
    cr_assert(D(nodes[2].node.prev) == &list);
    cr_assert(D(nodes[1].node.prev) == nodes + 2);
    cr_assert(D(nodes[0].node.prev) == nodes + 1);
}

Test(EmbedList, push_back) {
    SDR_DEFINE_LIST(list);
    SdrList nodes[3];
    sdr_list_push_back(&list, nodes);
    cr_assert(sdr_list_size(&list) == 1);
    sdr_list_push_back(&list, nodes + 1);
    cr_assert(sdr_list_size(&list) == 2);
    sdr_list_push_back(&list, nodes + 2);
    cr_assert(sdr_list_size(&list) == 3);

    cr_assert(D(list.node.next) == nodes);
    cr_assert(D(nodes[0].node.next) == nodes + 1);
    cr_assert(D(nodes[1].node.next) == nodes + 2);
    cr_assert(D(nodes[2].node.next) == &list);
    cr_assert(D(list.node.prev) == nodes + 2);
    cr_assert(D(nodes[2].node.prev) == nodes + 1);
    cr_assert(D(nodes[1].node.prev) == nodes);
    cr_assert(D(nodes[0].node.prev) == &list);
}

Test(EmbedList, insert) {
    SDR_DEFINE_LIST(list);
    SdrList nodes[5];
    sdr_list_push_back(&list, nodes);
    sdr_list_push_back(&list, nodes + 1);
    sdr_list_push_back(&list, nodes + 2);
    sdr_list_push_back(&list, nodes + 3);

    int ret = sdr_list_insert(&list, nodes + 4, 2);
    cr_assert(ret == 0);
    cr_assert(sdr_list_entry(&list, 1) == nodes + 1);
    cr_assert(sdr_list_entry(&list, 2) == nodes + 4);
    cr_assert(sdr_list_entry(&list, 3) == nodes + 2);

}

Test(EmbedList, get) {
    typedef struct user {
        int id;
        char *name;
        SdrEList node;
    } User;

    SDR_DEFINE_LIST(list);
    SdrList nodes[] = {
        [0] = {&(User) {87, "Frankyu"}},
        [1] = {&(User) {66, "Jason"}},
        [2] = {&(User) {69, "Sheep"}},
    };

    for (int i = 0; i < sizeof(nodes) / sizeof(nodes[0]); ++i) {
        SdrList *n = nodes + i;
        sdr_list_push_back(&list, n);
    }

    cr_assert(sdr_list_get(&list, 0) == nodes[0].data);
    cr_assert(sdr_list_get(&list, 1) == nodes[1].data);
    cr_assert(sdr_list_get(&list, 2) == nodes[2].data);
}
