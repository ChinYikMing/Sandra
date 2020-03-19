//
// Created by Jason Zheng on 2020/3/19.
//

#include <list.h>

typedef struct user {
    int id;
    char *name;
} User;

int main() {
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

    SdrList *ptr;
    sdr_list_for(&list, ptr) {
        User *u = sdr_list_data(ptr);
        printf("User[%d]: %s\n", u->id, u->name);
    }
}
