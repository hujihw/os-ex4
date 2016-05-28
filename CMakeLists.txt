cmake_minimum_required(VERSION 3.2)
project(ex4)
INCLUDE(FindPkgConfig)

# set the sources, flags and executable name
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")
set(SOURCE_FILES CachingFileSystem.cpp logManager.cpp logManager.h)
add_executable(CachingFileSystem ${SOURCE_FILES})

# use pkg-config to configure fuse compilation
pkg_check_modules(FUSE REQUIRED "fuse")

if (${FUSE_FOUND})
    target_link_libraries(CachingFileSystem ${FUSE_LIBRARIES})
    add_definitions(${FUSE_CFLAGS} ${FUSE_CFLAGS_OTHER})
endif (${FUSE_FOUND})


# recipe for testing file
#set(SOURCE_FILES Testing/TestModule.cpp Testing/TestModule.h)
#add_executable(TestModule ${SOURCE_FILES})
