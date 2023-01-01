#include "../../../include/core/control/vpushbutton.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VPushButton::VPushButton(const int& Width, const int& Height, const std::wstring& PlaneText, VUIObject* Parent) : VAbstractButton(Parent) {
    Theme = new VPushButtonTheme(*(static_cast<VPushButtonTheme*>(GetTargetTheme(VUIThemeType::VPushButton))));
    Resize(Width, Height);

    Theme->PlaneText = PlaneText;

    Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

    Update();
}
VPushButton::VPushButton(VUIObject* Parent) : VAbstractButton(Parent) {
    Theme = new VPushButtonTheme(*(static_cast<VPushButtonTheme*>(GetTargetTheme(VUIThemeType::VPushButton))));
    Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

    Update();
}
VPushButton::VPushButton(const int& Width, const int& Height, VUIObject* Parent) : VAbstractButton(Parent) {
    Theme = new VPushButtonTheme(*(static_cast<VPushButtonTheme*>(GetTargetTheme(VUIThemeType::VPushButton))));
    Resize(Width, Height);

    Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

    Update();
}
VPushButton::~VPushButton() noexcept {
    delete Theme;
    delete Interpolator;
}

VRect VPushButton::GetRegion() {
    VRect Rect = VUIObject::GetRegion();

    Rect.Extended(
            static_cast<int>(Theme->LocalTheme.BorderThickness), static_cast<int>(Theme->LocalTheme.BorderThickness),
            static_cast<int>(Theme->LocalTheme.BorderThickness), static_cast<int>(Theme->LocalTheme.BorderThickness));
    return Rect;
}
void VPushButton::OnPaint(VCanvasPainter* Painter) {
    Painter->BeginDraw();

    VSolidBrush BackgroundBrush(Theme->LocalTheme.BackgroundColor, CallWidgetGetRenderHandle());
    VPenBrush   PenBrush(Theme->LocalTheme.BorderColor, CallWidgetGetRenderHandle(), Theme->LocalTheme.BorderThickness);
    VSolidBrush TextBrush(Theme->LocalTheme.TextColor, CallWidgetGetRenderHandle());

    Painter->FillRoundedRectangle(
            {
                static_cast<int>(Theme->LocalTheme.BorderThickness),
                static_cast<int>(Theme->LocalTheme.BorderThickness),
                GetWidth(), GetHeight()
            },
            Theme->LocalTheme.Radius, &PenBrush, &BackgroundBrush);

    Painter->DrawString(Theme->PlaneText,
                        {
                                static_cast<int>(Theme->LocalTheme.BorderThickness),
                                static_cast<int>(Theme->LocalTheme.BorderThickness),
                                GetWidth(), GetHeight()
                        }, Theme->LabelFont, &TextBrush);

    Painter->EndDraw();
}

void VPushButton::SetPlaneText(const std::wstring& PlaneText) {
    Theme->PlaneText = PlaneText;

    Update();
}
const std::wstring& VPushButton::GetPlaneString() {
    return Theme->PlaneText;
}
void VPushButton::SetTheme(Core::VPushButtonTheme* NewTheme) {
    Theme = NewTheme;
}
VPushButtonTheme* VPushButton::GetTheme() {
    return Theme;
}
void VPushButton::CheckFrame() {
    if (!Interpolator->IsEnd() && InAnimation) {
        if (AnimationFrameTimer.End()) {
            AnimationFrameTimer.Start(16);

            auto AnimationCurvature = Interpolator->GetOneFrame();

            Theme->LocalTheme.BorderThickness = TargetTheme.BorderThickness +
                                                (OldTheme.BorderThickness - TargetTheme.BorderThickness) *
                                                (1.f - AnimationCurvature);

            Theme->LocalTheme.TextColor = VColorInterpolator::GetTheColorByPercent(OldTheme.TextColor,
                                                                                   TargetTheme.TextColor,
                                                                                   AnimationCurvature);
            Theme->LocalTheme.BackgroundColor = VColorInterpolator::GetTheColorByPercent(OldTheme.BackgroundColor,
                                                                                         TargetTheme.BackgroundColor,
                                                                                         AnimationCurvature);
            Theme->LocalTheme.BorderColor = VColorInterpolator::GetTheColorByPercent(OldTheme.BorderColor,
                                                                                     TargetTheme.BorderColor,
                                                                                     AnimationCurvature);

            Theme->LocalTheme.Radius.X = TargetTheme.Radius.X +
                                         (OldTheme.Radius.X - TargetTheme.Radius.X) * (1.f - AnimationCurvature);
            Theme->LocalTheme.Radius.Y = TargetTheme.Radius.Y +
                                         (OldTheme.Radius.Y - TargetTheme.Radius.Y) * (1.f - AnimationCurvature);

            Update();
        }
    }
    else if (InAnimation) {
        InAnimation = false;
    }
}

void VPushButton::LeftClickedDown() {
    InAnimation = true;

    OldTheme = Theme->LocalTheme;
    TargetTheme = Theme->ActiveTheme;

    Interpolator->Reset();
    AnimationFrameTimer.Start(0);

    Update();
}
void VPushButton::LeftClickedUp() {
    InAnimation = true;

    OldTheme = Theme->LocalTheme;
    TargetTheme = Theme->OnHoverTheme;

    Interpolator->Reset();
    AnimationFrameTimer.Start(0);

    Update();
}
void VPushButton::GotMouseFocus() {
    InAnimation = true;

    OldTheme = Theme->LocalTheme;
    TargetTheme = Theme->OnHoverTheme;

    Interpolator->Reset();
    AnimationFrameTimer.Start(0);

    Update();
}
void VPushButton::LostMouseFocus() {
    InAnimation = true;

    OldTheme = Theme->LocalTheme;
    TargetTheme = Theme->StaticTheme;

    Interpolator->Reset();
    AnimationFrameTimer.Start(0);

    Update();
}
void VPushButton::SetTextSize(const int& TextSize) {
    Theme->LabelFont->SetTextSize(TextSize);
}

}

VLIB_END_NAMESPACE