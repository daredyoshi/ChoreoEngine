
#include "ControllorEditorPanel.h"
#include "imgui.h"
namespace ChoreoGrapher{
namespace Panels{

void drawControllerEditorWindows(std::vector<std::pair<std::vector<ChoreoApp::Ref<ChoreoApp::FloatController>>, bool>>& controllerCollections){
    int idx{0};
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 base_pos = viewport->Pos;

    for(auto& controllerCollection : controllerCollections){
        if(controllerCollection.second){

            // from ImGui
            // We support a syntax of "label###id" where only "###id" is included in the hash, and only "label" gets displayed.
            char buf[128];
            if(controllerCollection.first.size() > 1){
                sprintf(buf, "%d Controllers ###ControllerEditor%d", static_cast<int>( controllerCollection.first.size()), idx);
            }
            else{
                sprintf(buf, "%s Controller ###ControllerEditor%d", controllerCollection.first[0]->getLabel().c_str(), idx);
            }
            ImGui::SetNextWindowPos(ImVec2(base_pos.x + 100, base_pos.y + 300), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);
            if (!ImGui::Begin(buf, &controllerCollection.second)){
                ImGui::End();
            }
            curveEditorPanel(controllerCollection.first);

            ImGui::End();
        }
        else{
            controllerCollections.erase(controllerCollections.begin()+idx);
        }
        idx++;
    }

}

// returns 1 if close has been pressed
int curveEditorPanel(std::vector<ChoreoApp::Ref<ChoreoApp::FloatController>> controllers){

    ChoreoApp::Ref<ChoreoApp::FloatController> controller = controllers[0];
    
    int value_changed{0};


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

    return value_changed;
    }
}}
