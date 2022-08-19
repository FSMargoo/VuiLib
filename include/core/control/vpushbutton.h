// License(MIT)
// Athuor: Margoo
// The widget (VMainWindow)
#pragma once

#include "../uibasic/vtimer.h"
#include "../object/vuiobject.h"
#include "vabstractbutton.h"

VLIB_BEGIN_NAMESPACE

namespace Core {
class VPushButton : public VAbstractButton {
 private:
     VPushButtonTheme*       Theme;

 private:
     VBasicTimer             AnimationFrameTimer;
     VAnimationInterpolator* Interpolator;

     VLabelStatusTheme       OldTheme;
     VLabelStatusTheme       TargetTheme;

     bool                    InAnimation = false;

 private:
     VRect GetRegion() override;

 public:
     void LeftClickedDown() override;
     void LeftClickedUp() override;
     void GotMouseFocus() override;
     void LosedMouseFocus() override;

 public:
     explicit VPushButton(VUIObject* Parent);
     VPushButton(const int& Width, const int& Height, VUIObject* Parent);
     VPushButton(const int& Width, const int& Height, const std::wstring& PlaneText, VUIObject* Parent);
     ~VPushButton() noexcept;

 public:
     void SetPlaneText(const std::wstring& PlaneText);
     void SetTextSize(const int& TextSize);
     const std::wstring& GetPlaneString();

 public:
     VPushButtonTheme* GetTheme();
     void SetTheme(VPushButtonTheme* Theme);

 public:
     void OnPaint(VCanvasPainter* Painter) override;
     void CheckFrame() override;
};

}


VLIB_END_NAMESPACE