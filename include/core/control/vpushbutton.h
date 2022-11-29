// License(MIT)
// Athuor: Margoo
#pragma once

#include "../uibasic/vtimer.h"
#include "../object/vuiobject.h"
#include "vabstractbutton.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

class VPushButton : public VAbstractButton {
 protected:
     VPushButtonTheme*       Theme;

     unsigned long dbgCount = 0;

 private:
     VBasicTimer             AnimationFrameTimer;
     VAnimationInterpolator* Interpolator;

     VLabelStatusTheme       OldTheme;
     VLabelStatusTheme       TargetTheme;

     bool                    InAnimation = false;

 public:
     VRect GetRegion() override;

 public:
     void LeftClickedDown() override;
     void LeftClickedUp() override;
     void GotMouseFocus() override;
     void LostMouseFocus() override;

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