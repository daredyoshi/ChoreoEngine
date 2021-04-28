# pragma once
#include "Application/Scene/Scene.h"
#include "ChoreoApp.h"
#include <unordered_map>


class MainLayer: public ChoreoApp::Layer{
public:
    MainLayer()
        : 
         Layer("Main"),
         m_camController{1280.0f / 780.0f, true}
    {
    };
    virtual ~MainLayer() {};
    void onUpdate(ChoreoApp::Timestep& timestep) override;
    void onImGuiRender() override;
    void onEvent(ChoreoApp::Event& e) override;

    void onAttach() override;
    void onDetach() override;


private:
    ChoreoApp::OrthographicCameraController m_camController;

    ChoreoApp::Ref<ChoreoApp::Framebuffer> m_framebuffer;
    ChoreoApp::FramebufferSpecification m_framebufferSpec;
    ChoreoApp::Ref<ChoreoApp::Texture2D> m_logoTexture{ChoreoApp::Texture2D::create(ChoreoApp::Application::get().getRootDir() + "assets/textures/graphic.png")};

    ChoreoApp::Ref<ChoreoApp::Scene> m_scene;
    ChoreoApp::Entity m_squareEntity;
    ChoreoApp::Entity m_cameraEntity;
    glm::vec4 m_squareColor;

    uint32_t m_mapWidth, m_mapHeight;
    bool m_viewportFocused{false};
    bool m_viewportHovered{false};

    glm::vec2 m_viewportSize;


};
