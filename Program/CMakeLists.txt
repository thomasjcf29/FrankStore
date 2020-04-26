cmake_minimum_required(VERSION 3.10)
project(FrankStore)

add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )

if(UNIX)
	find_package(PkgConfig)
	pkg_check_modules(LIBSODIUM "libsodium")
	add_definitions(${LIBSODIUM_CFLAGS})

	pkg_check_modules(EXIV "exiv2")
	add_definitions(${EXIV_CFLAGS})
endif(UNIX)

find_package(ImageMagick COMPONENTS Magick++ REQUIRED)

include_directories(${ImageMagick_INCLUDE_DIRS})

if(WIN32)
	add_compile_options(/wd4251 /wd4275 /wd4996)
endif(WIN32)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

file(GLOB FrankStore_SRC
        "object_files/*.cpp"
        "header_files/*.h"
        "*.cpp"
)

add_executable(FrankStore ${FrankStore_SRC})

target_link_libraries(FrankStore ${ImageMagick_LIBRARIES})

if(UNIX)
	target_link_libraries(FrankStore ${LIBSODIUM_LDFLAGS})
	target_link_libraries(FrankStore ${EXIV_LDFLAGS})
endif(UNIX)

if(WIN32)
	target_compile_definitions(FrankStore PUBLIC /wd4101 /wd4275 /wd4996)
endif(WIN32)
