#pragma once
#include "ChoreoApp.h"


namespace ChoreoGrapher { 
    class EntityPropertiesPanel {
    public:
        EntityPropertiesPanel() = default;
        EntityPropertiesPanel(const ChoreoApp::Ref<ChoreoApp::Scene>& scene);

        void setContext(const ChoreoApp::Ref<ChoreoApp::Scene>& scene);

        void onImGuiRender();

        

    private:
        void drawEntityComponents(ChoreoApp::Ref<ChoreoApp::Entity> entity);
        ChoreoApp::Ref<ChoreoApp::Scene> m_context;
    };

}
