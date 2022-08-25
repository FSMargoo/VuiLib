// License(MIT)
// Athuor: Margoo
#pragma once

#include "vdragcontrolbase.h"
#include "vpushbutton.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

enum class VDragTowardsMode {
    Vertical, Horizontal
};

class VCircleScrollBarButton : public VDragControlBaseOnPushButton {
private:
    VRect            DraggedRange;
    VDragTowardsMode DragTowardsMode;

private:
    void UserOnDrag(const int& MouseX, const int& MouseY);

public:
    VCircleScrollBarButton(const VDragTowardsMode& DragTowards, VUIObject* Parent);
    VCircleScrollBarButton(const VDragTowardsMode& DragTowards, const int& Width, const int& Height, VUIObject* Parent);

public:
    void OnPaint(VCanvasPainter* Painter) override;

public:
    void SetDragRange(const VRect& Range);
};

}


VLIB_END_NAMESPACE