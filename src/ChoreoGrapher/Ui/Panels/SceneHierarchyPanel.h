#pragma once
#include "ChoreoApp.h"

namespace ChoreoGrapher { 
    class SceneHierarchyPanel{
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const ChoreoApp::Ref<ChoreoApp::Scene>& scene);

        void setContext(const ChoreoApp::Ref<ChoreoApp::Scene>& scene);

        void onImGuiRender();

        

    private:
        void drawEntityNode(const ChoreoApp::Ref<ChoreoApp::Entity> entity);

        ChoreoApp::Ref<ChoreoApp::Scene> m_context;
    };

}
