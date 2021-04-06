#include "cepch.h"
#include "Application.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"


namespace ChoreoEngine {

#define CE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

    Application::Application(const std::string& name){
        m_window = std::unique_ptr<Window>{Window::create(WindowProps(name))};
        m_window->setEventCallback(CE_BIND_EVENT_FN(Application::onEvent));
    }
    Application::~Application(){

    }

    void Application::pushLayer(Layer* layer){
        m_layerStack.pushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer){
        m_layerStack.pushOverlay(layer);
    }

    void Application::onEvent(Event& e){
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(CE_BIND_EVENT_FN(Application::onWindowClose));
        CE_CORE_INFO("{0}", e);         

        // work backwards through the layer stack until event is handled
        // begin is lowest level, end is highest (ui) level
        for (auto it = m_layerStack.end(); it != m_layerStack.begin();){
            (*--it)->onEvent(e);
            if(e.getHandled())
                break;
        }
         
    }
    void Application::run(){
        while(m_running){
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            // draw layers from begin (lowest) to end (highest/ui)
            for (Layer* layer : m_layerStack)
                layer->onUpdate();

            m_window->onUpdate(); 
        };
    }

    bool Application::onWindowClose(WindowCloseEvent& e){
        (void)e;
        m_running = false;
        return true;
    }
}
