#include "Sandbox2D.h"
#include "Application/Log.h"
#include "Application/Renderer/Renderer.h"
#include "Application/Renderer/SubTexture2D.h"
#include "ChoreoApp.h"
#include "glm/ext/matrix_transform.hpp"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"



void Sandbox2D::onAttach() {

}
void Sandbox2D::onDetach() {

}


void Sandbox2D::onUpdate(ChoreoApp::TimeStep& timestep) {
    CE_PROFILE_FUNCTION();
    ChoreoApp::Renderer2D::resetStats();
    {
        CE_PROFILE_SCOPE("CamController::onUpdate");
        m_camController.onUpdate(timestep);

    }
    {
        CE_PROFILE_SCOPE("Draw Scene");

        ChoreoApp::RenderCommand::setClearColor(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
        ChoreoApp::RenderCommand::clear();



        ChoreoApp::Renderer2D::beginScene(m_camController.getCamera());
        ChoreoApp::Renderer2D::drawQuad(m_squarePos, glm::radians(m_squareAngle), m_squareScale, ChoreoApp::CreateRef<ChoreoApp::SubTexture2D>( m_logoTexture, glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 1.0f} ), m_squareCol);
        ChoreoApp::Renderer2D::endScene();

    }
    if (ChoreoApp::Input::isMouseButtonPressed(CE_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = ChoreoApp::Input::getMousePosition();
		auto width = ChoreoApp::Application::get().getWindow().getWidth();
		auto height = ChoreoApp::Application::get().getWindow().getHeight();

		auto bounds = m_camController.getBounds();
		auto pos = m_camController.getCamera().getPosition();
		x = (x / width) * bounds.getWidth() - bounds.getWidth() * 0.5f;
		y = bounds.getHeight() * 0.5f - (y / height) * bounds.getHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(timestep);
	m_ParticleSystem.OnRender(m_camController.getCamera());
}



void Sandbox2D::onEvent(ChoreoApp::Event& e) 
{
    m_camController.onEvent(e);
    if (e.getEventType() == ChoreoApp::EventType::WindowResize){
            auto& re = (ChoreoApp::WindowResizeEvent&) e;
    }
}

void Sandbox2D::onImGuiRender() 
{
        CE_PROFILE_FUNCTION();
        ImGui::Begin("ChoreoApp::Settings");
        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareCol));
        ImGui::DragFloat3("Square Pos", glm::value_ptr(m_squarePos), 0.001f);
        ImGui::DragFloat2("Square Scale", glm::value_ptr(m_squareScale), 0.001f);
        ImGui::DragFloat("Square Angle", &m_squareAngle, 2.0);

        auto stats = ChoreoApp::Renderer2D::getStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices: %d", stats.getTotalIndexCount());

        ImGui::End();
        ImGui::ShowDemoWindow();
}

