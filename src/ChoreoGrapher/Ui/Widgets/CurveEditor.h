#pragma once
#include "Application/Log.h"
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include <math.h>
#include "ChoreoApp.h"

namespace ChoreoGrapher{
namespace Widgets{


typedef int CurveEditorFlags;    // -> enum ImGuiControllerEditFlags_  // Flags: for ColorEdit4(), ColorPicker4() etc.

struct CurveEditorData{
    std::vector<ChoreoApp::Ref<ChoreoApp::FloatController>> floatControllers;
    unsigned int lastEditedKey;
    ImU32 curveEditorFlags; 
    bool open{1};

    // use this to re-frame the gridStart and Range values
    bool reset{true};
    // grid settings
    float gridStartT{0};
    float gridStartVal{0};
    // how wide and high the view is
    float timeRange{1.0};
    float valueRange{2.0};

    int windowWidth{300};
    int windowHeight{100};

    // is panning with mouse?
    bool isPanning;
    float panStartT{0};
    float panStartVal{0};

    // to reference the currently moving key 
    float pointStartT{0};
    float pointStartVal{0};

    //ui stuff
    float handleRadius{4.0f};

};

int CurveEditor(const char* label, CurveEditorData& curveEditorData);

enum CurveEditorFlags_
{
	CurveEditorFlags_ShowGrid = 1 << 1, 
};
// bool BeginResizablePopup(const char* str_id, const ImVec2& size_on_first_use);
// void HSplitter(const char* str_id, ImVec2* size);
// void VSplitter(const char* str_id, ImVec2* size);
// void Rect(float w, float h, ImU32 color);
//
// void Label(const char* label);
// void TextClipped(const char* text, float size);
// bool IconButton(const char* icon, const char* tooltip);
// bool Gradient4(const char* label, int max_count, int* count, float* keys, float* values);
// void PushReadOnly();
// void PopReadOnly();

}}
