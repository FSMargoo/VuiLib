/*
 * VImageLabel.hpp
 *	@description  : A Label Which Show Image
*/

#pragma once

#include "vuiobject.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VImageLabel class:
 *	@description  : A Label Show a Image
*/
class VImageLabel : public VUIObject {
public:
	VImageLabelTheme* Theme;

public:
	VImageLabel(VImage* Image, VUIObject* Parent) : VUIObject(Parent) {
		Theme = new VImageLabelTheme(*(static_cast<VImageLabelTheme*>(SearchThemeFromParent(VIMAGELABEL_THEME))));

		Theme->Image = Image;
	}

	void OnPaint(VCanvas* Canvas) override {
		if (Theme->Image != nullptr) {
			VPainterDevice Device(Canvas);

			Device.DrawImage(Theme->Image, { 0, 0, GetWidth(), GetHeight() });
		}
	}

	/*
	 * SetImage functional:
	 *	@description  : Set the Image
	*/
	void SetImage(VImage* Image) {
		Theme->Image = Image;

		UpdateObject();
	}
};

VLIB_END_NAMESPACE