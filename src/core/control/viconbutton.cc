#include "../../../include/core/control/viconbutton.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

	VIconButton::VIconButton(const int& Width, const int& Height, VImage* Image, VUIObject* Parent) : VAbstractButton(Parent) {
		Theme = new VIconButtonTheme(*(static_cast<VIconButtonTheme*>(GetTargetTheme(VUIThemeType::VIconButton))));
		Resize(Width, Height);

		Theme->Image = Image;

		Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

		Update();
	}
	VIconButton::VIconButton(VUIObject* Parent) : VAbstractButton(Parent) {
		Theme = new VIconButtonTheme(*(static_cast<VIconButtonTheme*>(GetTargetTheme(VUIThemeType::VIconButton))));
		Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

		Update();
	}
	VIconButton::VIconButton(const int& Width, const int& Height, VUIObject* Parent) : VAbstractButton(Parent) {
		Theme = new VIconButtonTheme(*(static_cast<VIconButtonTheme*>(GetTargetTheme(VUIThemeType::VIconButton))));
		Resize(Width, Height);

		Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

		Update();
	}
	VIconButton::~VIconButton() noexcept {
		delete Theme;
		delete Interpolator;
	}

	VRect VIconButton::GetRegion() {
		VRect Rect = VUIObject::GetRegion();

		Rect.Extended(
			static_cast<int>(Theme->LocalTheme.BorderThickness), static_cast<int>(Theme->LocalTheme.BorderThickness),
			static_cast<int>(Theme->LocalTheme.BorderThickness), static_cast<int>(Theme->LocalTheme.BorderThickness));
		return Rect;
	}
	void VIconButton::OnPaint(VCanvasPainter* Painter) {
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

		if (Theme->Image != nullptr) {
			auto Left = (GetWidth()) / 2 - Theme->Image->GetWidth() / 2;
			auto Top = (GetHeight()) / 2 - Theme->Image->GetHeight() / 2;

			Painter->DrawImage({
									   Left, Top,
									   Left + Theme->Image->GetWidth(),
									   Top + Theme->Image->GetHeight()
				}, Theme->Image, { 0, 0, Theme->Image->GetWidth(), Theme->Image->GetHeight() }, 1.f);
		}

		Painter->EndDraw();
	}

	void VIconButton::SetTheme(Core::VIconButtonTheme* NewTheme) {
		Theme = NewTheme;
	}
	VIconButtonTheme* VIconButton::GetTheme() {
		return Theme;
	}
	void VIconButton::CheckFrame() {
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

	void VIconButton::LeftClickedDown() {
		InAnimation = true;

		OldTheme = Theme->LocalTheme;
		TargetTheme = Theme->ActiveTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);

		Update();
	}
	void VIconButton::LeftClickedUp() {
		InAnimation = true;

		OldTheme = Theme->LocalTheme;
		TargetTheme = Theme->OnHoverTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);

		Update();
	}
	void VIconButton::GotMouseFocus() {
		InAnimation = true;

		OldTheme = Theme->LocalTheme;
		TargetTheme = Theme->OnHoverTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);

		Update();
	}
	void VIconButton::LostMouseFocus() {
		InAnimation = true;

		OldTheme = Theme->LocalTheme;
		TargetTheme = Theme->StaticTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);

		Update();
	}

	void VIconButton::SetIconImage(VImage* IconImage) {
		if (Theme->Image == nullptr) {
			Theme->Image = new VImage(*IconImage);
		}
		else {
			*Theme->Image = *IconImage;
		}
	}

}

VLIB_END_NAMESPACE