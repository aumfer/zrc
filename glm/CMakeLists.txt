add_library(glm INTERFACE)

target_compile_definitions(glm INTERFACE GLM_ENABLE_EXPERIMENTAL)
#target_compile_definitions(glm INTERFACE GLM_FORCE_MESSAGES)
target_compile_definitions(glm INTERFACE _USE_MATH_DEFINES)
target_compile_definitions(glm INTERFACE GLM_FORCE_INTRINSICS)
target_compile_definitions(glm INTERFACE GLM_FORCE_INLINE)

target_include_directories(glm INTERFACE glm)

if(WIN32)
    target_compile_options(glm INTERFACE "/wd4309")
endif()
