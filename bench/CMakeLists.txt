add_executable(bench-crole-vec src/bench-crole-vec.c)
target_link_libraries(bench-crole-vec crole)

add_executable(bench-std-vector src/bench-std-vector.cpp)

add_executable(bench-crole-dllist src/bench-crole-dllist.c)
target_link_libraries(bench-crole-dllist crole)

add_executable(bench-std-list src/bench-std-list.cpp)
