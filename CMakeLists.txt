cmake_minimum_required(VERSION 3.0)
project(zrc)

if(WIN32)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(chipmunk)
add_subdirectory(gl3w)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(par)
add_subdirectory(tinycthread)
add_subdirectory(vector)
add_subdirectory(zrc)
