cmake_minimum_required(VERSION 3.14)

project(shpong)

include(cmake/include.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
#    set(LINK_OPTIONS "/INCREMENTAL:NO")
#endif()

add_subdirectory(src)
