
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(spdlog REQUIRED)


include_directories(
    .
    ../dependencies/imgui/backends
    ../dependencies/stb
    ../dependencies/entt
    )


# Add the core lib
add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)

set(
    ChoreoAppSources 
    ../dependencies/imgui/backends/imgui_impl_opengl3.cpp
    ../dependencies/imgui/backends/imgui_impl_glfw.cpp
    ../dependencies/stb/stb_image.cpp

    Application/Application.cpp
    Application/Layer.cpp
    Application/LayerStack.cpp
    Application/CameraController.cpp

    Application/ImGui/ImGuiLayer.cpp

    Application/Platform/OpenGL/OpenGLContext.cpp
    Application/Platform/OpenGL/OpenGLBuffer.cpp
    Application/Platform/OpenGL/OpenGLFramebuffer.cpp
    Application/Platform/OpenGL/OpenGLVertexArray.cpp
    Application/Platform/OpenGL/OpenGLRendererAPI.cpp
    Application/Platform/OpenGL/OpenGLShader.cpp
    Application/Platform/OpenGL/OpenGLTexture.cpp

    Application/Renderer/GraphicsContext.cpp
    Application/Renderer/Renderer.cpp
    Application/Renderer/Buffer.cpp
    Application/Renderer/Framebuffer.cpp
    Application/Renderer/Shader.cpp
    Application/Renderer/Camera.cpp
    Application/Renderer/Texture.cpp
    Application/Renderer/SubTexture2D.cpp
    Application/Renderer/VertexArray.cpp
    Application/Renderer/RenderCommand.cpp
    Application/Renderer/RendererAPI.cpp

    Application/Scene/Scene.cpp
    Application/Scene/SceneCamera.cpp
    Application/Scene/Entity.cpp

    Application/Log.cpp
    

    # if on Linux
    Application/Platform/Linux/LinuxWindow.cpp
    Application/Platform/Linux/LinuxInput.cpp
)

add_library(
    ChoreoApp ${ChoreoAppSources}
)

target_precompile_headers(ChoreoApp 
    PRIVATE
    "capch.h"
    )
target_compile_features(ChoreoApp PUBLIC cxx_std_17)
target_link_libraries(ChoreoApp PUBLIC imgui glfw glm OpenGL::GL ${CMAKE_DL_LIBS} ${ASSIMP_LIBRARIES} spdlog::spdlog PRIVATE glad)
target_compile_options(ChoreoApp PRIVATE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${gcc_flags}>>" "$<${msvc_cxx}: $<BUILD_INTERFACE:${msvc_flags}>>"
    )

