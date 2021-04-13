#include "cepch.h"
#include "Application/Application.h"
#include "Application/Input.h"
#include "glad/glad.h"
#include <memory>


namespace ChoreoEngine {


    Application* Application::s_instance = nullptr;

    Application::Application(const std::string& name){
        CE_CORE_ASSERT(!s_instance, "Application already exists!");
        s_instance = this;

        m_window = std::unique_ptr<Window>{Window::create(WindowProps(name))};
        m_window->setEventCallback(CE_BIND_EVENT_FN(Application::onEvent));

        m_imGuiLayer = new ImGuiLayer();
        pushOverlay(m_imGuiLayer);


        // Vertex Array
        glGenVertexArrays(1, &m_vertexArray);
        glBindVertexArray(m_vertexArray);


        float vertices[ 3 * 3 ] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5, 0.0f,
            0.0f, 0.5f, 0.0f,
        };

        m_vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices) / sizeof(float)));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);


        // index buffer
        uint32_t indices[3] = { 0, 1, 2 };
        m_indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));


        std::string vertexSrc = R"(
#version 330 core

layout(location=0) in vec3 a_Position;

out vec3 v_position;

void main(){
    v_position = a_Position;
    gl_Position = vec4(a_Position - 0.2, 1.0); 
}
        )";

        std::string fragmentSrc= R"(
#version 330 core

layout(location=0) out vec4 color;
in vec3 v_position;

void main(){

    color = vec4(v_position * 0.5 + 0.5, 1.0);
}
        )";

        m_shader.reset(new Shader(vertexSrc, fragmentSrc));
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
            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_shader->Bind();
            glBindVertexArray(m_vertexArray);
            glDrawElements(GL_TRIANGLES, m_indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

            // draw layers from begin (lowest) to end (highest/ui)
            for (Layer* layer : m_layerStack)
                layer->onUpdate();

            // this will eventuall be executed no the render thread
            m_imGuiLayer->begin();
            // draw layers from begin (lowest) to end (highest/ui)
            for (Layer* layer : m_layerStack)
                layer->onImGuiRender();
            m_imGuiLayer->end();

            // auto[x, y] = Input::getMousePosition();


            m_window->onUpdate(); 
        };
    }

    bool Application::onWindowClose(WindowCloseEvent& e){
        (void)e;
        m_running = false;
        return true;
    }
}
