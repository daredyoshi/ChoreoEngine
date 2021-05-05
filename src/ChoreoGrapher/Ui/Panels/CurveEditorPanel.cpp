
#include "ChoreoApp.h"
#include "CurveEditorPanel.h"
#include "Ui/Widgets/Curve.h"
// #include "Ui/Widgets/CurveEditor.h"
#include "Ui/Widgets/curve-editor-lumix.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace ChoreoGrapher{

    CurveEditorPanel::CurveEditorPanel(const ChoreoApp::Ref<ChoreoApp::Scene>& scene){
        setContext(scene); 
    }

    void CurveEditorPanel::setContext(const ChoreoApp::Ref<ChoreoApp::Scene>& scene){
        m_context = scene;;
    }

    void CurveEditorPanel::onImGuiRender(){
        ImGui::Begin("ChoreoGrapher::Curve Editor");
        static float col1[3] = { 1.0f, 0.0f, 0.2f };

        curvePts[0].x = 0; // init data so editor knows to take it from here
        curvePts[0].y = 0; // init data so editor knows to take it from here
        curvePts[1].x = 0.5f; // init data so editor knows to take it from here
        curvePts[1].y = 0.5f; // init data so editor knows to take it from here
        curvePts[2].x = 1.0f; // init data so editor knows to take it from here
        curvePts[2].y = 1.0f; // init data so editor knows to take it from here

        ImVec2 imViewportSize= ImGui::GetContentRegionAvail();
// int CurveEditor(const char* label
// 		, float* values
// 		, int points_count
// 		, const ImVec2& editor_size
// 		, ImU32 flags
// 		, int* new_count)
// 	{
        int flags = (int)ImGui::CurveEditorFlags::NO_TANGENTS | (int)ImGui::CurveEditorFlags::SHOW_GRID;
        // int changed = ImGui::CurveEditor("Das Editor", (float*)curvePts, 3, imViewportSize);
        int new_count{0};
        int selectedPoint{0};
        
        int changed = ImGui::CurveEditor("curve", (float*)curvePts, 3, imViewportSize, flags, &new_count);
        if (changed >= 0)
		{
            // CE_TRACE("Changed");
			// curve.frames[changed] = int(points[changed].x + 0.5f);
			// curve.values[changed] = points[changed].y;
            // curvePts[0] = curvePts;
			// curve.frames.back() = last_frame;
			// curve.frames[0] = 0;
		}
		if (new_count != 3)
		{
            // CE_TRACE("Added point");
			// curve.frames.resize(new_count);
			// curve.values.resize(new_count);
			// for (int i = 0; i < new_count; ++i)
			// {
			// 	curve.frames[i] = int(points[i].x + 0.5f);
			// 	curve.values[i] = points[i].y;
			// }
		}
        // if (ImGui::Curve("Das editor", imViewportSize, 4, curvePts))
        // {
        //     // curve changed
        // }

        ImGui::End();
    }

    void CurveEditorPanel::drawEntityNode(ChoreoApp::Ref<ChoreoApp::Entity> entity){
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
