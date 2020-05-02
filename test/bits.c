//
// Created by Jason Zheng on 2020/4/12.
//

#include <criterion/criterion.h>
#include <sandra/basis.h>

Test(generic_ffs, uint_num) {
    unsigned int x = 0;
    cr_assert(sdr_u_ffs(x) == 0);
    x = 0b001;
    cr_assert(sdr_u_ffs(x) == 1);
    x = 0b010;
    cr_assert(sdr_u_ffs(x) == 2);
    x = 0b011;
    cr_assert(sdr_u_ffs(x) == 1);
    x = 0b100;
    cr_assert(sdr_u_ffs(x) == 3);
    x = 0b101;
    cr_assert(sdr_u_ffs(x) == 1);
    x = 0b110;
    cr_assert(sdr_u_ffs(x) == 2);
    x = 0b111;
    cr_assert(sdr_u_ffs(x) == 1);
    x = 0b1000;
    cr_assert(sdr_u_ffs(x) == 4);
}

Test(generic_clz, uint_num) {
    unsigned int x = 0;
    size_t bsize_uint = CHAR_BIT * sizeof x;
    cr_assert(sdr_u_clz(x) == bsize_uint);
    x = 0b001;
    cr_assert(sdr_u_clz(x) == bsize_uint - 1);
    x = 0b010;
    cr_assert(sdr_u_clz(x) == bsize_uint - 2);
    x = 0b011;
    cr_assert(sdr_u_clz(x) == bsize_uint - 2);
    x = 0b100;
    cr_assert(sdr_u_clz(x) == bsize_uint - 3);
    x = 0b101;
    cr_assert(sdr_u_clz(x) == bsize_uint - 3);
    x = 0b110;
    cr_assert(sdr_u_clz(x) == bsize_uint - 3);
    x = 0b111;
    cr_assert(sdr_u_clz(x) == bsize_uint - 3);
    x = 0b1000;
    cr_assert(sdr_u_clz(x) == bsize_uint - 4);
}

Test(round_up_nearest_power_of_two, uint_num) {
    unsigned int x = 0;
    cr_assert(sdr_nearest_geq_po2(x) == 1);
    x = 1;
    cr_assert(sdr_nearest_geq_po2(x) == 1);
    x = 2;
    cr_assert(sdr_nearest_geq_po2(x) == 2);
    x = 3;
    cr_assert(sdr_nearest_geq_po2(x) == 4, "num: %d\n", sdr_nearest_geq_po2(x));
    x = 4;
    cr_assert(sdr_nearest_geq_po2(x) == 4);
    x = 5;
    cr_assert(sdr_nearest_geq_po2(x) == 8);
    x = 6;
    cr_assert(sdr_nearest_geq_po2(x) == 8);
    x = 7;
    cr_assert(sdr_nearest_geq_po2(x) == 8);
    x = 8;
    cr_assert(sdr_nearest_geq_po2(x) == 8);

    x = 15;
    cr_assert(sdr_nearest_geq_po2(x) == 16);
    x = 16;
    cr_assert(sdr_nearest_geq_po2(x) == 16);

    x = 1000;
    cr_assert(sdr_nearest_geq_po2(x) == 1024);
    x = 1024;
    cr_assert(sdr_nearest_geq_po2(x) == 1024);
}
