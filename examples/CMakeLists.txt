project(sandra-example)

set(EXAMPLE_NAMES
        vector
        hash_map
        embed_list
        list)

foreach (t ${EXAMPLE_NAMES})
    add_executable(ex-${t} ${t}.c)
    target_link_libraries(ex-${t} sandra)
endforeach ()
