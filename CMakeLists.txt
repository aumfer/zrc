cmake_minimum_required(VERSION 3.0)
project(zrc)

if(WIN32)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(chipmunk)
add_subdirectory(glfw)
add_subdirectory(handmade-math)
add_subdirectory(klib)
add_subdirectory(tinycthread)
add_subdirectory(zrc)
