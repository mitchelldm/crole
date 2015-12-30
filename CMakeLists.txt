cmake_minimum_required(VERSION 3.4)
project(crole)

find_package(Threads)

set(crole_srcs
    src/vec.c
    src/dllist.c
    src/ref.c
    external/tinycthread/source/tinycthread.c)

include_directories(include external/tinycthread/source)
add_library(crole STATIC ${crole_srcs})
target_link_libraries(crole ${CMAKE_THREAD_LIBS_INIT})

add_subdirectory(test)
add_subdirectory(bench)
