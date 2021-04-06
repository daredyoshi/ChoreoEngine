#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/Application.h"
#include "Window.h"
#include <memory>

namespace ChoreoEngine{
    class Application {
    public:
        Application(const std::string& name);
        virtual ~Application();
        void run();
        void onEvent(Event& e);
    private:
        std::unique_ptr<Window> m_window;
        bool m_running{true};
    };

    // To be defined in CLIENT 
    Application* CreateApplication();
}
