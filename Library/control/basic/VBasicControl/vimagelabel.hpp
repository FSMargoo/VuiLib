/*
 * VImageLabel.hpp
 *	@description  : A Label Which Show Image
*/

#pragma once

#include "vuiobject.hpp"
#include "vboxshadowhelper.hpp"

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

	VRect SurfaceRegion() override {
		if (Theme->EnableBoxShadow == false) {
			return Surface()->Rect;
		}

		auto ShadowRect = Surface()->Rect;

		ShadowRect.left = ShadowRect.left - Theme->BoxShadowPixel;
		ShadowRect.top = ShadowRect.top - Theme->BoxShadowPixel;
		ShadowRect.bottom = ShadowRect.bottom + Theme->BoxShadowPixel * 2;
		ShadowRect.right = ShadowRect.right + Theme->BoxShadowPixel * 2;

		return ShadowRect;
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
				if (Theme->EnableBoxShadow == false) {
					VPainterDevice Device(Canvas);
					VTextureBrush  Brush(Theme->Image);

					Brush.SetWrapStyle(VTextureWrapStyle::Tile);

					Device.SolidRoundedRectangle(&Brush, Surface()->Rect, Theme->Radius);
				}
				else {
					VPainterDevice Device(Canvas);
					VTextureBrush  Brush(Theme->Image);

					Brush.SetWrapStyle(VTextureWrapStyle::Tile);

					Device.DrawBoxShadow(Theme->BoxShadowColor,
						VBoxShadowHelper::GetShadowRect(GetWidth(), GetHeight(), Theme->BoxShadowPixel),
						Theme->Radius, Theme->BoxShadowPixel);
					Device.SolidRoundedRectangle(&Brush,
						VBoxShadowHelper::GetShadowElementRect(GetWidth(), GetHeight(), Theme->BoxShadowPixel)
						, Theme->Radius);
				}
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

				if (Theme->EnableBoxShadow == false) {
					VPainterDevice Device(Canvas);
					VTextureBrush  Brush(ZoomedImage);

					Brush.SetWrapStyle(VTextureWrapStyle::Tile);

					Device.SolidRoundedRectangle(&Brush, Surface()->Rect, Theme->Radius);
				}
				else {
					VPainterDevice Device(Canvas);
					VTextureBrush  Brush(ZoomedImage);

					Brush.SetWrapStyle(VTextureWrapStyle::Tile);

					Device.DrawBoxShadow(Theme->BoxShadowColor,
						VBoxShadowHelper::GetShadowRect(GetWidth(), GetHeight(), Theme->BoxShadowPixel),
						Theme->Radius, Theme->BoxShadowPixel);
					Device.SolidRoundedRectangle(&Brush,
						VBoxShadowHelper::GetShadowElementRect(GetWidth(), GetHeight(), Theme->BoxShadowPixel)
						, Theme->Radius);
				}
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