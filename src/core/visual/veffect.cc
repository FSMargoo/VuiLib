#include "../../../include/core/visual/veffect.h"

#include <d2d1effects_2.h>

VLIB_BEGIN_NAMESPACE

namespace Core
{
VBasicEffect::VBasicEffect()
{
}

VGassuinBlurEffect::VGassuinBlurEffect(const float &BlurRadius)
{
	Radius = BlurRadius;
}

void VGassuinBlurEffect::ApplyEffect(const VRenderHandle &RenderHandle, Core::VCanvasPainter *Image)
{
	ID2D1Bitmap *Bitmap;
	Image->GetDXObject()->GetBitmap(&Bitmap);

	Core::VImage ImageWrapper(Bitmap);
	ImageWrapper.ApplyGassBlur(Radius, RenderHandle);
}
void VGassuinBlurEffect::ApplyEffect(const VRenderHandle &RenderHandle, Core::VImage *Image)
{
	Image->ApplyGassBlur(Radius, RenderHandle);
}

VExposureEffect::VExposureEffect(const float &ExposureValue)
{
	Exposure = ExposureValue;
}
void VExposureEffect::ApplyEffect(const VRenderHandle &RenderHandle, Core::VCanvasPainter *Image)
{
	ID2D1Bitmap *Bitmap;
	Image->GetDXObject()->GetBitmap(&Bitmap);

	{
		Microsoft::WRL::ComPtr<ID2D1Effect>		   BlurEffect;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> DeviceContext;
		Microsoft::WRL::ComPtr<ID2D1Image>		   Output;
		D2D1_RECT_F								   OutputRectangle;

		VLIB_REPORT_IF_FAILED_INFO(RenderHandle._IRenderTarget->QueryInterface(DeviceContext.GetAddressOf()),
								   L"Failed to query interface from render target");

		VLIB_CHECK_REPORT(FAILED(DeviceContext->CreateEffect(CLSID_D2D1Exposure, &BlurEffect)),
						  L"Failed to create effect");

		BlurEffect->SetInput(0, Bitmap);
		BlurEffect->SetValue(D2D1_EXPOSURE_PROP_EXPOSURE_VALUE, static_cast<float>(Exposure));
		BlurEffect->GetOutput(&Output);

		Microsoft::WRL::ComPtr<ID2D1BitmapRenderTarget> ResultRenderTarget;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext>		ResultDeviceContext;

		BlurEffect->GetOutput(&Output);

		DeviceContext->GetImageLocalBounds(Output.Get(), &OutputRectangle);

		RenderHandle._IRenderTarget->CreateCompatibleRenderTarget(
			D2D1_SIZE_F{static_cast<float>(OutputRectangle.right + abs(OutputRectangle.left)),
						static_cast<float>(OutputRectangle.bottom + abs(OutputRectangle.top))},
			&ResultRenderTarget);

		ResultRenderTarget->QueryInterface(ResultDeviceContext.GetAddressOf());

		ResultDeviceContext->BeginDraw();

		ResultDeviceContext->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));
		ResultDeviceContext->DrawImage(Output.Get(),
									   D2D1_POINT_2F{abs(OutputRectangle.left), abs(OutputRectangle.top)});

		ResultDeviceContext->EndDraw();

		D2D1_POINT_2U OriginPoint = {0, 0};
		D2D1_RECT_U	  Rect		  = {0, 0, static_cast<unsigned int>(OutputRectangle.right + abs(OutputRectangle.left)),
									 static_cast<unsigned int>(OutputRectangle.bottom + abs(OutputRectangle.left))};

		Bitmap->CopyFromRenderTarget(&OriginPoint, ResultRenderTarget.Get(), &Rect);

		while (!ResultDeviceContext.ReleaseAndGetAddressOf())
		{
		}
		while (!ResultRenderTarget.ReleaseAndGetAddressOf())
		{
		}
		while (!Output.ReleaseAndGetAddressOf())
		{
		}
	}
}
void VExposureEffect::ApplyEffect(const VRenderHandle &RenderHandle, Core::VImage *Image)
{
	ID2D1Bitmap *Bitmap;
	Bitmap = Image->GetDirectXObject();

	{
		Microsoft::WRL::ComPtr<ID2D1Effect>		   BlurEffect;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> DeviceContext;
		Microsoft::WRL::ComPtr<ID2D1Image>		   Output;
		D2D1_RECT_F								   OutputRectangle;

		VLIB_REPORT_IF_FAILED_INFO(RenderHandle._IRenderTarget->QueryInterface(DeviceContext.GetAddressOf()),
								   L"Failed to query interface from render target");

		VLIB_CHECK_REPORT(FAILED(DeviceContext->CreateEffect(CLSID_D2D1Exposure, &BlurEffect)),
						  L"Failed to create effect");

		BlurEffect->SetInput(0, Bitmap);
		BlurEffect->SetValue(D2D1_EXPOSURE_PROP_EXPOSURE_VALUE, static_cast<float>(Exposure));
		BlurEffect->GetOutput(&Output);

		Microsoft::WRL::ComPtr<ID2D1BitmapRenderTarget> ResultRenderTarget;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext>		ResultDeviceContext;

		BlurEffect->GetOutput(&Output);

		DeviceContext->GetImageLocalBounds(Output.Get(), &OutputRectangle);

		RenderHandle._IRenderTarget->CreateCompatibleRenderTarget(
			D2D1_SIZE_F{static_cast<float>(OutputRectangle.right + abs(OutputRectangle.left)),
						static_cast<float>(OutputRectangle.bottom + abs(OutputRectangle.top))},
			&ResultRenderTarget);

		ResultRenderTarget->QueryInterface(ResultDeviceContext.GetAddressOf());

		ResultDeviceContext->BeginDraw();

		ResultDeviceContext->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));
		ResultDeviceContext->DrawImage(Output.Get(),
									   D2D1_POINT_2F{abs(OutputRectangle.left), abs(OutputRectangle.top)});

		ResultDeviceContext->EndDraw();

		D2D1_POINT_2U OriginPoint = {0, 0};
		D2D1_RECT_U	  Rect		  = {0, 0, static_cast<unsigned int>(OutputRectangle.right + abs(OutputRectangle.left)),
									 static_cast<unsigned int>(OutputRectangle.bottom + abs(OutputRectangle.left))};

		Bitmap->CopyFromRenderTarget(&OriginPoint, ResultRenderTarget.Get(), &Rect);

		while (!ResultDeviceContext.ReleaseAndGetAddressOf())
		{
		}
		while (!ResultRenderTarget.ReleaseAndGetAddressOf())
		{
		}
		while (!Output.ReleaseAndGetAddressOf())
		{
		}
	}
}

} // namespace Core

VLIB_END_NAMESPACE