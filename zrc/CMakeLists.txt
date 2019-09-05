add_executable(zrc
    include/camera.hpp
    src/camera.cpp
    include/color.h
    src/color.c
    include/control.hpp
    src/control.cpp
    include/curves_map.hpp
    src/curves_map.cpp
    include/curves.hpp
    src/curves.cpp
    include/draw_curves_map.hpp
    src/draw_curves_map.cpp
    include/draw_curves.hpp
    src/draw_curves.cpp
    include/draw_map.hpp
    src/draw_map.cpp
    include/draw_spines.hpp
    src/draw_spines.cpp
    include/font.hpp
    src/font.cpp
    include/fsq.hpp
    src/fsq.cpp
    include/GL/gl3w.h
    src/GL/gl3w.c
    include/id.hpp
    src/id.cpp
    include/lsgl.h
    src/lsgl.c
    include/lsm.hpp
    src/lsm.cpp
    src/main.cpp
    include/map.hpp
    src/map.cpp
    include/moving_average.hpp
    src/moving_average.cpp
    src/par_streamlines.c
    include/physics.hpp
    src/physics.cpp
    include/spines.hpp
    src/spines.cpp
    include/timer.hpp
    src/timer.cpp
    include/ui.hpp
    src/ui.cpp
    include/visual.hpp
    src/visual.cpp
    include/zrc.hpp
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
target_link_libraries(zrc PUBLIC par)
target_link_libraries(zrc PUBLIC tinycthread)

set_target_properties(zrc PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")