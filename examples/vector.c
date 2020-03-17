//
// Created by Jason Zheng on 2020/3/17.
//

#include <vector.h>
#include <assert.h>

int main() {
    SdrVector(char) vec;
    sdr_vector_init(&vec, 2);

    sdr_vector_push_back(&vec, 'a');
    sdr_vector_push_back(&vec, 'b');
    sdr_vector_push_back(&vec, 'c');
    sdr_vector_push_back(&vec, 'd');

    char ret = sdr_vector_pop_front(&vec); // 'a'
    assert(ret == 'a');
    assert(sdr_vector_capacity(&vec) == 4);

    sdr_vector_push_back(&vec, 'e');
    assert(sdr_vector_capacity(&vec) == 4);

    sdr_vector_push_back(&vec, 'f');
    assert(sdr_vector_capacity(&vec) == 8);

    sdr_vector_push_front(&vec, 'a');

    sdr_vector_pop_back(&vec); // 'f'

    for (int i = 0; i < 6; i++) {
        int val = sdr_vector_get(&vec, i);
        assert(val == 'a' + i);
    }

}