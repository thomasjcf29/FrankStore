cmake_minimum_required(VERSION 3.10)
project(FrankStore)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )
find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)

file(GLOB FrankStore_SRC
        "*.cpp"
        "*.h"
)

add_executable(FrankStore ${FrankStore_SRC})

target_link_libraries(FrankStore ${ImageMagick_LIBRARIES})
target_link_libraries(FrankStore sodium)
