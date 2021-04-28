# pragma once
#include "ChoreoApp.h"
#include "Panels/SceneHierarchyPanel.h"
#include <unordered_map>
#include "imgui.h"


namespace ChoreoGrapher{

class EditorLayer: public ChoreoApp::Layer{
public:
    EditorLayer()
        : 
         Layer("Main"),
         m_camController{1280.0f / 780.0f, true}
    {
    };
    virtual ~EditorLayer() {};
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
    ChoreoApp::Entity m_secondCamera;
    glm::vec4 m_squareColor;

    uint32_t m_mapWidth, m_mapHeight;
    bool m_viewportFocused{false};
    bool m_viewportHovered{false};

    glm::vec2 m_viewportSize;

    // font
    ImFont* m_font = nullptr; 

    // panels
    SceneHierarchyPanel m_sceneHeirarchyPanel;


};
}
