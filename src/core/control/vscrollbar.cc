#include "../../../include/core/control/vscrollbar.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VCircleScrollBarButton::VCircleScrollBarButton(const VDragTowardsMode& DragTowards, VUIObject* Parent)
    : VDragControlBaseOnPushButton(Parent) {
    DragTowardsMode = DragTowards;
    MouseDragged.Connect(this, &VCircleScrollBarButton::UserOnDrag);

    delete Theme;
    Theme = new VCircleScrollBarTheme();
}
VCircleScrollBarButton::VCircleScrollBarButton(const VDragTowardsMode& DragTowards, const int& Width, const int& Height, VUIObject* Parent)
    : VDragControlBaseOnPushButton(Width, Height, Parent) {
    DragTowardsMode = DragTowards;
    MouseDragged.Connect(this, &VCircleScrollBarButton::UserOnDrag);

    delete Theme;
    Theme = new VCircleScrollBarTheme();
}
void VCircleScrollBarButton::OnPaint(Core::VCanvasPainter *Painter) {
    VSolidBrush BackgroundBrush(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(),
                                Theme->LocalTheme.BackgroundColor);
    VPenBrush   PenBrush(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(),
                         Theme->LocalTheme.BorderColor, Theme->LocalTheme.BorderThickness);
    VSolidBrush TextBrush(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(),
                          Theme->LocalTheme.TextColor);

    Painter->BeginDraw();

    Painter->FillEllipse({
                            static_cast<int>(Theme->LocalTheme.BorderThickness),
                            static_cast<int>(Theme->LocalTheme.BorderThickness),
                            GetWidth(), GetHeight()
                        }, &PenBrush, &BackgroundBrush);

    Painter->EndDraw();
}
void VCircleScrollBarButton::SetDragRange(const VRect& Range) {
    DraggedRange = Range;
}
void VCircleScrollBarButton::LosedMouseFocus() {
    if (!UserInDrag) {
        VPushButton::LosedMouseFocus();
    }
}
void VCircleScrollBarButton::UserOnDrag(const int &MouseX, const int &MouseY) {
    switch (DragTowardsMode) {
        case VDragTowardsMode::Vertical: {
            if (MouseY >= DraggedRange.Top &&
                MouseY <= DraggedRange.Bottom) {
                Move(GetX(), MouseY);
            }

            break;
        }
        case VDragTowardsMode::Horizontal: {
            if (MouseX >= DraggedRange.Left &&
                MouseX <= DraggedRange.Right) {
                Move(MouseX, GetY());
            }

            break;
        }
    }
}

}

VLIB_END_NAMESPACE