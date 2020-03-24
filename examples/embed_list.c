//
// Created by Jason Zheng on 2020/3/19.
//

#include <sandra/embed_list.h>

typedef struct user {
    int id;
    char *name;
    SdrEList node;
} User;

int user_equal(const void *a, const void *b);

int user_id_test(const void *a, int i);

int main() {
    /* declare and init */
    SDR_DEFINE_ELIST(list);

    /* user data which is embedded with list node */
    User users[] = {
        {87,   "Frankyu"},
        {66,   "Jason"},
        {69,   "Sheep"},
        {123,  "WanWan"},
        {5278, "Howard"},
        {94,   "LegBone"},
    };

    /* push data to list tail */
    for (int i = 0; i < sizeof(users) / sizeof(users[0]); ++i) {
        User *u = users + i;
        sdr_elist_push_back(&list, &u->node);
    }

    /* for-each loop */
    User *u;
    sdr_elist_for_data(&list, u, User, node) {
        printf("[%d]: %s\n", u->id, u->name);
    }
    printf("\n");

    /* find target */
    u = sdr_elist_find(&list, user_id_test, 87,
                       User, node);
    printf("search: [%d] %s\n\n", u->id, u->name);

    /* filter entries */
    SdrEList filtered_list;
    sdr_elist_filter(&list, &filtered_list, user_id_test, 100, User, node);

    printf("filter:\n");
    sdr_elist_for_data(&filtered_list, u, User, node) {
        printf("[%d] %s\n", u->id, u->name);
    }
    printf("\n");
}

int user_id_test(const void *a, int i) {
    const User *ua = a;
    return ua->id < i;
}
