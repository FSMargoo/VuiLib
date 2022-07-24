/*
 * VIconButton.hpp
 *	@description : VLib's Icon Button
 *	@birth		 : 2022/6.10
*/

#pragma once

#include "vabstractButton.hpp"
#include "vbasicanimation.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VIconButton class:
 *	@description  : The Control Push Button
*/
class VIconButton : public VAbstractButton {
private:
	VIconButtonTheme* Theme;

	VColorInterpolator TextColor;
	VColorInterpolator BackgroundColor;
	VColorInterpolator LineColor;

public:
	/*
	 * Build up Functional
	*/

	VIconButton(VUIObject* Parent) : VAbstractButton(Parent),
		TextColor(0.1, VInterpolatorType::AccelerateInterpolator),
		BackgroundColor(0.1, VInterpolatorType::AccelerateInterpolator),
		LineColor(0.1, VInterpolatorType::AccelerateInterpolator) {
		Theme = new VIconButtonTheme(*(static_cast<VIconButtonTheme*>(SearchThemeFromParent(VICONBUTTON_THEME))));

		if (VUnlikely(Theme == nullptr)) {
			VAssertBox(L"Error! Can't Get the Native Theme Of VPushButton");
		}
	}
	VIconButton(VUIObject* Parent, VImage* Icon) : VAbstractButton(Parent),
		TextColor(0.1, VInterpolatorType::AccelerateInterpolator),
		BackgroundColor(0.1, VInterpolatorType::AccelerateInterpolator),
		LineColor(0.1, VInterpolatorType::AccelerateInterpolator) {
		Theme = new VIconButtonTheme(*(static_cast<VIconButtonTheme*>(SearchThemeFromParent(VICONBUTTON_THEME))));

		if (VUnlikely(Theme == nullptr)) {
			VAssertBox(L"Error! Can't Get the Native Theme Of VIconButton");
		}

		SetIcon(Icon);
	}

	/*
	 * OnPaint override Functional
	*/
	void OnPaint(VCanvas* Canvas) override {
		VPen        BorderPen(Theme->CurrentLineColor);
		VSolidBrush FillBrush(Theme->CurrentBackgroundColor);
		VSolidBrush PenBrush(Theme->CurrentTextColor);

		VPainterDevice Device(Canvas);

		Device.FillRoundedRectangle(&BorderPen, &FillBrush, GetSourceRect(), Theme->Radius);
		Device.DrawImage(Theme->IconImage, { GetWidth() / 2 - Theme->IconImage->GetWidth() / 2,
			GetHeight() / 2 - Theme->IconImage->GetHeight() / 2,
			GetWidth() / 2 - Theme->IconImage->GetWidth() / 2 + Theme->IconImage->GetWidth(),
			GetHeight() / 2 - Theme->IconImage->GetHeight() / 2 + Theme->IconImage->GetHeight() });
	}

	void LeftClickedDown() override {
		BackgroundColor.Start(Theme->CurrentBackgroundColor, Theme->OnClickedBackgroundColor);
		TextColor.Start(Theme->CurrentTextColor, Theme->OnClickedTextColor);
		LineColor.Start(Theme->CurrentLineColor, Theme->OnClickedLineColor);

		ButtonPushed.Emit();
	}
	void LeftClickedUp() override {
		BackgroundColor.Start(Theme->CurrentBackgroundColor, Theme->OnHoverBackgroundColor);
		TextColor.Start(Theme->CurrentTextColor, Theme->OnHoverTextColor);
		LineColor.Start(Theme->CurrentLineColor, Theme->OnHoverLineColor);
	}
	void GotMouseFocus() override {
		BackgroundColor.Start(Theme->CurrentBackgroundColor, Theme->OnHoverBackgroundColor);
		TextColor.Start(Theme->CurrentTextColor, Theme->OnHoverTextColor);
		LineColor.Start(Theme->CurrentLineColor, Theme->OnHoverLineColor);
	}
	void LosedMouseFocus() override {
		BackgroundColor.Start(Theme->CurrentBackgroundColor, Theme->BackgroundColor);
		TextColor.Start(Theme->CurrentTextColor, Theme->TextColor);
		LineColor.Start(Theme->CurrentLineColor, Theme->LineColor);
	}

public:
	/*
	 * SetIcon Functional:
	 *	@description  : Set the Button's Icon
	*/
	void SetIcon(VImage* Icon) {
		Theme->IconImage = Icon;
	}

	VImage* GetIcon() const {
		return Theme->IconImage;
	}

	void CheckFrame() override {
		if (BackgroundColor.IsAnimationEnd() == false ||
			TextColor.IsAnimationEnd() == false ||
			LineColor.IsAnimationEnd() == false) {
			Theme->CurrentBackgroundColor = BackgroundColor.GetOneFrame();
			Theme->CurrentTextColor = TextColor.GetOneFrame();
			Theme->CurrentLineColor = LineColor.GetOneFrame();

			Update();
		}
	}

	void SetTheme(VIconButtonTheme* NewTheme) {
		delete Theme;

		Theme = new VIconButtonTheme(*Theme);

		Update();
	}
};

VLIB_END_NAMESPACE