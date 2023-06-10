#include "../../../include/core/render/vbrush.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
VPenBrush::VPenBrush(const VColor &Color, const VRenderHandle &RenderHandle, const float &LineThickness)
	: Brush(RenderHandle.Allocator), StrokeStyle(RenderHandle.Allocator), Thickness(LineThickness)
{
	VLIB_CHECK_REPORT(!RenderHandle._IRenderTarget, L"A invalid renderer handle has specified");

	VLIB_REPORT_IF_FAILED_INFO(RenderHandle._IRenderTarget->CreateSolidColorBrush(Color.GetDxObject(), &Brush.Object),
							   L"Failed to create ID2D1SolidColorBrush object");
	VLIB_REPORT_IF_FAILED_INFO(VDirectXD2DFactory.GetInstance()->CreateStrokeStyle(D2D1::StrokeStyleProperties(),
																				   nullptr, 0, &StrokeStyle.Object),
							   L"Failed to create ID2D1StrokeStyle object");
}
VPenBrush::~VPenBrush()
{
	Brush.Object->Release();
	Brush.Object = NULL;
}
ID2D1SolidColorBrush *VPenBrush::GetDxBrush() const
{
	return Brush.Get();
}
ID2D1StrokeStyle *VPenBrush::GetStrokeStyle() const
{
	return StrokeStyle.Get();
}
void VPenBrush::SetStrokeStyle(ID2D1StrokeStyle *Style)
{
	StrokeStyle.~VCOMPointer();
	StrokeStyle = Style;
}
float VPenBrush::GetThickness() const
{
	return Thickness;
}
void VPenBrush::SetThickness(const float &TargetThickness)
{
	VLIB_CHECK_REPORT(TargetThickness <= 0.f, L"Thickness shouldn't be a negative value nor zero value!");

	Thickness = TargetThickness;
}

VSolidBrush::VSolidBrush(const VColor &Color, const VRenderHandle &RenderHandle) : Brush(RenderHandle.Allocator)
{
	VLIB_REPORT_IF_FAILED_INFO(RenderHandle._IRenderTarget->CreateSolidColorBrush(Color.GetDxObject(), &Brush.Object),
							   L"Failed to create ID2D1SolidColorBrush object");
}
VSolidBrush::~VSolidBrush()
{
	if (Brush.Object != nullptr)
	{
		Brush.Object->Release();
		Brush.Object = NULL;
	}
}
ID2D1SolidColorBrush *VSolidBrush::GetDxBrush() const
{
	return Brush.Get();
}

VBitmapBrush::VBitmapBrush(VImage *Image, const VRenderHandle &RenderHandle) : Brush(RenderHandle.Allocator)
{
	VLIB_REPORT_IF_FAILED_INFO(RenderHandle._IRenderTarget->CreateBitmapBrush(
								   Image->GetDirectXObject(), D2D1::BitmapBrushProperties(), &Brush.Object),
							   L"Failed to create ID2D1BitmapBrush object");
}
VBitmapBrush::VBitmapBrush(ID2D1Bitmap *Image, const VRenderHandle &RenderHandle)
	: Brush(RenderHandle.Allocator) VRENDER_HELPER
{
	VLIB_REPORT_IF_FAILED_INFO(
		RenderHandle._IRenderTarget->CreateBitmapBrush(Image, D2D1::BitmapBrushProperties(), &Brush.Object),
		L"Failed to create ID2D1BitmapBrush object");
}
VBitmapBrush::~VBitmapBrush()
{
	Brush.Object->Release();
	Brush.Object = NULL;
}
ID2D1BitmapBrush *VBitmapBrush::GetDxBrush() const
{
	return Brush.Get();
}
} // namespace Core

VLIB_END_NAMESPACE