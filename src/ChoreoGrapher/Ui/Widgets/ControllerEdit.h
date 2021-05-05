
#include "Application/Core.h"
#include "Application/Scene/Controller.h"
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "ChoreoApp.h"
#include "Ui/Widgets/CurveEditor.h"

namespace ChoreoGrapher{


static ChoreoApp::Ref<ChoreoApp::FloatController> cilpboardControllerPtr{nullptr};

struct ChoreoGrapherImGuiContext{
    
};

void ControllerEditOptionsPopup(ChoreoApp::Ref<ChoreoApp::FloatAnimatedController>& controller, ImGuiColorEditFlags flags)
{
    bool allow_opt_inputs = !(flags & ImGuiColorEditFlags__DisplayMask);
    bool allow_opt_datatype = !(flags & ImGuiColorEditFlags__DataTypeMask);
    if ((!allow_opt_inputs && !allow_opt_datatype) || !ImGui::BeginPopup("context"))
        return;
    ImGuiContext& g = *GImGui;
    ImGuiColorEditFlags opts = g.ColorEditOptions;
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

    // if (allow_opt_inputs || allow_opt_datatype)
    //     Separator();
    if (ImGui::Button("Copy Controller", ImVec2(-1, 0)))
       cilpboardControllerPtr = ChoreoApp::CreateRef<ChoreoApp::FloatAnimatedController>(*controller); 
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
    // EndPopup();
}
// Edit controller values 
// See enum ImGuiColorEditFlags_ for available options. e.g. Only access 3 floats if ImGuiColorEditFlags_NoAlpha flag is set.
// With typical options: Left-click on color square to open color picker. Right-click to open option menu. CTRL-Click over input fields to edit them and TAB to go to next item.
// bool ImGui::ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags)
bool ControllerEdit(const char* label, ChoreoApp::Ref<ChoreoApp::FloatAnimatedController>& controller,const ChoreoApp::Scope<ChoreoApp::Time>& t, ImU32 flags){

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const float square_sz = ImGui::GetFrameHeight();
    const float w_full = ImGui::CalcItemWidth();
    const float w_button = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz + style.ItemInnerSpacing.x);
    const float w_inputs = w_full - w_button;
    const char* label_display_end = ImGui::FindRenderedTextEnd(label);
    g.NextItemData.ClearFlags();

    ImGui::BeginGroup();
    ImGui::PushID(label);

    // If we're not showing any slider there's no point in doing any HSV conversions
    const ImGuiColorEditFlags flags_untouched = flags;
    if (flags & ImGuiColorEditFlags_NoInputs)
        flags = (flags & (~ImGuiColorEditFlags__DisplayMask)) | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions;

    // Context menu: display and modify options (before defaults are applied)
    if (!(flags & ImGuiColorEditFlags_NoOptions))
        ControllerEditOptionsPopup(controller, flags);

    // Read stored options
    // if (!(flags & ImGuiColorEditFlags__DisplayMask))
    //     flags |= (g.ColorEditOptions & ImGuiColorEditFlags__DisplayMask);
    // if (!(flags & ImGuiColorEditFlags__DataTypeMask))
    //     flags |= (g.ColorEditOptions & ImGuiColorEditFlags__DataTypeMask);
    // if (!(flags & ImGuiColorEditFlags__PickerMask))
    //     flags |= (g.ColorEditOptions & ImGuiColorEditFlags__PickerMask);
    // if (!(flags & ImGuiColorEditFlags__InputMask))
    //     flags |= (g.ColorEditOptions & ImGuiColorEditFlags__InputMask);
    // flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags__DisplayMask | ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags__PickerMask | ImGuiColorEditFlags__InputMask));
    // IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags__DisplayMask)); // Check that only 1 is selected
    // IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags__InputMask));   // Check that only 1 is selected

    // const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
    // const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
    // const int components = alpha ? 4 : 3;

    // Convert to the formats we need
    // float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
    // if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB))
    //     ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
    // else if ((flags & ImGuiColorEditFlags_InputRGB) && (flags & ImGuiColorEditFlags_DisplayHSV))
    // {
    //     // Hue is lost when converting from greyscale rgb (saturation=0). Restore it.
    //     ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
    //     if (memcmp(g.ColorEditLastColor, col, sizeof(float) * 3) == 0)
    //     {
    //         if (f[1] == 0)
    //             f[0] = g.ColorEditLastHue;
    //         if (f[2] == 0)
    //             f[1] = g.ColorEditLastSat;
    //     }
    // }
    // int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

    bool value_changed = false;
    bool value_changed_as_float = false;

