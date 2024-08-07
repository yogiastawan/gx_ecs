cmake_minimum_required(VERSION 3.0.0...3.5.0)
project(gx_ecs VERSION 0.1.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/Wall)
    add_compile_options(/Wextra)
else()
    add_compile_options(-Wall)
    add_compile_options(-Wextra)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-DGX_DEBUG")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

add_subdirectory(src)

include(CTest)
enable_testing()

add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
