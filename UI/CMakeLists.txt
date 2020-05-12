cmake_minimum_required(VERSION 3.10)
project(FrankStoreUI)

find_package(PkgConfig)

pkg_check_modules(GTKMM "gtkmm-3.0")
add_definitions(${GTKMM_CFLAGS})

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")

file(GLOB FrankStoreUI_SRC
        "object_files/*.cpp"
        "header_files/*.h"
        "*.cpp"
        "resources.c"
)

add_executable(FrankStoreUI ${FrankStoreUI_SRC})

target_link_libraries(FrankStoreUI ${GTKMM_LIBRARIES})
