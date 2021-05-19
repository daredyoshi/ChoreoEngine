
#include "CurveEditor.h"
#include "imgui.h"
#include <memory>

namespace ChoreoGrapher{
namespace Widgets{

int CurveEditor(const char* label, CurveEditorData& curveEditorData){

        int changedIdx{-1};
        ImVec2 editorSize = ImGui::GetContentRegionAvail();

        ImU32 currentTimeColor = ImGui::ColorConvertFloat4ToU32({0.8f, 0.2f, 0.2f, 1.0f});
        ImU32 notInFrameRangeFillColor = ImGui::ColorConvertFloat4ToU32({0.1f, 0.1f, 0.1f, 0.5f});

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

		const ImRect inner_bb = window->InnerClipRect;
        int bb_width = inner_bb.Max.x - inner_bb.Min.x;
        int bb_height = inner_bb.Max.y - inner_bb.Min.y;
		const ImRect frame_bb(inner_bb.Min - style.FramePadding, inner_bb.Max + style.FramePadding);

        // reframe everything
        if(curveEditorData.reset){
            // find min and max key vals for gridStartVal and valueRange
            curveEditorData.gridStartT = curveEditorData.floatControllers[0]->getKeyFromIdx(0)->getTick();
            curveEditorData.gridStartVal = curveEditorData.floatControllers[0]->getKeyFromIdx(0)->getVal();

            float gridEndT{curveEditorData.gridStartT};
            float gridEndVal{curveEditorData.gridStartVal};
            // have to do this first because the code I copied from is using
            // start and range instead of start and end *facepalm*
            for(auto& controller : curveEditorData.floatControllers){
                for(auto& key : controller->getKeys()){
                    uint32_t t = key->getTick();
                    float v = key->getVal(); 
                    if(t < curveEditorData.gridStartT){
                        curveEditorData.gridStartT = t;
                    }
                    else if (t > gridEndT){
                        gridEndT = t;
                    }
                    if(v < curveEditorData.gridStartVal){
                        curveEditorData.gridStartVal = v;
                    }
                    else if (v > gridEndVal){
                        gridEndVal = v;
                    }
                }
            }

            float timeRange = gridEndT - curveEditorData.gridStartT;
            float valueRange= gridEndVal - curveEditorData.gridStartVal;


            // add some padding for shits and giggles
            float timePadding = timeRange / 10.0f;
            float valuePadding = valueRange / 10.0f;
            
            curveEditorData.gridStartT -= timePadding;
            curveEditorData.gridStartVal -= valuePadding;

            timeRange += timePadding;
            valueRange += valuePadding;
            
             
            curveEditorData.ticksPerPixel = timeRange / ( inner_bb.Max.x - inner_bb.Min.x) ;
            curveEditorData.valuesPerPixel = valueRange / ( inner_bb.Max.x - inner_bb.Min.x) ;

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
		// float timeRange = window->StateStorage.GetFloat((ImGuiID)StorageValues::WIDTH, points_max.x - points_min.x);
		// float valueRange = window->StateStorage.GetFloat((ImGuiID)StorageValues::HEIGHT, points_max.y - points_min.y);
		// window->StateStorage.SetFloat((ImGuiID)StorageValues::FROM_X, curveEditorData.gridStartT);
		// window->StateStorage.SetFloat((ImGuiID)StorageValues::FROM_Y, curveEditorData.gridStartVal);
		// window->StateStorage.SetFloat((ImGuiID)StorageValues::WIDTH, timeRange);
		// window->StateStorage.SetFloat((ImGuiID)StorageValues::HEIGHT, valueRange);


        // so that resizing doesn't rescale the graph
        float tickRange = ( inner_bb.Max.x - inner_bb.Min.x) * curveEditorData.ticksPerPixel;
        float valueRange = (inner_bb.Max.y - inner_bb.Min.y) * curveEditorData.valuesPerPixel;



		auto transform = [&](const ImVec2& pos) -> ImVec2
		{
			float x = (pos.x - curveEditorData.gridStartT) / tickRange;
			float y = (pos.y - curveEditorData.gridStartVal) / valueRange;

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
				curveEditorData.gridStartT + tickRange * x,
				curveEditorData.gridStartVal + valueRange * y
			);
		};

        // GRAPH NAVIGATION
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
                curveEditorData.gridStartT -= tickRange * (relativeWindowPosX);
                curveEditorData.gridStartVal -= valueRange * (relativeWindowPosY);

            }
            else{
                curveEditorData.gridStartT += tickRange * (relativeWindowPosX) * scale;
                curveEditorData.gridStartVal += valueRange * (relativeWindowPosY) * scale;

            }
            curveEditorData.ticksPerPixel *= scale;
            curveEditorData.valuesPerPixel *= scale;
            tickRange = ( inner_bb.Max.x - inner_bb.Min.x) * curveEditorData.ticksPerPixel;
            valueRange = (inner_bb.Max.y - inner_bb.Min.y) * curveEditorData.valuesPerPixel;
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
			curveEditorData.gridStartT -= drag_offset.x * tickRange / (inner_bb.Max.x - inner_bb.Min.x);
			curveEditorData.gridStartVal += drag_offset.y * valueRange / (inner_bb.Max.y - inner_bb.Min.y);
		}

