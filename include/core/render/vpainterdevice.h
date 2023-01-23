/*
 * File name	: vpainter.h
 * Author		: Margoo
 * Date			: 11/13/2022
 * Description	: This head define the painter device (wrapper of render target)
 */
#pragma once

#include "vrenderbasic.h"

#include <Windows.h>

VLIB_BEGIN_NAMESPACE

namespace Core
{
/*
 * VTargetRenderType enum:
 *	Description : This enum describe the type of a render target
 */
enum class VTargetRenderType
{
	UnknownRender,
	DCRender,
	IWICImageRender,
	HWNDRender
};

/*
 * VDCRender class:
 *	Description : This class describe a vuilib renderer on windows DC device
 */
VRENDER_HELPER class VDCRender
{
public:
	VDCRender(ID2D1Factory *DirectXFactory, const HDC &TargetDC, const VRect &DCRectangle, const bool &GDIMode = false,
			  const bool &HardwareAcceleration = true) VRENDER_HELPER;
	~VDCRender();

public:
	ID2D1DCRenderTarget *GetDirectXRenderTarget();

public:
	static VTargetRenderType GetType();

public:
	Microsoft::WRL::ComPtr<ID2D1DCRenderTarget> DirectXDCTarget;
};
VRENDER_HELPER class VHWNDRender
{
public:
	ID2D1HwndRenderTarget *GetDirectXRenderTarget();

	static VTargetRenderType GetType();

public:
	VHWNDRender(ID2D1Factory *DirectXFactory, const HWND &TargetHWND, const VRect &RenderRect,
				const bool &HardwareAcceleration = true);
	~VHWNDRender();

private:
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> DirectXDCTarget;
};
VRENDER_HELPER class VIWICIRender
{
public:
	ID2D1BitmapRenderTarget *GetDirectXRenderTarget();

	static VTargetRenderType GetType();

public:
	VIWICIRender(ID2D1Factory *DirectXFactory, IWICBitmap *TargetBitmap, const bool &HardwareAcceleration = true);
	~VIWICIRender();

private:
	Microsoft::WRL::ComPtr<ID2D1BitmapRenderTarget> DirectXDCTarget;
};
} // namespace Core

VLIB_END_NAMESPACE