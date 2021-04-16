#include "Sandbox2D.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

void Sandbox2D::onAttach()
{
        // square test
        float squareVertices[ 5 * 6] = {
            // positions                
            -0.5f,  -0.5f,   0.0f,   
             0.5f,  -0.5f,   0.0f,   
             0.5f,   0.5f,   0.0f,   
            -0.5f,   0.5f,   0.0f,   
        };
        m_SquareVA = ChoreoEngine::VertexArray::create();

        ChoreoEngine::Ref<ChoreoEngine::VertexBuffer> squareVB;
        squareVB = ChoreoEngine::VertexBuffer::create(squareVertices, sizeof(squareVertices));
        
        squareVB->setLayout({
            { ChoreoEngine::ShaderDataType::Float3, "a_Position" },
        });
        m_SquareVA->addVertexBuffer(squareVB);

        // index buffer
        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        ChoreoEngine::Ref<ChoreoEngine::IndexBuffer> squareIB;
        squareIB = ChoreoEngine::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        m_SquareVA->setIndexBuffer(squareIB);

        const std::string shaderSrcPath{ChoreoEngine::Application::get().getRootDir() + "assets/shaders/FlatColor.glsl"} ;
        CE_INFO("Shader Source Path = {0}",  shaderSrcPath);
        ChoreoEngine::Ref<ChoreoEngine::Shader> shader = m_shaderLibrary.load("MyShader", shaderSrcPath);

        CE_CORE_INFO("Cwd = {0}", ChoreoEngine::Application::get().getRootDir());

        std::dynamic_pointer_cast<ChoreoEngine::OpenGLShader>(shader)->bind();
        std::dynamic_pointer_cast<ChoreoEngine::OpenGLShader>(shader)->uploadUniformFloat3("u_Color", m_squareCol);
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onEvent(ChoreoEngine::Event& e) 
{
    m_camController.onEvent(e);

    if (e.getEventType() == ChoreoEngine::EventType::WindowResize){
            auto& re = (ChoreoEngine::WindowResizeEvent&) e;
    }
}

void Sandbox2D::onImGuiRender() 
{
        ImGui::Begin("ChoreoEngine::Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_squareCol));
        ImGui::End();
        // ImGui::ShowDemoWindow();
}

void Sandbox2D::onUpdate(ChoreoEngine::TimeStep& timestep) 
{
        ChoreoEngine::RenderCommand::setClearColor(glm::vec4{ 0.1f, 0.1f, 0.1f, 1 });
        ChoreoEngine::RenderCommand::clear();
        m_camController.onUpdate(timestep);
        ChoreoEngine::Renderer::beginScene(m_camController.getCamera());

                // this would go on a seperate thread at some point
        // this will go into materials that will go into meshes
        ChoreoEngine::Ref<ChoreoEngine::Shader> shader = m_shaderLibrary.get("MyShader");
        std::dynamic_pointer_cast<ChoreoEngine::OpenGLShader>(shader)->bind();
        std::dynamic_pointer_cast<ChoreoEngine::OpenGLShader>(shader)->uploadUniformFloat3("u_Color", m_squareCol);

        glm::mat4 xform = glm::translate(glm::mat4{1}, m_squarePos );
        ChoreoEngine::Renderer::submit(shader, m_SquareVA, xform);
        ChoreoEngine::Renderer::endScene();
}

