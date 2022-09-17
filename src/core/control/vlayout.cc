#include "../../../include/core/control/vlayout.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VLayout::VLayout(Core::VUIObject *Parent, Core::VUIObject *TargetWidget) : VUIObject(Parent) {
    TargetWindow = TargetWidget;

    TargetWidget->Resized.Connect(this, &VLayout::TargetWindowSizeChanged);
    Parent->Resized.Connect(this, &VLayout::ParentSizeChanged);

    VerticalLayoutMode = VLayoutMode::LayoutModeCenter;
    HorizontalLayoutMode = VLayoutMode::LayoutModeCenter;
}
VLayout::VLayout(Core::VUIObject *Parent, Core::VUIObject *TargetWidget,
                 const VLayoutMode& VerticalMode,
                 const VLayoutMode& HorizontalMode) : VUIObject(Parent) {
    TargetWindow = TargetWidget;

    TargetWidget->Resized.Connect(this, &VLayout::TargetWindowSizeChanged);
    Parent->Resized.Connect(this, &VLayout::ParentSizeChanged);

    VerticalLayoutMode   = VerticalMode;
    HorizontalLayoutMode = HorizontalMode;
}
void VLayout::TargetWindowSizeChanged(const int& Width, const int& Height) {
    int NewX = GetParent()->GetX();
    int NewY = GetParent()->GetY();

    switch (HorizontalLayoutMode) {
        case VLayoutMode::LayoutModeCenter: {
            NewX = Width / 2 - GetParent()->GetRegion().GetWidth() / 2;

            break;
        }
        case VLayoutMode::LayoutModeFar: {
            NewX = Width - GetParent()->GetRegion().GetWidth();

            break;
        }
        case VLayoutMode::LayoutModePercent: {
            NewX = Width * VerticalLayoutPercent;

            break;
        }
        case VLayoutMode::LayoutModeRelative: {
            NewX = Width - RelativeX;

            break;
        }
        case VLayoutMode::LayoutModeRelativeTop: {
            NewX = RelativeX;

            break;
        }
        case VLayoutMode::LayoutModeRelativeBottom: {
            NewX = Width - RelativeX;

            break;
        }
        case VLayoutMode::LayoutModeMiddleOffset: {
            NewX = Width / 2 - GetParent()->GetRegion().GetWidth() / 2 + XMiddleOffset;

            break;
        }
    }
    switch (VerticalLayoutMode) {
        case VLayoutMode::LayoutModeCenter: {
            NewY = Height / 2 - GetParent()->GetRegion().GetHeight() / 2;

            break;
        }
        case VLayoutMode::LayoutModeFar: {
            NewY = Height - GetParent()->GetRegion().GetHeight();

            break;
        }
        case VLayoutMode::LayoutModePercent: {
            NewY = Height * HorizontalLayoutPercent;

            break;
        }
        case VLayoutMode::LayoutModeRelative: {
            NewY = RelativeY;

            break;
        }
        case VLayoutMode::LayoutModeRelativeTop: {
            NewY = RelativeY;

            break;
        }
        case VLayoutMode::LayoutModeRelativeBottom: {
            NewY = Height - RelativeY;

            break;
        }
        case VLayoutMode::LayoutModeMiddleOffset: {
            NewY = Height / 2 - GetParent()->GetRegion().GetHeight() / 2 + YMiddleOffset;

            break;
        }
    }

    GetParent()->Move(NewX, NewY);
}
void VLayout::ParentSizeChanged(const int& Width, const int& Height) {
    int NewX = GetParent()->GetX();
    int NewY = GetParent()->GetY();

    switch (HorizontalLayoutMode) {
        case VLayoutMode::LayoutModeCenter: {
            NewX = TargetWindow->GetWidth() / 2 - Width / 2;

            break;
        }
        case VLayoutMode::LayoutModeFar: {
            NewX = TargetWindow->GetWidth() - Width;

            break;
        }
        case VLayoutMode::LayoutModePercent: {
            NewX = TargetWindow->GetWidth() * VerticalLayoutPercent;

            break;
        }
    }
    switch (VerticalLayoutMode) {
        case VLayoutMode::LayoutModeCenter: {
            NewY = TargetWindow->GetHeight() / 2 - Height / 2;

            break;
        }
        case VLayoutMode::LayoutModeFar: {
            NewY = TargetWindow->GetHeight() - Height;

            break;
        }
        case VLayoutMode::LayoutModePercent: {
            NewY = TargetWindow->GetHeight() * HorizontalLayoutPercent;

            break;
        }
    }

    GetParent()->Move(NewX, NewY);
}
void VLayout::SetVerticalLayoutMode(const VLayoutMode& Mode) {
    VerticalLayoutMode = Mode;

    TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
}
void VLayout::SetHorizontalLayoutMode(const VLayoutMode& Mode) {
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

VScaleLayout::VScaleLayout(Core::VUIObject *Parent, Core::VUIObject *TargetWidget) : VUIObject(Parent) {
    TargetWidget->Resized.Connect(this, &VScaleLayout::TargetWindowSizeChanged);

    TargetWindow = TargetWidget;
}
VScaleLayout::VScaleLayout(Core::VUIObject *Parent, Core::VUIObject *TargetWidget,
                           const double &WidthPercent, const double &HeightPercent) : VUIObject(Parent) {
    TargetWidget->Resized.Connect(this, &VScaleLayout::TargetWindowSizeChanged);

    LayoutWidthPercent  = WidthPercent;
    LayoutHeightPercent = HeightPercent;

    TargetWindow = TargetWidget;
}
void VScaleLayout::SetWidthScalePercent(const double& Percent) {
    LayoutWidthPercent = Percent;

    TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
}
void VScaleLayout::SetHeightScalePercent(const double& Percent) {
    LayoutHeightPercent = Percent;

    TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
}
void VScaleLayout::TargetWindowSizeChanged(const int &Width, const int &Height) {
    GetParent()->Resize(LayoutWidthPercent != 0.f ? Width * LayoutWidthPercent : GetParent()->GetWidth(),
                        LayoutHeightPercent != 0.f ? Height * LayoutHeightPercent : GetParent()->GetHeight());
}

};

VLIB_END_NAMESPACE
