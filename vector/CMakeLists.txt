add_library(vector
    Vector/Vector/vector_display_glinc.h
    Vector/Vector/vector_display_utils.c
    Vector/Vector/vector_display_utils.h
    Vector/Vector/vector_display.c
    Vector/Vector/vector_display.h
    Vector/Vector/vector_font_simplex.c
    Vector/Vector/vector_font_simplex.h
    Vector/Vector/vector_shapes.c
    Vector/Vector/vector_shapes.h
)

target_compile_definitions(vector PRIVATE _USE_MATH_DEFINES)

target_include_directories(vector PUBLIC Vector/Vector)

if(WIN32)
    target_compile_options(vector PRIVATE "/wd4244")
    target_compile_options(vector PRIVATE "/wd4305")
endif()

target_link_libraries(vector PUBLIC gl3w)