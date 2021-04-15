
#include <iostream>
#include "ChoreoEngine.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public ChoreoEngine::Layer{
public:
    ExampleLayer()
        : 
         Layer("Example") ,
         m_cam{ChoreoEngine::OrthographicCamera{-1.0f, 1.0f, -1.0f, 1.0f}},
         m_squarePos{0}
         {}

    virtual void onAttach() override {
        // Vertex Array
        m_vertexArray.reset(ChoreoEngine::VertexArray::create());




        // square test

        float squareVertices[ 5 * 6] = {
            // positions                
            -0.5f,  -0.5f,   0.0f,   0.0f, 0.0f,      
             0.5f,  -0.5f,   0.0f,   1.0f, 0.0f,         
             0.5f,   0.5f,   0.0f,   1.0f, 1.0f,        
            -0.5f,   0.5f,   0.0f,   0.0f, 1.0f        
        };
        m_SquareVA.reset(ChoreoEngine::VertexArray::create());

        ChoreoEngine::Ref<ChoreoEngine::VertexBuffer> squareVB;
        squareVB.reset((ChoreoEngine::VertexBuffer::create(squareVertices, sizeof(squareVertices))));
        
        squareVB->setLayout({
            { ChoreoEngine::ShaderDataType::Float3, "a_Position" },
            { ChoreoEngine::ShaderDataType::Float2, "a_Uv" }
        });
        m_SquareVA->addVertexBuffer(squareVB);

        // index buffer
        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        ChoreoEngine::Ref<ChoreoEngine::IndexBuffer> squareIB;
        squareIB.reset(ChoreoEngine::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->setIndexBuffer(squareIB);


        std::string vertexSrc = R"(
#version 330 core

layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_Uv;

uniform mat4 u_viewProjection;
uniform mat4 u_xform;

out vec2 v_Uv;


void main(){
    v_Uv = a_Uv;
    gl_Position = u_viewProjection * u_xform * vec4(a_Position , 1.0); 
}
        )";

        std::string fragmentSrc= R"(
#version 330 core

layout(location=0) out vec4 color;
uniform vec3 u_Color;

in vec2 v_Uv;

void main(){

    color = vec4(v_Uv, 0.0, 1.0);
}
        )";

        m_shader.reset(ChoreoEngine::Shader::create(vertexSrc, fragmentSrc));
        std::dynamic_pointer_cast<ChoreoEngine::OpenGLShader>(m_shader)->uploadUniformFloat3("u_Color", m_squareCol);

    }

    virtual void onUpdate(ChoreoEngine::TimeStep& timestep) override {
        ChoreoEngine::RenderCommand::setClearColor(glm::vec4{ 0.1f, 0.1f, 0.1f, 1 });
        ChoreoEngine::RenderCommand::clear();

        // CE_TRACE("Delta Time: {0}s ({1}ms)", timestep.getSeconds(), timestep.getMilliseconds());

        if(ChoreoEngine::Input::isKeyPressed(CE_KEY_LEFT)){
            m_camPos.x += m_camSpeed * timestep; 
        }
        if(ChoreoEngine::Input::isKeyPressed(CE_KEY_RIGHT)){
            m_camPos.x -= m_camSpeed* timestep; 
        }
        if(ChoreoEngine::Input::isKeyPressed(CE_KEY_DOWN)){
            m_camPos.y += m_camSpeed* timestep; 
        }
        if(ChoreoEngine::Input::isKeyPressed(CE_KEY_UP)){
            m_camPos.y -= m_camSpeed* timestep; 
        }
        
        if(ChoreoEngine::Input::isKeyPressed(CE_KEY_L)){
            m_squarePos.x += m_camSpeed * timestep; 
        }
        if(ChoreoEngine::Input::isKeyPressed(CE_KEY_H)){
            m_squarePos.x -= m_camSpeed* timestep; 
        }
        if(ChoreoEngine::Input::isKeyPressed(CE_KEY_K)){
            m_squarePos.y -= m_camSpeed* timestep; 
        }
        if(ChoreoEngine::Input::isKeyPressed(CE_KEY_J)){
            m_squarePos.y += m_camSpeed* timestep; 
        }
        if(ChoreoEngine::Input::isKeyPressed(CE_KEY_SPACE)){
            m_cam.setRotation(timestep* m_cam.getRotation() + m_camSpeed * 5.0); 
        }

        // test camera movement
        m_cam.setPosition(m_camPos);
        // m_cam.setRotation(45.0f);


        ChoreoEngine::Renderer::beginScene(m_cam);

        // Material concept
        // ChoreoEngine::MaterialRef material = new ChoreoEngine::Material(m_shader);
        // material->set("u_color", redColor);

        // this would go on a seperate thread at some point
        // this will go into materials that will go into meshes
        std::dynamic_pointer_cast<ChoreoEngine::OpenGLShader>(m_shader)->bind();

        for (int y{0}; y<20; y++){
            for (int x{0}; x<20; x++){

            }
        }
        std::dynamic_pointer_cast<ChoreoEngine::OpenGLShader>(m_shader)->uploadUniformFloat3("u_Color", m_squareCol);

        glm::mat4 xform = glm::translate(glm::mat4{1}, m_squarePos );
        ChoreoEngine::Renderer::submit(m_shader, m_SquareVA, xform);
        ChoreoEngine::Renderer::endScene();
    }
    
    virtual void onImGuiRender() override {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_squareCol));
        ImGui::End();
    }


private:
    ChoreoEngine::Ref<ChoreoEngine::VertexArray> m_vertexArray;
    ChoreoEngine::Ref<ChoreoEngine::Shader> m_shader;

    ChoreoEngine::Ref<ChoreoEngine::VertexArray> m_SquareVA;

    ChoreoEngine::OrthographicCamera m_cam;
    glm::vec3 m_camPos{0};
    float m_camSpeed{1.0};

    glm::vec3 m_squarePos{0};
    glm::vec3 m_squareCol{0.8, 0.2, 0.2};

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
