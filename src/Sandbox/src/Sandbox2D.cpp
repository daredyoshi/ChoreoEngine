#include "Sandbox2D.h"
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
    {
        CE_PROFILE_SCOPE("CamController::onUpdate");
        m_camController.onUpdate(timestep);

    }
    {
        CE_PROFILE_SCOPE("Draw Scene");
        ChoreoEngine::RenderCommand::setClearColor(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
        ChoreoEngine::RenderCommand::clear();
        ChoreoEngine::Renderer2D::beginScene(m_camController.getCamera());
        ChoreoEngine::Renderer2D::drawQuad(m_squarePos, m_squareAngle, m_squareScale, m_squareCol);
        ChoreoEngine::Renderer2D::drawQuad(m_squarePos + glm::vec3{0.2, 0.2, 0.0}, m_squareAngle, m_squareScale, m_texture, m_squareCol);
        ChoreoEngine::Renderer2D::endScene();
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


        ImGui::End();
        // ImGui::ShowDemoWindow();
}

