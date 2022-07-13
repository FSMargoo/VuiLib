/*
 * VBrush.hpp
 *	@description : The Brush Defition in VRender ( Gdiplus Wrapper )
 *	@brith		 : 2022/6.3
*/

#pragma once

#include "vrenderbasic.hpp"
#include "vcolor.hpp"
#include "vimage.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VBasicBrush class:
 *	@description : The basic brush in VRender, it only need a virtual functional
 *				   "GetNativeBrush"
*/
class VBasicBrush {
public:
	/* Empty Build Up Functional */
	VBasicBrush() {

	}

	/*
	 * GetNativeBrush virtual Functional:
	 *	@description : Get the Gdiplus Brush (Native Brush)
	*/
	virtual VGdiplus::Brush* GetNativeBrush() {
		return nullptr;
	}
};

/*
 * VSolidBrush class <- public VBasicBrush:
 *	@description : The Solid Brush
*/
class VSolidBrush : public VBasicBrush {
private:
	VMemoryPtr<VGdiplus::SolidBrush> NativeBrush;

public:
	/*
	 * operator= functional
	*/
	void operator=(const VSolidBrush& Brush) {
		NativeBrush.reset(static_cast<VGdiplus::SolidBrush*>(Brush.NativeBrush->Clone()));
	}

public:
	/*
	 * GetNativeBrush override Functional:
	 *	@description : See in VBasicBrush
	*/
	VGdiplus::Brush* GetNativeBrush() override {
		return NativeBrush.get();
	}

	/*
	 * Build up functional
	*/

	VSolidBrush(VColor Color) {
		NativeBrush.reset(new VGdiplus::SolidBrush(Color.GetNativeObject()));
	}
	VSolidBrush(const VSolidBrush& Brush) {
		NativeBrush.reset(static_cast<VGdiplus::SolidBrush*>(Brush.NativeBrush->Clone()));
	}

	/*
	 * GetColor Functional:
	 *	@description  : Get the Color of This Brush
	 *	@return value : The Color
	*/
	VColor GetColor() {
		VGdiplus::Color ObjectColor;
		NativeBrush->GetColor(&ObjectColor);

		return ObjectColor;
	}
	/*
	 * SetColor Functional:
	 *	@description  : Set the Color of This Brush
	*/
void   SetColor(VColor Color) {
	NativeBrush->SetColor(Color.GetNativeObject());
}
};

/*
 * VTexutreWrapStyle enum class:
 *	@description : The Wrap Of VTextureBrush
*/
enum class VTextureWrapStyle {
	Clamp, Tile, TileFlipX, TileFlipXY, TileFlipY
};

/*
 * VTextureBrush class <- public VBasicBrush:
 *	@description  : A Solid Brush Which is Texture Brush
*/
class VTextureBrush : public VBasicBrush {
private:
	VMemoryPtr<VGdiplus::TextureBrush> NativeBrush;

private:
	/*
	 * ConvertStyle Functional:
	 *	@description : Convert Gdiplus Wrap Style Into VTextureBrush Wrap Style
	*/
	VTextureWrapStyle  ConvertStyle(VGdiplus::WrapMode VWrapStyle) {
		switch (VWrapStyle) {
		case VGdiplus::WrapMode::WrapModeClamp: {
			return VTextureWrapStyle::Clamp;
		}
		case VGdiplus::WrapMode::WrapModeTile: {
			return VTextureWrapStyle::Tile;
		}
		case VGdiplus::WrapMode::WrapModeTileFlipX: {
			return VTextureWrapStyle::TileFlipX;
		}
		case VGdiplus::WrapMode::WrapModeTileFlipXY: {
			return VTextureWrapStyle::TileFlipXY;
		}
		case VGdiplus::WrapMode::WrapModeTileFlipY: {
			return VTextureWrapStyle::TileFlipY;
		}
		default: {
			return VTextureWrapStyle::Clamp;
		}
		}
	}
	/*
	 * ConvertStyle Functional:
	 *	@description : Convert VTextureBrush Wrap Style Into Gdiplus Wrap Style
	*/
	VGdiplus::WrapMode ConvertStyle(VTextureWrapStyle VWrapStyle) {
		switch (VWrapStyle) {
		case VTextureWrapStyle::Clamp: {
			return VGdiplus::WrapMode::WrapModeClamp;
		}
		case VTextureWrapStyle::Tile: {
			return VGdiplus::WrapMode::WrapModeTile;
		}
		case VTextureWrapStyle::TileFlipX: {
			return VGdiplus::WrapMode::WrapModeTileFlipX;
		}
		case VTextureWrapStyle::TileFlipXY: {
			return VGdiplus::WrapMode::WrapModeTileFlipXY;
		}
		case VTextureWrapStyle::TileFlipY: {
			return VGdiplus::WrapMode::WrapModeTileFlipY;
		}
		default: {
			return VGdiplus::WrapMode::WrapModeClamp;
		}
		}
	}

public:
	/*
	 * GetNativeBrush override Functional:
	 *	@description : See in VBasicBrush
	*/
	VGdiplus::Brush* GetNativeBrush() override {
		return NativeBrush.get();
	}

public:
	/*
	 * Build up Functional
	*/

	VTextureBrush(VImage ObjectImage, VTextureWrapStyle WrapStyle = VTextureWrapStyle::Clamp) {
		NativeBrush.reset(new VGdiplus::TextureBrush(ObjectImage.GetNativeImage(), ConvertStyle(WrapStyle)));
	}
	VTextureBrush(VImage* ObjectImage, VTextureWrapStyle WrapStyle = VTextureWrapStyle::Clamp) {
		NativeBrush.reset(new VGdiplus::TextureBrush(ObjectImage->GetNativeImage(), ConvertStyle(WrapStyle)));
	}

	/*
	 * SetWrapStyle Functional:
	 *	@description  : Set the Brush's Texure Wrap Style
	*/
	void              SetWrapStyle(VTextureWrapStyle Style) {
		NativeBrush->SetWrapMode(ConvertStyle(Style));
	}
	/*
	 * GetWrapStyle Functional:
	 *	@description  : Get the Brush's Texture Wrap Style
	 *	@return value : Texture Wrap Style
	*/
	VTextureWrapStyle GetWrapStyle() {
		return ConvertStyle(NativeBrush->GetWrapMode());
	}
};

VLIB_END_NAMESPACE