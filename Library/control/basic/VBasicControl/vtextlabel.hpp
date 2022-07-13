/*
 * VAbstractLabel.hpp
 *	@description : The Definition of Text Label
 *	@birth		 : 2022/6.10
*/

#pragma once

#include "vuiobject.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VTextLabel class:
 *	@description  : The Label Which Show Text
*/
class VTextLabel : public VUIObject {
private:
	VTextLabelTheme* Theme;

public:
	/*
	 * Build up Functional
	*/

	VTextLabel(VUIObject* Parent, std::wstring Text)
		: VUIObject(Parent) {
		Theme = new VTextLabelTheme(*(static_cast<VTextLabelTheme*>(SearchThemeFromParent(VTEXTLABEL_THEME))));

		Theme->PlaneString = Text;
	}

	/*
	 * OnPaint override:
	 *	@description  : Paint Functional
	*/
	void OnPaint(VCanvas* Canvas) override {
		VPen        BorderPen(Theme->LineColor);
		VSolidBrush FillBrush(Theme->BackgroundColor);
		VSolidBrush PenBrush(Theme->TextColor);
		VFont       Font(Theme->FontFamily, Theme->FontSize);

		VPainterDevice Device(Canvas);

		Device.FillRoundedRectangle(&BorderPen, &FillBrush, GetSourceRect(), Theme->Radius);
		Device.DrawString(Theme->PlaneString, &Font, &PenBrush, Theme->FontFormat, GetSourceRect());
	}

	/*
	 * SetPlaneText Functional:
	 *	@description  : Set the Text Label Text
	*/
	void SetPlaneText(std::wstring Text) {
		Theme->PlaneString = Text;

		UpdateObject();
	}


	void SetLineAlignment(VStringAlignment Alignment) {
		Theme->FontFormat->SetLineAlignment(Alignment);

		UpdateObject();
	}
	void SetAlignment(VStringAlignment Alignment) {
		Theme->FontFormat->SetAlignment(Alignment);

		UpdateObject();
	}

	void SetTextSize(int TextSize) {
		Theme->FontSize = TextSize;

		UpdateObject();
	}

	void SetBackgroundColor(VColor Color) {
		Theme->BackgroundColor = Color;

		UpdateObject();
	}
	void SetLineColor(VColor Color) {
		Theme->LineColor = Color;

		UpdateObject();
	}
	void SetTextColor(VColor Color) {
		Theme->TextColor = Color;

		UpdateObject();
	}

	void SetRadius(VPoint Radius) {
		Theme->Radius = Radius;

		UpdateObject();
	}
};

VLIB_END_NAMESPACE