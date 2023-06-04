#include "../../../include/core/control/vscroller.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

void VScrollerPushButton::MouseLeftClicked(const Core::VMouseClickedFlag &ClickedFlag)
{
	switch (ClickedFlag)
	{
	case VMouseClickedFlag::Down: {
		ScrollerInDragging = true;

		CallWidgetLockFocusID();

		LeftClickedDown();

		break;
	}

	case VMouseClickedFlag::Up: {
		ScrollerInDragging = false;

		CallWidgetUnlockFocusID();

		LeftClickedUp();
		LostMouseFocus();

		break;
	}
	}
}

void VScrollerPushButton::OnMessage(Core::VMessage *Message)
{
	if (Message->GetType() == VMessageType::MouseClickedMessage)
	{
		VMouseClickedMessage *ClickedMessage = static_cast<VMouseClickedMessage *>(Message);

		if (ClickedMessage->MousePosition.InsideRectangle(GetRegion()) ||
			(ClickedMessage->ClickedKey == VMouseKeyFlag::Left &&
			 ClickedMessage->ClickedMethod == VMouseClickedFlag::Down))
		{
			ScrollerDragOriginPoint = ClickedMessage->MousePosition;

			ScrollerDragOriginPoint.Offset(-GetX(), -GetY());
		}
	}

	if (ScrollerInDragging)
	{
		switch (Message->GetType())
		{
		case VMessageType::MouseClickedMessage: {
			VMouseClickedMessage *ClickedMessage = static_cast<VMouseClickedMessage *>(Message);

			if ((ClickedMessage->ClickedKey == VMouseKeyFlag::Left &&
				 ClickedMessage->ClickedMethod == VMouseClickedFlag::Up) &&
				(ClickedMessage->MousePosition.X < 0 || ClickedMessage->MousePosition.Y < 0) && ScrollerInDragging)
			{
				ScrollerInDragging = false;
				CallWidgetUnlockFocusID();
				CallWidgetSendMessage(Message);

				LostMouseFocus();
			}

			if (!ClickedMessage->MousePosition.InsideRectangle(GetRegion()) ||
				(ClickedMessage->ClickedKey == VMouseKeyFlag::Left &&
				 ClickedMessage->ClickedMethod == VMouseClickedFlag::Up))
			{
				ScrollerInDragging = false;
				CallWidgetUnlockFocusID();
				CallWidgetSendMessage(Message);

				LostMouseFocus();
			}
			if (ClickedMessage->MousePosition.InsideRectangle(GetRegion()) ||
				(ClickedMessage->ClickedKey == VMouseKeyFlag::Left &&
				 ClickedMessage->ClickedMethod == VMouseClickedFlag::Down))
			{
				ScrollerDragOriginPoint = ClickedMessage->MousePosition;

				ScrollerDragOriginPoint.Offset(-GetX(), -GetY());
			}

			break;
		}
		case VMessageType::KillFocusMessage: {
			ScrollerInDragging = false;
			CallWidgetUnlockFocusID();
			LostMouseFocus();

			break;
		}
		case VMessageType::MouseMoveMessage: {
			VMouseMoveMessage *MouseMoveMessage = static_cast<VMouseMoveMessage *>(Message);

			auto OffsetX = MouseMoveMessage->MousePosition.X - ScrollerDragOriginPoint.X;
			auto OffsetY = MouseMoveMessage->MousePosition.Y - ScrollerDragOriginPoint.Y;

			if (ScrollerType == VScrollerButtonType::Vertical)
			{
				if (OffsetY >= ScrollerMiniValue && OffsetY <= ScrollerMaxValue - GetHeight())
				{
					Move(GetX(), OffsetY);
				}
			}
			else
			{
				if (OffsetX >= ScrollerMiniValue && OffsetX <= ScrollerMaxValue - GetWidth())
				{
					Move(OffsetX, GetY());
				}
			}

			ScrollerOnTrigger.Emit();

			break;
		}
		}
	}
}

void VScrollerPushButton::SetMaxValue(const int &TargetValue)
{
	ScrollerMaxValue = TargetValue;
}
void VScrollerPushButton::SetMiniValue(const int &TargetValue)
{
	ScrollerMiniValue = TargetValue;
}
void VScrollerPushButton::SetScrollerType(const VScrollerButtonType &TargetType)
{
	ScrollerType = TargetType;
}

