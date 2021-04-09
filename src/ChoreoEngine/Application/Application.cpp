#include "cepch.h"
#include "Application.h"
#include "glad/glad.h"
#include "Input.h"


namespace ChoreoEngine {


    Application* Application::s_instance = nullptr;

    Application::Application(const std::string& name){
        CE_CORE_ASSERT(s_instance, "Application already exists!");
        m_window = std::unique_ptr<Window>{Window::create(WindowProps(name))};
        m_window->setEventCallback(CE_BIND_EVENT_FN(Application::onEvent));

        m_imGuiLayer = new ImGuiLayer();

        s_instance = this;
    }
    Application::~Application(){

    }

    void Application::pushLayer(Layer* layer){
        m_layerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Layer* layer){
        m_layerStack.pushOverlay(layer);
        layer->onAttach();
    }

    void Application::onEvent(Event& e){
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(CE_BIND_EVENT_FN(Application::onWindowClose));
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

            // this will eventuall be executed no the render thread
            m_imGuiLayer->begin();
            // draw layers from begin (lowest) to end (highest/ui)
            for (Layer* layer : m_layerStack)
                layer->onImGuiRender();
            m_imGuiLayer->end();

            auto[x, y] = Input::getMousePosition();
            CE_CORE_TRACE("{0}, {1}", x, y);


            m_window->onUpdate(); 
        };
    }

    bool Application::onWindowClose(WindowCloseEvent& e){
        (void)e;
        m_running = false;
        return true;
    }
}