		else if (ImGui::IsMouseDragging(2) && ImGui::IsWindowHovered())
		{
            curveEditorData.isPanning = true;
			curveEditorData.panStartT   = curveEditorData.gridStartT;
			curveEditorData.panStartVal = curveEditorData.gridStartVal;
		}

        // GET TIME INFO ASSUMING ALL CONTROLLERS ARE IN SAME SCENE BOI
        int tpf = ChoreoApp::Time::getTicksPerFrame();
        int fps = ChoreoApp::Time::getFramesPerSecond();

        float displayFrameRange =  tickRange/ (float)tpf;
        float displaySecondRange =  tickRange/ ( (float)tpf * (float)fps );
     
        float displaySpacing{1.0f}; 
        float displayRange{tickRange};

        if (curveEditorData.flags & CurveEditorFlags_DisplayFrames) {
            displaySpacing = tpf;
            displayRange = displayFrameRange;
        }
        else if (curveEditorData.flags & CurveEditorFlags_DisplaySeconds){
            displaySpacing = tpf * fps;
            displayRange = displaySecondRange;
        }

        std::weak_ptr<ChoreoApp::Scene> firstScene = curveEditorData.floatControllers[0]->getScene();

        uint32_t currentTick{firstScene.lock()->getTimeLine().getCurrentTime().getTick()};
        uint32_t startTick{firstScene.lock()->getTimeLine().getStartTime().getTick()};
        uint32_t endTick{firstScene.lock()->getTimeLine().getEndTime().getTick()};
        float displayTime{(float)currentTick / displaySpacing};



