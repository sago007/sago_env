cmake_minimum_required(VERSION 3.10.2)
project (community-archiver)
find_package(Boost COMPONENTS program_options REQUIRED)
add_executable(community-archiver src/community-archiver.cpp)
TARGET_LINK_LIBRARIES( community-archiver ${Boost_LIBRARIES} )
target_link_libraries( community-archiver rhash)

