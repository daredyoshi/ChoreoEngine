
#include <iostream>
#include "ChoreoApp.h"
#include "Application/EntryPoint.h"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "src/Sandbox2D.h"

class ExampleLayer : public ChoreoApp::Layer{
public:
    ExampleLayer()
        : 
         Layer("Example") ,
         m_camController{1280.0f / 780.0f, true},
         m_squarePos{0}
         {}

    virtual void onAttach() override {

        // square test
        float squareVertices[ 5 * 6] = {
            // positions                
            -0.5f,  -0.5f,   0.0f,   0.0f, 0.0f,      
             0.5f,  -0.5f,   0.0f,   1.0f, 0.0f,         
             0.5f,   0.5f,   0.0f,   1.0f, 1.0f,        
            -0.5f,   0.5f,   0.0f,   0.0f, 1.0f        
        };
        m_SquareVA = ChoreoApp::VertexArray::create();

        ChoreoApp::Ref<ChoreoApp::VertexBuffer> squareVB;
        squareVB = ChoreoApp::VertexBuffer::create(squareVertices, sizeof(squareVertices));
        
        squareVB->setLayout({
            { ChoreoApp::ShaderDataType::Float3, "a_Position" },
            { ChoreoApp::ShaderDataType::Float2, "a_Uv" }
        });
        m_SquareVA->addVertexBuffer(squareVB);

        // index buffer
        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        ChoreoApp::Ref<ChoreoApp::IndexBuffer> squareIB;
        squareIB = ChoreoApp::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        m_SquareVA->setIndexBuffer(squareIB);

        const std::string shaderSrcPath{ChoreoApp::Application::get().getRootDir() + "assets/shaders/Texture.glsl"} ;
        CE_INFO("Shader Source Path = {0}",  shaderSrcPath);
        ChoreoApp::Ref<ChoreoApp::Shader> shader = m_shaderLibrary.load("MyShader", shaderSrcPath);

        CE_CORE_INFO("Cwd = {0}", ChoreoApp::Application::get().getRootDir());
        m_texture = ChoreoApp::Texture2D::create(ChoreoApp::Application::get().getRootDir() + "assets/textures/ghoul.jpg");
        m_choreoLogo= ChoreoApp::Texture2D::create(ChoreoApp::Application::get().getRootDir() + "assets/textures/graphic.png");

        std::dynamic_pointer_cast<ChoreoApp::OpenGLShader>(shader)->bind();
        std::dynamic_pointer_cast<ChoreoApp::OpenGLShader>(shader)->uploadUniformInt("u_texture", 0);
        // std::dynamic_pointer_cast<ChoreoApp::OpenGLShader>(m_shader)->uploadUniformFloat3("u_Color", m_squareCol);
    }

    virtual void onUpdate(ChoreoApp::TimeStep& timestep) override {
        ChoreoApp::RenderCommand::setClearColor(glm::vec4{ 0.1f, 0.1f, 0.1f, 1 });
        ChoreoApp::RenderCommand::clear();


        m_camController.onUpdate(timestep);
        ChoreoApp::Renderer::beginScene(m_camController.getCamera());

                // this would go on a seperate thread at some point
        // this will go into materials that will go into meshes
        ChoreoApp::Ref<ChoreoApp::Shader> shader = m_shaderLibrary.get("MyShader");
        std::dynamic_pointer_cast<ChoreoApp::OpenGLShader>(shader)->bind();
        std::dynamic_pointer_cast<ChoreoApp::OpenGLTexture2D>(m_texture)->bind(0);

        for (int y{0}; y<20; y++){
            for (int x{0}; x<20; x++){

            }
        }
        std::dynamic_pointer_cast<ChoreoApp::OpenGLShader>(shader)->uploadUniformFloat3("u_Color", m_squareCol);

        glm::mat4 xform = glm::translate(glm::mat4{1}, m_squarePos );
        ChoreoApp::Renderer::submit(shader, m_SquareVA, xform);
        std::dynamic_pointer_cast<ChoreoApp::OpenGLTexture2D>(m_choreoLogo)->bind(0);
        ChoreoApp::Renderer::submit(shader, m_SquareVA, xform);
        ChoreoApp::Renderer::endScene();
    }

    virtual void onEvent(ChoreoApp::Event& e) override{
        m_camController.onEvent(e);

        if (e.getEventType() == ChoreoApp::EventType::WindowResize){
                auto& re = (ChoreoApp::WindowResizeEvent&) e;
        }
    }
    
    virtual void onImGuiRender() override {
        ImGui::Begin("ChoreoApp::Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_squareCol));
        ImGui::DragFloat3("Square Pos", glm::value_ptr(m_squarePos));
        ImGui::End();
        ImGui::ShowDemoWindow();
    }


private:
    ChoreoApp::ShaderLibrary m_shaderLibrary;

    ChoreoApp::Ref<ChoreoApp::VertexArray> m_SquareVA;

    ChoreoApp::OrthographicCameraController m_camController;

    glm::vec3 m_squarePos{0};
    glm::vec3 m_squareCol{0.8, 0.2, 0.2};

    ChoreoApp::Ref<ChoreoApp::Texture2D> m_texture;
    ChoreoApp::Ref<ChoreoApp::Texture2D> m_choreoLogo;
};

class Sandbox : public ChoreoApp::Application{
public:
    Sandbox(const std::string& rootDir, const std::string& name) : Application(rootDir, name){
        // pushLayer( new ExampleLayer() );
        pushLayer(new Sandbox2D());
        pushOverlay(new ChoreoApp::ImGuiLayer());
    }

    ~Sandbox(){

    }
};

// overwrite the application creator to create sandbox
// entry point is in Application/EntryPoint.h

ChoreoApp::Application* ChoreoApp::CreateApplication(const std::string& rootDir){
    return new Sandbox(rootDir, "ChoreoGrapher");
}
