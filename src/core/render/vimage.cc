#pragma warning(disable : 26495)

#include "../../../include/core/render/vimage.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VImage::VImage(const VImage& Image) {
	DirectXBitmap = Image.DirectXBitmap;
}
VImage::VImage(int Width, int Height, ID2D1RenderTarget* DirectXRenderTarget) {
	VLIB_CHECK_REPORT(DirectXRenderTarget == nullptr, L"DirectXRenderTarget should be not as value nullptr!");

	HRESULT Result = DirectXRenderTarget->CreateBitmap(
		D2D1::SizeU(Width, Height), D2D1::BitmapProperties(
			D2D1::PixelFormat(DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_STRAIGHT)
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

	Result = DirectXRenderTarget->CreateBitmapFromWicBitmap(IWICConverter, nullptr, &DirectXBitmap);
	VLIB_CHECK_REPORT(FAILED(Result), L"DirectX Convert bitmap initialize failed!");

	VDXObjectSafeFree(&IWICDecoder);
	VDXObjectSafeFree(&IWICFrame);
	VDXObjectSafeFree(&IWICConverter);
	
}
VImage::~VImage() {
	VDXObjectSafeFree(&DirectXBitmap);
}

}

VLIB_END_NAMESPACE