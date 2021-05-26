#pragma once
#include "ChoreoApp.h"
#include "Ui/Widgets/CurveEditor.h"


namespace ChoreoGrapher { 
    using floatControllerCollectorRef = std::vector<Widgets::CurveEditorData>&;

    class EntityPropertiesPanel {
    public:
        EntityPropertiesPanel(
                const ChoreoApp::Ref<ChoreoApp::Scene>& scene,
                floatControllerCollectorRef controllersBeingEdited) 

            :    m_context{scene}, m_controllersBeingEdited{controllersBeingEdited} {};

        void setContext(const ChoreoApp::Ref<ChoreoApp::Scene>& scene);

        void onImGuiRender();

        

    private:
        void drawEntityComponents(ChoreoApp::Ref<ChoreoApp::Entity> entity);
        ChoreoApp::Ref<ChoreoApp::Scene> m_context;
        floatControllerCollectorRef  m_controllersBeingEdited;
    };

}
