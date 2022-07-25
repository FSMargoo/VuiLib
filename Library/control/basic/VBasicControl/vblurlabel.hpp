/*
 * VBlurLabel.hpp
 *	@description  : A Blur Label
*/

#pragma once

#include "vuiobject.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VBlurLabel class:
 *	@description  : A Blur Label
*/
class VBlurLabel : public VUIObject {
private:
	VBlurLabelTheme* Theme;

public:
	/*
	 * Build up Functional
	*/

	VBlurLabel(VUIObject* Parent) : VUIObject(Parent) {
		Theme = new VBlurLabelTheme(*(static_cast<VBlurLabelTheme*>(SearchThemeFromParent(VBLURLABEL_THEME))));
	}

	void OnPaint(VCanvas* Canvas) override {
		VImage BackgroundImage(GetWidth(), GetHeight());

		for (int Y = GetY(), SY = 0; Y < Surface()->Rect.bottom; ++Y, ++SY) {
			for (int X = GetX(), SX = 0; X < Surface()->Rect.right; ++X, ++SX) {
				BackgroundImage.SetPixel(SX, SY, GetParentCanvas()->GetPixel(X, Y));
			}
		}

		BackgroundImage.ApplyBlurEffect(Theme->BlurRadius);

		VPainterDevice Device(Canvas);
		VTextureBrush  Brush(BackgroundImage);
		VSolidBrush    MixBrusher(Theme->MixedColor);

		Device.SolidRoundedRectangle(&Brush      , { 0, 0, GetWidth(), GetHeight() }, Theme->Radius);
		Device.SolidRoundedRectangle(&MixBrusher , { 0, 0, GetWidth(), GetHeight() }, Theme->Radius);
	}

	/*
	 * SetBlurRadius functional:
	 *	@description  : Get the Blur Radius
	 */
	void SetBlurRadius(int Radius) {
		Theme->BlurRadius = Radius;

		UpdateObject();
	}

	void SetTheme(VBlurLabelTheme* NewTheme) {
		delete Theme;

		Theme = new VBlurLabelTheme(*NewTheme);
	}
	VBlurLabelTheme* GetTheme() {
		return Theme;
	}
};

VLIB_END_NAMESPACE