cmake_minimum_required(VERSION 2.8)
project(librg)

find_package(Threads)

if (NOT LIBRG_VENDOR_FOLDER)
    set(LIBRG_VENDOR_FOLDER node_modules)
    set(LIBRG_POSTFIX ".c")
endif()

add_subdirectory(${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX} ${CMAKE_CURRENT_BINARY_DIR}/enet${LIBRG_POSTFIX})

include_directories(include
    ${LIBRG_VENDOR_FOLDER}/zpl${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_ent${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_math${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_cull${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_event${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX}/include
)

if (LIBRG_DEMO)

    add_subdirectory(${LIBRG_VENDOR_FOLDER}/sdl2${LIBRG_POSTFIX})
    include_directories(include ${LIBRG_VENDOR_FOLDER}/sdl2${LIBRG_POSTFIX}/include)

    add_executable(librg.demo-server test/demo-server.c)
    add_executable(librg.demo-client test/demo-client.c)

    target_link_libraries(librg.demo-server enet ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(librg.demo-client enet SDL2 SDL2main ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

    if (WIN32)
        target_link_libraries(librg.demo-server ws2_32 winmm)
        target_link_libraries(librg.demo-client ws2_32 winmm)
    endif()

    if (UNIX)
        target_link_libraries(librg.demo-server m)
        target_link_libraries(librg.demo-client m)
    endif()
	
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

elseif (LIBRG_TEST)

    add_executable(buildtest test/build-test.c)
    target_link_libraries(buildtest enet ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} m)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

else()

    add_executable(librg.server test/server.c)
    add_executable(librg.client test/client.c)
    #add_executable(librg.entity test/entity.c)

    target_link_libraries(librg.server enet ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(librg.client enet ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

    if (WIN32)
        target_link_libraries(librg.server ws2_32 winmm)
        target_link_libraries(librg.client ws2_32 winmm)
    endif()

    if (UNIX)
        target_link_libraries(librg.server m)
        target_link_libraries(librg.client m)
    endif()
    
    #target_link_libraries(librg.entity enet ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

endif()
