//
// Created by hew02 on 10/10/25.
//

#include "sproj_2024/gui/Timeline.hpp"

#include <cstdint>

#include "imgui.h"
#include "imgui_internal.h"


namespace ImTimeline {
#ifndef IMGUI_DEFINE_MATH_OPERATORS
    /**
     * Add two vectors.
     */
    static ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) {
        return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
    }
#endif

    bool Timeline(ImTimeline *timeline, int *currFrame, bool *expanded, int *selectedEntry, int *firstFrame, int sequenceOptions) {
        bool ret = false;
        ImGuiIO &io = ImGui::GetIO();
        int cx = (int)io.MousePos.x;
        int cy = (int)io.MousePos.y;

        static int movingEntry = -1;
        static int movingPos = -1;
        static int movingPart = -1;
        int delEntry = -1;
        int dupEntry = -1;
        int itemHeight = 20;

        bool popupOpened = false;
        uint64_t timelineCount = timeline->GetItemCount();

        // Early exit, Timeline is empty
        if (!timelineCount)
            return false;

        ImGui::BeginGroup();

        ImDrawList *drawList = ImGui::GetWindowDrawList();
        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
        ImVec2 canvas_size = ImGui::GetContentRegionAvail();
        int firstFrameUsed = firstFrame ? *firstFrame : 0;

        static bool isMovingScrollBar = false;
        static bool isMovingCurrentFrame = false;

        /**
         * Contains our drawables for this component.
         */
        struct CustomDraw {
            int index;
            ImRect customRect;
            ImRect legendRect;
            ImRect clippingRect;
            ImRect legendClippingRect;
        };
        ImVector<CustomDraw> customDraws;
        ImVector<CustomDraw> compactCustomDraws;

        // Zoom in/out


        ImRect regionRect(canvas_pos, canvas_pos + canvas_size);

        static bool panningView = false;
        static ImVec2 panningViewSource;
        static int panningViewFrame;


        if (expanded && !*expanded) {
            ImGui::InvisibleButton("canvas", ImVec2(canvas_size.x - canvas_pos.x, static_cast<float>(itemHeight)));
            drawList->AddRectFilled(canvas_pos, ImVec2(canvas_size.x + canvas_pos.x, canvas_pos.y + itemHeight), 0xFF3D3837, 0);
            char tmps[512];
            ImFormatString(tmps, IM_ARRAYSIZE(tmps), timeline->GetCollapseFmt(), frameCount, timelineCount);
            drawList->AddText(ImVec2(canvas_pos.x + 26, canvas_pos.y + 2),  0xFFFFFFFF, tmps);
        }

        ImGui::EndGroup();

        return ret;
    }
}
