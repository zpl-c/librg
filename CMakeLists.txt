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

# define librg as a library (not really)
add_library(librg INTERFACE)

# add those compile flags badboys
if (UNIX)
    target_compile_options(librg INTERFACE -std=gnu99)
else()
    target_compile_options(librg INTERFACE -std=c99)
endif()

### ENET STUFF
# The "configure" step.
include(CheckFunctionExists)
include(CheckStructHasMember)
include(CheckTypeSize)
check_function_exists("fcntl" HAS_FCNTL)
check_function_exists("poll" HAS_POLL)
check_function_exists("getaddrinfo" HAS_GETADDRINFO)
check_function_exists("getnameinfo" HAS_GETNAMEINFO)
check_function_exists("gethostbyname_r" HAS_GETHOSTBYNAME_R)
check_function_exists("gethostbyaddr_r" HAS_GETHOSTBYADDR_R)
check_function_exists("inet_pton" HAS_INET_PTON)
check_function_exists("inet_ntop" HAS_INET_NTOP)
check_struct_has_member("struct msghdr" "msg_flags" "sys/types.h;sys/socket.h" HAS_MSGHDR_FLAGS)
set(CMAKE_EXTRA_INCLUDE_FILES "sys/types.h" "sys/socket.h")
check_type_size("socklen_t" HAS_SOCKLEN_T BUILTIN_TYPES_ONLY)
unset(CMAKE_EXTRA_INCLUDE_FILES)
if(MSVC)
    add_definitions(-W3)
else()
    add_definitions(-Wno-error)
endif()

if(HAS_FCNTL)
    add_definitions(-DHAS_FCNTL=1)
endif()
if(HAS_POLL)
    add_definitions(-DHAS_POLL=1)
endif()
if(HAS_GETNAMEINFO)
    add_definitions(-DHAS_GETNAMEINFO=1)
endif()
if(HAS_GETADDRINFO)
    add_definitions(-DHAS_GETADDRINFO=1)
endif()
if(HAS_GETHOSTBYNAME_R)
    add_definitions(-DHAS_GETHOSTBYNAME_R=1)
endif()
if(HAS_GETHOSTBYADDR_R)
    add_definitions(-DHAS_GETHOSTBYADDR_R=1)
endif()
if(HAS_INET_PTON)
    add_definitions(-DHAS_INET_PTON=1)
endif()
if(HAS_INET_NTOP)
    add_definitions(-DHAS_INET_NTOP=1)
endif()
if(HAS_MSGHDR_FLAGS)
    add_definitions(-DHAS_MSGHDR_FLAGS=1)
endif()
if(HAS_SOCKLEN_T)
    add_definitions(-DHAS_SOCKLEN_T=1)
endif()
### END ENET STUFF

# proxy our includes to outside world
target_include_directories(librg INTERFACE include
    ${LIBRG_VENDOR_FOLDER}/zpl${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_ent${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_math${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_cull${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/zpl_event${LIBRG_POSTFIX}/include
    ${LIBRG_VENDOR_FOLDER}/enet${LIBRG_POSTFIX}/include)

# link all the deps
target_link_libraries(librg INTERFACE ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

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

if (LIBRG_CLI_TEST)
    add_executable(librg_cli_server test/cli-server.c)
    add_executable(librg_cli_client test/cli-client.c)

    target_link_libraries(librg_cli_server librg)
    target_link_libraries(librg_cli_client librg)
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
