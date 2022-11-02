#pragma warning(disable : 26495)

#include "../../../include/core/render/vimage.h"
#include "../../../include/core/render/vpainterdevice.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VImage::VImage(const VImage& Image) {
    VDXObjectSafeFree(&DirectXBitmap);

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
    D2D1_RECT_F OutputRectangle;

    DirectXRenderTarget->QueryInterface(&DeviceContext);
    VLIB_CHECK_REPORT(FAILED(DeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &BlurEffect)), L"Created D2D1Effect Failed!");

    BlurEffect->SetInput(0, DirectXBitmap);
    BlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, static_cast<float>(Radius));
    BlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
    BlurEffect->GetOutput(&BlurOutput);

    ID2D1BitmapRenderTarget* ResultRenderTarget;
    ID2D1DeviceContext*      ResultDeviceContext;

    D2D1_BITMAP_PROPERTIES1 BitmapProperties =
            D2D1::BitmapProperties1(
                    D2D1_BITMAP_OPTIONS_TARGET,
                    D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
            );

    ID2D1Image* EffectOutput;
    BlurEffect->GetOutput(&EffectOutput);

    DeviceContext->GetImageLocalBounds(EffectOutput, &OutputRectangle);

    DirectXRenderTarget->CreateCompatibleRenderTarget(D2D1_SIZE_F { static_cast<float>(OutputRectangle.right + abs(OutputRectangle.left)),
                                                                    static_cast<float>(OutputRectangle.bottom  + abs(OutputRectangle.top)) }, &ResultRenderTarget);

    ResultRenderTarget->QueryInterface(&ResultDeviceContext);

    ResultDeviceContext->BeginDraw();

    ResultDeviceContext->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));
    ResultDeviceContext->DrawImage(EffectOutput, D2D1_POINT_2F { abs(OutputRectangle.left), abs(OutputRectangle.top) });

    ResultDeviceContext->EndDraw();

    ResultRenderTarget->GetBitmap(&DirectXBitmap);

    D2D1_POINT_2U OriginPoint = { 0, 0 };
    D2D1_RECT_U   Rect        = { 0, 0, static_cast<unsigned int>(OutputRectangle.right + abs(OutputRectangle.left)),
                                  static_cast<unsigned int>(OutputRectangle.bottom + abs(OutputRectangle.left)) };

    DirectXBitmap->CopyFromRenderTarget(&OriginPoint, ResultRenderTarget, &Rect);

    VDXObjectSafeFree(&ResultDeviceContext);
    VDXObjectSafeFree(&ResultRenderTarget);
    VDXObjectSafeFree(&BlurEffect);
    VDXObjectSafeFree(&DeviceContext);
    VDXObjectSafeFree(&BlurOutput);
}

