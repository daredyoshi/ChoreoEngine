
#include "ControllorEditorPanel.h"
#include "ChoreoGrapher.h"
#include "Application/Scene/Controller.h"
#include "Ui/Widgets/CurveEditor.h"
#include "imgui.h"
namespace ChoreoGrapher{
namespace Panels{

void drawControllerEditorWindows(const ChoreoApp::Scene& scene, std::vector<Widgets::CurveEditorData>& curveEditorDatas){
    int idx{0};
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 base_pos = viewport->Pos;

    for(auto& curveEditorData : curveEditorDatas){
        if(curveEditorData.open){

            ImGuiContext& g = *GImGui;
            const ImGuiStyle& style = g.Style;

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

            // BUTTON TOOLBAR

            ChoreoGrapherApplication& app = static_cast<ChoreoGrapherApplication&>(ChoreoApp::Application::get());
            uint32_t moveTexId =  app.getIcons().move->getId();
            uint32_t addTexId =  app.getIcons().add->getId();
            uint32_t removeTexId =  app.getIcons().remove->getId();
            float fontSize = static_cast<float> ( app.getFontSize() );
            float borderSize = 2.0f;

            const ImVec4 pressedCol = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
            const ImVec4 unpressedCol = ImVec4(0,0,0,0);

            ImVec4 moveBgCol =  curveEditorData.mode == Widgets::CurveEditorMode::Move ? pressedCol : unpressedCol;
            if(ImGui::ImageButton((void*)(uintptr_t)moveTexId, {fontSize, fontSize}, {0.0f, 0.0f}, {1.0f, 1.0f}, borderSize, moveBgCol)){
                curveEditorData.mode = Widgets::CurveEditorMode::Move;
            }
            ImGui::SameLine(0, style.ItemInnerSpacing.x);

            ImVec4 addBgCol =  curveEditorData.mode == Widgets::CurveEditorMode::Add ? pressedCol : unpressedCol;
            if(ImGui::ImageButton((void*)(uintptr_t)addTexId, {fontSize, fontSize}, {0.0f, 0.0f}, {1.0f, 1.0f}, borderSize, addBgCol)){
                curveEditorData.mode = Widgets::CurveEditorMode::Add;
            }
            ImGui::SameLine(0, style.ItemInnerSpacing.x);

            ImVec4 removeBgCol = curveEditorData.mode == Widgets::CurveEditorMode::Remove ? pressedCol : unpressedCol;
            if(ImGui::ImageButton((void*)(uintptr_t)removeTexId, {fontSize, fontSize}, {0.0f, 0.0f}, {1.0f, 1.0f}, borderSize, removeBgCol)){
                curveEditorData.mode = Widgets::CurveEditorMode::Remove;
            }
            int changedIdx = Widgets::CurveEditor(scene, buf, curveEditorData);

            ImGui::End();
        }
        else{
            curveEditorDatas.erase(curveEditorDatas.begin()+idx);
        }
        idx++;
    }

}

}}
