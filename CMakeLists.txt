cmake_minimum_required(VERSION 2.8)
project(librg)

find_package(Threads)

add_subdirectory(node_modules/enet.c)

include_directories(include
    node_modules/zpl.c/include
    node_modules/zpl_ent.c/include
    node_modules/zpl_math.c/include
    node_modules/zpl_event.c/include
    node_modules/enet.c/include
)

add_executable(librg.server test/server.c)
add_executable(librg.client test/client.c)
add_executable(librg.entity test/entity.c)

target_link_libraries(librg.server enet ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} m)
target_link_libraries(librg.client enet ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} m)
target_link_libraries(librg.entity enet ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} m)