void VScrollerPushButton::LeftClickedDown()
{
	InAnimation = true;

	OldTheme	= Theme->LocalTheme;
	TargetTheme = Theme->ActiveTheme;

	Interpolator->Reset();
	AnimationFrameTimer.Start(0);

	Update();
}
void VScrollerPushButton::LeftClickedUp()
{
	InAnimation = true;

	OldTheme	= Theme->LocalTheme;
	TargetTheme = Theme->OnHoverTheme;

	Interpolator->Reset();
	AnimationFrameTimer.Start(0);

	Update();
}
void VScrollerPushButton::GotMouseFocus()
{
	if (!ScrollerInDragging)
	{
		InAnimation = true;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->OnHoverTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);

		OnMouseFocus.Emit();

		Update();
	}
}
void VScrollerPushButton::LostMouseFocus()
{
	if (!ScrollerInDragging)
	{
		InAnimation = true;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->StaticTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);

		LoseMouseFocus.Emit();

		Update();
	}
}
void VScrollerPushButton::CheckFrame()
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
VRect VScrollerPushButton::GetRegion()
{
	VRect Rect = VUIObject::GetRegion();

	Rect.Extended(
		static_cast<int>(Theme->LocalTheme.BorderThickness), static_cast<int>(Theme->LocalTheme.BorderThickness),
		static_cast<int>(Theme->LocalTheme.BorderThickness), static_cast<int>(Theme->LocalTheme.BorderThickness));
	return Rect;
}
void VScrollerPushButton::OnPaint(VCanvasPainter *Painter)
{
	Painter->BeginDraw();

	VSolidBrush BackgroundBrush(Theme->LocalTheme.BackgroundColor, CallWidgetGetRenderHandle());
	VPenBrush	PenBrush(Theme->LocalTheme.BorderColor, CallWidgetGetRenderHandle(), Theme->LocalTheme.BorderThickness);
	VSolidBrush TextBrush(Theme->LocalTheme.TextColor, CallWidgetGetRenderHandle());

	Painter->FillRoundedRectangle({static_cast<int>(Theme->LocalTheme.BorderThickness),
								   static_cast<int>(Theme->LocalTheme.BorderThickness), GetWidth(), GetHeight()},
								  Theme->LocalTheme.Radius, &PenBrush, &BackgroundBrush);

	Painter->EndDraw();
}

VViewScrollerButtonTheme *VScrollerPushButton::GetTheme()
{
	return Theme;
}

VScrollerPushButton::VScrollerPushButton(VUIObject *Parent, const VScrollerButtonType &Type) : VAbstractButton(Parent)
{
	Theme = new VViewScrollerButtonTheme(
		*(static_cast<VViewScrollerButtonTheme *>(GetTargetTheme(VUIThemeType::VViewScrollerPushButton))));
	Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);
	ScrollerType = Type;
}

VScrollerVertical::VScrollerVertical(VUIObject *Parent) : VAbstractButton(Parent)
{
	Theme = new VViewScrollerTheme(*(static_cast<VViewScrollerTheme *>(GetTargetTheme(VUIThemeType::VViewScroller))));
	Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

	InitViewButton();
	ResetViewButton();
}
VScrollerVertical::VScrollerVertical(const int &Width, const int &Height, VUIObject *Parent) : VAbstractButton(Parent)
{
	Theme = new VViewScrollerTheme(*(static_cast<VViewScrollerTheme *>(GetTargetTheme(VUIThemeType::VViewScroller))));
	Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

	InitViewButton();
	ResetViewButton();

	Resize(Width, Height);
}
VScrollerVertical::VScrollerVertical(const int &Width, const int &Height, const int &ViewHeight, VUIObject *Parent)
	: VAbstractButton(Parent)
{
	Theme = new VViewScrollerTheme(*(static_cast<VViewScrollerTheme *>(GetTargetTheme(VUIThemeType::VViewScroller))));
	Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

	ScrollerViewHeight = ViewHeight;

	InitViewButton();
	ResetViewButton();

	Resize(Width, Height);
}

