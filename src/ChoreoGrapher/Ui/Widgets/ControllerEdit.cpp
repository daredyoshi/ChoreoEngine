
#include "ControllerEdit.h"
#include "Application/Scene/Controller.h"
#include "ChoreoApp.h"
#include "MessageBox.h"
#include "imgui.h"
#include <memory>
#include <vector>

namespace ChoreoGrapher{
namespace Widgets{

static ChoreoApp::Ref<ChoreoApp::FloatController> cilpboardFloatControllerPtr{nullptr};


struct ChoreoGrapherImGuiContext{
        
};

void FloatControllerEditOptionsPopup(ChoreoApp::Ref<ChoreoApp::FloatController>& controller)
{
    ImGui::SetNextWindowSize(ImVec2{200.0f, -1});
    if(!ImGui::BeginPopupContextItem("context")){
        return;
    }


    ImGuiContext& g = *GImGui;

    ImGui::TextEx("Float Controller");
    ImGui::Separator();
    ImGui::TextEx("Name : ");
    ImGui::SameLine();

        // static char name[32] = "Label1";
        // char buf[64];
        // sprintf(buf, "Button: %s###Button", name); // ### operator override ID ignoring the preceding label
        // ImGui::Button(buf);
        // if (ImGui::BeginPopupContextItem())
        // {
        //     ImGui::Text("Edit name:");
        //     ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
        //     if (ImGui::Button("Close"))
        //         ImGui::CloseCurrentPopup();
        //     ImGui::EndPopup();
        // }

    char buffer[256];
    // initialize to 0
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, controller->getLabel().c_str());
    if (ImGui::InputText("Name: ", buffer, sizeof(buffer))){
        controller->setLabel(std::string(buffer));
    }
    // if (allow_opt_inputs)
    // {
    //     if (RadioButton("RGB", (opts & ImGuiColorEditFlags_DisplayRGB) != 0)) opts = (opts & ~ImGuiColorEditFlags__DisplayMask) | ImGuiColorEditFlags_DisplayRGB;
    //     if (RadioButton("HSV", (opts & ImGuiColorEditFlags_DisplayHSV) != 0)) opts = (opts & ~ImGuiColorEditFlags__DisplayMask) | ImGuiColorEditFlags_DisplayHSV;
    //     if (RadioButton("Hex", (opts & ImGuiColorEditFlags_DisplayHex) != 0)) opts = (opts & ~ImGuiColorEditFlags__DisplayMask) | ImGuiColorEditFlags_DisplayHex;
    // }
    // if (allow_opt_datatype)
    // {
    //     if (allow_opt_inputs) Separator();
    //     if (RadioButton("0..255",     (opts & ImGuiColorEditFlags_Uint8) != 0)) opts = (opts & ~ImGuiColorEditFlags__DataTypeMask) | ImGuiColorEditFlags_Uint8;
    //     if (RadioButton("0.00..1.00", (opts & ImGuiColorEditFlags_Float) != 0)) opts = (opts & ~ImGuiColorEditFlags__DataTypeMask) | ImGuiColorEditFlags_Float;
    // }

    ImGui::Separator();
    if (ImGui::Button("Copy Controller", ImVec2(-1, 0)))
       cilpboardFloatControllerPtr = controller; 
    ImGui::PushItemWidth(200.0f);

    if (ImGui::Button("Paste Controller Instance", ImVec2(-1, 0))){
        if (cilpboardFloatControllerPtr){
            controller = cilpboardFloatControllerPtr;
        } 
        else {
            Widgets::messageBox("Cannot Instance Controller", "You have not copied a controller to instance yet");
        }
    }
    
    if (ImGui::Button("Paste Controller Values", ImVec2(-1, 0))){
        Widgets::messageBox("Not implemented yet!", "Come back another time");
    }
    if (ImGui::Button("Rename Controller", ImVec2(-1, 0))){
        Widgets::messageBox("Not implemented yet!", "Come back another time");
    }


