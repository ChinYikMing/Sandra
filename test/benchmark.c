#include <stdio.h>
#include "../include/sandra/vector.h"
#include "utils/test_num.h"

int main(){
    SdrVector(int, 4) vec;
    sdr_vec_init(&vec);

    TestNumbers tnums;
    tnums_init(&tnums);
    tnums_read(&tnums, "test/test_num.txt", 0);

    int num;
    tnums_for(&tnums, num)
        sdr_vec_push_back(&vec, num);

    tnums_destroy(&tnums);
    sdr_vec_destroy(&vec);
}