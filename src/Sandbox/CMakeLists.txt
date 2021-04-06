
# Add an application to play around with
set(
    SandboxSources
    SandboxApp.cpp
    ${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/dependencies/glad.c
    )

include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine)
include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/dependencies)
include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/ChoreoEngine)
link_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/)
link_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/dependencies)
link_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/ChoreoEngine)
add_executable(
    Sandbox ${SandboxSources} 
)
target_link_libraries(Sandbox PUBLIC ChoreoEngine)
