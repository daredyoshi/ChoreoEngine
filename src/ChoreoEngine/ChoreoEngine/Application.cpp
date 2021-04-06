#include "cepch.h"
#include "Application.h"
#include "Events/Event.h"
#include "GLFW/glfw3.h"


namespace ChoreoEngine {

#define CE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

    Application::Application(const std::string& name){
        m_window = std::unique_ptr<Window>{Window::create(WindowProps(name))};
        m_window->setEventCallback(CE_BIND_EVENT_FN(Application::onEvent));
    }
    Application::~Application(){

    }

    void Application::onEvent(Event& e){
        CE_CORE_INFO("{0}", e);         
    }
    void Application::run(){
        while(m_running){
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            m_window->onUpdate(); 
        };
    }
}
