#pragma once

#include "Application/Core.h"
#include "Application/Events/Application.h"
#include "Application/LayerStack.h"
#include "Application/ImGui/ImGuiLayer.h"
#include "Application/Window.h"
#include <memory>

#include "Application/Renderer/Shader.h"
#include "Application/Renderer/Buffer.h"
#include "Application/Renderer/VertexArray.h"


namespace ChoreoApp{
    class Application {
    public:
        Application(const std::string& rootDir, const std::string& name);
        virtual ~Application();
        void run();
        void onEvent(Event& e);

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* layer);

        inline static Application& get() { return *s_instance; }
        inline const std::string& getRootDir() const { return m_cwd; }
        inline Window& getWindow() const { return *m_window; } 

    private:
        const std::string& m_cwd;
        bool onWindowClose(WindowCloseEvent& e);
        bool onWindowResize(WindowResizeEvent& e);
        Scope<Window> m_window;
        ImGuiLayer* m_imGuiLayer;
        bool m_running{true};
        bool m_minimized{false};
        LayerStack m_layerStack;
        float m_lastFrameTime;


        static Application* s_instance;
    };

    // To be defined in CLIENT 
    Application* CreateApplication(const std::string& rootDir);
}
