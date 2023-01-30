#pragma warning(disable : 26495)

#include "../../../include/core/render/vimage.h"

#include "../../../include/core/render/vpainterdevice.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
VImage::VImage(const VImage &Image) : DirectXBitmap(Image.DirectXBitmap.Object)
{
}
VImage::VImage(const int &Width, const int &Height, const VRenderHandle &RenderHandle)
	: DirectXBitmap(RenderHandle.Allocator){VLIB_REPORT_IF_FAILED_INFO(
		  RenderHandle._IRenderTarget->CreateBitmap(
			  D2D1::SizeU(Width, Height),
			  D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
													   D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_STRAIGHT)),
			  &DirectXBitmap.Object),
		  L"Failed to create a &DirectXBitmap.Object object"

		  )} VImage::VImage(const int &Width, const int &Height, const VSurfaceAlphaMode &AlphaMode,
							const VRenderHandle &RenderHandle)
	: DirectXBitmap(RenderHandle.Allocator)
{
	VLIB_REPORT_IF_FAILED_INFO(
		RenderHandle._IRenderTarget->CreateBitmap(
			D2D1::SizeU(Width, Height),
			D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM, DXAlphaMode(AlphaMode))),
			&DirectXBitmap.Object),
		L"Failed to create a &DirectXBitmap.Object object"

	);
}
VImage::VImage(const std::wstring &FilePath, const VRenderHandle &RenderHandle) : DirectXBitmap(RenderHandle.Allocator)
{
	IWICBitmapDecoder	  *IWICDecoder	 = nullptr;
	IWICBitmapFrameDecode *IWICFrame	 = nullptr;
	IWICFormatConverter	  *IWICConverter = nullptr;

	VLIB_REPORT_IF_FAILED_INFO(
		CoCreateInstance(CLSID_WICBmpDecoder, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&IWICDecoder)),
		L"Failed to create bitmap decoder");
	VLIB_REPORT_IF_FAILED_INFO(
		VDirectXIWICImagingFactory.GetInstance()->CreateDecoderFromFilename(
			FilePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &IWICDecoder),
		L"Failed to create decoder from file name");

	VLIB_REPORT_IF_FAILED_INFO(IWICDecoder->GetFrame(0, &IWICFrame),
							   L"Failed to get the first frame from target image");

	VLIB_REPORT_IF_FAILED_INFO(VDirectXIWICImagingFactory.GetInstance()->CreateFormatConverter(&IWICConverter),
							   L"Failed to create format converter from image factory");

	VLIB_REPORT_IF_FAILED_INFO(IWICConverter->Initialize(IWICFrame, GUID_WICPixelFormat32bppPBGRA,
														 WICBitmapDitherTypeNone, nullptr, 0.0f,
														 WICBitmapPaletteTypeCustom),
							   L"Failed to initialize IWICConverter object");

	D2D1_BITMAP_PROPERTIES1 BitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

	VLIB_REPORT_IF_FAILED_INFO(
		RenderHandle._IRenderTarget->CreateBitmapFromWicBitmap(IWICConverter, nullptr, &DirectXBitmap.Object),
		L"Failed to create Bitmap from WicBitmap through render target");

	VDXObjectSafeFree(&IWICDecoder);
	VDXObjectSafeFree(&IWICFrame);
	VDXObjectSafeFree(&IWICConverter);
}
VImage::VImage(ID2D1Bitmap *IBitmap) : DirectXBitmap(IBitmap)
{
}
VImage::~VImage()
{
	VDXObjectSafeFree(&DirectXBitmap.Object);
}

int VImage::GetWidth() const
{
	return DirectXBitmap.Get()->GetSize().width;
}
int VImage::GetHeight() const
{
	return DirectXBitmap.Get()->GetSize().height;
}

