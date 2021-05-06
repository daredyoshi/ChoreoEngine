
#include "imgui.h"

namespace ChoreoGrapher{
namespace Widgets{

void messageBox(const char* title, const char* message){
    
    if(ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_AlwaysAutoResize)){
        ImGui::Text("%s\n", message);
        if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::SetItemDefaultFocus();
        ImGui::EndPopup();
    }
}

int yesNoQueryBox(const char* title, const char* message){

    if (ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_AlwaysAutoResize)){
        ImGui::Text("%s\n", message);
        ImGui::Separator();

        //static int unused_i = 0;
        //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

        static bool dont_ask_me_next_time = false;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
        ImGui::PopStyleVar();

        if (ImGui::Button("Yes", ImVec2(120, 0))) { 
            ImGui::CloseCurrentPopup(); 
            return 1; 
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { 
            ImGui::CloseCurrentPopup(); 
            return 0;
        }
        ImGui::EndPopup();
        return 0;
    }
    return 0;
}
}
}
