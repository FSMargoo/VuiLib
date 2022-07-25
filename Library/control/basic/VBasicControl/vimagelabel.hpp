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
private:
	VImage* ZoomedImage = nullptr;

	VImage* ResizeImage(int Width, int Height) {
		VImage* Image = new VImage(Width, Height);

		VPainterDevice Device(Image);
		Device.DrawImage(Theme->Image, { 0, 0, Width, Height });

		return Image;
	}

private:
	VImageLabelTheme* Theme;

public:
	VImageLabel(VImage* Image, VUIObject* Parent) : VUIObject(Parent) {
		Theme = new VImageLabelTheme(*(static_cast<VImageLabelTheme*>(SearchThemeFromParent(VIMAGELABEL_THEME))));

		Theme->Image = Image;
	}

	void OnPaint(VCanvas* Canvas) override {
		if (Theme->Image != nullptr) {
			if (Theme->ImageRepeat == true) {
				VPainterDevice Device(Canvas);
				VTextureBrush  Brush(Theme->Image);
				VSolidBrush    DbgBrush(VColor(255, 255, 255));

				Brush.SetWrapStyle(VTextureWrapStyle::Tile);

				Device.SolidRoundedRectangle(&Brush, Surface()->Rect, Theme->Radius);
			}
			else {
				if (ZoomedImage == nullptr) {
					ZoomedImage = Theme->Image;
				}
				if (ZoomedImage->GetWidth()  != Surface()->Rect.GetWidth() &&
					ZoomedImage->GetHeight() != Surface()->Rect.GetHeight()) {
					if (ZoomedImage != Theme->Image) {
						delete ZoomedImage;
					}

					ZoomedImage = ResizeImage(Surface()->Rect.GetWidth(), Surface()->Rect.GetHeight());
				}

				VPainterDevice Device(Canvas);
				VTextureBrush  Brush(ZoomedImage);
				VSolidBrush    DbgBrush(VColor(255, 255, 255));

				Device.SolidRoundedRectangle(&Brush, Surface()->Rect, Theme->Radius);
			}
		}
	}

	VImage* GetImage() const {
		return Theme->Image;
	}

	/*
	 * SetImage functional:
	 *	@description  : Set the Image
	*/
	void SetImage(VImage* Image) {
		Theme->Image = Image;

		UpdateObject();
	}

	void SetRadius(VPoint Radius) {
		Theme->Radius = Radius;

		Update();
	}

	void SetTheme(VImageLabelTheme* NewTheme) {
		delete Theme;

		Theme = new VImageLabelTheme(*NewTheme);

		Update();
	}

	VImageLabelTheme* GetTheme() {
		return Theme;
	}
};

VLIB_END_NAMESPACE