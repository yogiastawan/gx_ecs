cmake_minimum_required(VERSION 3.0.0...3.5.0)

add_executable(test_comp_1 test_comp_1.c)
target_link_libraries(test_comp_1 ${PROJECT_NAME})
target_include_directories(test_comp_1 PRIVATE CMAKE_SOURCE_DIR/src)
add_test(gx_comp::insert test_comp_1)

add_executable(test_comp_2 test_comp_2.c)
target_link_libraries(test_comp_2 ${PROJECT_NAME})
target_include_directories(test_comp_2 PRIVATE CMAKE_SOURCE_DIR/src)
add_test(gx_comp::destroy test_comp_2)