    const ImVec2 pos = window->DC.CursorPos;
    const float inputs_offset_x = (style.ColorButtonPosition == ImGuiDir_Left) ? w_button : 0.0f;
    window->DC.CursorPos.x = pos.x + inputs_offset_x;

    // ImGui::DragFloat()
    float val = controller->eval(t);
    static const char* fmt {"%0.3f"};
    // value_changed |= ImGui::DragFloat(controller->getLabel().c_str(), &val, 100.0, 0.0f, 0.1f, fmt);
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

    ImGuiWindow* picker_active_window = NULL;
    // if (!(flags & ImGuiColorEditFlags_NoSmallPreview))
    // {
        // const float button_offset_x = ((flags & ImGuiColorEditFlags_NoInputs) || (style.ColorButtonPosition == ImGuiDir_Left)) ? 0.0f : w_inputs + style.ItemInnerSpacing.x;
        //
        const float button_offset_x{0.0f} ;
        window->DC.CursorPos = ImVec2(pos.x + button_offset_x, pos.y);

        // const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
        if (ImGui::Button("*"))
        {
            // if (!(flags & ImGuiColorEditFlags_NoPicker))
            // {
                // Store current color and open a picker
                // g.ColorPickerRef = col_v4;
                ImGui::OpenPopup("ChoreoGrapher::Curve Editor");
                ImGui::SetNextWindowPos(window->DC.LastItemRect.GetBL() + ImVec2(-1, style.ItemSpacing.y));
                ImGui::SetNextWindowSize(ImVec2{ 600.0f, 200.0f }, true);
            // }
        }
        // if (!(flags & ImGuiColorEditFlags_NoOptions))
        //     OpenPopupOnItemClick("context");
        if (ImGui::BeginPopup("ChoreoGrapher::Curve Editor"))
        {
            picker_active_window = g.CurrentWindow;
            if (label != label_display_end)
            {
                ImGui::TextEx(label, label_display_end);
                ImGui::Spacing();
            }
            // ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags__PickerMask | ImGuiColorEditFlags__InputMask | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
            // ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | ImGuiColorEditFlags__DisplayMask | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;
            ImGui::SetNextItemWidth(square_sz * 12.0f); // Use 256 + bar sizes?
            
            float values[4]{ 0.0f, 0.2f, 1.0f, 0.8f};
            ImVec2 imViewportSize= ImGui::GetContentRegionAvail();
            int newCount{4};
            int curveFlags = (int)ImGui::CurveEditorFlags::NO_TANGENTS | (int)ImGui::CurveEditorFlags::SHOW_GRID;
            value_changed |= ImGui::CurveEditor("##picker", values, 2, imViewportSize, curveFlags, &newCount);
            ImGui::EndPopup();
        }
    // }

    if (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel))
    {
        const float text_offset_x = (flags & ImGuiColorEditFlags_NoInputs) ? w_button : w_full + style.ItemInnerSpacing.x;
        window->DC.CursorPos = ImVec2(pos.x + text_offset_x, pos.y + style.FramePadding.y);
        ImGui::TextEx(label, label_display_end);
    }

    // Convert back
    if (value_changed && picker_active_window == NULL)
    {
        // if (!value_changed_as_float)
        //     for (int n = 0; n < 4; n++)
        //         f[n] = i[n] / 255.0f;
        // if ((flags & ImGuiColorEditFlags_DisplayHSV) && (flags & ImGuiColorEditFlags_InputRGB))
        // {
        //     g.ColorEditLastHue = f[0];
        //     g.ColorEditLastSat = f[1];
        //     ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
        //     memcpy(g.ColorEditLastColor, f, sizeof(float) * 3);
        // }
        // if ((flags & ImGuiColorEditFlags_DisplayRGB) && (flags & ImGuiColorEditFlags_InputHSV))
        //     ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

        // col[0] = f[0];
        // col[1] = f[1];
        // col[2] = f[2];
        // if (alpha)
        //     col[3] = f[3];
    }

    ImGui::PopID();
    ImGui::EndGroup();

    // Drag and Drop Target
    // NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
    // if ((window->DC.LastItemStatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(flags & ImGuiColorEditFlags_NoDragDrop) && BeginDragDropTarget())
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
    //     if (accepted_drag_drop && (flags & ImGuiColorEditFlags_InputHSV))
    //         ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
    //     EndDragDropTarget();
    // }
    //
    // When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
    if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
        window->DC.LastItemId = g.ActiveId;

    if (value_changed)
        ImGui::MarkItemEdited(window->DC.LastItemId);

    return value_changed;
}
}
