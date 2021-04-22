
# Add an application to play around with
set(

    SandboxSources
    SandboxApp.cpp
    src/Sandbox2D.cpp
    src/ParticleSystem.cpp
    )


include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine)
include_directories(${ChoreoEngine_SOURCE_DIR}/src/dependencies/imgui/)
link_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/)

add_executable(
    Sandbox ${SandboxSources} 
)

target_compile_options(Sandbox PRIVATE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${gcc_flags}>>" "$<${msvc_cxx}: $<BUILD_INTERFACE:${msvc_flags}>>"
    )
target_compile_features(Sandbox PUBLIC cxx_std_17)
target_link_libraries(Sandbox PUBLIC imgui glm ChoreoEngine)

add_custom_target(
    copy_shader_files
    ${CMAKE_COMMAND}
    -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders
    COMMENT "Copying shader files" VERBATIM
    )

add_dependencies( Sandbox copy_shader_files )

# add custom target to copy texture files across

add_custom_target(
    copy_texture_files 
    ${CMAKE_COMMAND}
    -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/textures
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures
    COMMENT "Copying texture files" VERBATIM
    )

add_dependencies( Sandbox copy_texture_files )

# add custom tareget to copy obj files across
# add_custom_target(
#     copy_obj_files
#     ${CMAKE_COMMAND}
#     -E copy_directory
#     ${CMAKE_CURRENT_SOURCE_DIR}/obj
#     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/obj
#     COMMENT "Copying object files" VERBATIM
#     )
