
#include "ChoreoApp.h"
#include "SceneHierarchyPanel.h"
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
        m_context->getRegistry().each([this](auto entityID){
            ChoreoApp::Ref<ChoreoApp::Entity> entity {ChoreoApp::CreateRef<ChoreoApp::Entity>( entityID, m_context->shared_from_this() )};
            drawEntityNode(entity);
        });

        ImGui::End();
        ImGui::ShowDemoWindow();
    }

    void SceneHierarchyPanel::drawEntityNode(ChoreoApp::Ref<ChoreoApp::Entity> entity){
        auto& tc = entity->getComponent<ChoreoApp::TagComponent>();
        
        ImGuiTreeNodeFlags flags =(*(m_context->getLastSelectedEntity()) == *entity ?  ImGuiTreeNodeFlags_Selected : 0) |  ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity->getEntityHandle(), flags, "%s", tc.tag.c_str());
        if(ImGui::IsItemClicked()){
            m_context->setLastSelectedEntity(entity);
        }
        else{
            if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
                m_context->clearLastSelectedEntity();
        }

        if (opened){
            // recursive stuff here
            ImGui::TreePop();
        }
    }
}
