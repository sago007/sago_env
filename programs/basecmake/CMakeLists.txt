cmake_minimum_required(VERSION 3.10.2..4.1.2)

# Use Boost's native CMake config instead of FindBoost module
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

project(basecmake
    VERSION 0.1.0
    DESCRIPTION "A base CMake project demonstrating Boost program_options"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Find Boost
find_package(Boost COMPONENTS program_options REQUIRED)

add_executable(basecmake src/basecmake.cpp)
target_compile_definitions(basecmake PRIVATE VERSIONNUMBER="${PROJECT_VERSION}")
target_link_libraries(basecmake PRIVATE Boost::program_options)
