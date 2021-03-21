
# Add an application to play around with
set(
    SandboxSources
    SandboxApp.cpp
    )
include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/)
link_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/)
add_executable(
    Sandbox ${SandboxSources} 
)
target_link_libraries(Sandbox PUBLIC ChoreoEngine)
