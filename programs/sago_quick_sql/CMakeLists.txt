cmake_minimum_required(VERSION 3.14)
project (sago_quick_sql)

#Compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")

include(FetchContent)
FetchContent_Declare(
  platform_folders
  GIT_REPOSITORY https://github.com/sago007/PlatformFolders.git
  GIT_TAG        f2625faed48bc891eb624187f8a0e4a1d1cd6ad6
  FIND_PACKAGE_ARGS NAMES platform_folders
)
FetchContent_MakeAvailable(platform_folders)

find_package(Boost COMPONENTS program_options REQUIRED)
file(GLOB SOURCES "src/*.cpp")

add_executable(sago_quick_sql ${SOURCES})
TARGET_LINK_LIBRARIES( sago_quick_sql ${Boost_LIBRARIES} cppdb sago::platform_folders )
