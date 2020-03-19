//
// Created by Jason Zheng on 2020/3/19.
//

#include <embed_list.h>

typedef struct user {
    int id;
    char *name;
    SdrEmbedList node;
} User;

int main() {
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

    SdrEmbedList *ptr;
    sdr_elist_for(&list, ptr) {
        User *u = sdr_elist_data(ptr, User, node);
        printf("User[%d]: %s\n", u->id, u->name);
    }
}
