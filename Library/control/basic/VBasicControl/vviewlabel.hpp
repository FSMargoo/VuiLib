/*
 * VViewLabel.hpp
 *	@description : A View Label
 *	@birth		 : 2022/7.14
*/

#pragma once

#include "vuiobject.hpp"

VLIB_BEGIN_NAMESPACE

class VViewLabel : public VUIObject {
private:
	VViewLabelTheme* Theme;

public:
	VViewLabel(int Width, int Height, VUIObject* Parent) : VUIObject(Parent) {
		Resize(Width, Height);

		Theme = new VViewLabelTheme(*(static_cast<VViewLabelTheme*>(SearchThemeFromParent(VVIEWLABEL_THEME))));
	}

	void OnPaint(VCanvas* Canvas) override {
		if (Theme->EnableBoxShadow == false) {
			VPainterDevice Device(Canvas);
			VSolidBrush    Brush(Theme->BackgroundColor);

			Device.SolidRoundedRectangle(&Brush, { 0, 0, GetWidth(), GetHeight() }, Theme->Radius);
		}
		else {
			VPainterDevice Device(Canvas);
			VSolidBrush    Brush(Theme->BackgroundColor);

			Device.DrawBoxShadow(Theme->BoxShadowColor,
				VBoxShadowHelper::GetShadowRect(GetWidth(), GetHeight(), Theme->BoxShadowPixel),
				Theme->Radius, Theme->BoxShadowPixel);
			Device.SolidRoundedRectangle(&Brush, 
				VBoxShadowHelper::GetShadowElementRect(GetWidth(), GetHeight(), Theme->BoxShadowPixel),
				Theme->Radius);
		}
	}

	void SetBackgroundColor(VColor Color) {
		Theme->BackgroundColor = Color;

		Update();
	}

	void SetTheme(VViewLabelTheme* NewTheme) {
		delete Theme;

		Theme = new VViewLabelTheme(*NewTheme);
	}

	VViewLabelTheme* GetTheme() {
		return Theme;
	}
};

VLIB_END_NAMESPACE