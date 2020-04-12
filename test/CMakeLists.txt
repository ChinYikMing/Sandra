project(sandra-test LANGUAGES C)
set(CMAKE_C_STANDARD 11)

find_path(CRITERION_INCLUDE_DIR criterion/criterion.h)
find_library(CRITERION_LIB NAMES criterion libcriterion)

include_directories(${CRITERION_INCLUDE_DIR})
set(TESTING_NAMES
        vector
        hash_map
        embed_list
        list
        numgen
        list_sort
        bits)
set(TESTING_UTILS
        utils/test_num.h
        utils/test_num.c)

foreach (t ${TESTING_NAMES})
    add_executable(test-${t} ${t}.c ${TESTING_UTILS})
    target_link_libraries(test-${t} sandra_static ${CRITERION_LIB})
    add_test(test-${t} test-${t})
endforeach ()

add_executable(tdd tdd.c)
target_link_libraries(tdd sandra)
