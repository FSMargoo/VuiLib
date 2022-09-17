#pragma once

#include "vabstractbutton.h"
#include "../uibasic/vtimer.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

class VTextLabel : public VAbstractButton {
private:
    VTextLabelTheme* Theme;

private:
    VRect GetRegion() override;

private:
    VBasicTimer             AnimationFrameTimer;
    VAnimationInterpolator* Interpolator;

    VLabelStatusTheme       OldTheme;
    VLabelStatusTheme       TargetTheme;

    bool                    InAnimation = false;

public:
    void LeftClickedDown() override;
    void LeftClickedUp() override;
    void GotMouseFocus() override;
    void LosedMouseFocus() override;

public:
    void SetTheme(VTextLabelTheme* Theme);
    VTextLabelTheme* GetTheme();

public:
    VTextLabel(VUIObject* Parent);
    VTextLabel(int Width, int Height, VUIObject* Parent);
    VTextLabel(int Width, int Height, const std::wstring& PlaneText, VUIObject* Parent);
    ~VTextLabel();

public:
    void SetPlaneText(const std::wstring& PlaneText);
    void SetTextColor(const VColor& TextColor);
    void SetBackgroundColor(const VColor& BackgroundColor);
    void SetRadius(const VPoint& Radius);
    void SetTextSize(const int& TextSize);
    void SetLineAlignment(const Core::VFontAlignment& Alignment);
    void SetParagraphAlignment(const Core::VFontParagraphAlignment& Alignment);

public:
   void OnPaint(VCanvasPainter* Painter) override;

public:
    void CheckFrame() override;
};

}

VLIB_END_NAMESPACE