#include "../include/core/control/vdropdown.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
bool VDropContext::IsGroup()
{
	return false;
}
bool VDropGroup::IsGroup()
{
	return true;
}
VDropDownContenxt::VDropDownContenxt(VUIObject *Object) : VUIObject(Object)
{
	Theme = new VDropDownContextTheme(
		*(static_cast<VDropDownContextTheme *>(GetTargetTheme(VUIThemeType::VDropDownContext))));

	SetDropContextView();
}
VDropDownContenxt::VDropDownContenxt(const int &Width, const int &Height, VUIObject *Parent) : VUIObject(Parent)
{
	Theme = new VDropDownContextTheme(
		*(static_cast<VDropDownContextTheme *>(GetTargetTheme(VUIThemeType::VDropDownContext))));

	SetDropContextView();

	Resize(Width, Height);
}
VDropDownContenxt::~VDropDownContenxt()
{
	delete Theme;
}
void VDropDownContenxt::SetDropContextView()
{
	SetShadowStats(true);
	SetShadowColor(Theme->ShadowColor);
	SetShadowRadius(Theme->ShadowPixel);

	ViewLabel = new VViewLabel(0, 0, this);
	ViewLabel->SetTieWithWheel(true);
	ViewLabel->SetVerticalAlign(VViewLabelVerticalAlign::Right);

	ViewLabel->GetHorizontalScrollerInstance()->Hide();
	ViewLabel->GetVerticalScrollerInstance()->Hide();
}
void VDropDownContenxt::Resize(const int &Width, const int &Height)
{
	ViewLabel->Resize(Width, Height);

	if (Height > ViewLabel->GetViewRegion().GetHeight())
	{
		ViewLabel->SetViewRegion({Width, Height});
	}
	else
	{
		ViewLabel->SetViewRegion({Width, ViewLabel->GetViewRegion().GetHeight()});
	}

	VUIObject::Resize(Width, Height);

	ViewLabel->GetHorizontalScrollerInstance()->Hide();
	ViewLabel->GetVerticalScrollerInstance()->Hide();
}
VDropDownContextTheme *VDropDownContenxt::GetTheme()
{
	return Theme;
}
void VDropDownContenxt::SetTheme(VDropDownContextTheme *TargetTheme)
{
	Theme = TargetTheme;
}
void VDropDownContenxt::SetLayout()
{
	if (Context.empty())
	{
		return;
	}

	int Y = 20;

	for (auto &Instance : Context)
	{
		if (Instance->Instance != nullptr)
		{
			delete Instance->Instance;
		}

		if (Instance->IsGroup())
		{
			Instance->Instance	  = new VTextLabel(ViewLabel);
			VTextLabel *TextLabel = static_cast<VTextLabel *>(Instance->Instance);

			TextLabel->SetTextColor(Theme->TitleColor);
			delete TextLabel->GetTheme()->LabelFont;
			TextLabel->GetTheme()->LabelFont = Theme->GroupFont;
			TextLabel->SetPlainText(Instance->Text);

			TextLabel->Move(16, Y);
			TextLabel->ResizeByText();

			Y += Theme->GroupFont->GetTextSize() + 12;
		}
		else
		{
			Instance->Instance		= new VPushButton(ViewLabel);
			VPushButton *PushButton = static_cast<VPushButton *>(Instance->Instance);

			delete PushButton->GetTheme()->LabelFont;

			PushButton->GetTheme()->LabelFont = Theme->ItemFont;

			PushButton->GetTheme()->StaticTheme.BackgroundColor	 = VColor::FromBYTERGBA(255, 255, 255, 0);
			PushButton->GetTheme()->StaticTheme.BorderColor		 = VColor::FromBYTERGBA(255, 255, 255, 0);
			PushButton->GetTheme()->StaticTheme.TextColor		 = Theme->ItemColor;
			PushButton->GetTheme()->StaticTheme.Radius			 = {0, 0};
			PushButton->GetTheme()->OnHoverTheme.BackgroundColor = VColor::FromBYTERGBA(255, 255, 255, 60);
			PushButton->GetTheme()->OnHoverTheme.BorderColor	 = VColor::FromBYTERGBA(255, 255, 255, 0);
			PushButton->GetTheme()->OnHoverTheme.TextColor		 = Theme->ItemColor;
			PushButton->GetTheme()->OnHoverTheme.Radius			 = {0, 0};
			PushButton->GetTheme()->ActiveTheme.BackgroundColor	 = VColor::FromBYTERGBA(255, 255, 255, 60);
			PushButton->GetTheme()->ActiveTheme.BorderColor		 = VColor::FromBYTERGBA(255, 255, 255, 0);
			PushButton->GetTheme()->ActiveTheme.TextColor		 = Theme->ItemColor;
			PushButton->GetTheme()->ActiveTheme.Radius			 = {0, 0};
			PushButton->GetTheme()->LocalTheme					 = PushButton->GetTheme()->StaticTheme;

			PushButton->Resize(GetWidth(), 60);
			PushButton->SetPlainText(VStr("\t") + Instance->Text);
			PushButton->Move(0, Y);

			Y += 60;

			PushButton->_PtrButtonPushed.Connect(this, &VDropDownContenxt::ButtonSignalEmit);
		}
	}

	if (!Context[Context.size() - 1]->IsGroup())
	{
		Y -= 28;
	}
	else
	{
		Y -= 12;
	}

	ViewLabel->Resize(GetWidth(), GetHeight());
	if (Y + 10 > GetHeight())
	{
		ViewLabel->SetViewRegion({GetWidth(), Y + 10});
	}
	else
	{
		ViewLabel->SetViewRegion({GetWidth(), GetHeight()});
	}

	ViewLabel->GetHorizontalScrollerInstance()->Hide();
	ViewLabel->GetVerticalScrollerInstance()->Hide();
}
void VDropDownContenxt::ButtonSignalEmit(VAbstractButton *Button)
{
	for (auto &Instance : Context)
	{
		if (Instance->Instance == Button)
		{
			Instance->OnTrigger.Emit(Instance);
		}
	}
}

