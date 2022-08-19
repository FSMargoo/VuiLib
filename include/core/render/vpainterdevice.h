// License(MIT)
// Athuor: Margoo
// This is the painter device file in vuilib
#pragma once

#include "vrenderbasic.h"

#include <Windows.h>

VLIB_BEGIN_NAMESPACE

namespace Core {

enum class VTargetRenderType {
	UnknownRender, DCRender, IWICImageRender, HWNDRender
};

class VDCRender {
 private:
	 ID2D1DCRenderTarget* DirectXDCTarget{};

 public:
	 ID2D1DCRenderTarget* GetDirectXRenderTarget();

	 static VTargetRenderType GetType();

 public:
	 VDCRender(ID2D1Factory* DirectXFactory, const HDC& TargetDC, const VRect& DCRectangle, const bool& HardwareAcceleration = true);
	 ~VDCRender();
};
class VHWNDRender {
private:
	ID2D1HwndRenderTarget* DirectXDCTarget{};

public:
	ID2D1HwndRenderTarget* GetDirectXRenderTarget();

	static VTargetRenderType GetType();

public:
	VHWNDRender(ID2D1Factory* DirectXFactory, const HWND& TargetHWND, const VRect& RenderRect, const bool& HardwareAcceleration = true);
	~VHWNDRender();
};
class VIWICIRender {
private:
	ID2D1BitmapRenderTarget* DirectXDCTarget{};

public:
	ID2D1BitmapRenderTarget* GetDirectXRenderTarget();

	static VTargetRenderType GetType();

public:
	VIWICIRender(ID2D1Factory* DirectXFactory, IWICBitmap* TargetBitmap, const bool& HardwareAcceleration = true);
	~VIWICIRender();
};

}

VLIB_END_NAMESPACE