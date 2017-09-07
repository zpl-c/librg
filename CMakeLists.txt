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

# test for travis/etc
if (LIBRG_TEST)
    add_executable(librg_test test/build-test.c)
    target_link_libraries(librg_test librg)
endif()
