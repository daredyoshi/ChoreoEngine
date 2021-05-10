
#include "ChoreoApp.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Ui/Widgets/CurveEditor.h"

namespace ChoreoGrapher{
namespace Panels{

// returns 1 if close has been pressed
int CurveEditorPanel(ChoreoApp::Ref<ChoreoApp::FloatController> controller, bool* p_open){

    int value_changed{0};


    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 base_pos = viewport->Pos;

    char buf[128];
    sprintf(buf, "%s Controller###AnimatedTitle", controller->getLabel().c_str());
    ImGui::SetNextWindowPos(ImVec2(base_pos.x + 100, base_pos.y + 300), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(buf, p_open)){
        ImGui::End();
        return 0;
    }

    ImGui::Text("This window has a changing title.");


    // if (ImGui::Begin(name.c_str())){
    //     ImGui::End();
    //     return 0;
    // }
    ImGuiContext& g = *GImGui;

    ImGuiWindow* picker_active_window = g.CurrentWindow;

    std::string popupTxt{ "Editing Controller: " + controller->getLabel()};
    const char* controllerLabel = popupTxt.c_str();
    const char* controllerLabelTextEnd= ImGui::FindRenderedTextEnd(controllerLabel);
    if (controllerLabel!= controllerLabelTextEnd)
    {
        ImGui::TextEx(controllerLabel, controllerLabelTextEnd);
        ImGui::Spacing();
    }
    // ImGuiControllerEditFlags picker_flags_to_forward = ImGuiControllerEditFlags__DataTypeMask | ImGuiControllerEditFlags__PickerMask | ImGuiControllerEditFlags__InputMask | ImGuiControllerEditFlags_HDR | ImGuiControllerEditFlags_NoAlpha | ImGuiControllerEditFlags_AlphaBar;
    // ImGuiControllerEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | ImGuiControllerEditFlags__DisplayMask | ImGuiControllerEditFlags_NoLabel | ImGuiControllerEditFlags_AlphaPreviewHalf;
    // ImGui::SetNextItemWidth(square_sz * 12.0f); // Use 256 + bar sizes?
    
    std::vector<ChoreoApp::Ref<ChoreoApp::FloatKey>> keys = controller->getKeys();
    // unsigned int numKeys = controller->
    std::vector<float> values;
    values.reserve(keys.size() * 2);
    for(auto& key : keys){
        // TODO: add flags here for second/frame/tick
        values.push_back(key->getTime().getFrame());
        values.push_back(key->eval());
    }
    
    ImVec2 imViewportSize= ImGui::GetContentRegionAvail();
    int newCount{4};
    int changedIdx{-1};
    int selectedIdx{-1};
    int curveFlags = (int)Widgets::CurveEditorFlags::NO_TANGENTS | (int)Widgets::CurveEditorFlags::SHOW_GRID;
    value_changed |= Widgets::CurveEditor("##picker", &values[0], keys.size(), imViewportSize, curveFlags, &changedIdx, &newCount, &selectedIdx);
    ImGui::End();

    return value_changed;
    }

    }
}
