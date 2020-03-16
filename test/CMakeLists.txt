project(sandra-test LANGUAGES C)
set(CMAKE_C_STANDARD 11)

find_path(CRITERION_INCLUDE_DIR criterion/criterion.h)
find_library(CRITERION_LIB NAMES criterion libcriterion)

include_directories(${CRITERION_INCLUDE_DIR})
set(TESTING_NAMES
        vector
        hash_map)

foreach (t ${TESTING_NAMES})
    add_executable(test-${t} ${t}.c)
    target_link_libraries(test-${t} sandra ${CRITERION_LIB})
    add_test(test-${t} test-${t})
endforeach ()

add_executable(tdd tdd.c)
target_link_libraries(tdd sandra)

