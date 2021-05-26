#pragma once
#include "Application/Log.h"
#include "Application/Scene/Scene.h"
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include <math.h>
#include "ChoreoApp.h"

namespace ChoreoGrapher{
namespace Widgets{


typedef int CurveEditorFlags;    // -> enum ImGuiControllerEditFlags_  // Flags: for ColorEdit4(), ColorPicker4() etc.

struct CurveEditorData;
int CurveEditor(const ChoreoApp::Scene& scene, const char* label, CurveEditorData& curveEditorData);

enum CurveEditorFlags_
{

    CurveEditorFlags_DisplayTicks            = 1 << 1,  // [Display]      // ColorEdit, ColorPicker: input and output data in HSV format.
    CurveEditorFlags_DisplayFrames           = 1 << 2,  // [Display]      // ColorEdit, ColorPicker: input and output data in RGB format.
    CurveEditorFlags_DisplaySeconds          = 1 << 3,  // [Display]      // ColorEdit, ColorPicker: input and output data in HSV format.

    CurveEditorFlags_SnapTicks               = 1 << 4,  // [Snap]      // ColorEdit, ColorPicker: input and output data in HSV format.
    CurveEditorFlags_SnapFrames              = 1 << 5,  // [Snap]      // ColorEdit, ColorPicker: input and output data in RGB format.
    CurveEditorFlags_SnapSeconds             = 1 << 6,  // [Snap]      // ColorEdit, ColorPicker: input and output data in HSV format.
    
	CurveEditorFlags_ShowGrid = 1 << 7, 
    
    CurveEditor__OptionsDefault            = CurveEditorFlags_DisplayFrames | CurveEditorFlags_ShowGrid
};

// this has everything the curve editor window needs
struct CurveEditorData{
    std::vector<ChoreoApp::Ref<ChoreoApp::FloatController>> floatControllers{};

    int lastEditedKey{-1};
    CurveEditorFlags flags{CurveEditor__OptionsDefault}; 
    bool open{1};

    // use this to re-frame the gridStart and Range values
    bool reset{true};
    // grid settings
    float gridStartT{0};
    float gridStartVal{0};
    // how wide and high the view is
    float ticksPerPixel{1.0};
    float valuesPerPixel{1.0};

    int windowWidth{300};
    int windowHeight{100};

    // is panning with mouse?
    bool isPanning{false};
    float panStartT{0};
    float panStartVal{0};

    // to reference the currently moving key 
    float pointStartT{0};
    float pointStartVal{0};

    //ui stuff
    float handleRadius{4.0f};

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
