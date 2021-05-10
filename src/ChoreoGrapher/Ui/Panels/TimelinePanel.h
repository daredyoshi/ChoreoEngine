
#pragma once
#include "ChoreoApp.h"

namespace ChoreoGrapher { 
    class TimelinePanel{
    public:
        TimelinePanel() = default;
        TimelinePanel(const ChoreoApp::Ref<ChoreoApp::Scene>& scene);
        void setContext(const ChoreoApp::Ref<ChoreoApp::Scene>& scene);
        void onImGuiRender();

    private:
        ChoreoApp::Ref<ChoreoApp::Scene> m_context;
    };

}