void VScrollerVertical::SwitchDraggingStatusAnimation()
{
	InAnimation = true;

	OldTheme	= Theme->LocalTheme;
	TargetTheme = Theme->ActiveTheme;

	Interpolator->Reset();
	AnimationFrameTimer.Start(0);

	Update();
}
void VScrollerVertical::TurnToNormalStatusAnimation()
{
	InAnimation = true;

	OldTheme	= Theme->LocalTheme;
	TargetTheme = Theme->OnHoverTheme;

	Interpolator->Reset();
	AnimationFrameTimer.Start(0);

	Update();
}
void VScrollerVertical::GotMouseFocus()
{
	InAnimation = true;

	OldTheme	= Theme->LocalTheme;
	TargetTheme = Theme->OnHoverTheme;

	Interpolator->Reset();
	AnimationFrameTimer.Start(0);

	OnMouseHover = true;

	Update();
}
void VScrollerVertical::LostMouseFocus()
{
	InAnimation = true;

	OldTheme	= Theme->LocalTheme;
	TargetTheme = Theme->StaticTheme;

	Interpolator->Reset();
	AnimationFrameTimer.Start(0);

	OnMouseHover = false;

	Update();
}
void VScrollerVertical::CheckFrame()
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
VRect VScrollerVertical::GetRegion()
{
	VRect Rect = VUIObject::GetRegion();

	Rect.Extended(
		static_cast<int>(Theme->LocalTheme.BorderThickness), static_cast<int>(Theme->LocalTheme.BorderThickness),
		static_cast<int>(Theme->LocalTheme.BorderThickness), static_cast<int>(Theme->LocalTheme.BorderThickness));
	return Rect;
}
void VScrollerVertical::OnPaint(VCanvasPainter *Painter)
{
	Painter->BeginDraw();

	VSolidBrush BackgroundBrush(Theme->LocalTheme.BackgroundColor, CallWidgetGetRenderHandle());
	VPenBrush	PenBrush(Theme->LocalTheme.BorderColor, CallWidgetGetRenderHandle(), Theme->LocalTheme.BorderThickness);
	VSolidBrush TextBrush(Theme->LocalTheme.TextColor, CallWidgetGetRenderHandle());

	Painter->FillRoundedRectangle({static_cast<int>(Theme->LocalTheme.BorderThickness),
								   static_cast<int>(Theme->LocalTheme.BorderThickness), GetWidth(), GetHeight()},
								  Theme->LocalTheme.Radius, &PenBrush, &BackgroundBrush);

	Painter->EndDraw();
}

void VScrollerVertical::ScrollerValueOnChange()
{
	ValueOnChange.Emit(double(GetHeight()) / double(ScrollerButton->GetY()));
}
double VScrollerVertical::GetValue() const
{
	if (GetHeight() >= ScrollerViewHeight)
	{
		return 1.f;
	}

	return double(ScrollerButton->GetY()) / double(GetHeight() - ScrollerButton->GetHeight());
}

void VScrollerVertical::OnMessage(Core::VMessage *Message)
{
	if (Message->GetType() == VMessageType::MouseWheelMessage)
	{
		VMouseWheelMessage *WheelMessage = static_cast<VMouseWheelMessage *>(Message);

		if (WheelMessage->MousePosition.InsideRectangle(GetRegion()))
		{
			auto ScrollerButtonNewY = ScrollerButton->GetY() + (WheelMessage->WheelValue >= 0 ? -10 : 10);

			if (ScrollerButtonNewY >= 0 && ScrollerButtonNewY <= GetHeight() - ScrollerButton->GetHeight())
			{
				ScrollerButton->Move(ScrollerButton->GetX(), ScrollerButtonNewY);
			}
			else if (ScrollerButtonNewY < 0)
			{
				ScrollerButton->Move(ScrollerButton->GetX(), 0);
			}
			else
			{
				ScrollerButton->Move(ScrollerButton->GetX(), GetHeight() - ScrollerButton->GetHeight());
			}

			ScrollerValueOnChange();
		}
	}
}

void VScrollerVertical::InitViewButton()
{
	ScrollerButton = new VScrollerPushButton(this);

	ScrollerButton->OnMouseFocus.Connect(this, &VScrollerVertical::GotMouseFocus);
	ScrollerButton->LoseMouseFocus.Connect(this, &VScrollerVertical::LostMouseFocus);
	ScrollerButton->ScrollerOnTrigger.Connect(this, &VScrollerVertical::ScrollerValueOnChange);
}
void VScrollerVertical::ResetViewButton()
{
	if (ScrollerViewHeight > 0)
	{
		auto ViewRatioRegion = double(GetHeight()) / double(ScrollerViewHeight);

		ScrollerButton->Resize(GetWidth(), GetHeight() * ViewRatioRegion);

		if (ScrollerButton->GetHeight() <= 20)
		{
			ScrollerButton->Resize(GetWidth(), 20);
		}

		ScrollerButton->SetMiniValue(0);
		ScrollerButton->SetMaxValue(GetHeight());
	}
}

void VScrollerVertical::Resize(const int &Width, const int &Height)
{
	VUIObject::Resize(Width, Height);

	ResetViewButton();

	if (ScrollerButton->GetY() > Height - ScrollerButton->GetHeight())
	{
		ScrollerButton->Move(GetX(), Height - ScrollerButton->GetHeight());
	}
}

