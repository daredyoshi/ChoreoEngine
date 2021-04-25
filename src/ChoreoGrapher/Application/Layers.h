# pragma once
#include "Application/Renderer/Framebuffer.h"
#include "Application/Renderer/SubTexture2D.h"
#include "Application/Timestep.h"
#include "ChoreoApp.h"
#include <unordered_map>


class MainLayer: public ChoreoApp::Layer{
public:
    MainLayer()
        : 
         Layer("Main"),
         m_camController{1280.0f / 780.0f, true}
         {};
    virtual ~MainLayer() {};
    void onUpdate(ChoreoApp::TimeStep& timestep) override;
    void onImGuiRender() override;
    void onEvent(ChoreoApp::Event& e) override;

    void onAttach() override;
    void onDetach() override;


private:
    ChoreoApp::OrthographicCameraController m_camController;

    ChoreoApp::Ref<ChoreoApp::Framebuffer> m_framebuffer;
    ChoreoApp::FramebufferSpecification m_framebufferSpec;
    ChoreoApp::Ref<ChoreoApp::Texture2D> m_logoTexture{ChoreoApp::Texture2D::create(ChoreoApp::Application::get().getRootDir() + "assets/textures/graphic.png")};

    uint32_t m_mapWidth, m_mapHeight;

    glm::vec2 m_viewportSize;
};
