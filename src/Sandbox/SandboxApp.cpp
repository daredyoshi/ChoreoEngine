
#include <iostream>
#include "ChoreoEngine.h"
#include "imgui.h"
#include <glm/glm.hpp>

class ExampleLayer : public ChoreoEngine::Layer{
public:
    ExampleLayer()
        : Layer("Example"){}

    virtual void onAttach() override {

        // Vertex Array
        m_vertexArray.reset(ChoreoEngine::VertexArray::create());


        float vertices[ 3 * 6] = {
            // positions                // colors
            -0.5f, -0.5f, 0.0f,         1.0f, 1.0f, 1.0f,
            0.5f, -0.5, 0.0f,           0.0f, 1.0f, 0.0f, 
            0.0f, 0.5f, 0.0f,           0.5f, 1.0f, 0.5f
        };

        std::shared_ptr<ChoreoEngine::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(ChoreoEngine::VertexBuffer::create(vertices,  sizeof(vertices)));

        // this is the order in which the data interweaves
        ChoreoEngine::BufferLayout layout = {
            { ChoreoEngine::ShaderDataType::Float3, "a_Position" },
            { ChoreoEngine::ShaderDataType::Float3, "a_Color"}
        };
        vertexBuffer->setLayout(layout);

        // it's important to set the buffer AFTER the layout has been setj
        m_vertexArray->addVertexBuffer(vertexBuffer);

        // index buffer
        uint32_t indices[3] = { 0, 1, 2 };
        std::shared_ptr<ChoreoEngine::IndexBuffer> indexBuffer;
        indexBuffer.reset(ChoreoEngine::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

        m_vertexArray->setIndexBuffer(indexBuffer);



        // square test

        float squareVertices[ 3 * 6] = {
            // positions                
            -0.5f, -0.5f, 0.0f,         
            0.5f, -0.5, 0.0f,            
            0.5f, 0.5f, 0.0f,           
            -0.5f, 0.5f, 0.0f,           
        };
        m_SquareVA.reset(ChoreoEngine::VertexArray::create());

        std::shared_ptr<ChoreoEngine::VertexBuffer> squareVB;
        squareVB.reset((ChoreoEngine::VertexBuffer::create(squareVertices, sizeof(squareVertices))));
        
        squareVB->setLayout({
            { ChoreoEngine::ShaderDataType::Float3, "a_Position" }
        });
        m_SquareVA->addVertexBuffer(squareVB);

        // index buffer
        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<ChoreoEngine::IndexBuffer> squareIB;
        squareIB.reset(ChoreoEngine::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

        m_shader.reset(new ChoreoEngine::Shader(vertexSrc, fragmentSrc));

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

        m_SquareShader.reset(new ChoreoEngine::Shader(vertexSrc2, fragmentSrc2));
    }

    virtual void onUpdate() override {
        ChoreoEngine::RenderCommand::setClearColor(glm::vec4{ 0.1f, 0.1f, 0.1f, 1 });
        ChoreoEngine::RenderCommand::clear();

        ChoreoEngine::Renderer::beginScene();
        // this can be a mesh, or something else with overloaded fn

        m_SquareShader->Bind();
        // this would go on a seperate thread at some point
        // this will go into materials that will go into meshes
        ChoreoEngine::Renderer::submit(m_SquareVA);

        m_shader->Bind();
        ChoreoEngine::Renderer::submit(m_vertexArray);

        ChoreoEngine::Renderer::endScene();
        CE_INFO("test udpate");
    }
    
    virtual void onImGuiRender() override {
        ImGui::Begin("ChoreoGrapher::Test");
        ImGui::Text("HelloWorld");
        ImGui::End();
    }

private:
    std::shared_ptr<ChoreoEngine::VertexArray> m_vertexArray;
    std::shared_ptr<ChoreoEngine::Shader> m_shader;

    std::shared_ptr<ChoreoEngine::VertexArray> m_SquareVA;
    std::shared_ptr<ChoreoEngine::Shader> m_SquareShader;

};

class Sandbox : public ChoreoEngine::Application{
public:
    Sandbox(const std::string& name) : Application(name){
        pushLayer( new ExampleLayer() );
        pushOverlay(new ChoreoEngine::ImGuiLayer());
    }

    ~Sandbox(){

    }
};

// overwrite the application creator to create sandbox
// entry point is in Application/EntryPoint.h
ChoreoEngine::Application* ChoreoEngine::CreateApplication(){
    return new Sandbox("ChoreoGrapher");
}
