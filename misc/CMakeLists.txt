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

add_executable(unit ../code/tests/unit.c)
