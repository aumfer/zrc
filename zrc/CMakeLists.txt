add_executable(zrc
    include/camera.h
    src/camera.c
    include/color.h
    src/color.c
    include/curves_map.h
    src/curves_map.c
    include/curves.h
    src/curves.c
    include/draw_curves.h
    src/draw_curves.c
    include/draw_lines_map.h
    src/draw_lines_map.c
    include/draw_lines.h
    src/draw_lines.c
    include/draw.h
    src/draw.c
    include/font.h
    src/font.c
    include/fsq.h
    src/fsq.c
    include/GL/gl3w.h
    src/GL/gl3w.c
    include/hash.h
    include/id.h
    src/id.c
    include/lines_map.h
    src/lines_map.c
    include/lines.h
    src/lines.c
    include/lsgl.h
    src/lsgl.c
    include/lsm.h
    src/lsm.c
    src/main.c
    include/map.h
    src/map.c
    include/moving_average.h
    src/moving_average.c
    include/physics.h
    src/physics.c
    include/timer.h
    src/timer.c
    include/ui.h
    src/ui.c
    include/visual.h
    src/visual.c
    include/zrc.h
    src/zrc.c
)

target_include_directories(zrc PUBLIC include)
target_include_directories(zrc PRIVATE src)

target_link_libraries(zrc PUBLIC chipmunk_static)
target_link_libraries(zrc PUBLIC glfw)
target_link_libraries(zrc PUBLIC handmade-math)
target_link_libraries(zrc PUBLIC klib)
target_link_libraries(zrc PUBLIC tinycthread)

set_target_properties(zrc PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")