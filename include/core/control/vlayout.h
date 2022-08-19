#pragma once

#include "../object/vuiobject.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

enum class VLayoutMode {
    LayoutModeCenter, LayoutModeFar, LayoutModePercent,
    LayoutModeRelative, LayoutModeRelativeBottom, LayoutModeRelativeTop,
    LayoutModeMiddleOffset
};

class VLayout : public VUIObject {
private:
    VUIObject* TargetWindow;

    VLayoutMode  VerticalLayoutMode;
    VLayoutMode  HorizontalLayoutMode;

    double       VerticalLayoutPercent   = 0.f;
    double       HorizontalLayoutPercent = 0.f;

    int          RelativeX = 0;
    int          RelativeY = 0;

    int          XMiddleOffset = 0;
    int          YMiddleOffset = 0;

private:
    void TargetWindowSizeChanged(const int& Width, const int& Height) {
        int NewX = 0;
        int NewY = 0;

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
                NewY = Height * VerticalLayoutPercent;

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
    void ParentSizeChanged(const int& Width, const int& Height) {
        int NewX = 0;
        int NewY = 0;

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
                NewY = TargetWindow->GetHeight() * VerticalLayoutPercent;

                break;
            }
        }

        GetParent()->Move(NewX, NewY);
    }

public:
    /*
     * Build up Functional
    */

    VLayout(VUIObject* Parent, VUIObject* TargetWidget);

    void SetVerticalLayoutMode(VLayoutMode Mode);
    void SetHorizontalLayoutMode(VLayoutMode Mode);

    void SetVerticalLayoutPercent(const double& Percent);
    void SetHorizontalLayoutPercent(const double& Percent);

    void SetRelativeX(const int& X);
    void SetRelativeY(const int& Y);

    void SetXMiddleOffset(const int& XOffset);
    void SetYMiddleOffset(const int& YOffset);

    int GetRelativeX() const;
    int GetRelativeY() const;
    int GetXMiddleOffset() const;
    int GetYMiddleOffset() const;
};
};

VLIB_END_NAMESPACE