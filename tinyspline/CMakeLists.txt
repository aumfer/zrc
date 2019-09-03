add_library(tinyspline STATIC
    tinyspline/src/tinyspline.h
    tinyspline/src/tinyspline.c)

target_include_directories(tinyspline PUBLIC tinyspline/src)
