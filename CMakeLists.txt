cmake_minimum_required(VERSION 2.8)
project(librg)

find_package(Threads)

if (NOT LIBRG_VENDOR_FOLDER)
    set(LIBRG_VENDOR_FOLDER node_modules)
    set(LIBRG_POSTFIX ".c")
endif()

add_subdirectory(${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX})

include_directories(include
    ${LIBRG_VENDOR_FOLDER}/zpl${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_ent${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_math${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_cull${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_event${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX}/include
)

add_executable(librg.server test/server.c)
add_executable(librg.client test/client.c)
#add_executable(librg.entity test/entity.c)

target_link_libraries(librg.server enet ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} m)
target_link_libraries(librg.client enet ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} m)
#target_link_libraries(librg.entity enet ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} m)

if (LIBRG_DEMO)
add_subdirectory(${LIBRG_VENDOR_FOLDER}/sdl2${LIBRG_POSTFIX})
include_directories(include ${LIBRG_VENDOR_FOLDER}/sdl2${LIBRG_POSTFIX}/include)
add_executable(librg.demo-server test/demo-server.c)
add_executable(librg.demo-client test/demo-client.c)
target_link_libraries(librg.demo-server enet ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} m)
target_link_libraries(librg.demo-client enet SDL2 SDL2main ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} m)
endif()