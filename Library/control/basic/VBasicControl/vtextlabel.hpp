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
		VPen        BorderPen(Theme->LineColor, Theme->BorderThinkness);
		VSolidBrush FillBrush(Theme->BackgroundColor);
		VSolidBrush PenBrush(Theme->TextColor);
		VFont       Font(Theme->FontFamily, Theme->FontSize);

		VPainterDevice Device(Canvas);

		BorderPen.SetStyle(Theme->BorderStyle);

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

	std::wstring GetPlaneText() const {
		return Theme->PlaneString;
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

	void Resize(int Width, int Height) override {
		if (Theme->AutoSize == false) {
			VUIObject::Resize(Width, Height);
		}
		else {
			ResizeAsPlaneText();
		}
	}

	void SetAutoSize(bool Status) {
		Theme->AutoSize = Status;

		Update();
	}

	void ResizeAsPlaneText() {
		VFont Font(Theme->FontFamily, Theme->FontSize);

		VUIObject::Resize(Font.GetWidth(Theme->PlaneString, Theme->FontFormat, Surface()->Rect), 
			Font.GetHeight(Theme->PlaneString, Theme->FontFormat, Surface()->Rect));
	}

	void SetTheme(VTextLabelTheme* NewTheme) {
		delete Theme;
		Theme = new VTextLabelTheme(*NewTheme);

		Update();
	}
};

VLIB_END_NAMESPACE