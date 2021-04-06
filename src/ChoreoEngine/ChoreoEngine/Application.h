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
        void PushOverlay(Layer* layer);

        bool onWindowClose(WindowCloseEvent& e);
    private:
        std::unique_ptr<Window> m_window;
        bool m_running{true};
        LayerStack m_layerStack;
    };

    // To be defined in CLIENT 
    Application* CreateApplication();
}
