
#include <iostream>
#include "Application/Renderer/Shader.h"
#include "ChoreoEngine.h"
#include "glm/fwd.hpp"
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

        // square test
        float squareVertices[ 5 * 6] = {
            // positions                
            -0.5f,  -0.5f,   0.0f,   0.0f, 0.0f,      
             0.5f,  -0.5f,   0.0f,   1.0f, 0.0f,         
             0.5f,   0.5f,   0.0f,   1.0f, 1.0f,        
            -0.5f,   0.5f,   0.0f,   0.0f, 1.0f        
        };
        m_SquareVA = ChoreoEngine::VertexArray::create();

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

        const std::string shaderSrcPath{ChoreoEngine::Application::get().getRootDir() + "assets/shaders/Texture.glsl"} ;
        CE_INFO("Shader Source Path = {0}",  shaderSrcPath);
        ChoreoEngine::Ref<ChoreoEngine::Shader> shader = m_shaderLibrary.load("MyShader", shaderSrcPath);

        CE_CORE_INFO("Cwd = {0}", ChoreoEngine::Application::get().getRootDir());
        m_texture = ChoreoEngine::Texture2D::create(ChoreoEngine::Application::get().getRootDir() + "assets/textures/ghoul.jpg");
        m_choreoLogo= ChoreoEngine::Texture2D::create(ChoreoEngine::Application::get().getRootDir() + "assets/textures/graphic.png");

        std::dynamic_pointer_cast<ChoreoEngine::OpenGLShader>(shader)->bind();
        std::dynamic_pointer_cast<ChoreoEngine::OpenGLShader>(shader)->uploadUniformInt("u_texture", 0);
        // std::dynamic_pointer_cast<ChoreoEngine::OpenGLShader>(m_shader)->uploadUniformFloat3("u_Color", m_squareCol);
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
        ChoreoEngine::Ref<ChoreoEngine::Shader> shader = m_shaderLibrary.get("MyShader");
        std::dynamic_pointer_cast<ChoreoEngine::OpenGLShader>(shader)->bind();
        std::dynamic_pointer_cast<ChoreoEngine::OpenGLTexture2D>(m_texture)->bind(0);

        for (int y{0}; y<20; y++){
            for (int x{0}; x<20; x++){

            }
        }
        std::dynamic_pointer_cast<ChoreoEngine::OpenGLShader>(shader)->uploadUniformFloat3("u_Color", m_squareCol);

        glm::mat4 xform = glm::translate(glm::mat4{1}, m_squarePos );
        ChoreoEngine::Renderer::submit(shader, m_SquareVA, xform);
        std::dynamic_pointer_cast<ChoreoEngine::OpenGLTexture2D>(m_choreoLogo)->bind(0);
        ChoreoEngine::Renderer::submit(shader, m_SquareVA, xform);
        ChoreoEngine::Renderer::endScene();
    }
    
    virtual void onImGuiRender() override {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_squareCol));
        ImGui::End();
    }


private:
    ChoreoEngine::Ref<ChoreoEngine::VertexArray> m_vertexArray;
    ChoreoEngine::ShaderLibrary m_shaderLibrary;

    ChoreoEngine::Ref<ChoreoEngine::VertexArray> m_SquareVA;

    ChoreoEngine::OrthographicCamera m_cam;
    glm::vec3 m_camPos{0};
    float m_camSpeed{1.0};

    glm::vec3 m_squarePos{0};
    glm::vec3 m_squareCol{0.8, 0.2, 0.2};

    ChoreoEngine::Ref<ChoreoEngine::Texture2D> m_texture;
    ChoreoEngine::Ref<ChoreoEngine::Texture2D> m_choreoLogo;
};

class Sandbox : public ChoreoEngine::Application{
public:
    Sandbox(const std::string& rootDir, const std::string& name) : Application(rootDir, name){
        pushLayer( new ExampleLayer() );
        pushOverlay(new ChoreoEngine::ImGuiLayer());
    }

    ~Sandbox(){

    }
};

// overwrite the application creator to create sandbox
// entry point is in Application/EntryPoint.h
ChoreoEngine::Application* ChoreoEngine::CreateApplication(const std::string& rootDir){
    return new Sandbox(rootDir, "ChoreoGrapher");
}