void VImage::ApplyGassBlur(const int &Radius, const VRenderHandle &RenderHandle)
{
	Microsoft::WRL::ComPtr<ID2D1Effect>		   BlurEffect;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> DeviceContext;
	Microsoft::WRL::ComPtr<ID2D1Image>		   BlurOutput;
	D2D1_RECT_F								   OutputRectangle;

	VLIB_REPORT_IF_FAILED_INFO(RenderHandle._IRenderTarget->QueryInterface(DeviceContext.GetAddressOf()),
							   L"Failed to query interface from render target");

	VLIB_CHECK_REPORT(FAILED(DeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &BlurEffect)),
					  L"Failed to create effect");

	BlurEffect->SetInput(0, DirectXBitmap.Get());
	BlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, static_cast<float>(Radius));
	BlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
	BlurEffect->GetOutput(&BlurOutput);

	Microsoft::WRL::ComPtr<ID2D1BitmapRenderTarget> ResultRenderTarget;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext>		ResultDeviceContext;

	BlurEffect->GetOutput(&BlurOutput);

	DeviceContext->GetImageLocalBounds(BlurOutput.Get(), &OutputRectangle);

	RenderHandle._IRenderTarget->CreateCompatibleRenderTarget(
		D2D1_SIZE_F{static_cast<float>(OutputRectangle.right + abs(OutputRectangle.left)),
					static_cast<float>(OutputRectangle.bottom + abs(OutputRectangle.top))},
		&ResultRenderTarget);

	ResultRenderTarget->QueryInterface(ResultDeviceContext.GetAddressOf());

	ResultDeviceContext->BeginDraw();

	ResultDeviceContext->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));
	ResultDeviceContext->DrawImage(BlurOutput.Get(),
								   D2D1_POINT_2F{abs(OutputRectangle.left), abs(OutputRectangle.top)});

	ResultDeviceContext->EndDraw();

	D2D1_POINT_2U OriginPoint = {0, 0};
	D2D1_RECT_U	  Rect		  = {0, 0, static_cast<unsigned int>(OutputRectangle.right + abs(OutputRectangle.left)),
								 static_cast<unsigned int>(OutputRectangle.bottom + abs(OutputRectangle.left))};

	DirectXBitmap.Get()->CopyFromRenderTarget(&OriginPoint, ResultRenderTarget.Get(), &Rect);

	while (!ResultDeviceContext.ReleaseAndGetAddressOf())
	{
	}
	while (!ResultRenderTarget.ReleaseAndGetAddressOf())
	{
	}
	while (!BlurOutput.ReleaseAndGetAddressOf())
	{
	}
}

void VImage::ApplyShadowEffect(const float &ShadowRadius, const VColor &ShadowColor, const VRenderHandle &RenderHandle,
							   VPoint *Offset)
{
	Microsoft::WRL::ComPtr<ID2D1Effect>		   BlurEffect	 = nullptr;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> DeviceContext = nullptr;
	Microsoft::WRL::ComPtr<ID2D1Image>		   BlurOutput	 = nullptr;
	D2D1_RECT_F								   OutputRectangle;

	RenderHandle._IRenderTarget->QueryInterface(DeviceContext.GetAddressOf());
	VLIB_CHECK_REPORT(FAILED(DeviceContext->CreateEffect(CLSID_D2D1Shadow, &BlurEffect)), L"Failed to create effect");

	BlurEffect->SetInput(0, DirectXBitmap.Get());
	BlurEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, static_cast<float>(ShadowRadius));
	BlurEffect->SetValue(D2D1_SHADOW_PROP_COLOR, D2D1_VECTOR_4F{ShadowColor.GetR(), ShadowColor.GetG(),
																ShadowColor.GetB(), ShadowColor.GetA()});
	BlurEffect->GetOutput(&BlurOutput);

	UINT Width	= DirectXBitmap.Get()->GetSize().width;
	UINT Height = DirectXBitmap.Get()->GetSize().height;

	Microsoft::WRL::ComPtr<ID2D1BitmapRenderTarget> ResultRenderTarget;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext>		ResultDeviceContext;

	D2D1_BITMAP_PROPERTIES1 BitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

	Microsoft::WRL::ComPtr<ID2D1Image> EffectOutput;
	BlurEffect->GetOutput(&EffectOutput);

	DeviceContext->GetImageLocalBounds(EffectOutput.Get(), &OutputRectangle);

	if (Offset != nullptr)
	{
		*Offset = {static_cast<int>(OutputRectangle.left), static_cast<int>(OutputRectangle.top)};
	}

	RenderHandle._IRenderTarget->CreateCompatibleRenderTarget(
		D2D1_SIZE_F{static_cast<float>(OutputRectangle.right + abs(OutputRectangle.left)),
					static_cast<float>(OutputRectangle.bottom + abs(OutputRectangle.top))},
		&ResultRenderTarget);

	ResultRenderTarget->QueryInterface(ResultDeviceContext.GetAddressOf());

	ResultDeviceContext->BeginDraw();

	ResultDeviceContext->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));
	ResultDeviceContext->DrawImage(EffectOutput.Get(),
								   D2D1_POINT_2F{abs(OutputRectangle.left), abs(OutputRectangle.top)});

	ResultDeviceContext->EndDraw();

	ResultRenderTarget->GetBitmap(&DirectXBitmap.Object);

	D2D1_POINT_2U OriginPoint = {0, 0};
	D2D1_RECT_U	  Rect		  = {0, 0, static_cast<unsigned int>(OutputRectangle.right + abs(OutputRectangle.left)),
								 static_cast<unsigned int>(OutputRectangle.bottom + abs(OutputRectangle.left))};

	DirectXBitmap.Get()->CopyFromRenderTarget(&OriginPoint, ResultRenderTarget.Get(), &Rect);

	while (!ResultDeviceContext.ReleaseAndGetAddressOf())
	{
	}
	while (!ResultRenderTarget.ReleaseAndGetAddressOf())
	{
	}
	while (!BlurOutput.ReleaseAndGetAddressOf())
	{
	}
}