void VImage::ApplyShadowEffect(const float &ShadowRadius, const VColor &ShadowColor,
                               ID2D1RenderTarget *DirectXRenderTarget, VPoint* Offset) {
    ID2D1Effect* BlurEffect;
    ID2D1DeviceContext* DeviceContext;
    ID2D1Image* BlurOutput;
    D2D1_RECT_F OutputRectangle;

    DirectXRenderTarget->QueryInterface(&DeviceContext);
    VLIB_CHECK_REPORT(FAILED(DeviceContext->CreateEffect(CLSID_D2D1Shadow, &BlurEffect)), L"Created D2D1Effect Failed!");

    BlurEffect->SetInput(0, DirectXBitmap);
    BlurEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, static_cast<float>(ShadowRadius));
    BlurEffect->SetValue(D2D1_SHADOW_PROP_COLOR, D2D1_VECTOR_4F{
        ShadowColor.GetR(), ShadowColor.GetG(), ShadowColor.GetB(), ShadowColor.GetA()
    });
    BlurEffect->GetOutput(&BlurOutput);

    UINT Width  = DirectXBitmap->GetSize().width;
    UINT Height = DirectXBitmap->GetSize().height;

    UINT ShadowOffset = ShadowRadius * 2;

    ID2D1BitmapRenderTarget* ResultRenderTarget;
    ID2D1DeviceContext*      ResultDeviceContext;

    D2D1_BITMAP_PROPERTIES1 BitmapProperties =
            D2D1::BitmapProperties1(
                    D2D1_BITMAP_OPTIONS_TARGET,
                    D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
            );

    ID2D1Image* EffectOutput;
    BlurEffect->GetOutput(&EffectOutput);

    DeviceContext->GetImageLocalBounds(EffectOutput, &OutputRectangle);

    if (Offset != nullptr) {
        *Offset = { static_cast<int>(OutputRectangle.left), static_cast<int>(OutputRectangle.top) };
    }

    DirectXRenderTarget->CreateCompatibleRenderTarget(D2D1_SIZE_F { static_cast<float>(OutputRectangle.right + abs(OutputRectangle.left)),
                                                                    static_cast<float>(OutputRectangle.bottom  + abs(OutputRectangle.top)) }, &ResultRenderTarget);

    ResultRenderTarget->QueryInterface(&ResultDeviceContext);

    ResultDeviceContext->BeginDraw();

    ResultDeviceContext->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));
    ResultDeviceContext->DrawImage(EffectOutput, D2D1_POINT_2F { abs(OutputRectangle.left), abs(OutputRectangle.top) });

    ResultDeviceContext->EndDraw();

    ResultRenderTarget->GetBitmap(&DirectXBitmap);

    D2D1_POINT_2U OriginPoint = { 0, 0 };
    D2D1_RECT_U   Rect        = { 0, 0, static_cast<unsigned int>(OutputRectangle.right + abs(OutputRectangle.left)),
                                  static_cast<unsigned int>(OutputRectangle.bottom + abs(OutputRectangle.left)) };

    DirectXBitmap->CopyFromRenderTarget(&OriginPoint, ResultRenderTarget, &Rect);

    VDXObjectSafeFree(&ResultDeviceContext);
    VDXObjectSafeFree(&ResultRenderTarget);
    VDXObjectSafeFree(&BlurEffect);
    VDXObjectSafeFree(&DeviceContext);
    VDXObjectSafeFree(&BlurOutput);
}

bool VImage::IsValidBitmapFile(const std::wstring& FilePath) {
	IWICBitmapDecoder* IWICDecoder = nullptr;
	IWICBitmapFrameDecode* IWICFrame = nullptr;
	IWICFormatConverter* IWICConverter = nullptr;

	HRESULT Result = CoCreateInstance(CLSID_WICBmpDecoder, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&IWICDecoder));

	if (FAILED(Result)) {
		VDXObjectSafeFree(&IWICDecoder);
		VDXObjectSafeFree(&IWICFrame);

		return false;
	}

	Result = VDirectXIWICImagingFactory.GetInstance()->CreateDecoderFromFilename(
		FilePath.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&IWICDecoder
	);
	if (FAILED(Result)) {
		VDXObjectSafeFree(&IWICDecoder);
		VDXObjectSafeFree(&IWICFrame);

		return false;
	}

	Result = IWICDecoder->GetFrame(0, &IWICFrame);
	if (FAILED(Result)) {
		VDXObjectSafeFree(&IWICDecoder);
		VDXObjectSafeFree(&IWICFrame);
		VDXObjectSafeFree(&IWICConverter);

		return false;
	}

	VDirectXIWICImagingFactory.GetInstance()->CreateFormatConverter(&IWICConverter);

	Result = IWICConverter->Initialize(IWICFrame, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);
	if (FAILED(Result)) {
		VDXObjectSafeFree(&IWICDecoder);
		VDXObjectSafeFree(&IWICFrame);
		VDXObjectSafeFree(&IWICConverter);

		return false;
	}

	VDXObjectSafeFree(&IWICDecoder);
	VDXObjectSafeFree(&IWICFrame);
	VDXObjectSafeFree(&IWICConverter);

	return true;
}

}

VLIB_END_NAMESPACE