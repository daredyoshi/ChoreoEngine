
#include "Application/Core.h"
#include "Application/Scene/Controller.h"
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "ChoreoApp.h"
#include "Ui/Widgets/CurveEditor.h"

namespace ChoreoGrapher{
namespace Widgets{

typedef int ImGuiControllerEditFlags;    // -> enum ImGuiControllerEditFlags_  // Flags: for ColorEdit4(), ColorPicker4() etc.

using floatControllerCollectorRef = std::vector<CurveEditorData>;

void FloatControllerEditOptionsPopup(ChoreoApp::Ref<ChoreoApp::AnimatedFloatController>& controller, ImGuiColorEditFlags flags=0);
bool FloatControllerEdit(
        ChoreoApp::Ref<ChoreoApp::FloatController>& controller,
        const ChoreoApp::Time& t, 
        floatControllerCollectorRef controllersBeingEdited,
        ImGuiControllerEditFlags flags = 0);

enum ImGuiControllerEditFlags_
{
    ImGuiControllerEditFlags_None            = 0,
    ImGuiControllerEditFlags_NoAlpha         = 1 << 1,   //              // ControllerEdit, ColorPicker, ColorButton: ignore Alpha component (will only read 3 components from the input pointer).
    ImGuiControllerEditFlags_NoPicker        = 1 << 2,   //              // ControllerEdit: disable picker when clicking on color square.
    ImGuiControllerEditFlags_NoOptions       = 1 << 3,   //              // ControllerEdit: disable toggling options menu when right-clicking on inputs/small preview.
    ImGuiControllerEditFlags_NoSmallPreview  = 1 << 4,   //              // ControllerEdit, ColorPicker: disable color square preview next to the inputs. (e.g. to show only the inputs)
    ImGuiControllerEditFlags_NoInputs        = 1 << 5,   //              // ControllerEdit, ColorPicker: disable inputs sliders/text widgets (e.g. to show only the small preview color square).
    ImGuiControllerEditFlags_NoTooltip       = 1 << 6,   //              // ControllerEdit, ColorPicker, ColorButton: disable tooltip when hovering the preview.
    ImGuiControllerEditFlags_NoLabel         = 1 << 7,   //              // ControllerEdit, ColorPicker: disable display of inline text label (the label is still forwarded to the tooltip and picker).
    ImGuiControllerEditFlags_NoSidePreview   = 1 << 8,   //              // ColorPicker: disable bigger color preview on right side of the picker, use small color square preview instead.
    ImGuiControllerEditFlags_NoDragDrop      = 1 << 9,   //              // ControllerEdit: disable drag and drop target. ColorButton: disable drag and drop source.
    ImGuiControllerEditFlags_NoBorder        = 1 << 10,  //              // ColorButton: disable border (which is enforced by default)

    // User Options (right-click on widget to change some of them).
    ImGuiControllerEditFlags_Float           = 1 << 24,  // [DataType]   // ControllerEdit, ColorPicker, ColorButton: _display_ values formatted as 0.0f..1.0f floats instead of 0..255 integers. No round-trip of value via integers.

    // Defaults Options. You can set application defaults using SetControllerEditOptions(). The intent is that you probably don't want to
    // override them in most of your calls. Let the user choose via the option menu and/or call SetControllerEditOptions() once during startup.
    // ImGuiControllerEditFlags__OptionsDefault = ,
};

}

}
