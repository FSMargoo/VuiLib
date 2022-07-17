/*
 * VImage.hpp
 *	@description : The Image Defition in VRender ( Gdiplus Wrapper )
 *	@birth		 : 2022/6.3
*/

#pragma once

#include "vrenderbasic.hpp"
#include "vcolor.hpp"
#include "vpainterdevice.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VImage class <- public VPaintbleObject:
 *	@description : The Wrapper of Gdiplus Bitmap
*/
class VImage : public VPaintbleObject {
private:
	VMemoryPtr<VGdiplus::ImageAttributes>    NativeAttributes;
	VMemoryPtr<VGdiplus::Bitmap>             NativeImage;

private:
	void InitAttribute() {
		VGdiplus::ColorMatrix Matrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
										   0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
										   0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
										   0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
										   0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

		NativeAttributes->SetColorMatrix(&Matrix);
	}

public:
	/*
	 * operator= Functional
	*/
	void operator=(const VImage& Object) {
		NativeImage.reset(Object.NativeImage->Clone(
			0, 0, Object.NativeImage->GetWidth(), Object.NativeImage->GetHeight(),
			Object.NativeImage->GetPixelFormat()
		));
	}

	/*
	 * GetNativeImage Functional:
	 *	@description  : Get the Native Image Of Gdiplus
	*/
	VGdiplus::Bitmap*         GetNativeImage() {
		return NativeImage.get();
	}
	/*
	 * GetNativeAttribute Functional:
	 *	@description  : Get the Native Attribute Of Gdiplus
	*/
	VGdiplus::ImageAttributes* GetNativeAttributes() {
		return NativeAttributes.get();
	}

public:
	/*
	 * GetTargetBitmap override Functional:
	 *	@description  : Get the Target Paint Bitmap
	*/
	VGdiplus::Bitmap* GetTargetBitmap() override {
		/* Return Back Native Image */
		return GetNativeImage();
	}

public:
	/*
	 * IsEmpty Functional:
	 *	@description  : Is the Image Empty
	*/
	bool IsEmpty() {
		if (NativeImage.get() == nullptr) {
			return false;
		}

		return true;
	}

public:
	int GetWidth() {
		return NativeImage->GetWidth();
	}
	int GetHeight() {
		return NativeImage->GetHeight();
	}

public:
	/*
	 * Build up functional
	*/

	VImage()
		: VPaintbleObject(VPaintbleType::ImagePainter) {
		NativeImage.reset(nullptr);
		
		NativeAttributes.reset(new VGdiplus::ImageAttributes);

		InitAttribute();
	}

	VImage(const VImage& Object)
		: VPaintbleObject(VPaintbleType::ImagePainter) {
		NativeImage.reset(Object.NativeImage->Clone(
			0, 0, Object.NativeImage->GetWidth(), Object.NativeImage->GetHeight(),
			PixelFormat32bppPARGB
		));

		NativeAttributes.reset(Object.NativeAttributes->Clone());
	}

	VImage(int Width, int Height)
		: VPaintbleObject(VPaintbleType::ImagePainter) {
		NativeImage.reset(new VGdiplus::Bitmap(Width, Height, PixelFormat32bppPARGB));

		NativeAttributes.reset(new VGdiplus::ImageAttributes);
		InitAttribute();
	}

	VImage(HICON IconHandle)
		: VPaintbleObject(VPaintbleType::ImagePainter) {
		NativeImage.reset(new VGdiplus::Bitmap(IconHandle));

		NativeAttributes.reset(new VGdiplus::ImageAttributes);
		InitAttribute();
	}
	VImage(HBITMAP BitmapHandle, HPALETTE PaletteHandle)
		: VPaintbleObject(VPaintbleType::ImagePainter) {
		NativeImage.reset(new VGdiplus::Bitmap(BitmapHandle, PaletteHandle));

		NativeAttributes.reset(new VGdiplus::ImageAttributes);
		InitAttribute();
	}
	VImage(HINSTANCE ResourceHandle, std::wstring ResourceName)
		: VPaintbleObject(VPaintbleType::ImagePainter) {
		NativeImage.reset(new VGdiplus::Bitmap(ResourceHandle, ResourceName.c_str()));

		NativeAttributes.reset(new VGdiplus::ImageAttributes);
		InitAttribute();
	}

	VImage(std::wstring FilePath)
		: VPaintbleObject(VPaintbleType::ImagePainter) {
		NativeImage.reset(new VGdiplus::Bitmap(FilePath.c_str(), PixelFormat32bppPARGB));

		NativeAttributes.reset(new VGdiplus::ImageAttributes);
		InitAttribute();
	}

	/*
	 * GetPixel Functional:
	 *	@description  : Get the Target Pixel Color
	 *	@return value : Target Pixel Color
	 */
	VColor  GetPixel(int X, int Y) const {
		VGdiplus::Color PixelColor;
		NativeImage->GetPixel(X, Y, &PixelColor);

		return PixelColor;
	}
	/*
	 * SetPixel Functional:
	 *	@description  : Set the Target Pixel Color
	*/
	void    SetPixel(int X, int Y, VColor TargetColor) {
		NativeImage->SetPixel(X, Y, TargetColor.GetNativeObject());
	}

	/*
	 * GetHBITMAP Functional:
	 *	@description  : Convert VImage Into GDI Object HBITMAP
	 *	@return value : Converted Object
	*/
	HBITMAP GetHBITMAP(VColor BackgroundColor = VColor(0, 0, 0, 0)) {
		HBITMAP ConvertResult;
		NativeImage->GetHBITMAP(BackgroundColor.GetNativeObject(), &ConvertResult);

		return ConvertResult;
	}

	/*
	 * ApplyBlurEffect Functional:
	 *	@description  : Apply the Blur Effect
	*/
	void ApplyBlurEffect(int Radius) {
		VGdiplus::Blur       BlurEffect;
		VGdiplus::BlurParams Parameter;

		RECT                 ImageRect{ 0, 0, static_cast<LONG>(NativeImage->GetWidth()), static_cast<LONG>(NativeImage->GetHeight()) };

		Parameter.radius = Radius;
		Parameter.expandEdge = false;

		BlurEffect.SetParameters(&Parameter);

		NativeImage->ApplyEffect(&BlurEffect, &ImageRect);
	}

	/*
	 * SetTransparency Functional:
	 *	@description  : Set the Transparency Of Image
	*/
	void SetTransparency(int Transparency) {
		VGdiplus::ColorMatrix ColorMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
						   0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
						   0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
						   0.0f, 0.0f, 0.0f, float(Transparency) / 255, 0.0f,
						   0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

		NativeAttributes->SetColorMatrix(&ColorMatrix);
	}

	/*
	 * IsValidBitmap static Functional:
	 *	@description  : Judge a File is Bitmap or not
	*/
	static bool IsValidBitmap(std::wstring FilePath) {
		VGdiplus::Bitmap Bitmap(FilePath.c_str());

		if (Bitmap.GetLastStatus() == VGdiplus::Ok) {
			return true;
		}

		return false;
	}
};

VLIB_END_NAMESPACE