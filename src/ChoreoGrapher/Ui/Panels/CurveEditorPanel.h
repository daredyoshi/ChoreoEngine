#pragma once
#include "ChoreoApp.h"
#include "imgui.h"

namespace ChoreoGrapher { 
    class CurveEditorPanel{
    public:
        CurveEditorPanel() = default;
        CurveEditorPanel(const ChoreoApp::Ref<ChoreoApp::Scene>& scene);

        void setContext(const ChoreoApp::Ref<ChoreoApp::Scene>& scene);

        void onImGuiRender();

        

    private:
        ImVec2 curvePts[6];
        void drawEntityNode(const ChoreoApp::Ref<ChoreoApp::Entity> entity);

        ChoreoApp::Ref<ChoreoApp::Scene> m_context;
    };

}