const bool VImage::IsValidBitmapFile(const std::wstring &FilePath)
{
	IWICBitmapDecoder	  *IWICDecoder	 = nullptr;
	IWICBitmapFrameDecode *IWICFrame	 = nullptr;
	IWICFormatConverter	  *IWICConverter = nullptr;

	HRESULT Result = CoCreateInstance(CLSID_WICBmpDecoder, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&IWICDecoder));
	if (FAILED(Result))
	{
		VDXObjectSafeFree(&IWICDecoder);
		VDXObjectSafeFree(&IWICFrame);

		return false;
	}

	Result = VDirectXIWICImagingFactory.GetInstance()->CreateDecoderFromFilename(
		FilePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &IWICDecoder);
	if (FAILED(Result))
	{
		VDXObjectSafeFree(&IWICDecoder);
		VDXObjectSafeFree(&IWICFrame);

		return false;
	}

	Result = IWICDecoder->GetFrame(0, &IWICFrame);
	if (FAILED(Result))
	{
		VDXObjectSafeFree(&IWICDecoder);
		VDXObjectSafeFree(&IWICFrame);
		VDXObjectSafeFree(&IWICConverter);

		return false;
	}

	VDirectXIWICImagingFactory.GetInstance()->CreateFormatConverter(&IWICConverter);

	Result = IWICConverter->Initialize(IWICFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f,
									   WICBitmapPaletteTypeCustom);
	if (FAILED(Result))
	{
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

VColor VImage::GetPixel(const int &X, const int &Y, const VRenderHandle &RenderHandle) const
{
	VLIB_CHECK_REPORT(!(X >= 0 && Y >= 0), L"Invalid pixel position was specified");

	ID2D1DeviceContext	   *DeviceContext;
	ID2D1Bitmap1		   *D2D1BitmapInstance;
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
								D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

	VLIB_REPORT_IF_FAILED_INFO(RenderHandle._IRenderTarget->QueryInterface(&DeviceContext),
							   L"Failed to query interface from render target");

	DeviceContext->CreateBitmap(D2D1::SizeU(GetWidth(), GetHeight()), 0, 0, bitmapProperties, &D2D1BitmapInstance);

	auto OriginPoint = D2D1::Point2U(0, 0);
	auto RectArea	 = D2D1::RectU(0, 0, GetWidth(), GetHeight());

	D2D1BitmapInstance->CopyFromBitmap(&OriginPoint, DirectXBitmap.Get(), &RectArea);

	D2D1_MAPPED_RECT MapRect;

	VLIB_REPORT_IF_FAILED_INFO(D2D1BitmapInstance->Map(D2D1_MAP_OPTIONS_READ, &MapRect),
							   L"Failed to map image surface");

	unsigned long Base = X + Y * GetWidth() * 4;

	auto RByte = MapRect.bits[Base];
	auto GByte = MapRect.bits[Base + 1];
	auto BByte = MapRect.bits[Base + 2];
	auto AByte = MapRect.bits[Base + 3];

	VDXObjectSafeFree(&DeviceContext);
	VDXObjectSafeFree(&D2D1BitmapInstance);

	return VColor::FromBYTERGBA(BByte, GByte, RByte, AByte);
}
} // namespace Core

VLIB_END_NAMESPACE