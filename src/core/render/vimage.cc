#pragma warning(disable : 26495)

#include "../../../include/core/render/vimage.h"
#include "../../../include/core/render/vpainterdevice.h"

#include <graphics.h>

VLIB_BEGIN_NAMESPACE

namespace Core {

VImage::VImage(const VImage& Image) {
	DirectXBitmap = Image.DirectXBitmap;
}
VImage::VImage(int Width, int Height, ID2D1RenderTarget* DirectXRenderTarget) {
	VLIB_CHECK_REPORT(DirectXRenderTarget == nullptr, L"DirectXRenderTarget should be not as value nullptr!");

	HRESULT Result = DirectXRenderTarget->CreateBitmap(
		D2D1::SizeU(Width, Height), D2D1::BitmapProperties(
			D2D1::PixelFormat(DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
                              D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_STRAIGHT)
		), &DirectXBitmap);

	VLIB_CHECK_REPORT(FAILED(Result), L"DirectX create ID2D1Bitmap failed!");
}
VImage::VImage(int Width, int Height, ID2D1RenderTarget* DirectXRenderTarget, D2D1_ALPHA_MODE DirectXAlphaMode) {
    VLIB_CHECK_REPORT(DirectXRenderTarget == nullptr, L"DirectXRenderTarget should be not as value nullptr!");

    HRESULT Result = DirectXRenderTarget->CreateBitmap(
            D2D1::SizeU(Width, Height), D2D1::BitmapProperties(
                    D2D1::PixelFormat(DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
                                      DirectXAlphaMode)
            ), &DirectXBitmap);

    VLIB_CHECK_REPORT(FAILED(Result), L"DirectX create ID2D1Bitmap failed!");
}
VImage::VImage(const std::wstring& FromFile, ID2D1RenderTarget* DirectXRenderTarget) {
	IWICBitmapDecoder* IWICDecoder     = nullptr;
	IWICBitmapFrameDecode* IWICFrame   = nullptr;
	IWICFormatConverter* IWICConverter = nullptr;

	HRESULT Result = CoCreateInstance(CLSID_WICBmpDecoder, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&IWICDecoder));

	VLIB_CHECK_REPORT(FAILED(Result), L"DirectX create WICIBitmapDecoder failed!");

	Result = VDirectXIWICImagingFactory.GetInstance()->CreateDecoderFromFilename(
		FromFile.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&IWICDecoder
	);
	VLIB_CHECK_REPORT(FAILED(Result), L"DirectX WICIBitmapDecoder decode from file failed!");

	Result = IWICDecoder->GetFrame(0, &IWICFrame);
	VLIB_CHECK_REPORT(FAILED(Result), L"DirectX WICIBitmapDecoder decode get frame from memory failed!");

	VDirectXIWICImagingFactory.GetInstance()->CreateFormatConverter(&IWICConverter);

	Result = IWICConverter->Initialize(IWICFrame, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);
	VLIB_CHECK_REPORT(FAILED(Result), L"DirectX IWICConverter initialize failed!");

    D2D1_BITMAP_PROPERTIES1 BitmapProperties =
            D2D1::BitmapProperties1(
                    D2D1_BITMAP_OPTIONS_TARGET,
                    D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
            );

	Result = DirectXRenderTarget->CreateBitmapFromWicBitmap(IWICConverter, nullptr, &DirectXBitmap);
	VLIB_CHECK_REPORT(FAILED(Result), L"DirectX Convert bitmap initialize failed!");

	VDXObjectSafeFree(&IWICDecoder);
	VDXObjectSafeFree(&IWICFrame);
	VDXObjectSafeFree(&IWICConverter);
	
}
VImage::~VImage() {
	VDXObjectSafeFree(&DirectXBitmap);
}

int VImage::GetWidth() const {
    return DirectXBitmap->GetSize().width;
}
int VImage::GetHeight() const {
    return DirectXBitmap->GetSize().height;
}

void VImage::ApplyGassBlur(const int &Radius, ID2D1RenderTarget* DirectXRenderTarget) {
    ID2D1Effect* BlurEffect;
    ID2D1DeviceContext* DeviceContext;
    ID2D1Image* BlurOutput;

    DirectXRenderTarget->QueryInterface(&DeviceContext);
    VLIB_CHECK_REPORT(FAILED(DeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &BlurEffect)), L"Created D2D1Effect Failed!");

    BlurEffect->SetInput(0, DirectXBitmap);
    BlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, static_cast<float>(Radius));
    BlurEffect->GetOutput(&BlurOutput);

    UINT Width  = DirectXBitmap->GetSize().width;
    UINT Height = DirectXBitmap->GetSize().height;

    ID2D1Bitmap1* TargetBitmap = NULL;

    D2D1_BITMAP_PROPERTIES1 BitmapProperties =
            D2D1::BitmapProperties1(
                    D2D1_BITMAP_OPTIONS_TARGET,
                    D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
            );
    DeviceContext->CreateBitmap({ Width, Height }, 0, 0, BitmapProperties, &TargetBitmap);

    ID2D1Image* OldDCTarget;
    DeviceContext->GetTarget(&OldDCTarget);
    DeviceContext->SetTarget(TargetBitmap);

    DeviceContext->BeginDraw();
    DeviceContext->DrawImage(BlurEffect);
    DeviceContext->EndDraw();

    DeviceContext->SetTarget(OldDCTarget);

    VDXObjectSafeFree(&DirectXBitmap);

    DirectXBitmap = TargetBitmap;

    VDXObjectSafeFree(&BlurEffect);
    VDXObjectSafeFree(&DeviceContext);
    VDXObjectSafeFree(&BlurOutput);
}

}

VLIB_END_NAMESPACE