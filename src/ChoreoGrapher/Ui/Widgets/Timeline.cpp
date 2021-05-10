
#include "Timeline.h"
#include "Application/Scene/Time.h"
#include "imgui.h"

namespace ChoreoGrapher{
namespace Widgets{
    // int edit = ImGui::ColorEdit4("test", {0.2f, });
    //
int TimeLineSlider(ChoreoApp::TimeLine& timeline, int flags){

    enum class StorageValues : ImGuiID
    {
        FROM_T,
        TO_T,
        HEIGHT,
        IS_PANNING,
        IS_DRAGGING,
    };

    int changed = 0;

	static int start_pan;
	static int start_drag;
    ImVec2 imSize= ImGui::GetContentRegionAvail();

    ImGuiWindow* parent_window = ImGui::GetCurrentWindow();
    ImGuiID id = parent_window->GetID("Timeline");

	ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    ImVec2 size = imSize;
    size.x = size.x < 0 ? ImGui::CalcItemWidth() + (style.FramePadding.x * 2) : size.x;
    size.y = size.y < 0 ? 100: size.y;

    if (!ImGui::BeginChildFrame(id, size, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
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


    // int step = TimeLine

    int tpf = ChoreoApp::Time::getTicksPerFrame();
    int fps = ChoreoApp::Time::getFramesPerSecond();

    unsigned int startTick{timeline.getStartTime()->getTick()};
    unsigned int endTick{timeline.getEndTime()->getTick()};
    unsigned int currentTick{timeline.getCurrentTime()->getTick()};

    unsigned int startFrame = timeline.getStartTime()->getFrame(); 
    unsigned int endFrame = timeline.getEndTime()->getFrame();
    unsigned int currentFrame = timeline.getCurrentTime()->getFrame();

    unsigned int startSecond = timeline.getStartTime()->getSecond(); 
    unsigned int endSecond = timeline.getEndTime()->getSecond();
    unsigned int currentSecond = timeline.getCurrentTime()->getSecond();

    unsigned int time_start{0};
    unsigned int time_end{1};

    // unsigned int time{0};

    int fromTick= window->StateStorage.GetInt((ImGuiID)StorageValues::FROM_T, startTick);
    unsigned int firstTickToDraw = fromTick > 0 ? fromTick : 0;
    int toTick = window->StateStorage.GetInt((ImGuiID)StorageValues::TO_T, endTick);

    float fromFrame = (float)fromTick / tpf;
    unsigned int firstFrameToDraw = fromFrame > 0 ? fromFrame : 0;
    float toFrame = (float)toTick / tpf;

    float fromSecond= (float)fromTick / ( tpf * fps );
    unsigned int firstSecondToDraw = fromSecond > 0 ? fromSecond : 0;
    float toSecond = (float)toTick / ( tpf * fps );

    // int height = window->StateStorage.GetInt((ImGuiID)StorageValues::HEIGHT, 50);
    //
    float displayTickRange = toTick - fromTick;
    float displayFrameRange =  displayTickRange / (float)tpf;
    float displaySecondRange =  displayTickRange / ( (float)tpf * (float)fps );
    float displaySpacing{1.0f}; 
    float displayRange{displayTickRange};

    if (flags & TimelineFlags_SnapFrames) {
        displaySpacing = tpf;
        displayRange = displayFrameRange;
    }
    else if (flags & TimelineFlags_SnapSeconds){
        displaySpacing = tpf * fps;
        displayRange = displaySecondRange;
    }
    float displayTime{(float)currentTick / displaySpacing};

    window->StateStorage.SetInt((ImGuiID)StorageValues::FROM_T, fromTick);
    window->StateStorage.SetInt((ImGuiID)StorageValues::TO_T, toTick);

    const ImRect inner_bb = window->InnerClipRect;
    const ImRect frame_bb(inner_bb.Min - style.FramePadding, inner_bb.Max + style.FramePadding);
    int bb_width = inner_bb.Max.x - inner_bb.Min.x;
    int bb_height = inner_bb.Max.y - inner_bb.Min.y;


    // TICK HEIGHTS
    float tickHeight = (float)bb_height - ( (float)bb_height / 3.0f);
    float frameHeight = (float)bb_height - ( (float)bb_height / 3.0f);
    float secondHeight= (float)bb_height - ( (float)bb_height / 3.0f);
    float textHeight = (float)bb_height;

    // COLORS
    ImU32 currentTimeColor = ImGui::ColorConvertFloat4ToU32({0.8f, 0.2f, 0.2f, 1.0f});
    ImU32 tickColor = ImGui::ColorConvertFloat4ToU32({0.2f, 0.2f, 0.2f, 0.8f});
    ImU32 frameColor = ImGui::ColorConvertFloat4ToU32({0.1f, 0.1f, 0.1f, 1.0f});
    ImU32 secondColor = ImGui::ColorConvertFloat4ToU32({0.5f, 0.5f, 0.5f, 0.5f});
    ImU32 notInFrameRangeFillColor = ImGui::ColorConvertFloat4ToU32({0.1f, 0.1f, 0.1f, 0.5f});


    // draw ticks?
    auto transform = [&](const float tick, const float height) -> ImVec2
    {
        float x = (tick - fromTick) / displayTickRange;
        float y = (height) / bb_height;

        return ImVec2(
            inner_bb.Min.x * (1 - x) + inner_bb.Max.x * x,
            // pos.y
            inner_bb.Min.y * y + inner_bb.Max.y * (1 - y)
        );
    };

    float tickStep = (float)bb_width / (float)displayTickRange;

    // draw ticks if enabled and they are not closer than pixels
    if((flags & TimelineFlags_DisplayTicks) && displayTickRange < (float)bb_width / 3.0){
        for (int i = firstTickToDraw; i<toTick; ++i){

            float tick = i;
            ImVec2 a = transform(tick, 0  );
            ImVec2 b = transform(tick, tickHeight);

            window->DrawList->AddLine(a, b, tickColor);
        }
    }
    //
    // draw the frames if the resolution is high enough
    float frameStep = (float)bb_width / (float)displayFrameRange;
    if((flags & TimelineFlags_DisplayFrames) && displayFrameRange < (float)bb_width / 3.0){
        for (unsigned int i{firstFrameToDraw}; i<toFrame; ++i){

            float tick = i * tpf;
            ImVec2 a = transform( tick, 0  );
            ImVec2 b = transform( tick, frameHeight);

            window->DrawList->AddLine(a, b, frameColor);
        }
    }

    // draw the seconds if the resolution is high enough
    float secondStep = (float)bb_width / (float)displaySecondRange;
    if((flags & TimelineFlags_DisplaySeconds) && displaySecondRange < (float)bb_width / 3.0){
    // if((flags & TimelineFlags_DisplaySeconds)){
        for (unsigned int i{firstSecondToDraw}; i<toSecond; ++i){

            float tick = i * tpf * fps;
            ImVec2 a = transform( tick, 0  );
            ImVec2 b = transform(  tick, secondHeight);

            window->DrawList->AddLine(a, b, secondColor);
        }
    }


    // strings 
    int exp;
    frexp(displayRange / 5, &exp);
    float step_x = ldexp(1.0f, exp);
    int cell_cols = (float)bb_width / 100.0 > 3.0f ? bb_width / 100.0 : 3;
    int step = (float)displayTickRange / (float)cell_cols;


    for (int i ={0}; i < cell_cols; ++i)
    {
        // float tick = i * step_x * displaySpacing;
        float tick = fromTick + (i * step);

        ImVec2 a = transform( tick, 0);
        ImVec2 b = transform( tick, textHeight);
        window->DrawList->AddLine(a, b, 0x55000000);
        char buf[12];
        if (exp > 0)
        {
            ImFormatString(buf, sizeof(buf), " %d", int(tick / displaySpacing));
        }
        else
        {
            ImFormatString(buf, sizeof(buf), " %f", tick / displaySpacing);
        }
        window->DrawList->AddText(b, 0x55000000, buf);
    }
    // draw the current time with red line
    {
        ImVec2 a = transform( currentTick, 0  );
        ImVec2 b = transform(  currentTick, (float)bb_height);
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
    if((int)startTick > fromTick)
    {
        ImVec2 a = transform( fromTick, 0 );
        ImVec2 b = transform( startTick, bb_height);
        window->DrawList->AddRectFilled(a, b, notInFrameRangeFillColor);
    }
    if((int)endTick < toTick)
    {
        ImVec2 a = transform( toTick, 0 );
        ImVec2 b = transform( endTick, bb_height);
        window->DrawList->AddRectFilled(a, b, notInFrameRangeFillColor);
    }
        
    


    // INTERACTION
    // ZOOMING 
    if (ImGui::GetIO().MouseWheel != 0 && ImGui::IsWindowHovered())
    {
        float scale = -ImGui::GetIO().MouseWheel;
        ImVec2 mousePos = ImGui::GetMousePos(); 
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 windowPos = ImGui::GetWindowPos();
        float relativeWindowPosX = (( mousePos.x - windowPos.x) / windowSize.x) ;

        if(scale >= 1.0){
            fromTick -= displayTickRange * (relativeWindowPosX) * 0.1f;
            toTick += displayTickRange * ( 1.0f - (relativeWindowPosX)) * 0.1f;

        }
        else{
            fromTick += displayTickRange * (relativeWindowPosX) * 0.1f;
            toTick -= displayTickRange * ( 1.0f - (relativeWindowPosX)) * 0.1f;

        }
        window->StateStorage.SetInt((ImGuiID)StorageValues::FROM_T, fromTick);
        window->StateStorage.SetInt((ImGuiID)StorageValues::TO_T, toTick);
    }
    // PANNING
    if (ImGui::IsMouseReleased(2))
    {
        window->StateStorage.SetBool((ImGuiID)StorageValues::IS_PANNING, false);
    }
    if (window->StateStorage.GetBool((ImGuiID)StorageValues::IS_PANNING, false))
    {
        ImVec2 drag_offset = ImGui::GetMouseDragDelta(2);
        fromTick = start_pan;
        toTick = start_pan + displayTickRange;
        int ticksDragged = drag_offset.x * (displayTickRange / bb_width);
        fromTick  -= ticksDragged;
        toTick  -= ticksDragged;
        window->StateStorage.SetInt((ImGuiID)StorageValues::FROM_T, fromTick);
        window->StateStorage.SetInt((ImGuiID)StorageValues::TO_T, toTick);
    }

    else if (ImGui::IsMouseDragging(2) && ImGui::IsWindowHovered())
    {
        window->StateStorage.SetBool((ImGuiID)StorageValues::IS_PANNING, true);
        start_pan = fromTick;
    }

    // SET TIME  
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()){
        ImVec2 mousePos = ImGui::GetMousePos();
        int hoverTick = ( mousePos.x - inner_bb.Min.x ) * (displayTickRange / (float)bb_width) + fromTick;
        hoverTick = hoverTick > 0 ? hoverTick : 0;
        ChoreoApp::Time newT{};
        newT.setTick(hoverTick);
        timeline.setCurrentTime(newT);
        changed = hoverTick; 
    }

    ImGui::EndChildFrame();
    // ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);
    return changed;
}
}
}
