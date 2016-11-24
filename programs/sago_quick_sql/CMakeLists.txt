cmake_minimum_required(VERSION 2.8.9)
project (sago_quick_sql)

#Compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")

find_package(Boost COMPONENTS program_options REQUIRED)
file(GLOB SOURCES "src/*.cpp")

add_executable(sago_quick_sql ${SOURCES})
TARGET_LINK_LIBRARIES( sago_quick_sql ${Boost_LIBRARIES} cppdb )
