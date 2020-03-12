# macro from CLion
macro(fetch_googletest _download_module_path _download_root)
    set(GOOGLETEST_DOWNLOAD_ROOT ${_download_root})
    configure_file(
            ${_download_module_path}/googletest-download.cmake
            ${_download_root}/CMakeLists.txt
            @ONLY
    )
    unset(GOOGLETEST_DOWNLOAD_ROOT)

    execute_process(
            COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${_download_root}
    )
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()

    execute_process(
            COMMAND "${CMAKE_COMMAND}" --build .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${_download_root}
    )
    if(result)
        message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # adds the targets: gtest, gtest_main, gmock, gmock_main
    add_subdirectory(
            ${_download_root}/googletest-src
            ${_download_root}/googletest-build
    )
endmacro()
