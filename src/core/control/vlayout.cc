#include "../../../include/core/control/vlayout.h"

VLIB_BEGIN_NAMESPACE

namespace Core {
VLayout::VLayout(Core::VUIObject *Parent, Core::VUIObject *TargetWidget): VUIObject(Parent) {
    TargetWindow = TargetWidget;

    TargetWidget->Resized.Connect(this, &VLayout::TargetWindowSizeChanged);
    Parent->Resized.Connect(this, &VLayout::ParentSizeChanged);

    VerticalLayoutMode = VLayoutMode::LayoutModeCenter;
    HorizontalLayoutMode = VLayoutMode::LayoutModeCenter;
}
void VLayout::SetVerticalLayoutMode(VLayoutMode Mode) {
    VerticalLayoutMode = Mode;

    TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
}
void VLayout::SetHorizontalLayoutMode(VLayoutMode Mode) {
    HorizontalLayoutMode = Mode;

    TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
}
void VLayout::SetVerticalLayoutPercent(const double& Percent) {
    VerticalLayoutPercent = Percent;

    TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
}
void VLayout::SetHorizontalLayoutPercent(const double& Percent) {
    HorizontalLayoutPercent = Percent;

    TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
}
void VLayout::SetXMiddleOffset(const int& XOffset) {
    XMiddleOffset = XOffset;

    TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
}
void VLayout::SetYMiddleOffset(const int& YOffset) {
    YMiddleOffset = YOffset;

    TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
}

void VLayout::SetRelativeX(const int& X) {
    RelativeX = X;

    TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
}
void VLayout::SetRelativeY(const int& Y) {
    RelativeY = Y;

    TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
}
int VLayout::GetRelativeX() const {
    return RelativeX;
}
int VLayout::GetRelativeY() const {
    return RelativeY;
}
int VLayout::GetXMiddleOffset() const {
    return XMiddleOffset;
}
int VLayout::GetYMiddleOffset() const {
    return YMiddleOffset;
}
};

VLIB_END_NAMESPACE
