# pragma once
#include "Application/Scene/Controller.h"
#include "ChoreoApp.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/EntityPropertiesPanel.h"
#include "Panels/TimelinePanel.h"
#include <unordered_map>
#include "imgui.h"


namespace ChoreoGrapher{

class EditorLayer: public ChoreoApp::Layer{
public:
    EditorLayer()
        :  Layer("Editor Layer") {};
    virtual ~EditorLayer() {};
    void onUpdate(ChoreoApp::Timestep& timestep) override;
    void onImGuiRender() override;
    void onEvent(ChoreoApp::Event& e) override;

    void onAttach() override;
    void onDetach() override;


private:

    ChoreoApp::Ref<ChoreoApp::Framebuffer> m_framebuffer;
    ChoreoApp::FramebufferSpecification m_framebufferSpec;
    ChoreoApp::Ref<ChoreoApp::Texture2D> m_logoTexture{ChoreoApp::Texture2D::create(ChoreoApp::Application::get().getRootDir() + "assets/textures/graphic.png")};

    ChoreoApp::Entity m_squareEntity;
    ChoreoApp::Entity m_cameraEntity;
    glm::vec4 m_squareColor { 0.8f, 0.2f, 0.2f, 1.0 };

    uint32_t m_mapWidth, m_mapHeight;
    bool m_viewportFocused{false};
    bool m_viewportHovered{false};

    glm::vec2 m_viewportSize{ 0.0f, 0.0f };

    // font
    ImFont* m_font = nullptr; 

    // panels
    ChoreoApp::Ref<ChoreoApp::Scene> m_scene{ChoreoApp::CreateRef<ChoreoApp::Scene>()};

    using floatControllerCollector = std::vector<std::pair<std::vector<ChoreoApp::Ref<ChoreoApp::FloatController>>, bool>>;
    floatControllerCollector m_floatControllers{};

    SceneHierarchyPanel m_sceneHeirarchyPanel{m_scene};
    EntityPropertiesPanel m_entityPropertiesPanel{m_scene, m_floatControllers};
    TimelinePanel m_timelinePanel{m_scene};


};
}