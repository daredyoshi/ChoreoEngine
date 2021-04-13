#include "Application/Renderer/Buffer.h"
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

        // this will be handled in sandbox
        m_imGuiLayer = new ImGuiLayer();
        pushOverlay(m_imGuiLayer);

        // Vertex Array
        m_vertexArray.reset(VertexArray::create());


        float vertices[ 3 * 6] = {
            // positions                // colors
            -0.5f, -0.5f, 0.0f,         1.0f, 1.0f, 1.0f,
            0.5f, -0.5, 0.0f,           0.0f, 1.0f, 0.0f, 
            0.0f, 0.5f, 0.0f,           0.5f, 1.0f, 0.5f
        };

        std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::create(vertices,  sizeof(vertices)));

        // this is the order in which the data interweaves
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Color"}
        };
        vertexBuffer->setLayout(layout);

        // it's important to set the buffer AFTER the layout has been setj
        m_vertexArray->addVertexBuffer(vertexBuffer);

        // index buffer
        uint32_t indices[3] = { 0, 1, 2 };
        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

        m_vertexArray->setIndexBuffer(indexBuffer);



        // square test

        float squareVertices[ 3 * 6] = {
            // positions                
            -0.5f, -0.5f, 0.0f,         
            0.5f, -0.5, 0.0f,            
            0.5f, 0.5f, 0.0f,           
            -0.5f, 0.5f, 0.0f,           
        };
        m_SquareVA.reset(VertexArray::create());

        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset((VertexBuffer::create(squareVertices, sizeof(squareVertices))));
        
        squareVB->setLayout({
            { ShaderDataType::Float3, "a_Position" }
        });
        m_SquareVA->addVertexBuffer(squareVB);

        // index buffer
        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->setIndexBuffer(squareIB);


        std::string vertexSrc = R"(
#version 330 core

layout(location=0) in vec3 a_Position;
layout(location=1) in vec3 a_Color;

out vec3 v_color;

void main(){
    v_color = a_Color;
    
    gl_Position = vec4(a_Position + 0.2, 1.0); 
}
        )";

        std::string fragmentSrc= R"(
#version 330 core

layout(location=0) out vec4 color;
in vec3 v_color;

void main(){

    color = vec4(v_color, 1.0);
}
        )";

        m_shader.reset(new Shader(vertexSrc, fragmentSrc));

        std::string vertexSrc2 = R"(
#version 330 core

layout(location=0) in vec3 a_Position;


void main(){
    gl_Position = vec4(a_Position, 1.0); 
}
        )";

        std::string fragmentSrc2= R"(
#version 330 core

layout(location=0) out vec4 color;

void main(){

    color = vec4(1.0, 1.0, 0.1, 1.0);
}
        )";

        m_SquareShader.reset(new Shader(vertexSrc2, fragmentSrc2));
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

            m_SquareShader->Bind();
            m_SquareVA->bind();

            glDrawElements(GL_TRIANGLES, m_SquareVA->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

            m_shader->Bind();
            m_vertexArray->bind();
            glDrawElements(GL_TRIANGLES, m_vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);



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
