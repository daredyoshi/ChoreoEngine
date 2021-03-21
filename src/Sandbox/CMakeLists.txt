
# Add an application to play around with
set(
    SandboxSources
    SandboxApp.cpp
    )
include_directories(${ChoreoFlocking_SOURCE_DIR}/src/ChoreoFlocking/)
message(${ChoreoFlocking_SOURCE_DIR}/src/ChoreoFlocking/include)
link_directories(${ChoreoFlocking_SOURCE_DIR}/src/ChoreoFlocking/src)
add_executable(
    Sandbox ${SandboxSources} 
)
target_link_libraries(Sandbox PUBLIC ChoreoFlocking)
