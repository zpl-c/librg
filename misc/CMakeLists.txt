cmake_minimum_required(VERSION 2.8)
project(librg)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories("../code/")

set(apps_dir "../code/apps/")
file(GLOB apps ${apps_dir}/*)

foreach(apps_source ${apps})
    get_filename_component(apps_name ${apps_source} NAME)
    string(REPLACE ".c" "${OUTPUT_EXT}" apps_name ${apps_name})

    # Setup the apps
    add_executable(${apps_name} ${apps_source})
endforeach()

if (EMSCRIPTEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g --profiling -s ASSERTIONS=1 -s WASM=1 -s INITIAL_MEMORY=268435456 -s FORCE_FILESYSTEM=1")
    set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)
endif ()

add_executable(unit ../code/tests/unit.c)
