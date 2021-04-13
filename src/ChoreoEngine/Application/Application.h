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


namespace ChoreoEngine{
    class Application {
    public:
        Application(const std::string& name);
        virtual ~Application();
        void run();
        void onEvent(Event& e);

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* layer);

        inline static Application& get() { return *s_instance; }
        inline Window& getWindow() const { return *m_window; } 

    private:
        bool onWindowClose(WindowCloseEvent& e);
        std::unique_ptr<Window> m_window;
        ImGuiLayer* m_imGuiLayer;
        bool m_running{true};
        LayerStack m_layerStack;


        static Application* s_instance;
    };

    // To be defined in CLIENT 
    Application* CreateApplication();
}