        // OpenPopup("Copy");
    // if (BeginPopup("Copy"))
    // {
        // int cr = IM_F32_TO_INT8_SAT(col[0]), cg = IM_F32_TO_INT8_SAT(col[1]), cb = IM_F32_TO_INT8_SAT(col[2]), ca = (flags & ImGuiColorEditFlags_NoAlpha) ? 255 : IM_F32_TO_INT8_SAT(col[3]);
        // char buf[64];
        // ImFormatString(buf, IM_ARRAYSIZE(buf), "(%.3ff, %.3ff, %.3ff, %.3ff)", col[0], col[1], col[2], (flags & ImGuiColorEditFlags_NoAlpha) ? 1.0f : col[3]);
        // if (Selectable(buf))
        //     SetClipboardText(buf);
        // ImFormatString(buf, IM_ARRAYSIZE(buf), "(%d,%d,%d,%d)", cr, cg, cb, ca);
        // if (Selectable(buf))
        //     SetClipboardText(buf);
        // ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", cr, cg, cb);
        // if (Selectable(buf))
        //     SetClipboardText(buf);
        // if (!(flags & ImGuiColorEditFlags_NoAlpha))
        // {
        //     ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", cr, cg, cb, ca);
        //     if (Selectable(buf))
        //         SetClipboardText(buf);
        // }
        // EndPopup();
    // }

