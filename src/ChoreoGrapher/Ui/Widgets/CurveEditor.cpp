
#include "CurveEditor.h"
#include "imgui.h"

namespace ChoreoGrapher{
namespace Widgets{

int CurveEditor(const char* label, CurveEditorData& curveEditorData){

        int changedIdx{-1};
        ImVec2 editorSize = ImGui::GetContentRegionAvail();
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		// editorSize.x = editorSize.x < 0 ? ImGui::CalcItemWidth() + (style.FramePadding.x * 2) : editorSize.x;
		// editorSize.y = editorSize.y < 0 ? HEIGHT : editorSize.y;

		ImGuiWindow* parent_window = ImGui::GetCurrentWindow();
		ImGuiID id = parent_window->GetID(label);
		// if (new_count) *new_count = points_count;

		if (!ImGui::BeginChildFrame(id, editorSize, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
            ImGui::EndChildFrame();
			return -1;
		}

		int hovered_idx = -1;

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
		{
            ImGui::EndChildFrame();
			return -1;
		}

        // reframe everything
        if(curveEditorData.reset){
            // find min and max key vals for gridStartVal and valueRange
            curveEditorData.gridStartT = curveEditorData.floatControllers[0]->getKeyFromIdx(0)->getTick();
            curveEditorData.gridStartVal = curveEditorData.floatControllers[0]->getKeyFromIdx(0)->getVal();

            curveEditorData.timeRange = curveEditorData.gridStartT + 1.0;
            curveEditorData.valueRange= curveEditorData.gridStartVal + 1.0;
            // have to do this first because the code I copied from is using
            // start and range instead of start and end *facepalm*
            for(auto& controller : curveEditorData.floatControllers){
                for(auto& key : controller->getKeys()){
                    uint32_t t = key->getTick();
                    float v = key->getVal(); 
                    if(t < curveEditorData.gridStartT){
                        curveEditorData.gridStartT = t;
                    }
                    if(v < curveEditorData.gridStartVal){
                        curveEditorData.gridStartVal = v;
                    }
                }
            }
            // now that we KNOW the start time and val we can calucate the range
            for(auto& controller : curveEditorData.floatControllers){
                for(auto& key : controller->getKeys()){
                    uint32_t t = key->getTick();
                    float v = key->getVal(); 
                    if (t > curveEditorData.gridStartT + curveEditorData.timeRange){
                        curveEditorData.timeRange = t - curveEditorData.gridStartT;
                    }
                    if (v > curveEditorData.gridStartVal+ curveEditorData.valueRange){
                        curveEditorData.valueRange= v - curveEditorData.gridStartVal;
                    }
                }
            }


            // add some padding for shits and giggles
            float timePadding = curveEditorData.timeRange / 10.0f;
            float valuePadding = curveEditorData.valueRange / 10.0f;
            CE_TRACE("time padding {0}", timePadding);
            
            curveEditorData.gridStartT -= timePadding;
            curveEditorData.gridStartVal -= valuePadding;

            curveEditorData.timeRange += timePadding;
            curveEditorData.valueRange += valuePadding;

            

            curveEditorData.reset = false;
        }
		
		// ImVec2 points_min(FLT_MAX, FLT_MAX);
		// ImVec2 points_max(-FLT_MAX, -FLT_MAX);
		// for (int point_idx = 0; point_idx < points_count; ++point_idx)
		// {
		// 	ImVec2 point;
		// 	if (flags & (int)CurveEditorFlags::NO_TANGENTS)
		// 	{
		// 		point = ((ImVec2*)values)[point_idx];
		// 	}
		// 	else
		// 	{
		// 		point = ((ImVec2*)values)[1 + point_idx * 3];
		// 	}
		// 	points_max = ImMax(points_max, point);
		// 	points_min = ImMin(points_min, point);
		// }
		// points_max.y = ImMax(points_max.y, points_min.y + 0.0001f);

		// if (flags & (int)CurveEditorFlags::RESET) window->StateStorage.Clear();


		// float curveEditorData.gridStartT = window->StateStorage.GetFloat((ImGuiID)StorageValues::FROM_X, points_min.x);
		// float curveEditorData.gridStartVal = window->StateStorage.GetFloat((ImGuiID)StorageValues::FROM_Y, points_min.y);
		// float curveEditorData.timeRange = window->StateStorage.GetFloat((ImGuiID)StorageValues::WIDTH, points_max.x - points_min.x);
		// float curveEditorData.valueRange = window->StateStorage.GetFloat((ImGuiID)StorageValues::HEIGHT, points_max.y - points_min.y);
		// window->StateStorage.SetFloat((ImGuiID)StorageValues::FROM_X, curveEditorData.gridStartT);
		// window->StateStorage.SetFloat((ImGuiID)StorageValues::FROM_Y, curveEditorData.gridStartVal);
		// window->StateStorage.SetFloat((ImGuiID)StorageValues::WIDTH, curveEditorData.timeRange);
		// window->StateStorage.SetFloat((ImGuiID)StorageValues::HEIGHT, curveEditorData.valueRange);

		const ImRect inner_bb = window->InnerClipRect;
		const ImRect frame_bb(inner_bb.Min - style.FramePadding, inner_bb.Max + style.FramePadding);

		auto transform = [&](const ImVec2& pos) -> ImVec2
		{
			float x = (pos.x - curveEditorData.gridStartT) / curveEditorData.timeRange;
			float y = (pos.y - curveEditorData.gridStartVal) / curveEditorData.valueRange;

			return ImVec2(
				inner_bb.Min.x * (1 - x) + inner_bb.Max.x * x,
				inner_bb.Min.y * y + inner_bb.Max.y * (1 - y)
			);
		};

		auto invTransform = [&](const ImVec2& pos) -> ImVec2
		{
			float x = (pos.x - inner_bb.Min.x) / (inner_bb.Max.x - inner_bb.Min.x);
			float y = (inner_bb.Max.y - pos.y) / (inner_bb.Max.y - inner_bb.Min.y);

			return ImVec2(
				curveEditorData.gridStartT + curveEditorData.timeRange * x,
				curveEditorData.gridStartVal + curveEditorData.valueRange * y
			);
		};

		if (curveEditorData.curveEditorFlags & CurveEditorFlags_ShowGrid)
		{
			int exp;
			frexp(curveEditorData.timeRange / 5, &exp);
			float step_x = (float)ldexp(1.0, exp);
			int cell_cols = int(curveEditorData.timeRange / step_x);

			float x = step_x * int(curveEditorData.gridStartT / step_x);
			for (int i = -1; i < cell_cols + 2; ++i)
			{
				ImVec2 a = transform({ x + i * step_x, curveEditorData.gridStartVal });
				ImVec2 b = transform({ x + i * step_x, curveEditorData.gridStartVal + curveEditorData.valueRange });
				window->DrawList->AddLine(a, b, 0x55000000);
				char buf[64];
				if (exp > 0)
				{
					ImFormatString(buf, sizeof(buf), " %d", int(x + i * step_x));
				}
				else
				{
					ImFormatString(buf, sizeof(buf), " %f", x + i * step_x);
				}
				window->DrawList->AddText(b, 0x55000000, buf);
			}

			frexp(curveEditorData.valueRange / 5, &exp);
			float step_y = (float)ldexp(1.0, exp);
			int cell_rows = int(curveEditorData.valueRange / step_y);

			float y = step_y * int(curveEditorData.gridStartVal / step_y);
			for (int i = -1; i < cell_rows + 2; ++i)
			{
				ImVec2 a = transform({ curveEditorData.gridStartT, y + i * step_y });
				ImVec2 b = transform({ curveEditorData.gridStartT + curveEditorData.timeRange, y + i * step_y });
				window->DrawList->AddLine(a, b, 0x55000000);
				char buf[64];
				if (exp > 0)
				{
					ImFormatString(buf, sizeof(buf), " %d", int(y + i * step_y));
				}
				else
				{
					ImFormatString(buf, sizeof(buf), " %f", y + i * step_y);
				}
				window->DrawList->AddText(a, 0x55000000, buf);
			}
		}

		if (ImGui::GetIO().MouseWheel != 0 && ImGui::IsWindowHovered())
		{
			float scale = powf(2, -ImGui::GetIO().MouseWheel);
            // TODO add something here to offset the position to center around the scal
            ImVec2 mousePos = ImGui::GetMousePos(); 
            ImVec2 windowSize = ImGui::GetWindowSize();
            ImVec2 windowPos = ImGui::GetWindowPos();
            float relativeWindowPosX = (( mousePos.x - windowPos.x) / windowSize.x) ;
            float relativeWindowPosY = (1 - ( mousePos.y  - windowPos.y ) / windowSize.y) ;
            if(scale >= 1.0){
                curveEditorData.gridStartT -= curveEditorData.timeRange * (relativeWindowPosX);
                curveEditorData.gridStartVal -= curveEditorData.valueRange * (relativeWindowPosY);

            }
            else{
                curveEditorData.gridStartT += curveEditorData.timeRange * (relativeWindowPosX) * scale;
                curveEditorData.gridStartVal += curveEditorData.valueRange * (relativeWindowPosY) * scale;

            }
            curveEditorData.timeRange *= scale;
            curveEditorData.valueRange *= scale;
		}
		if (ImGui::IsMouseReleased(2))
		{
            curveEditorData.isPanning = false;
		}
		if (curveEditorData.isPanning)
		{
			ImVec2 drag_offset = ImGui::GetMouseDragDelta(2);
			curveEditorData.gridStartT = curveEditorData.panStartT;
			curveEditorData.gridStartVal = curveEditorData.panStartVal;
			curveEditorData.gridStartT -= drag_offset.x * curveEditorData.timeRange / (inner_bb.Max.x - inner_bb.Min.x);
			curveEditorData.gridStartVal += drag_offset.y * curveEditorData.valueRange / (inner_bb.Max.y - inner_bb.Min.y);
		}

		else if (ImGui::IsMouseDragging(2) && ImGui::IsWindowHovered())
		{
            curveEditorData.isPanning = true;
			curveEditorData.panStartT   = curveEditorData.gridStartT;
			curveEditorData.panStartVal = curveEditorData.gridStartVal;
		}

        for (auto& controller : curveEditorData.floatControllers){
            unsigned int keyIdx{0};

			// ImVec2* points = ((ImVec2*)values) + point_idx;
            ImVec2 pPrev{0.0f, 0.0f};

            for (auto& key: controller->getKeys()){
                ImVec2 p { static_cast<float>(key->getTick()), key->eval() };
                    
                auto handlePoint = [&](ImVec2& _p, unsigned int idx) -> bool
                {
                    static const float SIZE = 3;

                    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
                    ImVec2 pos = transform(_p);

                    ImGui::SetCursorScreenPos(pos - ImVec2(SIZE, SIZE));
                    ImGui::PushID(idx);
                    ImGui::InvisibleButton("", ImVec2(2 * curveEditorData.handleRadius, 2 * curveEditorData.handleRadius));

                    bool is_selected = curveEditorData.lastEditedKey && curveEditorData.lastEditedKey == idx;
                    float thickness = is_selected ? 2.0f : 1.0f;
                    ImU32 col = ImGui::IsItemActive() || 
                        ImGui::IsItemHovered() ? ImGui::GetColorU32(ImGuiCol_PlotLinesHovered) : 
                        ImGui::GetColorU32(ImGuiCol_PlotLines);

                    window->DrawList->AddLine(pos + ImVec2(-SIZE, 0), pos + ImVec2(0, SIZE), col, thickness);
                    window->DrawList->AddLine(pos + ImVec2(SIZE, 0), pos + ImVec2(0, SIZE), col, thickness);
                    window->DrawList->AddLine(pos + ImVec2(SIZE, 0), pos + ImVec2(0, -SIZE), col, thickness);
                    window->DrawList->AddLine(pos + ImVec2(-SIZE, 0), pos + ImVec2(0, -SIZE), col, thickness);

                    if (ImGui::IsItemHovered()) hovered_idx = idx;

                    bool changed = false;
                    if (ImGui::IsItemActive() && ImGui::IsMouseClicked(0))
                    {
                        if (curveEditorData.lastEditedKey) curveEditorData.lastEditedKey = idx;
                        curveEditorData.pointStartT = pos.x;
                        curveEditorData.pointStartVal = pos.y;
                    }

                    if (ImGui::IsItemHovered() || (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)))
                    {
                        char tmp[64];
                        ImFormatString(tmp, sizeof(tmp), "%0.2f, %0.2f", p.x, p.y);
                        window->DrawList->AddText({ pos.x, pos.y - ImGui::GetTextLineHeight() }, 0xff000000, tmp);
                    }

                    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
                    {
                        pos.x = curveEditorData.pointStartT;
                        pos.y = curveEditorData.pointStartVal;
                        pos += ImGui::GetMouseDragDelta();
                        ImVec2 v = invTransform(pos);

                        _p = v;
                        changed = true;
                    }
                    ImGui::PopID();

                    ImGui::SetCursorScreenPos(cursor_pos);
                    return changed;
                };

                ImGui::PushID(keyIdx);
				if (handlePoint(p, 1))
				{
                    key->setVal(p.y);
					// if (p.x <= pPrev.x) p.x = pPrev.x + 0.001f;
					// if (point_idx < points_count - 2 && p.x >= points[2].x)
					// {
					// 	p.x = points[2].x - 0.001f;
					// }
					// points[1] = p;
					changedIdx= keyIdx;
				}
                // don't draw a line the first time
                // TODO replace this with evaluating controller for x number of segs
                if(keyIdx > 0)
				    window->DrawList->AddLine(transform(pPrev), transform(p), ImGui::GetColorU32(ImGuiCol_PlotLines), 1.0f);
                ImGui::PopID();
                pPrev = p;
                ++keyIdx;
            }
        }
			// auto handleTangent = [&](ImVec2& t, const ImVec2& _p, int idx) -> bool
			// {
			// 	static const float SIZE = 2;
			// 	static const float LENGTH = 18;
            //
			// 	auto normalized = [](const ImVec2& v) -> ImVec2
			// 	{
			// 		float len = 1.0f / sqrtf(v.x *v.x + v.y * v.y);
			// 		return ImVec2(v.x * len, v.y * len);
			// 	};
            //
			// 	ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
			// 	ImVec2 pos = transform(_p);
			// 	ImVec2 tang = pos + normalized(ImVec2(t.x, -t.y)) * LENGTH;
            //
            //     ImGui::SetCursorScreenPos(tang - ImVec2(SIZE, SIZE));
            //     ImGui::PushID(-idx);
            //     ImGui::InvisibleButton("", ImVec2(2 * curveEditorData.handleRadius, 2 * curveEditorData.handleRadius));
            //
			// 	window->DrawList->AddLine(pos, tang, ImGui::GetColorU32(ImGuiCol_PlotLines));
            //
			// 	ImU32 col = ImGui::IsItemHovered() ? ImGui::GetColorU32(ImGuiCol_PlotLinesHovered) : ImGui::GetColorU32(ImGuiCol_PlotLines);
            //
			// 	window->DrawList->AddLine(tang + ImVec2(-SIZE, SIZE), tang + ImVec2(SIZE, SIZE), col);
			// 	window->DrawList->AddLine(tang + ImVec2(SIZE, SIZE), tang + ImVec2(SIZE, -SIZE), col);
			// 	window->DrawList->AddLine(tang + ImVec2(SIZE, -SIZE), tang + ImVec2(-SIZE, -SIZE), col);
			// 	window->DrawList->AddLine(tang + ImVec2(-SIZE, -SIZE), tang + ImVec2(-SIZE, SIZE), col);
            //
			// 	bool changed = false;
			// 	if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
			// 	{
			// 		tang = ImGui::GetIO().MousePos - pos;
			// 		tang = normalized(tang);
			// 		tang.y *= -1;
            //
			// 		t = tang;
			// 		changed = true;
			// 	}
            //     ImGui::PopID();
            //
            //     ImGui::SetCursorScreenPos(cursor_pos);
			// 	return changed;
			// 
		// for (int point_idx = points_count - 2; point_idx >= 0; --point_idx)
		// {
		// 	ImVec2* points;
		// 	if (flags & (int)CurveEditorFlags::NO_TANGENTS)
		// 	{
		// 		points = ((ImVec2*)values) + point_idx;
		// 	}
		// 	else
		// 	{
		// 		points = ((ImVec2*)values) + 1 + point_idx * 3;
		// 	}
			



			// if ((flags & (int)CurveEditorFlags::NO_TANGENTS) == 0)
			// {
			// 	window->DrawList->AddBezierCubic(
			// 		transform(p_prev),
			// 		transform(p_prev + tangent_last),
			// 		transform(p + tangent),
			// 		transform(p),
			// 		ImGui::GetColorU32(ImGuiCol_PlotLines),
			// 		1.0f,
			// 		20);
			// 	if (handleTangent(tangent_last, p_prev, 0))
			// 	{
			// 		points[1] = ImClamp(tangent_last, ImVec2(0, -1), ImVec2(1, 1));
			// 		*changed_idx = point_idx;
			// 	}
			// 	if (handleTangent(tangent, p, 1))
			// 	{
			// 		points[2] = ImClamp(tangent, ImVec2(-1, -1), ImVec2(0, 1));
			// 		*changed_idx = point_idx + 1;
			// 	}
			// 	if (handlePoint(p, 1))
			// 	{
			// 		if (p.x <= p_prev.x) p.x = p_prev.x + 0.001f;
			// 		if (point_idx < points_count - 2 && p.x >= points[6].x)
			// 		{
			// 			p.x = points[6].x - 0.001f;
			// 		}
			// 		points[3] = p;
			// 		*changed_idx = point_idx + 1;
			// 	}
            //
			// }
			// else
			// {
			// 	window->DrawList->AddLine(transform(p_prev), transform(p), ImGui::GetColorU32(ImGuiCol_PlotLines), 1.0f);
			// 	if (handlePoint(p, 1))
			// 	{
			// 		if (p.x <= p_prev.x) p.x = p_prev.x + 0.001f;
			// 		if (point_idx < points_count - 2 && p.x >= points[2].x)
			// 		{
			// 			p.x = points[2].x - 0.001f;
			// 		}
			// 		points[1] = p;
			// 		*changed_idx = point_idx + 1;
			// 	}
			// }
			// if (point_idx == 0)
			// {
			// 	if (handlePoint(p_prev, 0))
			// 	{
			// 		if (p.x <= p_prev.x) p_prev.x = p.x - 0.001f;
			// 		points[0] = p_prev;
			// 		*changed_idx = point_idx;
			// 	}
			// }
            // ImGui::PopID();
		// }

        // ImGui::SetCursorScreenPos(inner_bb.Min);
        //
        // ImGui::InvisibleButton("bg", inner_bb.Max - inner_bb.Min);
        //
		// if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0) && new_count)
		// {
		// 	ImVec2 mp = ImGui::GetMousePos();
		// 	ImVec2 new_p = invTransform(mp);
        //
		// 	ImVec2* points = (ImVec2*)values;
		// 	if ((flags & (int)CurveEditorFlags::NO_TANGENTS) == 0)
		// 	{
		// 		points[points_count * 3 + 0] = ImVec2(-0.2f, 0);
		// 		points[points_count * 3 + 1] = new_p;
		// 		points[points_count * 3 + 2] = ImVec2(0.2f, 0);;
		// 		++*new_count;
        //
		// 		auto compare = [](const void* a, const void* b) -> int
		// 		{
		// 			float fa = (((const ImVec2*)a) + 1)->x;
		// 			float fb = (((const ImVec2*)b) + 1)->x;
		// 			return fa < fb ? -1 : (fa > fb) ? 1 : 0;
		// 		};
        //
		// 		qsort(values, points_count + 1, sizeof(ImVec2) * 3, compare);
        //
		// 	}
		// 	else
		// 	{
		// 		points[points_count] = new_p;
		// 		++*new_count;
        //
		// 		auto compare = [](const void* a, const void* b) -> int
		// 		{
		// 			float fa = ((const ImVec2*)a)->x;
		// 			float fb = ((const ImVec2*)b)->x;
		// 			return fa < fb ? -1 : (fa > fb) ? 1 : 0;
		// 		};
        //
		// 		qsort(values, points_count + 1, sizeof(ImVec2), compare);
		// 	}
		// }
        //
		// if (hovered_idx >= 0 && ImGui::IsMouseDoubleClicked(0) && new_count && points_count > 2)
		// {
		// 	ImVec2* points = (ImVec2*)values;
		// 	--*new_count;
		// 	if ((flags & (int)CurveEditorFlags::NO_TANGENTS) == 0)
		// 	{
		// 		for (int j = hovered_idx * 3; j < points_count * 3 - 3; j += 3)
		// 		{
		// 			points[j + 0] = points[j + 3];
		// 			points[j + 1] = points[j + 4];
		// 			points[j + 2] = points[j + 5];
		// 		}
		// 	}
		// 	else
		// 	{
		// 		for (int j = hovered_idx; j < points_count - 1; ++j)
		// 		{
		// 			points[j] = points[j + 1];
		// 		}
		// 	}
		// }
        //
        ImGui::EndChildFrame();
        ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);
        
		return changedIdx;
	}
}}
