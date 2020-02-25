//
// Created by Jason Zheng on 2020/2/7.
//

#include "version.h"
#include "hello.h"
#include <stdio.h>

int main() {
    printf("Sandra Version: %s\n", sdr_version());
    say_hello("World!");

    return 0;
}