void VDropDownContenxt::OnPaint(VCanvasPainter *Painter)
{
	VPenBrush	BorderPen(Theme->BorderColor, CallWidgetGetRenderHandle(), Theme->BorderThickness);
	VSolidBrush FillBrush(Theme->BackgroundColor, CallWidgetGetRenderHandle());

	Painter->BeginDraw();

	Painter->FillRoundedRectangle({0, 0, GetWidth(), GetHeight()}, Theme->Radius, &BorderPen, &FillBrush);

	Painter->EndDraw();
}

VDropDown::VDropDown(VUIObject *Parent) : VAbstractButton(Parent)
{
	Theme		 = new VDropDownTheme(*(static_cast<VDropDownTheme *>(GetTargetTheme(VUIThemeType::VDropDown))));
	Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);
	Complex		 = new VDropDownContenxt(GetWidth(), 250, Parent);

	Complex->Move(GetX(), GetY() + GetHeight() + 4);
	Complex->Hide();

	ComplexPositionAnimation = new VPositionAnimation(
		Complex, VAnimationCurveFactory::GetCurve(Core::VAnimationCurveFlag::EaseInCubicCurve), 100);
	ComplexOpacityAnimation = new VOpacityAnimation(
		Complex, VAnimationCurveFactory::GetCurve(Core::VAnimationCurveFlag::EaseInCubicCurve), 100);

	ComplexOpacityAnimation->AnimationEnd.Connect(this, &VDropDown::ContextDialogAnimationEnd);

	Update();
}
VDropDown::VDropDown(const int &Width, const int &Height, VUIObject *Parent) : VAbstractButton(Parent)
{
	Theme		 = new VDropDownTheme(*(static_cast<VDropDownTheme *>(GetTargetTheme(VUIThemeType::VDropDown))));
	Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);
	Complex		 = new VDropDownContenxt(GetWidth(), 250, Parent);

	Resize(Width, Height);

	Complex->Move(GetX(), GetY() + GetHeight() + 4);
	Complex->Hide();

	ComplexPositionAnimation = new VPositionAnimation(
		Complex, VAnimationCurveFactory::GetCurve(Core::VAnimationCurveFlag::EaseInCubicCurve), 100);
	ComplexOpacityAnimation = new VOpacityAnimation(
		Complex, VAnimationCurveFactory::GetCurve(Core::VAnimationCurveFlag::EaseInCubicCurve), 100);

	ComplexOpacityAnimation->AnimationEnd.Connect(this, &VDropDown::ContextDialogAnimationEnd);

	Update();
}
VDropDown::~VDropDown() noexcept
{
	delete Theme;
	delete Interpolator;
}
void VDropDown::Resize(const int &Width, const int &Height)
{
	VUIObject::Resize(Width, Height);

	Complex->Resize(Width, 250);
	Complex->Move(GetX(), GetY() + GetHeight() + 4);
}
void VDropDown::Move(const int &X, const int &Y)
{
	VUIObject::Move(X, Y);

	Complex->Move(GetX(), GetY() + GetHeight() + 4);
}
VRect VDropDown::GetRegion()
{
	VRect Rect = VUIObject::GetRegion();

	Rect.Extended(
		static_cast<int>(Theme->LocalTheme.BorderThickness), static_cast<int>(Theme->LocalTheme.BorderThickness),
		static_cast<int>(Theme->LocalTheme.BorderThickness), static_cast<int>(Theme->LocalTheme.BorderThickness));
	return Rect;
}
void VDropDown::OnPaint(VCanvasPainter *Painter)
{
	Painter->BeginDraw();

	VSolidBrush BackgroundBrush(Theme->LocalTheme.BackgroundColor, CallWidgetGetRenderHandle());
	VPenBrush	PenBrush(Theme->LocalTheme.BorderColor, CallWidgetGetRenderHandle(), Theme->LocalTheme.BorderThickness);
	VSolidBrush TextBrush(Theme->LocalTheme.TextColor, CallWidgetGetRenderHandle());

	Painter->FillRoundedRectangle({static_cast<int>(Theme->LocalTheme.BorderThickness),
								   static_cast<int>(Theme->LocalTheme.BorderThickness), GetWidth(), GetHeight()},
								  Theme->LocalTheme.Radius, &PenBrush, &BackgroundBrush);

	Painter->DrawString(Theme->PlainText,
						{static_cast<int>(Theme->LocalTheme.BorderThickness) + 10,
						 static_cast<int>(Theme->LocalTheme.BorderThickness), GetWidth(), GetHeight()},
						Theme->LabelFont, &TextBrush);

	VCanvasPainter *Icon;
	if (InSelect)
	{
		Icon =
			VNativeIcon::DropDownDownIcon(Theme->IconColor, 15, 8, CallWidgetGetRenderHandle(), Theme->IconThickness);
		Painter->DrawCanvas({GetWidth() - 25, GetHeight() / 2 - 4, GetWidth() - 10, GetHeight() / 2 + 4}, Icon,
							{0, 0, 15, 8}, 1.f);
	}
	else
	{
		Icon = VNativeIcon::DropDownUpIcon(Theme->IconColor, 15, 8, CallWidgetGetRenderHandle(), Theme->IconThickness);
		Painter->DrawCanvas({GetWidth() - 25, GetHeight() / 2 - 4, GetWidth() - 10, GetHeight() / 2 + 4}, Icon,
							{0, 0, 15, 8}, 1.f);
	}

	delete Icon;

	Painter->EndDraw();
}