        // DRAW GRID
		if (curveEditorData.flags & CurveEditorFlags_ShowGrid)
		{
			int exp;
			frexp(tickRange/ 5, &exp);
			float step_x = (float)ldexp(1.0, exp);
			int cell_cols = int(tickRange/ step_x);

			float x = step_x * int(curveEditorData.gridStartVal / step_x);
			for (int i = -1; i < cell_cols + 2; ++i)
			{
				ImVec2 a = transform({ x + i * step_x, curveEditorData.gridStartVal });
				ImVec2 b = transform({ x + i * step_x, curveEditorData.gridStartVal + valueRange });
				window->DrawList->AddLine(a, b, 0x55000000);
				char buf[64];
				if (exp > 0)
				{
					ImFormatString(buf, sizeof(buf), " %d", int(( x + i * step_x  )/ displaySpacing));
				}
				else
				{
					ImFormatString(buf, sizeof(buf), " %f", ( x + i * step_x  )/ displaySpacing);
				}
				window->DrawList->AddText(b, 0x55000000, buf);
			}

			frexp(valueRange / 5, &exp);
			float step_y = (float)ldexp(1.0, exp);
			int cell_rows = int(valueRange / step_y);

			float y = step_y * int(curveEditorData.gridStartVal / step_y);
			for (int i = -1; i < cell_rows + 2; ++i)
			{
				ImVec2 a = transform({ curveEditorData.gridStartT, y + i * step_y });
				ImVec2 b = transform({ curveEditorData.gridStartT + tickRange, y + i * step_y });
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

        // draw the current time with red line
        int exp;
        frexp(displayRange / 5, &exp);
        float step_x = ldexp(1.0f, exp);
        int cell_cols = (float)bb_width / 100.0 > 3.0f ? bb_width / 100.0 : 3;
        int step = (float)tickRange / (float)cell_cols;
        {
            ImVec2 a = transform( { (float)currentTick, curveEditorData.gridStartVal} );
            ImVec2 b = transform(  { (float)currentTick, curveEditorData.gridStartVal + valueRange});
            window->DrawList->AddLine(a, b, currentTimeColor, 2.0f);

            char buf[64];
            if (exp > 0)
            {
                ImFormatString(buf, sizeof(buf), " %d", int(displayTime));
            }
            else
            {
                ImFormatString(buf, sizeof(buf), " %f", displayTime);
            }
            window->DrawList->AddText(b, currentTimeColor, buf);
        }

        // shade the parts of the timeline that are not in the framerange
        if((int)startTick > curveEditorData.gridStartT)
        {
            ImVec2 a = transform( { curveEditorData.gridStartT, curveEditorData.gridStartVal} );
            ImVec2 b = transform( { static_cast<float>(startTick),curveEditorData.gridStartVal + valueRange});
            window->DrawList->AddRectFilled(a, b, notInFrameRangeFillColor);
        }
        if((int)endTick < curveEditorData.gridStartT + tickRange)
        {
            ImVec2 a = transform({ curveEditorData.gridStartT + tickRange, curveEditorData.gridStartVal});
            ImVec2 b = transform({  static_cast<float>(endTick),curveEditorData.gridStartVal + valueRange });
            window->DrawList->AddRectFilled(a, b, notInFrameRangeFillColor);
        }

        // clear the lastEdited key if clicking in empty part of graph
        if(ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()){
            curveEditorData.lastEditedKey = -1; 
        }

        for (auto& controller : curveEditorData.floatControllers){
            unsigned int keyIdx{0};

			// ImVec2* points = ((ImVec2*)values) + point_idx;
            ImVec2 pPrev{0.0f, 0.0f};

            for (auto& key: controller->getKeys()){
                ImVec2 p { static_cast<float>(key->getTick()), key->eval() };
                    
                // maybe put this somewhere else?
                auto handlePoint = [&](ImVec2& _p, uint32_t keyID) -> bool
                {
                    static const float SIZE = 3;

                    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
                    ImVec2 pos = transform(_p);

                    ImGui::SetCursorScreenPos(pos - ImVec2(SIZE, SIZE));
                    ImGui::PushID(key->getID());
                    ImGui::InvisibleButton("", ImVec2(2 * curveEditorData.handleRadius, 2 * curveEditorData.handleRadius));

                    bool isSelected = curveEditorData.lastEditedKey && curveEditorData.lastEditedKey == static_cast<int>(keyID);

                    float thickness = isSelected ? 2.0f : 1.0f;
                    ImU32 col = ImGui::IsItemActive() || 
                        ImGui::IsItemHovered() || 
                        isSelected ? ImGui::GetColorU32(ImGuiCol_PlotLinesHovered) : 
                        ImGui::GetColorU32(ImGuiCol_PlotLines);

                    window->DrawList->AddLine(pos + ImVec2(-SIZE, 0), pos + ImVec2(0, SIZE), col, thickness);
                    window->DrawList->AddLine(pos + ImVec2(SIZE, 0), pos + ImVec2(0, SIZE), col, thickness);
                    window->DrawList->AddLine(pos + ImVec2(SIZE, 0), pos + ImVec2(0, -SIZE), col, thickness);
                    window->DrawList->AddLine(pos + ImVec2(-SIZE, 0), pos + ImVec2(0, -SIZE), col, thickness);

                    // if (ImGui::IsItemHovered()) hovered_idx = idx;

                    bool changed = false;
                    if (ImGui::IsItemActive() && ImGui::IsMouseClicked(0))
                    {
                        // if(!ImGui::IsMouseDragging(0))
                        curveEditorData.lastEditedKey = keyID;
                        curveEditorData.pointStartT = pos.x;
                        curveEditorData.pointStartVal = pos.y;
                        // CE_TRACE("last edited key {0}", idx);
                    }

                    if (ImGui::IsItemHovered() || (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)))
                    {
                        char tmp[64];
                        ImFormatString(tmp, sizeof(tmp), "%0.2f, %0.2f", p.x / displaySpacing, p.y);
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

                ImGui::PushID(key->getID());
				if (handlePoint(p, key->getID()))
				{
                    key->setVal(p.y);
                    ChoreoApp::Time& t = key->getTime(); 
                    if(p.x >=0.0f)
                        t.setTick(p.x);
                    else
                        t.setTick(0);
                    
					changedIdx= keyIdx;
				}

                // no use drawing samples that are not in the graph
                uint32_t firstTick = curveEditorData.gridStartT > 0.0f ? curveEditorData.gridStartT : 0 ; 
                if(firstTick < startTick){
                    firstTick = startTick;
                }
                uint32_t lastTick{static_cast<uint32_t>(curveEditorData.gridStartT + tickRange)};
                if(lastTick > endTick){
                    lastTick = endTick;
                }
                float firstValue{controller->eval(firstTick)};
                ImVec2 firstP{(float)firstTick, firstValue};


                // no use sampling segment if it's not even in the graph
                ImVec2 prevP{firstP};


                // default to a sample every three pixels 
                uint32_t ticksPerSample = 3;

                for(uint32_t sampleTick{firstTick}; sampleTick<lastTick; sampleTick+=ticksPerSample){

                    // don't sample same tick twice 
                    ChoreoApp::Time t;
                    t.setTick(sampleTick);
                    controller->dirty();
                    float v = controller->eval(t);

                    ImVec2 sampleP = {(float)sampleTick, v};
                    window->DrawList->AddLine(transform(prevP), transform(sampleP), ImGui::GetColorU32(ImGuiCol_PlotLines), 1.0f);
                    prevP = sampleP;
                }
                controller->dirty();
                // }
                ImGui::PopID();
                pPrev = p;
                ++keyIdx;
            }

            // this might go into the panel instead of the curve editor
            // swap keys if their time has changed to be greater/less than the next key
            if(changedIdx >= 0){
                auto key = controller->getKeyFromIdx(changedIdx);
                // check if new time is greater than next key
                ChoreoApp::Time& t = key->getTime(); 
                if(changedIdx != static_cast<int>( controller->getKeys().size() - 1)){
                    auto nextT = controller->getKeyFromIdx( changedIdx+1 )->getTime();
                    if(  nextT.getTick() < t.getTick()){
                        controller->swapKeys(changedIdx, changedIdx+1); 
                        ++changedIdx;
                    }
                    if(nextT.getTick() == t.getTick()){
                        nextT.setTick(t.getTick() - 1);
                        controller->swapKeys(changedIdx, changedIdx+1);
                        ++changedIdx;
                    }
                }
                if(changedIdx != 0){
                    auto prevT  = controller->getKeyFromIdx(changedIdx-1)->getTime();
                    if(prevT.getTick()  > t.getTick()){
                        controller->swapKeys(changedIdx, changedIdx-1);
                        --changedIdx;
                    }
                    if(prevT.getTick() == t.getTick()){
                        t.setTick(t.getTick() - 1);
                        controller->swapKeys(changedIdx, changedIdx-1);
                        --changedIdx;
                    }
                }

                controller->dirty();
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
