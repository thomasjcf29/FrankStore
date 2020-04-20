cmake_minimum_required(VERSION 3.10)
project(FrankStore)

add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )

find_package(PkgConfig)
pkg_check_modules(LIBSODIUM "libsodium")
add_definitions(${LIBSODIUM_CFLAGS})

find_package(ImageMagick COMPONENTS Magick++ REQUIRED)

include_directories(${ImageMagick_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

file(GLOB FrankStore_SRC
        "object_files/*.cpp"
        "header_files/*.h"
        "*.cpp"
)

add_executable(FrankStore ${FrankStore_SRC})

target_link_libraries(FrankStore ${ImageMagick_LIBRARIES})
target_link_libraries(FrankStore ${LIBSODIUM_LDFLAGS})
