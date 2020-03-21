//
// Created by Jason Zheng on 2020/3/19.
//

#include <criterion/criterion.h>
#include <embed_list.h>

Test(init, point_to_self) {
    SDR_DEFINE_ELIST(list);
    cr_assert(list.next == &list);
    cr_assert(list.prev == &list);
}

Test(push_front, insert_to_head_in_order) {
    SDR_DEFINE_ELIST(list);
    SdrEList nodes[3];
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

Test(push_back, insert_to_tail_in_order) {
    SDR_DEFINE_ELIST(list);
    SdrEList nodes[3];
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

Test(remove_entry, remove_first_entry) {
    SDR_DEFINE_ELIST(list);
    SdrEList nodes[5];
    sdr_elist_push_back(&list, nodes);
    sdr_elist_push_back(&list, nodes + 1);
    sdr_elist_push_back(&list, nodes + 2);

    sdr_elist_remove_entry(nodes);
    cr_assert(sdr_elist_size(&list) == 2);
    cr_assert(list.next == nodes + 1);
    cr_assert(nodes[1].next == nodes + 2);
}

Test(remove_entry, remove_last_entry) {
    SDR_DEFINE_ELIST(list);
    SdrEList nodes[5];
    sdr_elist_push_back(&list, nodes);
    sdr_elist_push_back(&list, nodes + 1);
    sdr_elist_push_back(&list, nodes + 2);

    sdr_elist_remove_entry(nodes + 2);
    cr_assert(sdr_elist_size(&list) == 2);
    cr_assert(list.next == nodes);
    cr_assert(nodes[0].next == nodes + 1);
}

Test(replace, replace_with_independent_node) {
    SDR_DEFINE_ELIST(list);
    SdrEList nodes[5];
    sdr_elist_push_back(&list, nodes);
    sdr_elist_push_back(&list, nodes + 1);
    sdr_elist_push_back(&list, nodes + 2);

    sdr_elist_replace(nodes + 1, nodes + 3);
    cr_assert(sdr_elist_size(&list) == 3);
    cr_assert(list.next == nodes);
    cr_assert(nodes[0].next == nodes + 3);
    cr_assert(nodes[3].next == nodes + 2);
}

Test(swap, swap_consecutive_nodes) {
    SDR_DEFINE_ELIST(list);
    SdrEList nodes[5];
    sdr_elist_push_back(&list, nodes);
    sdr_elist_push_back(&list, nodes + 1);
    sdr_elist_push_back(&list, nodes + 2);

    sdr_elist_swap(nodes, nodes + 1);
    cr_assert(sdr_elist_size(&list) == 3);
    cr_assert(list.next == nodes + 1);
    cr_assert(nodes[1].next == nodes + 0);
    cr_assert(nodes[0].next == nodes + 2);
}

Test(swap, swap_non_consecutive_nodes) {
    SDR_DEFINE_ELIST(list);
    SdrEList nodes[5];
    sdr_elist_push_back(&list, nodes);
    sdr_elist_push_back(&list, nodes + 1);
    sdr_elist_push_back(&list, nodes + 2);

    sdr_elist_swap(nodes, nodes + 2);
    cr_assert(sdr_elist_size(&list) == 3);
    cr_assert(list.next == nodes + 2);
    cr_assert(nodes[2].next == nodes + 1);
    cr_assert(nodes[1].next == nodes);
}

Test(insert, insert_node_by_index) {
    SDR_DEFINE_ELIST(list);
    SdrEList nodes[5];
    sdr_elist_push_back(&list, nodes);
    sdr_elist_push_back(&list, nodes + 1);
    sdr_elist_push_back(&list, nodes + 2);
    sdr_elist_push_back(&list, nodes + 3);

    int ret = sdr_elist_insert(&list, nodes + 4, 2);
    cr_assert(ret == 0);
    cr_assert(sdr_elist_size(&list) == 5);
    cr_assert(sdr_elist_entry(&list, 1) == nodes + 1);
    cr_assert(sdr_elist_entry(&list, 2) == nodes + 4);
    cr_assert(sdr_elist_entry(&list, 3) == nodes + 2);
}

Test(remove, remove_node_by_index) {
    SDR_DEFINE_ELIST(list);
    SdrEList nodes[5];
    sdr_elist_push_back(&list, nodes);
    sdr_elist_push_back(&list, nodes + 1);
    sdr_elist_push_back(&list, nodes + 2);
    sdr_elist_push_back(&list, nodes + 3);

    int ret = sdr_elist_remove(&list, 2);
    cr_assert(ret == 0);
    cr_assert(sdr_elist_size(&list) == 3);
    cr_assert(sdr_elist_entry(&list, 0) == nodes );
    cr_assert(sdr_elist_entry(&list, 1) == nodes + 1);
    cr_assert(sdr_elist_entry(&list, 2) == nodes + 3);
}

Test(get, get_data_by_index) {
    typedef struct user {
        int id;
        char *name;
        SdrEList node;
    } User;

    SDR_DEFINE_ELIST(list);
    User users[] = {
        {87, "Frankyu"},
        {66, "Jason"},
        {69, "Sheep"},
    };

    for (int i = 0; i < sizeof(users) / sizeof(users[0]); ++i) {
        User *u = users + i;
        sdr_elist_push_back(&list, &u->node);
    }

    cr_assert(sdr_elist_get(&list, 0, User, node) == users);
    cr_assert(sdr_elist_get(&list, 1, User, node) == users + 1);
    cr_assert(sdr_elist_get(&list, 2, User, node) == users + 2);
}

/* MDN splice examples */
typedef struct fish {
    char *type;
    SdrEList node;
} Fish;

Test(splice, remove_0_before_index_2_and_insert_drum) {
    SDR_DEFINE_ELIST(list);
    Fish fishes[] = {
        {"angel"},
        {"clown"},
        {"mandarin"},
        {"sturgeon"},
    };
    for (int i = 0; i < sizeof(fishes) / sizeof(fishes[0]); ++i) {
        Fish *f = fishes + i;
        sdr_elist_push_back(&list, &f->node);
    }

    SDR_DEFINE_ELIST(list2);
    Fish drum = {"drum"};
    sdr_elist_push_back(&list2, &drum.node);

    sdr_elist_splice(&list, 2, 0, &list2);

    char *expect[] = {"angel", "clown", "drum", "mandarin", "sturgeon"};
    int i = 0;
    Fish *ptr;
    sdr_elist_for_data(&list, ptr, Fish, node) {
        cr_assert(strcmp(ptr->type, expect[i]) == 0);
        i++;
    }
}

Test(splice, remove_0_before_index_2_and_insert_drum_and_guitar) {
    SDR_DEFINE_ELIST(list);
    Fish fishes[] = {
        {"angel"},
        {"clown"},
        {"mandarin"},
        {"sturgeon"},
    };
    for (int i = 0; i < sizeof(fishes) / sizeof(fishes[0]); ++i) {
        Fish *f = fishes + i;
        sdr_elist_push_back(&list, &f->node);
    }

    SDR_DEFINE_ELIST(list2);
    Fish drum = {"drum"};
    Fish guitar = {"guitar"};
    sdr_elist_push_back(&list2, &drum.node);
    sdr_elist_push_back(&list2, &guitar.node);

    sdr_elist_splice(&list, 2, 0, &list2);

    char *expect[] = {"angel", "clown", "drum", "guitar", "mandarin", "sturgeon"};
    int i = 0;
    Fish *ptr;
    sdr_elist_for_data(&list, ptr, Fish, node) {
        cr_assert(strcmp(ptr->type, expect[i]) == 0);
        i++;
    }
}

Test(splice, remove_1_element_at_index_3) {
    SDR_DEFINE_ELIST(list);
    Fish fishes[] = {
        {"angel"},
        {"clown"},
        {"drum"},
        {"mandarin"},
        {"sturgeon"},
    };
    for (int i = 0; i < sizeof(fishes) / sizeof(fishes[0]); ++i) {
        Fish *f = fishes + i;
        sdr_elist_push_back(&list, &f->node);
    }

    sdr_elist_splice(&list, 3, 1, NULL);

    char *expect[] = {"angel", "clown", "drum", "sturgeon"};
    int i = 0;
    Fish *ptr;
    sdr_elist_for_data(&list, ptr, Fish, node) {
        cr_assert(strcmp(ptr->type, expect[i]) == 0);
        i++;
    }
}

Test(splice, remove_1_element_at_index_2_and_insert_trumpet) {
    SDR_DEFINE_ELIST(list);
    Fish fishes[] = {
        {"angel"},
        {"clown"},
        {"drum"},
        {"sturgeon"},
    };
    for (int i = 0; i < sizeof(fishes) / sizeof(fishes[0]); ++i) {
        Fish *f = fishes + i;
        sdr_elist_push_back(&list, &f->node);
    }

    SDR_DEFINE_ELIST(list2);
    Fish trumpet = {"trumpet"};
    sdr_elist_push_back(&list2, &trumpet.node);

    sdr_elist_splice(&list, 2, 1, &list2);

    char *expect[] = {"angel", "clown", "trumpet", "sturgeon"};
    int i = 0;
    Fish *ptr;
    sdr_elist_for_data(&list, ptr, Fish, node) {
        cr_assert(strcmp(ptr->type, expect[i]) == 0);
        i++;
    }
}

Test(splice, remove_2_elements_from_index_0_and_insert_parrot_anemone_and_blue) {

    SDR_DEFINE_ELIST(list);
    Fish fishes[] = {
        {"angel"},
        {"clown"},
        {"trumpet"},
        {"sturgeon"},
    };
    for (int i = 0; i < sizeof(fishes) / sizeof(fishes[0]); ++i) {
        Fish *f = fishes + i;
        sdr_elist_push_back(&list, &f->node);
    }

    SDR_DEFINE_ELIST(list2);
    Fish parrot = {"parrot"};
    Fish anemone = {"anemone"};
    Fish blue = {"blue"};
    sdr_elist_push_back(&list2, &parrot.node);
    sdr_elist_push_back(&list2, &anemone.node);
    sdr_elist_push_back(&list2, &blue.node);

    sdr_elist_splice(&list, 0, 2, &list2);

    char *expect[] = {"parrot", "anemone", "blue", "trumpet", "sturgeon"};
    int i = 0;
    Fish *ptr;
    sdr_elist_for_data(&list, ptr, Fish, node) {
        cr_assert(strcmp(ptr->type, expect[i]) == 0);
        i++;
    }
}

Test(splice, remove_2_elements_from_index_2) {

    SDR_DEFINE_ELIST(list);
    Fish fishes[] = {
        {"parrot"},
        {"anemone"},
        {"blue"},
        {"trumpet"},
        {"sturgeon"},
    };
    for (int i = 0; i < sizeof(fishes) / sizeof(fishes[0]); ++i) {
        Fish *f = fishes + i;
        sdr_elist_push_back(&list, &f->node);
    }

    sdr_elist_splice(&list, 2, 2, NULL);

    char *expect[] = {"parrot", "anemone", "sturgeon"};
    int i = 0;
    Fish *ptr;
    sdr_elist_for_data(&list, ptr, Fish, node) {
        cr_assert(strcmp(ptr->type, expect[i]) == 0);
        i++;
    }
}

Test(splice, remove_1_element_from_negative_index_2) {

    SDR_DEFINE_ELIST(list);
    Fish fishes[] = {
        {"angel"},
        {"clown"},
        {"mandarin"},
        {"sturgeon"},
    };
    for (int i = 0; i < sizeof(fishes) / sizeof(fishes[0]); ++i) {
        Fish *f = fishes + i;
        sdr_elist_push_back(&list, &f->node);
    }

    sdr_elist_splice(&list, -2, 1, NULL);

    char *expect[] = {"angel", "clown", "sturgeon"};
    int i = 0;
    Fish *ptr;
    sdr_elist_for_data(&list, ptr, Fish, node) {
        cr_assert(strcmp(ptr->type, expect[i]) == 0);
        i++;
    }
}
