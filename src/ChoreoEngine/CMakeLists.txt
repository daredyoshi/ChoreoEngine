
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(spdlog REQUIRED)


include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/)
include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/Application/Platform/OpenGL/)
include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/dependencies/imgui/backends/)

# Add the core lib
add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)

set(
    ChoreoEngineSources 
    Application/Application.cpp
    Application/Layer.cpp
    Application/LayerStack.cpp
    Application/ImGui/ImGuiLayer.cpp
    Application/Platform/Linux/LinuxWindow.cpp
    Application/Platform/Linux/LinuxInput.cpp
    ../dependencies/imgui/backends/imgui_impl_opengl3.cpp
    ../dependencies/imgui/backends/imgui_impl_glfw.cpp
    Application/Log.cpp
    )
add_library(
    ChoreoEngine ${ChoreoEngineSources}
)

target_precompile_headers(ChoreoEngine 
    PRIVATE
    "cepch.h"
    )
target_compile_features(ChoreoEngine PUBLIC cxx_std_17)
target_link_libraries(ChoreoEngine PUBLIC imgui glfw glm OpenGL::GL ${CMAKE_DL_LIBS} ${ASSIMP_LIBRARIES} spdlog::spdlog PRIVATE glad)
target_compile_options(ChoreoEngine PRIVATE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${gcc_flags}>>" "$<${msvc_cxx}: $<BUILD_INTERFACE:${msvc_flags}>>"
    )

