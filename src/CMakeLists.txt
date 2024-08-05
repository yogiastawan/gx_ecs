cmake_minimum_required(VERSION 3.0.0...3.5.0)

file(GLOB SRCS *.c utils/*.c)

add_library(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
