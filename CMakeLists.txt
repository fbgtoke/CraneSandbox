project(CraneSandbox)
cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

set(appName CraneSandbox)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -Wall -std=c++11")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

## Placeholder ##
set(CRANE_ROOT_DIR "/home/fabio/Documents/Crane/build/install")
set(CMAKE_INSTALL_PREFIX "/home/fabio/Documents/CraneSandbox/build")

find_package(Crane REQUIRED)
find_package(GLM REQUIRED)
find_package(Freetype REQUIRED)

include_directories(${CRANE_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})

link_directories(${CRANE_LIBRARY_DIRS})


file(GLOB sources
  "*.hpp"
  "*.cpp"
)

add_executable(${appName} ${sources})

target_link_libraries(${appName} crane)

install(TARGETS ${appName} DESTINATION .)
install(DIRECTORY resources DESTINATION .)