set(crole_test_srcs
    src/main.c
    src/test_vec.c
    src/test_dllist.c
    src/test_ref.c)

add_executable(crole-test ${crole_test_srcs})
target_include_directories(crole-test PUBLIC include)
target_link_libraries(crole-test crole) 
