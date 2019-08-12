cmake_minimum_required(VERSION 3.1.3)
project(librg)

find_package(Threads)

# if you didnt install the library via
# npm, and you dont have node_modules folder
# you can redefine this option to point onto differnt folder
if (NOT DEFINED LIBRG_VENDOR_FOLDER)
    set(LIBRG_VENDOR_FOLDER ${CMAKE_SOURCE_DIR}/node_modules)
    set(LIBRG_POSTFIX ".c")
else()
    set(LIBRG_VENDOR_FOLDER ${CMAKE_SOURCE_DIR}/${LIBRG_VENDOR_FOLDER})
endif()

# define librg as a library (not really)
add_library(librg INTERFACE)

# add those compile flags badboys
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# proxy our includes to outside world
target_include_directories(librg INTERFACE include
    ${LIBRG_VENDOR_FOLDER}/zpl${LIBRG_POSTFIX}/code
    ${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX}/include)

# link all the deps
target_link_libraries(librg INTERFACE ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

if (WIN32)
    # windows libraries for enet
    target_link_libraries(librg INTERFACE ws2_32 winmm)
elseif (UNIX)
    # unix math library for us
    target_link_libraries(librg INTERFACE m)
endif()

# sdl2 demo
if (LIBRG_DEMO)
    add_subdirectory(${LIBRG_VENDOR_FOLDER}/sdl2${LIBRG_POSTFIX})
    include_directories(${LIBRG_VENDOR_FOLDER}/sdl2${LIBRG_POSTFIX}/include)

    add_executable(librg_demo_server test/demo-server.c)
    add_executable(librg_demo_client test/demo-client.c)

    target_link_libraries(librg_demo_server librg)
    target_link_libraries(librg_demo_client librg SDL2)
endif()

if (LIBRG_CLI_TEST)
    add_executable(librg_cli_server test/cli-server.c)
    add_executable(librg_cli_client test/cli-client.c)

    target_link_libraries(librg_cli_server librg)
    target_link_libraries(librg_cli_client librg)
endif()

# test for travis/etc
if (LIBRG_TEST)
    add_executable(librg_test test/build-test.c)
    add_executable(librg_test_cpp test/build-test.cpp)
    target_link_libraries(librg_test librg)
    target_link_libraries(librg_test_cpp librg)
endif()

# static library
if (LIBRG_STATIC)
    # special flags for MSVC
    if (MSVC OR "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")

        # This piece of crap could be very well replaced by something
        # like replace(ARRAY PRED VALUE)
        # But instead, we get to use this. Uh!
        set(CompilerFlags
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
        )
        foreach(CompilerFlag ${CompilerFlags})
          string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
        endforeach()
    endif()

    add_definitions(-DLIBRG_STATIC)

    # Once again, one has to hack in cmake to achieve what he needs...
    add_library(librg_static STATIC test/library.c)

    # This doesn't actually make any significant difference, only
    # makes sure the includes are further propagated.
    target_link_libraries(librg_static librg)
endif()

# shared library
if (LIBRG_SHARED)
    add_definitions(-DLIBRG_SHARED -DENET_DLL)
    add_library(librg_shared SHARED test/library.c)

    # if (WIN32)
    #     set_target_properties(librg_shared PROPERTIES
    #         LINK_FLAGS "/WHOLEARCHIVE"
    #     )
    # elseif (APPLE)
    #     set_target_properties(librg_shared PROPERTIES
    #         LINK_FLAGS "-Wl,-all_load"
    #     )
    # else ()
    #     set_target_properties(librg_shared PROPERTIES
    #         LINK_FLAGS "-Wl,--whole-archive"
    #     )
    # endif ()

    target_link_libraries(librg_shared librg)
endif()
