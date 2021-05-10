


#include "ChoreoApp.h"
#include "TimelinePanel.h"
#include "Ui/Widgets/Timeline.h"
#include "imgui.h"

namespace ChoreoGrapher{

    TimelinePanel::TimelinePanel(const ChoreoApp::Ref<ChoreoApp::Scene>& scene){
        setContext(scene); 
    }

    void TimelinePanel::setContext(const ChoreoApp::Ref<ChoreoApp::Scene>& scene){
        m_context = scene;;
    }

    void TimelinePanel::onImGuiRender(){
        ImGui::Begin("ChoreoGrapher::Timeline");
        // int flags = Widgets::TimelineFlags_DisplayTicks | Widgets::TimelineFlags_SnapTicks;
        int flags = Widgets::TimelineFlags_DisplayFrames | 
            Widgets::TimelineFlags_DisplaySeconds | 
            Widgets::TimelineFlags_DisplayTicks | 
            Widgets::TimelineFlags_SnapFrames;
        if(Widgets::TimeLineSlider(m_context->getTimeLine(), flags)){
            m_context->onTimeChanged(); 
        }
        // int frame = m_context->getTime()->getFrame();
        // int startTime = m_context->getTimeLine().getStartTime()->getFrame();
        // int endTime = m_context->getTimeLine().getEndTime()->getFrame();
        // if(ImGui::SliderInt("##label", &frame, startTime, endTime )){
        //     m_context->getTime()->setFrame(frame);
        // }

        ImGui::End();
    }
}
