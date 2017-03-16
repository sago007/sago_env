cmake_minimum_required(VERSION 2.8.9)
project (hello_world)
find_package(Boost COMPONENTS program_options REQUIRED)

find_package(wxWidgets REQUIRED net gl core base)
include(${wxWidgets_USE_FILE})

add_executable(hello_world src/hello_world.cpp)
TARGET_LINK_LIBRARIES( hello_world ${Boost_LIBRARIES} )
target_link_libraries( hello_world ${wxWidgets_LIBRARIES})
