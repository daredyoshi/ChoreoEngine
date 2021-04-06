
# Add an application to play around with
set(

    SandboxSources
    SandboxApp.cpp
    )

include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine)
include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/ChoreoEngine)
link_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/)
link_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/ChoreoEngine)

add_executable(
    Sandbox ${SandboxSources} 
)

target_compile_options(Sandbox PRIVATE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${gcc_flags}>>" "$<${msvc_cxx}: $<BUILD_INTERFACE:${msvc_flags}>>"
    )
target_compile_features(Sandbox PUBLIC cxx_std_17)
target_link_libraries(Sandbox PUBLIC ChoreoEngine)
