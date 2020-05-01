//
// Created by Jason Zheng on 2020/3/17.
//

#include <sandra/vector.h>
#include <assert.h>

int main() {
    SdrVector(char) vec;
    sdr_vec_init(&vec, 2);

    sdr_vec_push_back(&vec, 'a');
    sdr_vec_push_back(&vec, 'b');
    sdr_vec_push_back(&vec, 'c');
    sdr_vec_push_back(&vec, 'd');

    char ret = sdr_vec_pop_front(&vec); // 'a'
    assert(ret == 'a');
    assert(sdr_vec_capacity(&vec) == 4);

    sdr_vec_push_back(&vec, 'e');
    assert(sdr_vec_capacity(&vec) == 4);

    sdr_vec_push_back(&vec, 'f');
    assert(sdr_vec_capacity(&vec) == 8);

    sdr_vec_push_front(&vec, 'a');

    sdr_vec_pop_back(&vec); // 'f'

    for (int i = 0; i < 6; i++) {
        int val = sdr_vec_get(&vec, i);
        assert(val == 'a' + i);
    }

}