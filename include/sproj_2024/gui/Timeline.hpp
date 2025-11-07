//
// Created by hew02 on 10/10/25.
//
// This module is an edited version of:
//  https://github.com/CedricGuillemet/ImGuizmo/blob/master/ImSequencer.h

#ifndef SPROJ2024_TIMELINE_HPP
#define SPROJ2024_TIMELINE_HPP

#define FRAME_PIXEL_WIDTH 10.f
#define LEGEND_WIDTH 200
#define ITEM_HEIGHT 20
#include <vector>

#include "imgui_internal.h"

namespace ImTimeline {
    enum TIMELINE_OPTIONS {
        TIMELINE_EDIT_NONE = 0,
        TIMELINE_EDIT_STARTEND = 1 << 1,
        TIMELINE_CHANGE_FRAME = 1 << 3,
        TIMELINE_ADD = 1 << 4,
        TIMELINE_DEL = 1 << 5,
        TIMELINE_COPYPASTE = 1 << 6,
        TIMELINE_EDIT_ALL = TIMELINE_EDIT_STARTEND | TIMELINE_CHANGE_FRAME
    };

    struct ImTimeline {
        bool focused = false;
        int frameMin = 0;
        int frameMax = 0;

        int GetFrameMin() const {
            return frameMin;
        }

        int GetFrameMax() const {
            return frameMax;
        }

        int GetItemCount() const {
            return static_cast<int>(items.size());
        }

        /* Sequence item specific */
        struct TimelineItem {
            int type;
            int frameStart, frameEnd;
            bool expanded;
        };
        std::vector<TimelineItem> items;
        // RampEdit rampEdit

        virtual ~ImTimeline() = default;

        virtual void CustomDraw(int index, ImDrawList *drawList, const ImRect &rc, const ImRect &legentRect,
            const ImRect &clippingRect, const ImRect &legendClippingRect) {
            static const char *labels[] = { "Translation", "Rotation", "Scale" };

        }
    };

    /**
     * @brief Returns true if a selection is made. This is the component
     * function one should call to use a timeline.
     */
    bool Timeline(ImTimeline *timeline, int *currFrame, bool *expanded, int *selectedEntry, int *firstFrame, int sequenceOptions = 0);
}

#endif //SPROJ2024_TIMELINE_HPP