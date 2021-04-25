
# Add an application to play around with
set(

    SandboxSources
    SandboxApp.cpp
    src/Sandbox2D.cpp
    )


include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoApp)
include_directories(${ChoreoEngine_SOURCE_DIR}/src/dependencies/imgui/)
link_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoApp/)

add_executable(
    Sandbox ${SandboxSources} 
)

target_compile_options(Sandbox PRIVATE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${gcc_flags}>>" "$<${msvc_cxx}: $<BUILD_INTERFACE:${msvc_flags}>>"
    )
target_compile_features(Sandbox PUBLIC cxx_std_17)
target_link_libraries(Sandbox PUBLIC imgui glm ChoreoApp)
