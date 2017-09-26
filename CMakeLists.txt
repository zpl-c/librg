cmake_minimum_required(VERSION 2.8)
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

add_subdirectory(
    ${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/enet)

# define librg as a library (not really)
add_library(librg INTERFACE)

# add those compile flags badboys
if (UNIX)
    target_compile_options(librg INTERFACE -std=gnu99)
else()
    target_compile_options(librg INTERFACE -std=c99)
endif()

# proxy our includes to outside world
target_include_directories(librg INTERFACE include
    ${LIBRG_VENDOR_FOLDER}/zpl${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_ent${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_math${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_cull${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_event${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX}/include)

# link all the deps
target_link_libraries(librg INTERFACE
    enet ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

if (WIN32)
    # windows libraries for enet
    target_link_libraries(librg INTERFACE Ws2_32 Winmm)
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

# test for travis/etc
if (LIBRG_TEST)
    add_executable(librg_test test/build-test.c)
    target_link_libraries(librg_test librg)
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
    add_library(librg_static STATIC test/library.c
        ${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX}/callbacks.c
        ${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX}/compress.c
        ${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX}/host.c
        ${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX}/list.c
        ${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX}/packet.c
        ${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX}/peer.c
        ${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX}/protocol.c
        ${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX}/unix.c
        ${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX}/win32.c
    )

    # This doesn't actually make any significant difference, only
    # makes sure the includes are further propagated.
    target_link_libraries(librg_static librg enet)
endif()

# shared library
if (LIBRG_SHARED)
    add_definitions(-DLIBRG_SHARED)
    add_library(librg_shared SHARED test/library.c)
    target_link_libraries(librg_shared librg)
endif()
