/*
 * VPushButton.hpp
 *	@description : VLib's Push Button
 *	@birth		 : 2022/6.10
*/

#pragma once

#include "vabstractButton.hpp"
#include "vbasicanimation.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VPushButton class:
 *	@description  : The Control Push Button
*/
class VPushButton : public VAbstractButton {
private:
	VPushButtonTheme* Theme;

	VColorInterpolator TextColor;
	VColorInterpolator BackgroundColor;
	VColorInterpolator LineColor;

public:
	/*
	 * Build up Functional
	*/

	VPushButton(VUIObject* Parent, std::wstring PlaneString = L"") : VAbstractButton(Parent),
		TextColor(0.1, VInterpolatorType::AccelerateInterpolator),
		BackgroundColor(0.1, VInterpolatorType::AccelerateInterpolator),
		LineColor(0.1, VInterpolatorType::AccelerateInterpolator) {
		Theme = new VPushButtonTheme(*(static_cast<VPushButtonTheme*>(SearchThemeFromParent(VPUSHBUTTON_THEME))));
		Theme->PlaneString = PlaneString;

		if (VUnlikely(Theme == nullptr)) {
			VAssertBox(L"Error! Can't Get the Native Theme Of VPushButton");
		}
	}

	/*
	 * OnPaint override Functional
	*/
	void OnPaint(VCanvas* Canvas) override {
		VPen        BorderPen(Theme->CurrentLineColor);
		VSolidBrush FillBrush(Theme->CurrentBackgroundColor);
		VSolidBrush PenBrush(Theme->CurrentTextColor);
		VFont       Font(Theme->FontFamily, Theme->FontSize);
		VFontFormat FontFormat;

		FontFormat.SetAlignment(VStringAlignment::AlignmentCenter);
		FontFormat.SetLineAlignment(VStringAlignment::AlignmentCenter);

		VPainterDevice Device(Canvas);

		Device.FillRoundedRectangle(&BorderPen, &FillBrush, GetSourceRect(), Theme->Radius);
		Device.DrawString(Theme->PlaneString, &Font, &PenBrush, &FontFormat, GetSourceRect());
	}

	void LeftClickedDown() override {
		BackgroundColor.Start(Theme->CurrentBackgroundColor, Theme->OnClickedBackgroundColor);
		TextColor.Start(Theme->CurrentTextColor, Theme->OnClickedTextColor);
		LineColor.Start(Theme->CurrentLineColor, Theme->OnClickedLineColor);
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
	 * SetPlaneText Functional:
	 *	@description  : Set the Button's Plane Text
	*/
	void SetPlaneText(std::wstring PlaneText) {
		Theme->PlaneString = PlaneText;

		Update();
	}
	/*
	 * SetTextSize Functional:
	 *	@description  : Set the Button Font's Size
	*/
	void SetTextSize(int Size) {
		Theme->FontSize = Size;

		Update();
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
};

VLIB_END_NAMESPACE