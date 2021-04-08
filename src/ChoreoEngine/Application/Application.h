#pragma once

#include "Core.h"
#include "Events/Application.h"
#include "LayerStack.h"
#include "Window.h"
#include <memory>

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
        bool m_running{true};
        LayerStack m_layerStack;

        static Application* s_instance;
    };

    // To be defined in CLIENT 
    Application* CreateApplication();
}