VViewScrollerTheme *VScrollerVertical::GetTheme()
{
	return Theme;
}

VScrollerPushButton *VScrollerVertical::GetScrollerButtonInstance()
{
	return ScrollerButton;
}

void VScrollerVertical::SetViewHeight(const int &Height)
{
	ScrollerViewHeight = Height;

	ResetViewButton();

	Update();
}
void VScrollerVertical::SetViewPercent(const double &ViewPercent)
{
	auto Percent = ViewPercent >= 1.f ? 1.f : ViewPercent;
	Percent		 = ViewPercent <= 0.f ? 0.f : ViewPercent;

	ScrollerButton->Move(0, GetHeight() * Percent);
}
void VScrollerVertical::SetViewPoint(const int &TargetPoint)
{
	auto MaxValue = GetHeight() - ScrollerButton->GetHeight();
	auto Point	  = TargetPoint >= MaxValue ? MaxValue : TargetPoint;
	Point		  = TargetPoint <= 0 ? 0 : TargetPoint;

	ScrollerButton->Move(0, Point);
}

VScrollerHorizontal::VScrollerHorizontal(VUIObject *Parent) : VAbstractButton(Parent)
{
	Theme = new VViewScrollerTheme(*(static_cast<VViewScrollerTheme *>(GetTargetTheme(VUIThemeType::VViewScroller))));
	Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

	InitViewButton();
	ResetViewButton();
}
VScrollerHorizontal::VScrollerHorizontal(const int &Width, const int &Height, VUIObject *Parent)
	: VAbstractButton(Parent)
{
	Theme = new VViewScrollerTheme(*(static_cast<VViewScrollerTheme *>(GetTargetTheme(VUIThemeType::VViewScroller))));
	Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

	InitViewButton();
	ResetViewButton();

	Resize(Width, Height);
}
VScrollerHorizontal::VScrollerHorizontal(const int &Width, const int &Height, const int &ViewHeight, VUIObject *Parent)
	: VAbstractButton(Parent)
{
	Theme = new VViewScrollerTheme(*(static_cast<VViewScrollerTheme *>(GetTargetTheme(VUIThemeType::VViewScroller))));
	Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

	ScrollerViewWidth = ViewHeight;

	InitViewButton();
	ResetViewButton();

	Resize(Width, Height);
}

void VScrollerHorizontal::SwitchDraggingStatusAnimation()
{
	InAnimation = true;

	OldTheme	= Theme->LocalTheme;
	TargetTheme = Theme->ActiveTheme;

	Interpolator->Reset();
	AnimationFrameTimer.Start(0);

	Update();
}
void VScrollerHorizontal::TurnToNormalStatusAnimation()
{
	InAnimation = true;

	OldTheme	= Theme->LocalTheme;
	TargetTheme = Theme->OnHoverTheme;

	Interpolator->Reset();
	AnimationFrameTimer.Start(0);

	Update();
}
void VScrollerHorizontal::GotMouseFocus()
{
	InAnimation = true;

	OldTheme	= Theme->LocalTheme;
	TargetTheme = Theme->OnHoverTheme;

	Interpolator->Reset();
	AnimationFrameTimer.Start(0);

	OnMouseHover = true;

	Update();
}
void VScrollerHorizontal::LostMouseFocus()
{
	InAnimation = true;

	OldTheme	= Theme->LocalTheme;
	TargetTheme = Theme->StaticTheme;

	Interpolator->Reset();
	AnimationFrameTimer.Start(0);

	OnMouseHover = false;

	Update();
}

VViewScrollerTheme *VScrollerHorizontal::GetTheme()
{
	return Theme;
}

VScrollerPushButton *VScrollerHorizontal::GetScrollerButtonInstance()
{
	return ScrollerButton;
}

void VScrollerHorizontal::CheckFrame()
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
VRect VScrollerHorizontal::GetRegion()
{
	VRect Rect = VUIObject::GetRegion();

	Rect.Extended(
		static_cast<int>(Theme->LocalTheme.BorderThickness), static_cast<int>(Theme->LocalTheme.BorderThickness),
		static_cast<int>(Theme->LocalTheme.BorderThickness), static_cast<int>(Theme->LocalTheme.BorderThickness));
	return Rect;
}
void VScrollerHorizontal::OnPaint(VCanvasPainter *Painter)
{
	Painter->BeginDraw();

	VSolidBrush BackgroundBrush(Theme->LocalTheme.BackgroundColor, CallWidgetGetRenderHandle());
	VPenBrush	PenBrush(Theme->LocalTheme.BorderColor, CallWidgetGetRenderHandle(), Theme->LocalTheme.BorderThickness);
	VSolidBrush TextBrush(Theme->LocalTheme.TextColor, CallWidgetGetRenderHandle());

	Painter->FillRoundedRectangle({static_cast<int>(Theme->LocalTheme.BorderThickness),
								   static_cast<int>(Theme->LocalTheme.BorderThickness), GetWidth(), GetHeight()},
								  Theme->LocalTheme.Radius, &PenBrush, &BackgroundBrush);

	Painter->EndDraw();
}

