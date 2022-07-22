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
		VPainterDevice Device(Canvas);
		VSolidBrush    Brush(Theme->BackgroundColor);
		
		Device.SolidRoundedRectangle(&Brush, { 0, 0, GetWidth(), GetHeight() }, Theme->Radius);
	}
};

VLIB_END_NAMESPACE