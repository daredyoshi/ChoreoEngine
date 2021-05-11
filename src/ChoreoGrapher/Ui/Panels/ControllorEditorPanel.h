
#include "Application/Scene/Controller.h"
#include "ChoreoApp.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Ui/Widgets/CurveEditor.h"

namespace ChoreoGrapher{
namespace Panels{

int curveEditorPanel(std::vector<ChoreoApp::Ref<ChoreoApp::FloatController>> controllers);
void drawControllerEditorWindows(std::vector<std::pair<std::vector<ChoreoApp::Ref<ChoreoApp::FloatController>>, bool>>& controllerCollections);

}
}
