
#include "ControllorEditorPanel.h"
#include "Application/Scene/Controller.h"
#include "imgui.h"
namespace ChoreoGrapher{
namespace Panels{

void drawControllerEditorWindows(std::vector<Widgets::CurveEditorData>& curveEditorDatas){
    int idx{0};
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 base_pos = viewport->Pos;

    for(auto& curveEditorData : curveEditorDatas){
        if(curveEditorData.open){

            // from ImGui
            // We support a syntax of "label###id" where only "###id" is included in the hash, and only "label" gets displayed.
            char buf[128];
            if(curveEditorData.floatControllers.size() > 1){
                sprintf(buf, "%d Controllers ###ControllerEditor%d", static_cast<int>( curveEditorData.floatControllers.size()), idx);
            }
            else{
                sprintf(buf, "%s Controller ###ControllerEditor%d", curveEditorData.floatControllers[0]->getLabel().c_str(), idx);
            }


            ImGui::SetNextWindowPos(ImVec2(base_pos.x + 100, base_pos.y + 300), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(curveEditorData.windowHeight, curveEditorData.windowWidth), ImGuiCond_FirstUseEver);
            if (!ImGui::Begin(buf, &curveEditorData.open)){
                ImGui::End();
            }
            int changedIdx = Widgets::CurveEditor(buf, curveEditorData);

            ImGui::End();
        }
        else{
            curveEditorDatas.erase(curveEditorDatas.begin()+idx);
        }
        idx++;
    }

}

}}