void VDropDown::SetTheme(Core::VDropDownTheme *NewTheme)
{
	Theme = NewTheme;
}
VDropDownTheme *VDropDown::GetTheme()
{
	return Theme;
}
void VDropDown::CheckFrame()
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
void VDropDown::AddContext(VDropContextBase *Context)
{
	Context->OnTrigger.Connect(this, &VDropDown::ContextOnClicked);
	Complex->Context.push_back(Context);
	Complex->SetLayout();
}
void VDropDown::ContextOnClicked(VDropContextBase *Context)
{
	OnSelectContext	 = Context;
	Theme->PlainText = Context->Text;
	ComplexReset();

	ContextChanged.Emit(Context);
}
void VDropDown::ContextDialogAnimationEnd()
{
	if (!InSelect)
	{
		Complex->Hide();
		Complex->Move(Complex->GetX(), GetY() + GetHeight() + 10);
	}
}
void VDropDown::ComplexReset()
{
	InSelect = false;

	InAnimation = true;

	OldTheme	= Theme->LocalTheme;
	TargetTheme = Theme->StaticTheme;

	Interpolator->Reset();
	AnimationFrameTimer.Start(0);

	ComplexPositionAnimation->SetTargetPosition({Complex->GetX(), GetY() + GetHeight() - 10});
	ComplexOpacityAnimation->SetTargetValue(0);

	ComplexPositionAnimation->Start();
	ComplexOpacityAnimation->Start();
}

void VDropDown::LeftClickedDown()
{
	if (!InSelect)
	{
		InAnimation = true;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->ActiveTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);

		InSelect = true;

		Update();

		Complex->SetTransparency(0);
		Complex->Show();
		Complex->Move(Complex->GetX(), Complex->GetY() - 10);

		ComplexPositionAnimation->SetTargetPosition({Complex->GetX(), Complex->GetY() + 10});
		ComplexOpacityAnimation->SetTargetValue(255);

		ComplexPositionAnimation->Start();
		ComplexOpacityAnimation->Start();
	}
	else
	{
		ComplexReset();
	}
}
void VDropDown::LeftClickedUp()
{
}
void VDropDown::GotMouseFocus()
{
	if (!InSelect)
	{
		InSelect = false;

		InAnimation = true;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->OnHoverTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);
	}
}
void VDropDown::LostMouseFocus()
{
	if (!InSelect)
	{
		InAnimation = true;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->StaticTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);

		Update();
	}
}
void VDropDown::OnMessage(VMessage *Message)
{
	if (Message->GetType() == VMessageType::KillFocusMessage && InSelect)
	{
		ComplexReset();
	}
	if (Message->GetType() == VMessageType::MouseClickedMessage)
	{
		auto MouseMessage = static_cast<VMouseClickedMessage *>(Message);

		if (MouseMessage->ClickedKey == Left && MouseMessage->ClickedMethod == Down &&
			!MouseMessage->MousePosition.InsideRectangle(GetRegion()) &&
			!MouseMessage->MousePosition.InsideRectangle(Complex->GetRegion()))
		{
			ComplexReset();
		}
	}
}
VDropDownContenxt *VDropDown::GetComplex()
{
	return Complex;
}

} // namespace Core

VLIB_END_NAMESPACE