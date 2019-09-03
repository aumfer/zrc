add_executable(zrc
    include/camera_control.h
    src/camera_control.cpp
    include/camera.h
    src/camera.cpp
    include/color.h
    src/color.cpp
    include/curves_map.h
    src/curves_map.cpp
    include/curves.h
    src/curves.cpp
    include/draw_curves_map.h
    src/draw_curves_map.cpp
    include/draw_curves.h
    src/draw_curves.cpp
    include/draw_map.h
    src/draw_map.cpp
    include/font.h
    src/font.cpp
    include/fsq.h
    src/fsq.cpp
    include/GL/gl3w.h
    src/GL/gl3w.c
    include/hash.h
    include/id.h
    src/id.cpp
    include/lsgl.h
    src/lsgl.cpp
    include/lsm.h
    src/lsm.cpp
    src/main.cpp
    include/map.h
    src/map.cpp
    include/moving_average.h
    src/moving_average.cpp
    include/physics.h
    src/physics.cpp
    include/timer.h
    src/timer.cpp
    include/ui.h
    src/ui.cpp
    include/visual.h
    src/visual.cpp
    include/zrc.h
    src/zrc.cpp
    include/zsys.hpp
    src/zsys.cpp
)

target_include_directories(zrc PUBLIC include)
target_include_directories(zrc PRIVATE src)

target_link_libraries(zrc PUBLIC chipmunk_static)
target_link_libraries(zrc PUBLIC glfw)
target_link_libraries(zrc PUBLIC glm)
target_link_libraries(zrc PUBLIC klib)
target_link_libraries(zrc PUBLIC tinycthread)
target_link_libraries(zrc PUBLIC tinyspline)

set_target_properties(zrc PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")