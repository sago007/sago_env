cmake_minimum_required(VERSION 3.14)
project (sago_quick_sql)

#Compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")

include(FetchContent)
FetchContent_Declare(
  platform_folders
  URL https://github.com/sago007/PlatformFolders/archive/refs/tags/4.3.0.tar.gz
  FIND_PACKAGE_ARGS NAMES platform_folders
)
FetchContent_MakeAvailable(platform_folders)

find_package(Boost COMPONENTS program_options REQUIRED)
file(GLOB SOURCES "src/*.cpp")

add_executable(sago_quick_sql ${SOURCES})
target_include_directories(sago_quick_sql PRIVATE ${platform_folders_SOURCE_DIR})
TARGET_LINK_LIBRARIES( sago_quick_sql ${Boost_LIBRARIES} cppdb sago::platform_folders )
