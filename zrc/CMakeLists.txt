add_executable(zrc
    include/ability.hpp
    src/ability.cpp
    include/camera.hpp
    src/camera.cpp
    include/caster.hpp
    src/caster.cpp
    include/color.h
    src/color.c
    include/control.hpp
    src/control.cpp
    include/curves.hpp
    src/curves.cpp
    include/draw_curves.hpp
    src/draw_curves.cpp
    include/draw_lines.hpp
    src/draw_lines.cpp
    include/draw_map.hpp
    src/draw_map.cpp
    include/draw_spines.hpp
    src/draw_spines.cpp
    include/flight.hpp
    src/flight.cpp
    include/font.hpp
    src/font.cpp
    include/fsq.hpp
    src/fsq.cpp
    include/id.hpp
    src/id.cpp
    include/life.hpp
    src/life.cpp
    include/locomotion.hpp
    src/locomotion.cpp
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
    include/seek.hpp
    src/seek.cpp
    include/sense.hpp
    src/sense.cpp
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
    include/zset.hpp
    src/zset.cpp
    include/zsys.hpp
    src/zsys.cpp
)

target_include_directories(zrc PUBLIC include)
target_include_directories(zrc PRIVATE src)

target_link_libraries(zrc PUBLIC chipmunk_static)
target_link_libraries(zrc PUBLIC gl3w)
target_link_libraries(zrc PUBLIC glfw)
target_link_libraries(zrc PUBLIC glm)
target_link_libraries(zrc PUBLIC par)
target_link_libraries(zrc PUBLIC tinycthread)
target_link_libraries(zrc PUBLIC vector)

set_target_properties(zrc PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")