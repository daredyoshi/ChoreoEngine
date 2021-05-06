#include "Application/Log.h"
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include <math.h>

namespace ChoreoGrapher{
static constexpr float HANDLE_RADIUS = 4.0f;

static ImVec2 node_pos;
static ImGuiID last_node_id;

enum class CurveEditorFlags

{
	NO_TANGENTS = 1 << 0,
	SHOW_GRID = 1 << 1,
	RESET = 1 << 2
};

int CurveEditor(const char* label
	, float* values
	, int points_count
	, const ImVec2& size = ImVec2(-1, -1)
	, ImU32 flags = 0
	, int* new_count = nullptr
	, int* selected_point = nullptr);

bool BeginResizablePopup(const char* str_id, const ImVec2& size_on_first_use);
void HSplitter(const char* str_id, ImVec2* size);
void VSplitter(const char* str_id, ImVec2* size);
void Rect(float w, float h, ImU32 color);

void Label(const char* label);
void TextClipped(const char* text, float size);
bool IconButton(const char* icon, const char* tooltip);
bool Gradient4(const char* label, int max_count, int* count, float* keys, float* values);
void PushReadOnly();
void PopReadOnly();

}
