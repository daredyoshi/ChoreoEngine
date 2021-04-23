#include "Sandbox2D.h"
#include "Application/Log.h"
#include "Application/Renderer/Renderer.h"
#include "ChoreoEngine.h"
#include "glm/ext/matrix_transform.hpp"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"


static const uint32_t s_mapWith = 23;
static const char* s_mapTiles = 
"00000000000____________"
"00000000000000000000000"
"00000000000000000000000"
"00000000000000000000000"
"0000000000000000D000000"
"_______________________"
"00000000000000000000000"
;

void Sandbox2D::onAttach() {
    // init particle system
    m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.1f, m_Particle.SizeVariation = 0.03f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

    m_camController.setZoomLevel(5.0f);

    m_mapWidth = s_mapWith;
    m_mapHeight = strlen(s_mapTiles) / m_mapWidth;

    s_textureMap['_'] = m_brick;
    s_textureMap['D'] = m_door;


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
        // ChoreoEngine::Renderer2D::drawQuad({0.0, 0.8, -0.1}, glm::radians(m_squareAngle), m_squareScale, {0.0, 0.1, 0.7, 1.0});
        // ChoreoEngine::Renderer2D::drawQuad(m_squarePos, glm::radians(m_squareAngle), m_squareScale, m_squareCol);

        
        // ChoreoEngine::Renderer2D::drawQuad(m_squarePos + glm::vec3{0.2, 0.2, 0.0}, glm::radians(m_squareAngle), m_squareScale, m_logoTexture, m_squareCol);
        // ChoreoEngine::Renderer2D::drawQuad(m_squarePos + glm::vec3{-0.2, 0.2, 0.0}, glm::radians(m_squareAngle), m_squareScale, m_brick);
        // ChoreoEngine::Renderer2D::drawQuad(m_squarePos + glm::vec3{0.2, -0.2, 0.0}, glm::radians(m_squareAngle), {1.5f, 2.0f}, m_door);

        for(uint32_t y{0}; y < m_mapHeight; ++y){
            for(uint32_t x{0}; x < m_mapWidth; ++x){
                char tileType = s_mapTiles[x + y*m_mapWidth];
                // door is an exra big texture
                if(tileType == 'D'){ 
                    ChoreoEngine::Renderer2D::drawQuad(glm::vec3{x + 2.0f, m_mapHeight - y + 1.5, 0.0}, glm::radians(0.0), {3.0f, 4.0f}, s_textureMap[tileType]);
                }
                // all 1x1 textures
                else if (tileType != '0'){
                    ChoreoEngine::Renderer2D::drawQuad(glm::vec3{x, m_mapHeight - y, 0.0}, glm::radians(0.0), {1.0f, 1.0f}, s_textureMap[tileType]);
                } 
                        
            }        
        }

        ChoreoEngine::Renderer2D::endScene();

        // m_squareAngle += 0.5;

    }
    if (ChoreoEngine::Input::isMouseButtonPressed(CE_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = ChoreoEngine::Input::getMousePosition();
		auto width = ChoreoEngine::Application::get().getWindow().getWidth();
		auto height = ChoreoEngine::Application::get().getWindow().getHeight();

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

