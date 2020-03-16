# ================= BASIC CONFIGS ==================
cmake_minimum_required(VERSION 3.5)

project(sandra
        VERSION 0.0.1
        LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-initializer-overrides")
enable_testing()

set(UPPER_PREFIX "SDR_")
set(LOWER_PREFIX "sdr_")
set(CAMEL_PREFIX "Sdr")
# ==================================================

# ================= BUILD OPTIONS ==================
set(USE_PREFIX ON CACHE STRING "Build with identifier prefix")
set(BUILD_TYPE "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE ${BUILD_TYPE} CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()
# ==================================================


# ============== PROJECT DEPENDENCIES ==============
set(SANDRA_SRC_DIR src)
set(SANDRA_SRC src/abs_hash_map.c src/hash_map.c)

set(SANDRA_INC_DIR include)
set(SANDRA_INC
        include/version.h
        include/vector.h
        include/hash_map.h
        include/map.h
        include/abs_hash_map.h
        include/hash.h
        include/basis.h)

set(MUST_PREFIX_PATTERN
        "sdr_version|SDR_VERSION|SDR_VER_MAJOR|SDR_VER_MINOR|SDR_VER_PATCH")

set(SANDRA_CONFIG_FILES include/version.h)
foreach (cf ${SANDRA_CONFIG_FILES})
    configure_file(${cf}.in ${cf})
endforeach ()
# ==================================================


# ================= BUILD LIBRARY ==================
if (USE_PREFIX)
    message(STATUS "Building with the identifier prefix")
    add_library(sandra SHARED ${SANDRA_SRC} ${SANDRA_INC})
    foreach (cf ${SANDRA_CONFIG_FILES})
        target_sources(sandra PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/${cf})
    endforeach ()
    target_include_directories(sandra PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/${SANDRA_INC_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}/${SANDRA_INC_DIR})
else ()
    message(STATUS "Building without the identifier prefix")
    add_library(sandra SHARED)

    # Duplicate sources & headers to BINARY_DIR for scripting
    message(STATUS "Duplicating sandra dependencies...")
    foreach (s IN LISTS SANDRA_SRC_DIR SANDRA_INC_DIR)
        file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${s}
                DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
                PATTERN "*.in" EXCLUDE)
    endforeach ()

    foreach (s IN LISTS SANDRA_SRC SANDRA_INC)
        # Remove prefix if necessary
        execute_process(COMMAND bash -c "
            perl -pi -e \
            's/(?!${MUST_PREFIX_PATTERN})(${UPPER_PREFIX}|${LOWER_PREFIX}|${CAMEL_PREFIX})([a-zA-Z0-9_]+)/\\2/g' \
            ${CMAKE_CURRENT_BINARY_DIR}/${s}")

        # Add sources from BIN_DIR rather than SRC_DIR.
        target_sources(sandra PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/${s})
    endforeach ()

    target_include_directories(sandra PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/${SANDRA_INC_DIR})
endif ()
# ==================================================


# ================ INSTALL LIBRARY =================
set(INC_INSTALL_RELATIVE_DIR "include/sandra")

install(TARGETS sandra
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

if (USE_PREFIX)
    install(DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}/${SANDRA_INC_DIR}/
            ${CMAKE_CURRENT_BINARY_DIR}/${SANDRA_INC_DIR}/
            DESTINATION ${INC_INSTALL_RELATIVE_DIR}
            FILES_MATCHING PATTERN "*.h")
else ()
    install(DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}/${SANDRA_INC_DIR}/
            DESTINATION ${INC_INSTALL_RELATIVE_DIR}
            FILES_MATCHING PATTERN "*.h")
endif ()
# ==================================================


# =============== UNINSTALL LIBRARY ================
add_custom_target(uninstall
        COMMAND xargs rm < install_manifest.txt
        && rm -r ${CMAKE_INSTALL_PREFIX}/${INC_INSTALL_RELATIVE_DIR})
# ==================================================


# ================== SUB PROJECTS ==================
add_subdirectory(examples)
add_subdirectory(test)
# ==================================================

