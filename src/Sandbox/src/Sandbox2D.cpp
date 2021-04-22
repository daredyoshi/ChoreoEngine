#include "Sandbox2D.h"
#include "Application/Renderer/Renderer.h"
#include "ChoreoEngine.h"
#include "glm/ext/matrix_transform.hpp"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"



void Sandbox2D::onAttach() {
}
void Sandbox2D::onDetach() {

}


void Sandbox2D::onUpdate(ChoreoEngine::TimeStep& timestep) {
    CE_PROFILE_FUNCTION();
    ChoreoEngine::Renderer2D::resetStats();
    {
        CE_PROFILE_SCOPE("CamController::onUpdate");
        m_camController.onUpdate(timestep);

    }
    {
        CE_PROFILE_SCOPE("Draw Scene");

        ChoreoEngine::RenderCommand::setClearColor(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
        ChoreoEngine::RenderCommand::clear();

        ChoreoEngine::Renderer2D::beginScene(m_camController.getCamera());
        for(float y{-5.0f}; y < 5.0f; y +=0.1){
            for(float x{-5.0f}; x < 5.0f; x +=0.1){
                glm::vec4 color= { ( x + 5.0f  )/ 10.0f, ( y + 5.0  )/ 10.0f, 0.5f, 0.5f };
                ChoreoEngine::Renderer2D::drawQuad({x, y}, m_squareAngle, { 0.045f, 0.045f}  , color);
            }        
                    
        }
        ChoreoEngine::Renderer2D::endScene();


        ChoreoEngine::Renderer2D::beginScene(m_camController.getCamera());
        ChoreoEngine::Renderer2D::drawQuad({0.0, 0.8, -0.1}, m_squareAngle, m_squareScale, {0.0, 0.1, 0.7, 1.0});
        ChoreoEngine::Renderer2D::drawQuad(m_squarePos, m_squareAngle, m_squareScale, m_squareCol);
        ChoreoEngine::Renderer2D::drawQuad(m_squarePos + glm::vec3{0.2, 0.2, 0.0}, m_squareAngle, m_squareScale, m_logoTexture, m_squareCol);
        ChoreoEngine::Renderer2D::drawQuad(m_squarePos + glm::vec3{-0.2, 0.2, 0.0}, m_squareAngle, m_squareScale, m_texture);
        ChoreoEngine::Renderer2D::endScene();

        m_squareAngle += 0.5;


    }

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
        CE_PROFILE_FUNCTION();
        ImGui::Begin("ChoreoEngine::Settings");
        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareCol));
        ImGui::DragFloat3("Square Pos", glm::value_ptr(m_squarePos), 0.001f);
        ImGui::DragFloat2("Square Scale", glm::value_ptr(m_squareScale), 0.001f);
        ImGui::DragFloat("Square Angle", &m_squareAngle, 2.0);

        auto stats = ChoreoEngine::Renderer2D::getStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices: %d", stats.getTotalIndexCount());

        ImGui::End();
        // ImGui::ShowDemoWindow();
}

