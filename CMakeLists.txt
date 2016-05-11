cmake_minimum_required(VERSION 3.5)
project(ex4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_FILE_OFFSET_BITS=64")

set(SOURCE_FILES CachingFileSystem.cpp)
add_executable(CachingFileSystem ${SOURCE_FILES})