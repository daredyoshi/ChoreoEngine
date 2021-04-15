#include "Application/Renderer/Buffer.h"
#include "cepch.h"
#include "Application/Application.h"
#include "Application/Renderer/Renderer.h"
#include "Application/Input.h"
#include <memory>
#include "Renderer/Renderer.h"

// temporary
#include <GLFW/glfw3.h> 


namespace ChoreoEngine {


    Application* Application::s_instance = nullptr;


    Application::Application(const std::string& cwd, const std::string& name)
        : m_cwd{cwd} {
        CE_CORE_ASSERT(!s_instance, "Application already exists!");
        s_instance = this;

        m_window = Scope<Window>{Window::create(WindowProps(name))};
        m_window->setEventCallback(CE_BIND_EVENT_FN(Application::onEvent));

        Renderer::init();

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
        // debug all events
        // CE_CORE_INFO("{0}", e);         

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

            // take this through and api layer
            float time = (float)glfwGetTime();
            TimeStep timestep {time - m_lastFrameTime};
            m_lastFrameTime = time;

            // draw layers from begin (lowest) to end (highest/ui)
            for (Layer* layer : m_layerStack)
                layer->onUpdate(timestep);

            // this will eventuall be executed no the render thread
            m_imGuiLayer->begin();
            // draw layers from begin (lowest) to end (highest/ui)
            for (Layer* layer : m_layerStack)
                layer->onImGuiRender();
            m_imGuiLayer->end();

            // auto[x, y] = Input::getMousePosition();
            float deltaTime{0};

            m_window->onUpdate(); 
        };
    }

    bool Application::onWindowClose(WindowCloseEvent& e){
        (void)e;
        m_running = false;
        return true;
    }
    }
