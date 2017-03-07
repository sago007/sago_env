cmake_minimum_required(VERSION 2.8.9)
project (tiledtest)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${tiledtest_SOURCE_DIR}/src/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11 -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

find_package(Boost COMPONENTS program_options REQUIRED)

#Setup SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

#Setup things that use pkg-config
find_package(PkgConfig REQUIRED)
pkg_search_module(SDL2MIXER REQUIRED SDL2_mixer)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf)

pkg_search_module(JSONCPP REQUIRED jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})

pkg_search_module(XML2 REQUIRED libxml-2.0)
include_directories(${XML2_INCLUDE_DIRS})

message("${CMAKE_EXE_LINKER_FLAGS}")
message("${CMAKE_MODULE_PATH}")
message("${SDL2_INCLUDE_DIR}")

file(GLOB SOURCES "src/*.cpp" "src/*/*.cpp" "src/Libs/*/*.c" "src/Libs/*/*.cpp")

add_executable(tiledtest ${SOURCES})
target_link_libraries( tiledtest ${SDL2_LIBRARY})
target_link_libraries( tiledtest physfs z b64)
target_link_libraries( tiledtest ${XML2_LIBRARIES})
target_link_libraries( tiledtest ${JSONCPP_LIBRARIES} )
target_link_libraries( tiledtest ${SDL2MIXER_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES})
TARGET_LINK_LIBRARIES( tiledtest ${Boost_LIBRARIES} )
