#pragma warning(disable : 26495)

#include "../../../include/core/render/vpainterdevice.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VDCRender::VDCRender(ID2D1Factory* DirectXFactory, const HDC& TargetDC, const VRect& DCRectangle, const bool& GDIMode, const bool& HardwareAcceleration) {
		VLIB_CHECK_REPORT(DirectXFactory == nullptr, L"Factory should not be nullptr!");

		D2D1_RENDER_TARGET_PROPERTIES Property;

		if (HardwareAcceleration) {
			if (GDIMode) {
				Property = D2D1::RenderTargetProperties(
					D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_HARDWARE,
					D2D1::PixelFormat(
						DXGI_FORMAT_B8G8R8A8_UNORM,
						D2D1_ALPHA_MODE_PREMULTIPLIED
					), 0.0, 0.0, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE, D2D1_FEATURE_LEVEL_DEFAULT
				);
			}
			else {
				Property = D2D1::RenderTargetProperties(
					D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_HARDWARE,
					D2D1::PixelFormat(
						DXGI_FORMAT_B8G8R8A8_UNORM,
						D2D1_ALPHA_MODE_PREMULTIPLIED
					), 0.0, 0.0, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT
				);
			}
		}
		else {
			if (GDIMode) {
				Property = D2D1::RenderTargetProperties(
					D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_SOFTWARE,
					D2D1::PixelFormat(
						DXGI_FORMAT_B8G8R8A8_UNORM,
						D2D1_ALPHA_MODE_PREMULTIPLIED
					), 0.0, 0.0, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE, D2D1_FEATURE_LEVEL_DEFAULT
				);
			}
			else {
				Property = D2D1::RenderTargetProperties(
					D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_SOFTWARE,
					D2D1::PixelFormat(
						DXGI_FORMAT_B8G8R8A8_UNORM,
						D2D1_ALPHA_MODE_PREMULTIPLIED
					), 0.0, 0.0, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT
				);
			}
		}

		HRESULT ProduceResult = DirectXFactory->CreateDCRenderTarget(
			&Property,
			&DirectXDCTarget
		);

		RECT DCRect = DCRectangle.ToRECT();

		VLIB_CHECK_REPORT(ProduceResult != S_OK, L"DirectX Render Target Produce Failed!");
		VLIB_CHECK_REPORT(DirectXDCTarget->BindDC(TargetDC, &DCRect) != S_OK, L"TargetRender Bind DC Failed!");
	}
VDCRender::~VDCRender() {
	VDXObjectSafeFree(&DirectXDCTarget);
}
ID2D1DCRenderTarget* VDCRender::GetDirectXRenderTarget() {
	return DirectXDCTarget;
}
VTargetRenderType VDCRender::GetType() {
	return VTargetRenderType::DCRender;
}

VHWNDRender::VHWNDRender(ID2D1Factory* DirectXFactory, const HWND& TargetHWND, const VRect& RenderRect, const bool& HardwareAcceleration) {
	VLIB_CHECK_REPORT(DirectXFactory == nullptr, L"Factory should not be nullptr!");

	D2D1_RENDER_TARGET_PROPERTIES Property;

	if (HardwareAcceleration) {
		Property = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_HARDWARE,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_STRAIGHT
			), 0.0, 0.0, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE, D2D1_FEATURE_LEVEL_DEFAULT
		);
	}
	else {
		Property = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_SOFTWARE,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_STRAIGHT
			), 0.0, 0.0, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE, D2D1_FEATURE_LEVEL_DEFAULT
		);
	}

	HRESULT ProduceResult = DirectXFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(
		TargetHWND, RenderRect.ToDxPoint2U()
	), &DirectXDCTarget);

	VLIB_CHECK_REPORT(FAILED(ProduceResult), L"DirectX Render Target Produce Failed!");
}
VHWNDRender::~VHWNDRender() {
	VDXObjectSafeFree(&DirectXDCTarget);
}
ID2D1HwndRenderTarget* VHWNDRender::GetDirectXRenderTarget() {
	return DirectXDCTarget;
}
VTargetRenderType VHWNDRender::GetType() {
	return VTargetRenderType::DCRender;
}

VIWICIRender::VIWICIRender(ID2D1Factory* DirectXFactory, IWICBitmap* TargetBitmap, const bool& HardwareAcceleration) {
	VLIB_CHECK_REPORT(DirectXFactory == nullptr, L"Factory should not be nullptr!");

	D2D1_RENDER_TARGET_PROPERTIES Property;

	if (HardwareAcceleration) {
		Property = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_HARDWARE,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_IGNORE
			), 0.0, 0.0, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE, D2D1_FEATURE_LEVEL_DEFAULT
		);
	}
	else {
		Property = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_SOFTWARE,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_IGNORE
			), 0.0, 0.0, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE, D2D1_FEATURE_LEVEL_DEFAULT
		);
	}

	WICPixelFormatGUID PixelFormat;
	TargetBitmap->GetPixelFormat(&PixelFormat);

	HRESULT ProduceResult = DirectXFactory->CreateWicBitmapRenderTarget((IWICBitmap*)TargetBitmap, (D2D1_RENDER_TARGET_PROPERTIES)Property, (ID2D1RenderTarget**)(&DirectXDCTarget));

	VLIB_CHECK_REPORT(FAILED(ProduceResult), L"DirectX Render Target Produce Failed!");
}
VIWICIRender::~VIWICIRender() {
	VDXObjectSafeFree(&DirectXDCTarget);
}
ID2D1BitmapRenderTarget* VIWICIRender::GetDirectXRenderTarget() {
	return DirectXDCTarget;
}
VTargetRenderType VIWICIRender::GetType() {
	return VTargetRenderType::DCRender;
}

}

VLIB_END_NAMESPACE