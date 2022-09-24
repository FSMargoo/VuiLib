#include "../../../include/core/render/vbrush.h"

VLIB_BEGIN_NAMESPACE

namespace Core {
ID2D1SolidColorBrush* VPenBrush::GetDxBrush() {
	return Brush;
}
ID2D1StrokeStyle* VPenBrush::GetStrokeStyle() {
	return StrokeStyle;
}

float VPenBrush::GetThickness() const {
	return Thickness;
}
void VPenBrush::SetThickness(const float& TargetThickness) {
	Thickness = TargetThickness;
}
VPenBrush::VPenBrush(ID2D1RenderTarget* Target, const VColor& Color, const float& LineThickness)
	: Thickness(LineThickness) {
	HRESULT Result = Target->CreateSolidColorBrush(Color.GetDxObject(), &Brush);
	VLIB_CHECK_REPORT(Result != S_OK, L"DirectX Created Brush Failed!");

	Result = VDirectXD2DFactory.GetInstance()->CreateStrokeStyle(
		D2D1::StrokeStyleProperties(), nullptr, 0, &StrokeStyle
	);
	VLIB_CHECK_REPORT(Result != S_OK, L"DirectX Created Stroke Style Failed!");
}
VPenBrush::~VPenBrush() {
	VDXObjectSafeFree(&Brush);
}

ID2D1SolidColorBrush* VSolidBrush::GetDxBrush() {
	return Brush;
}

VSolidBrush::VSolidBrush(ID2D1RenderTarget* Target, const VColor& Color) {
    HRESULT Result = Target->CreateSolidColorBrush(Color.GetDxObject(), &Brush);

    VLIB_CHECK_REPORT(Result != S_OK, L"DirectX Created Brush Failed!");
}
VSolidBrush::~VSolidBrush() {
	VDXObjectSafeFree(&Brush);
}

ID2D1BitmapBrush* VBitmapBrush::GetDxBrush() {
    return Brush;
}

VBitmapBrush::VBitmapBrush(ID2D1RenderTarget* Target, VImage* Image) {
    Target->CreateBitmapBrush(Image->GetDirectXObject(), D2D1::BitmapBrushProperties(), &Brush);
}
VBitmapBrush::VBitmapBrush(ID2D1RenderTarget* Target, ID2D1Bitmap* Image) {
    Target->CreateBitmapBrush(Image, D2D1::BitmapBrushProperties(), &Brush);
}
VBitmapBrush::~VBitmapBrush() {
	VDXObjectSafeFree(&Brush);
}

}

VLIB_END_NAMESPACE