void VScrollerHorizontal::ScrollerValueOnChange()
{
	ValueOnChange.Emit(double(GetWidth()) / double(ScrollerButton->GetX()));
}
double VScrollerHorizontal::GetValue() const
{
	if (GetWidth() >= ScrollerViewWidth)
	{
		return 1.f;
	}

	return double(ScrollerButton->GetX()) / double(GetWidth() - ScrollerButton->GetWidth());
}

void VScrollerHorizontal::OnMessage(Core::VMessage *Message)
{
	if (Message->GetType() == VMessageType::MouseWheelMessage)
	{
		VMouseWheelMessage *WheelMessage = static_cast<VMouseWheelMessage *>(Message);

		if (WheelMessage->MousePosition.InsideRectangle(GetRegion()))
		{
			auto ScrollerButtonNewX = ScrollerButton->GetX() + (WheelMessage->WheelValue >= 0 ? -10 : 10);

			if (ScrollerButtonNewX >= 0 && ScrollerButtonNewX <= GetWidth() - ScrollerButton->GetWidth())
			{
				ScrollerButton->Move(ScrollerButtonNewX, ScrollerButton->GetY());
			}
			else if (ScrollerButtonNewX < 0)
			{
				ScrollerButton->Move(0, ScrollerButton->GetY());
			}
			else
			{
				ScrollerButton->Move(GetWidth() - ScrollerButton->GetWidth(), GetWidth() - ScrollerButton->GetWidth());
			}

			ScrollerValueOnChange();
		}
	}
}

void VScrollerHorizontal::InitViewButton()
{
	ScrollerButton = new VScrollerPushButton(this, VScrollerButtonType::Horizontal);

	ScrollerButton->OnMouseFocus.Connect(this, &VScrollerHorizontal::GotMouseFocus);
	ScrollerButton->LoseMouseFocus.Connect(this, &VScrollerHorizontal::LostMouseFocus);
	ScrollerButton->ScrollerOnTrigger.Connect(this, &VScrollerHorizontal::ScrollerValueOnChange);

	ScrollerButton->Move(0, 0);
}
void VScrollerHorizontal::ResetViewButton()
{
	if (ScrollerViewWidth > 0)
	{
		auto ViewRatioRegion = double(GetWidth()) / double(ScrollerViewWidth);

		ScrollerButton->Resize(GetWidth() * ViewRatioRegion, GetHeight());

		if (ScrollerButton->GetWidth() <= 20)
		{
			ScrollerButton->Resize(ViewRatioRegion, 20);
		}

		ScrollerButton->SetMiniValue(0);
		ScrollerButton->SetMaxValue(GetWidth());
	}
}

void VScrollerHorizontal::Resize(const int &Width, const int &Height)
{
	VUIObject::Resize(Width, Height);

	ResetViewButton();

	if (ScrollerButton->GetX() > Width - ScrollerButton->GetWidth())
	{
		ScrollerButton->Move(Width - ScrollerButton->GetWidth(), GetY());
	}
}

void VScrollerHorizontal::SetViewWidth(const int &Width)
{
	ScrollerViewWidth = Width;

	ResetViewButton();

	Update();
}
void VScrollerHorizontal::SetViewPercent(const double &ViewPercent)
{
	auto Percent = ViewPercent >= 1.f ? 1.f : ViewPercent;
	Percent		 = ViewPercent <= 0.f ? 0.f : ViewPercent;

	ScrollerButton->Move(GetWidth() * Percent, 0);
}
void VScrollerHorizontal::SetViewPoint(const int &TargetPoint)
{
	auto MaxValue = GetWidth() - ScrollerButton->GetWidth();
	auto Point	  = TargetPoint >= MaxValue ? MaxValue : TargetPoint;
	Point		  = TargetPoint <= 0 ? 0 : TargetPoint;

	ScrollerButton->Move(Point, 0);
}

} // namespace Core

VLIB_END_NAMESPACE