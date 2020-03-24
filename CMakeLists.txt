# ================= BASIC CONFIGS ==================
cmake_minimum_required(VERSION 3.5)

project(sandra
        VERSION 0.0.1
        LANGUAGES C)

set(CMAKE_C_STANDARD 11)
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
set(SANDRA_INC_DIR include)
set(SANDRA_INC
        include/sandra/version.h
        include/sandra/vector.h
        include/sandra/hash_map.h
        include/sandra/map.h
        include/sandra/abs_hash_map.h
        include/sandra/hash.h
        include/sandra/basis.h
        include/sandra/sgr.h
        include/sandra/strbuf.h
        include/sandra/size.h
        include/sandra/embed_list.h
        include/sandra/list.h
        include/sandra/compiler_attr.h
        include/sandra/str.h)

set(SANDRA_SRC_DIR src)
set(SANDRA_SRC
        src/abs_hash_map.c
        src/hash_map.c
        src/strbuf.c
        src/embed_list.c)

set(MUST_PREFIX_PATTERN
        "sdr_version|SDR_VERSION|SDR_VER_MAJOR|SDR_VER_MINOR|SDR_VER_PATCH")

set(SANDRA_CONFIG_FILES include/sandra/version.h)
foreach (cf ${SANDRA_CONFIG_FILES})
    configure_file(${cf}.in ${cf})
endforeach ()
# ==================================================


# ================= BUILD LIBRARY ==================
if (USE_PREFIX)
    message(STATUS "Building with the identifier prefix")
    add_library(sandra SHARED ${SANDRA_SRC} ${SANDRA_INC})
    add_library(sandra_static STATIC ${SANDRA_SRC} ${SANDRA_INC})
    foreach (cf ${SANDRA_CONFIG_FILES})
        target_sources(sandra PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/${cf})
        target_sources(sandra_static PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/${cf})
    endforeach ()
    target_include_directories(sandra PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/${SANDRA_INC_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}/${SANDRA_INC_DIR})
    target_include_directories(sandra_static PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/${SANDRA_INC_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}/${SANDRA_INC_DIR})
else ()
    message(STATUS "Building without the identifier prefix")
    add_library(sandra SHARED)
    add_library(sandra_static STATIC)

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
            's/(?!${MUST_PREFIX_PATTERN})(${UPPER_PREFIX}|${LOWER_PREFIX}|${CAMEL_PREFIX})#{0,2}([a-zA-Z0-9_]+)/\\2/g' \
            ${CMAKE_CURRENT_BINARY_DIR}/${s}")

        # Add sources from BIN_DIR rather than SRC_DIR.
        target_sources(sandra PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/${s})
        target_sources(sandra_static PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/${s})
    endforeach ()

    target_include_directories(sandra PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/${SANDRA_INC_DIR})
    target_include_directories(sandra_static PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/${SANDRA_INC_DIR})
endif ()
# ==================================================


# ================ INSTALL LIBRARY =================
install(TARGETS sandra sandra_static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)

if (USE_PREFIX)
    install(DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}/${SANDRA_INC_DIR}/
            ${CMAKE_CURRENT_BINARY_DIR}/${SANDRA_INC_DIR}/
            DESTINATION include
            FILES_MATCHING PATTERN "*.h")
else ()
    install(DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}/${SANDRA_INC_DIR}/
            DESTINATION include
            FILES_MATCHING PATTERN "*.h")
endif ()
# ==================================================


# =============== UNINSTALL LIBRARY ================
add_custom_target(uninstall
        COMMAND xargs rm < install_manifest.txt
        && rmdir ${CMAKE_INSTALL_PREFIX}/include/sandra)
# ==================================================


# ================== SUB PROJECTS ==================
if (USE_PREFIX)
    add_subdirectory(examples)
    add_subdirectory(test)
endif ()
# ==================================================

