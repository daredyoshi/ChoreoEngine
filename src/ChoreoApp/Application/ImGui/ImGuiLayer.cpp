#include "Application/ImGui/ImGuiLayer.h"

#include "Application/ImGui/ImGuiLayer.h"
#include "Application/Application.h"
#include "Application/KeyCodes.h"
#include "Application/Events/MouseEvent.h"
#include "Application/Events/KeyEvent.h"
#include "Application/Log.h"
// The macro is defined in the cmake
// #define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui.h>
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

// TEMPORARY until OpenGL layer
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace ChoreoApp {

    ImGuiLayer::ImGuiLayer(){}
    ImGuiLayer::~ImGuiLayer(){}

    void ImGuiLayer::onAttach(){
        CE_PROFILE_FUNCTION();  
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        
        Application& app = Application::get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        
    }

    void ImGuiLayer::onDetach(){
        CE_PROFILE_FUNCTION();  
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

    }

    void ImGuiLayer::onEvent(Event& e){
        if(m_consumeImGuiEvents){
            ImGuiIO& io= ImGui::GetIO();
            if(e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse)
                e.setHandled(true);
            if(e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard)
                e.setHandled(true);
        }

    }

    void ImGuiLayer::begin(){
        CE_PROFILE_FUNCTION();  
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::end(){
        CE_PROFILE_FUNCTION();  
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::get();
        io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

        // Rendering 
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

}
