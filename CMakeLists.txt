cmake_minimum_required(VERSION 2.8)
project(librg)

add_subdirectory(vendor/enet)

include_directories(include vendor/zpl/include vendor/enet/include)

add_executable(librg.server test/server.c)
add_executable(librg.client test/client.c)


target_link_libraries(librg.server enet)
target_link_libraries(librg.client enet)
