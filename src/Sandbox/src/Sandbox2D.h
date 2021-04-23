# pragma once
#include "Application/Renderer/SubTexture2D.h"
#include "Application/Timestep.h"
#include "ParticleSystem.h"
#include "ChoreoEngine.h"
#include <unordered_map>


class Sandbox2D : public ChoreoEngine::Layer{
public:
    Sandbox2D()
        : 
         Layer("Sandbox2D"),
         m_camController{1280.0f / 780.0f, true},
         m_squarePos{0}
         {};
    virtual ~Sandbox2D() {};
    void onUpdate(ChoreoEngine::TimeStep& timestep) override;
    void onImGuiRender() override;
    void onEvent(ChoreoEngine::Event& e) override;

    void onAttach() override;
    void onDetach() override;

private:
    ChoreoEngine::OrthographicCameraController m_camController;
    ChoreoEngine::Ref<ChoreoEngine::Texture2D> m_texture{ChoreoEngine::Texture2D::create(ChoreoEngine::Application::get().getRootDir() + "assets/textures/ghoul.jpg")};
    ChoreoEngine::Ref<ChoreoEngine::Texture2D> m_logoTexture{ChoreoEngine::Texture2D::create(ChoreoEngine::Application::get().getRootDir() + "assets/textures/graphic.png")};
    ChoreoEngine::Ref<ChoreoEngine::Texture2D> m_spriteSheet{ChoreoEngine::Texture2D::create(ChoreoEngine::Application::get().getRootDir() + "assets/textures/gothicvania/old-dark-castle-interior-tileset.png")};
    ChoreoEngine::Ref<ChoreoEngine::SubTexture2D> m_brick{ChoreoEngine::SubTexture2D::createFromCoords(m_spriteSheet, glm::vec2{3.0f, 3.0f}, glm::vec2{1.0f, 1.0f}, 8)};
    ChoreoEngine::Ref<ChoreoEngine::SubTexture2D> m_door{ChoreoEngine::SubTexture2D::createFromCoords(m_spriteSheet, glm::vec2{41.0f, 8.0f}, glm::vec2{3.0f, 4.0f}, 8)};
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

    std::unordered_map<char, ChoreoEngine::Ref<ChoreoEngine::SubTexture2D>> s_textureMap;
    uint32_t m_mapWidth, m_mapHeight;
};
