#include "../../../include/core/control/vradiobutton.h"
#include "../../../include/core/uibasic/vnativeicon.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

VRect VRadioButton::GetRegion()
{
	VRect Rect = VUIObject::GetRegion();

	Rect.Extended(
		static_cast<int>(Theme->LocalTheme.BorderThickness), static_cast<int>(Theme->LocalTheme.BorderThickness),
		static_cast<int>(Theme->LocalTheme.BorderThickness), static_cast<int>(Theme->LocalTheme.BorderThickness));
	return Rect;
}
VRadioButton::VRadioButton(VUIObject *Parent) : VAbstractButton(Parent)
{
	Theme = new VRadioButtonTheme(*(static_cast<VRadioButtonTheme *>(GetTargetTheme(VUIThemeType::VRadioButton))));
	Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

	Update();
}
VRadioButton::VRadioButton(const int &Width, const int &Height, VUIObject *Parent) : VAbstractButton(Parent)
{
	Theme = new VRadioButtonTheme(*(static_cast<VRadioButtonTheme *>(GetTargetTheme(VUIThemeType::VRadioButton))));
	Resize(Width, Height);

	Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

	Update();
}
VRadioButton::~VRadioButton() noexcept
{
	delete Theme;
	delete Interpolator;
}

void VRadioButton::SetSwitchStatus(const bool &Status)
{
	Theme->SwitchStatus = Status;

	if (Theme->SwitchStatus)
	{
		Theme->LocalTheme = Theme->ActiveTheme;
	}
	else
	{
		Theme->LocalTheme = Theme->StaticTheme;
	}

	Update();
}
void VRadioButton::SetPlainText(const VString &PlainText)
{
	Theme->PlainText = PlainText;

	Update();
}
void VRadioButton::SetTheme(Core::VRadioButtonTheme *NewTheme)
{
	Theme = NewTheme;
}
VRadioButtonTheme *VRadioButton::GetTheme()
{
	return Theme;
}
void VRadioButton::CheckFrame()
{
	if (!Interpolator->IsEnd() && InAnimation)
	{
		if (AnimationFrameTimer.End())
		{
			AnimationFrameTimer.Start(16);

			auto AnimationCurvature = Interpolator->GetOneFrame();

			Theme->LocalTheme.BorderThickness =
				TargetTheme.BorderThickness +
				(OldTheme.BorderThickness - TargetTheme.BorderThickness) * (1.f - AnimationCurvature);

			Theme->LocalTheme.TextColor =
				VColorInterpolator::GetTheColorByPercent(OldTheme.TextColor, TargetTheme.TextColor, AnimationCurvature);
			Theme->LocalTheme.BackgroundColor = VColorInterpolator::GetTheColorByPercent(
				OldTheme.BackgroundColor, TargetTheme.BackgroundColor, AnimationCurvature);
			Theme->LocalTheme.BorderColor = VColorInterpolator::GetTheColorByPercent(
				OldTheme.BorderColor, TargetTheme.BorderColor, AnimationCurvature);

			Theme->LocalTheme.Radius.X =
				TargetTheme.Radius.X + (OldTheme.Radius.X - TargetTheme.Radius.X) * (1.f - AnimationCurvature);
			Theme->LocalTheme.Radius.Y =
				TargetTheme.Radius.Y + (OldTheme.Radius.Y - TargetTheme.Radius.Y) * (1.f - AnimationCurvature);

			Update();
		}
	}
	else if (InAnimation)
	{
		InAnimation = false;
	}
}
void VRadioButton::OnPaint(VCanvasPainter *Painter)
{
	VCanvasPainter *CheckIcon = VNativeIcon::CheckImage(Theme->LocalTheme.TextColor, GetWidth(), GetHeight(),
														CallWidgetGetRenderHandle(), Theme->IconThickness);

	Painter->BeginDraw();

	VSolidBrush BackgroundBrush(Theme->LocalTheme.BackgroundColor, CallWidgetGetRenderHandle());
	VSolidBrush TextBrush(Theme->LocalTheme.TextColor, CallWidgetGetRenderHandle());
	VPenBrush	PenBrush(Theme->LocalTheme.BorderColor, CallWidgetGetRenderHandle(), Theme->LocalTheme.BorderThickness);

	Painter->FillRoundedRectangle({static_cast<int>(Theme->LocalTheme.BorderThickness),
								   static_cast<int>(Theme->LocalTheme.BorderThickness), GetWidth(), GetHeight()},
								  Theme->LocalTheme.Radius, &PenBrush, &BackgroundBrush);
	Painter->DrawCanvas({0, 0, GetWidth(), GetHeight()}, CheckIcon, {0, 0, GetWidth(), GetHeight()}, 1.f);

	Painter->EndDraw();

	delete CheckIcon;
};
void VRadioButton::SetLockBackStatus(const bool &Status)
{
	LockBack = Status;
}
bool VRadioButton::GetLockBackStatus() const
{
	return LockBack;
}
bool VRadioButton::GetSwitchStatus() const
{
	return Theme->SwitchStatus;
}
void VRadioButton::SwitchStatusIgnoreLockBack()
{
	Theme->SwitchStatus = !Theme->SwitchStatus;

	if (Theme->SwitchStatus)
	{
		InAnimation = true;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->ActiveTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);
	}
	else
	{
		InAnimation = true;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->StaticTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);
	}

	Update();
}
void VRadioButton::LeftClickedDown()
{
	if (LockBack && Theme->SwitchStatus)
	{
		return;
	}

	Theme->SwitchStatus = !Theme->SwitchStatus;

	if (Theme->SwitchStatus)
	{
		InAnimation = true;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->ActiveTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);
	}
	else
	{
		InAnimation = true;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->StaticTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);
	}

	Update();
}
void VRadioButton::GotMouseFocus()
{
	if (!Theme->SwitchStatus)
	{
		InAnimation = true;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->OnHoverTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);

		Update();
	}
}
void VRadioButton::LostMouseFocus()
{
	if (Theme->SwitchStatus)
	{
		InAnimation = true;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->ActiveTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);
	}
	else
	{
		InAnimation = true;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->StaticTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);
	}

	Update();
}

} // namespace Core

VLIB_END_NAMESPACE
