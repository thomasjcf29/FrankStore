cmake_minimum_required(VERSION 3.10)
project(FrankStoreUI)

find_package(PkgConfig)

pkg_check_modules(GTKMM "gtkmm-3.0")
add_definitions(${GTKMM_CFLAGS})

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -mwindows")
endif()

file(GLOB FrankStoreUI_SRC
        "object_files/*.cpp"
        "header_files/*.h"
        "*.cpp"
        "*.c"
)

add_executable(FrankStoreUI ${FrankStoreUI_SRC})

target_link_libraries(FrankStoreUI ${GTKMM_LIBRARIES})

if(WIN32)
    SET(FrankStoreUI_LINKFLAGS project.res)
    SET_TARGET_PROPERTIES(FrankStoreUI PROPERTIES LINK_FLAGS ${FrankStoreUI_LINKFLAGS})
endif()
