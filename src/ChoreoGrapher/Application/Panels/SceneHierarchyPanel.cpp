
#include "SceneHierarchyPanel.h"
#include "Application/Scene/Entity.h"
#include "imgui.h"

namespace ChoreoGrapher{

    SceneHierarchyPanel::SceneHierarchyPanel(const ChoreoApp::Ref<ChoreoApp::Scene>& scene){
        setContext(scene); 
    }

    void SceneHierarchyPanel::setContext(const ChoreoApp::Ref<ChoreoApp::Scene>& scene){
        m_context = scene;;
    }

    void SceneHierarchyPanel::onImGuiRender(){
        ImGui::Begin("ChoreoGrapher::Scene Hierarchy");

        m_context->m_registry.each([this](auto entityID){
            ChoreoApp::Entity entity { entityID, m_context->shared_from_this()};
            drawEntityNode(entity);
        });

        ImGui::End();
        ImGui::ShowDemoWindow();
    }

    void SceneHierarchyPanel::drawEntityNode(const ChoreoApp::Entity entity){
            auto& tc = entity.getComponent<ChoreoApp::TagComponent>();
            ImGui::Text("%s", tc.tag.c_str());
        
    }
}