    // g.ColorEditOptions = opts;
    ImGui::EndPopup();
}
// Edit controller values 
// See enum ImGuiColorEditFlags_ for available options. e.g. Only access 3 floats if ImGuiColorEditFlags_NoAlpha flag is set.
// With typical options: Left-click on color square to open color picker. Right-click to open option menu. CTRL-Click over input fields to edit them and TAB to go to next item.
// bool ImGui::ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags)

bool FloatControllerEdit(
        ChoreoApp::Ref<ChoreoApp::FloatController>& controller, 
        ChoreoApp::Scene& scene,
        floatControllerCollectorRef controllersBeingEdited,
        ImGuiControllerEditFlags flags){

    const char* label = controller->getLabel().c_str();

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;


    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const float square_sz = ImGui::GetFrameHeight();
    const float w_full = ImGui::CalcItemWidth();
    const float w_button = square_sz + style.ItemInnerSpacing.x;
    const float w_inputs = w_full - w_button;
    const char* label_display_end = ImGui::FindRenderedTextEnd(label);
    g.NextItemData.ClearFlags();

    ImGui::BeginGroup();
    ImGui::PushID(controller->getID());

    // for reference
    // ImGui::ColorEdit4

    // Context menu: display and modify options (before defaults are applied)
    FloatControllerEditOptionsPopup(controller);

    bool value_changed = false;
    bool value_changed_as_float = false;

    const ImVec2 pos = window->DC.CursorPos;
    ImGuiWindow* picker_active_window = NULL;

    float val = controller->eval(scene.getTime());
    static const char* fmt {"%0.3f"};

    ImGui::SetNextItemWidth(w_inputs);
    value_changed |= ImGui::DragFloat("", &val, 100.0, 0.0f, 0.1f, fmt);
    ImGuiPopupFlags contextMenuFlags = ImGuiPopupFlags_MouseButtonRight;        // For BeginPopupContext*(): open on Right Mouse release. Guaranteed to always be == 1 (same as ImGuiMouseButton_Right)
    ImGui::OpenPopupOnItemClick("context", contextMenuFlags);

    ImGui::SameLine(0, style.ItemInnerSpacing.x);
    window->DC.CursorPos.x = pos.x + w_inputs;

    if (ImGui::Button("*", ImVec2(square_sz, square_sz)))
    {
        std::vector<ChoreoApp::Ref<ChoreoApp::FloatController>> floatControllers{{controller}};
        CurveEditorData newControllerBeingEdited{floatControllers};
        controllersBeingEdited.push_back(newControllerBeingEdited);
    }

    // }

    // Convert back
    if (value_changed && picker_active_window == NULL)
    {
        // if (!value_changed_as_float)
        //     for (int n = 0; n < 4; n++)
        //         f[n] = i[n] / 255.0f;
        // if ((flags & ImGuiControllerEditFlags_DisplayHSV) && (flags & ImGuiControllerEditFlags_InputRGB))
        // {
        //     g.ControllerEditLastHue = f[0];
        //     g.ControllerEditLastSat = f[1];
        //     ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
        //     memcpy(g.ControllerEditLastColor, f, sizeof(float) * 3);
        // }
        // if ((flags & ImGuiControllerEditFlags_DisplayRGB) && (flags & ImGuiControllerEditFlags_InputHSV))
        //     ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

        // col[0] = f[0];
        // col[1] = f[1];
        // col[2] = f[2];
        // if (alpha)
        //     col[3] = f[3];
    }


    if (label != label_display_end && !(flags & ImGuiControllerEditFlags_NoLabel))
    {
        const float text_offset_x = (flags & ImGuiControllerEditFlags_NoInputs) ? w_button : w_full + style.ItemInnerSpacing.x;
        window->DC.CursorPos = ImVec2(pos.x + text_offset_x, pos.y + style.FramePadding.y);
        ImGui::TextEx(label, label_display_end);
    }
    // if ((flags & (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV)) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
    // {
        // RGB/HSV 0..255 Sliders
    //     const float w_item_one  = ImMax(1.0f, IM_FLOOR((w_inputs - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
    //     const float w_item_last = ImMax(1.0f, IM_FLOOR(w_inputs - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));
    //
    //     const bool hide_prefix = (w_item_one <= CalcTextSize((flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
    //     static const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
    //     static const char* fmt_table_int[3][4] =
    //     {
    //         {   "%3d",   "%3d",   "%3d",   "%3d" }, // Short display
    //         { "R:%3d", "G:%3d", "B:%3d", "A:%3d" }, // Long display for RGBA
    //         { "H:%3d", "S:%3d", "V:%3d", "A:%3d" }  // Long display for HSVA
    //     };
    //     static const char* fmt_table_float[3][4] =
    //     {
    //         {   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" }, // Short display
    //         { "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" }, // Long display for RGBA
    //         { "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }  // Long display for HSVA
    //     };
    //     const int fmt_idx = hide_prefix ? 0 : (flags & ImGuiColorEditFlags_DisplayHSV) ? 2 : 1;
    //
    //     for (int n = 0; n < components; n++)
    //     {
    //         if (n > 0)
    //             SameLine(0, style.ItemInnerSpacing.x);
    //         SetNextItemWidth((n + 1 < components) ? w_item_one : w_item_last);
    //
    //         // FIXME: When ImGuiColorEditFlags_HDR flag is passed HS values snap in weird ways when SV values go below 0.
    //         if (flags & ImGuiColorEditFlags_Float)
    //         {
    //             value_changed |= DragFloat(ids[n], &f[n], 1.0f / 255.0f, 0.0f, hdr ? 0.0f : 1.0f, fmt_table_float[fmt_idx][n]);
    //             value_changed_as_float |= value_changed;
    //         }
    //         else
    //         {
    //             value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255, fmt_table_int[fmt_idx][n]);
    //         }
    //         if (!(flags & ImGuiColorEditFlags_NoOptions))
    //             OpenPopupOnItemClick("context");
    //     }
    // }
    // else if ((flags & ImGuiColorEditFlags_DisplayHex) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
    // {
    //     // RGB Hexadecimal Input
    //     char buf[64];
    //     if (alpha)
    //         ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255), ImClamp(i[3], 0, 255));
    //     else
    //         ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));
    //     SetNextItemWidth(w_inputs);
    //     if (InputText("##Text", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
    //     {
    //         value_changed = true;
    //         char* p = buf;
    //         while (*p == '#' || ImCharIsBlankA(*p))
    //             p++;
    //         i[0] = i[1] = i[2] = 0;
    //         i[3] = 0xFF; // alpha default to 255 is not parsed by scanf (e.g. inputting #FFFFFF omitting alpha)
    //         if (alpha)
    //             sscanf(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]); // Treat at unsigned (%X is unsigned)
    //         else
    //             sscanf(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
    //     }
    //     if (!(flags & ImGuiColorEditFlags_NoOptions))
    //         OpenPopupOnItemClick("context");
    // }

    // Drag and Drop Target
    // NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
    // if ((window->DC.LastItemStatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(flags & ImGuiControllerEditFlags_NoDragDrop) && BeginDragDropTarget())
    // {
    //     bool accepted_drag_drop = false;
    //     if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
    //     {
    //         memcpy((float*)col, payload->Data, sizeof(float) * 3); // Preserve alpha if any //-V512
    //         value_changed = accepted_drag_drop = true;
    //     }
    //     if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
    //     {
    //         memcpy((float*)col, payload->Data, sizeof(float) * components);
    //         value_changed = accepted_drag_drop = true;
    //     }
    //
    //     // Drag-drop payloads are always RGB
    //     if (accepted_drag_drop && (flags & ImGuiControllerEditFlags_InputHSV))
    //         ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
    //     EndDragDropTarget();
    // }
    //
    ImGui::PopID();
    ImGui::EndGroup();

    // When picker is being actively used, use its active id so IsItemActive() will function on ControllerEdit4().
    if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
        window->DC.LastItemId = g.ActiveId;

    if (value_changed)
        ImGui::MarkItemEdited(window->DC.LastItemId);

    return value_changed;
}
}}
