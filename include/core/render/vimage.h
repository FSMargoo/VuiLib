#pragma once

#include "vdirectxfactory.h"

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
	 VImage(const std::wstring& FromFile, ID2D1RenderTarget* DirectXRenderTarget);
	 ~VImage();

     void ApplyGassBlur(const int& Radius, ID2D1RenderTarget* DirectXRenderTarget);
};

};

VLIB_END_NAMESPACE
