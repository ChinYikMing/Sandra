//
// Created by Jason Zheng on 2020/3/3.
//

#include <gtest/gtest.h>
#include "hello.h"

TEST(SumTest, Simple) {
    int ret = sdr_hello("World!");
    ASSERT_EQ(ret, printf("hello, World!\n"));
}
