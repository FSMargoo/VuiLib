#pragma once

#include "vdirectxfactory.h"
#include "vcolor.h"

#include <d2d1effects.h>
#include <d2d1_1.h>
#pragma comment(lib, "dxguid.lib")

VLIB_BEGIN_NAMESPACE

namespace Core {

class VImage {
 private:
	 ID2D1Bitmap* DirectXBitmap{};

 public:
	 ID2D1Bitmap* GetDirectXObject() {
		 return DirectXBitmap;
	 }

 public:
	 VImage(const VImage& Image);
	 VImage(int Width, int Height, ID2D1RenderTarget* DirectXRenderTarget);
     VImage(int Width, int Height, ID2D1RenderTarget* DirectXRenderTarget, D2D1_ALPHA_MODE DirectXAlphaMode);
	 VImage(const std::wstring& FromFile, ID2D1RenderTarget* DirectXRenderTarget);

     // Helper
     VImage(ID2D1Bitmap* HelperIBitmap) { DirectXBitmap = HelperIBitmap; }

	 ~VImage();

     int GetWidth() const;
     int GetHeight() const;

     void ApplyGassBlur(const int& Radius, ID2D1RenderTarget* DirectXRenderTarget);
     void ApplyShadowEffect(const float& ShadowRadius, const VColor& ShadowColor, ID2D1RenderTarget* DirectXRenderTarget, VPoint* Offset = nullptr);

	 static bool IsValidBitmapFile(const std::wstring& FilePath);
};

};

VLIB_END_NAMESPACE
