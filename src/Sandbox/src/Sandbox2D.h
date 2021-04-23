# pragma once
#include "Application/Renderer/SubTexture2D.h"
#include "Application/Timestep.h"
#include "ParticleSystem.h"
#include "ChoreoApp.h"
#include <unordered_map>


class Sandbox2D : public ChoreoApp::Layer{
public:
    Sandbox2D()
        : 
         Layer("Sandbox2D"),
         m_camController{1280.0f / 780.0f, true},
         m_squarePos{0}
         {};
    virtual ~Sandbox2D() {};
    void onUpdate(ChoreoApp::TimeStep& timestep) override;
    void onImGuiRender() override;
    void onEvent(ChoreoApp::Event& e) override;

    void onAttach() override;
    void onDetach() override;

private:
    ChoreoApp::OrthographicCameraController m_camController;
    ChoreoApp::Ref<ChoreoApp::Texture2D> m_texture{ChoreoApp::Texture2D::create(ChoreoApp::Application::get().getRootDir() + "assets/textures/ghoul.jpg")};
    ChoreoApp::Ref<ChoreoApp::Texture2D> m_logoTexture{ChoreoApp::Texture2D::create(ChoreoApp::Application::get().getRootDir() + "assets/textures/graphic.png")};
    ChoreoApp::Ref<ChoreoApp::Texture2D> m_spriteSheet{ChoreoApp::Texture2D::create(ChoreoApp::Application::get().getRootDir() + "assets/textures/gothicvania/old-dark-castle-interior-tileset.png")};
    ChoreoApp::Ref<ChoreoApp::SubTexture2D> m_brick{ChoreoApp::SubTexture2D::createFromCoords(m_spriteSheet, glm::vec2{3.0f, 3.0f}, glm::vec2{1.0f, 1.0f}, 8)};
    ChoreoApp::Ref<ChoreoApp::SubTexture2D> m_door{ChoreoApp::SubTexture2D::createFromCoords(m_spriteSheet, glm::vec2{41.0f, 8.0f}, glm::vec2{3.0f, 4.0f}, 8)};
    glm::vec3 m_squarePos{0};
    glm::vec4 m_squareCol{0.8, 0.2, 0.2, 1.0};
    glm::vec2 m_squareScale{1.0f, 1.0f};
    float m_squareAngle{0};

    struct ProfileResult{
        const char* name;
        float time;
    };

    std::vector<ProfileResult> m_profileResults;
	ParticleProps m_Particle;
	ParticleSystem m_ParticleSystem;

    std::unordered_map<char, ChoreoApp::Ref<ChoreoApp::SubTexture2D>> s_textureMap;
    uint32_t m_mapWidth, m_mapHeight;
};
