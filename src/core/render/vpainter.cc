#pragma warning(disable : 4244)

#include "../../../include/core/render/vpainter.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VCanvasPainter::VCanvasPainter(const int& Width, const int& Height, ID2D1RenderTarget* SourceDevice) {
	SourceDevice->CreateCompatibleRenderTarget(D2D1::SizeF(Width, Height), &TargetDevice);
}
VCanvasPainter::~VCanvasPainter() {
	VDXObjectSafeFree(&TargetDevice);
}
void VCanvasPainter::Clear(const VColor& Color) {
	TargetDevice->Clear(Color.GetDxObject());
}
void VCanvasPainter::BeginDraw() {
	TargetDevice->BeginDraw();
}
void VCanvasPainter::EndDraw() {
	TargetDevice->EndDraw();
}
void VCanvasPainter::DrawLine(const VPoint& StartPoint, const VPoint& EndPoint, VPenBrush* PenBrush) {
	TargetDevice->DrawLine(D2D1::Point2F(static_cast<FLOAT>(StartPoint.X), static_cast<FLOAT>(StartPoint.Y)), D2D1::Point2F(static_cast<FLOAT>(EndPoint.X), static_cast<FLOAT>(EndPoint.Y)),
		PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle());
}
void VCanvasPainter::DrawRectangle(const VRect& Rectangle, VPenBrush* PenBrush) {
	TargetDevice->DrawRectangle(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top), static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
		PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle());
}
void VCanvasPainter::DrawEllipse(const VRect& EllipseRectangle, VPenBrush* PenBrush) {
	int RadiusX = EllipseRectangle.GetWidth() / 2;
	int RadiusY = EllipseRectangle.GetHeight() / 2;

	TargetDevice->DrawEllipse(
		D2D1_ELLIPSE
		{
			{
				static_cast<FLOAT>(EllipseRectangle.Left + RadiusX),
				static_cast<FLOAT>(EllipseRectangle.Top + RadiusY)
			},
			static_cast<FLOAT>(RadiusX), static_cast<FLOAT>(RadiusY)
		},
		PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle()
	);
}
void VCanvasPainter::DrawRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VPenBrush* PenBrush) {
	TargetDevice->DrawRoundedRectangle(D2D1::RoundedRect(
		D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top), static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
		static_cast<FLOAT>(Radius.X), static_cast<FLOAT>(Radius.Y)
	),
		PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle());
}

void VCanvasPainter::SolidRectangle(const VRect& Rectangle, VSolidBrush* PenBrush) {
	TargetDevice->FillRectangle(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top), static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
		PenBrush->GetDxBrush());
}
void VCanvasPainter::SolidEllipse(const VRect& EllipseRectangle, VSolidBrush* PenBrush) {
	int RadiusX = EllipseRectangle.GetWidth() / 2;
	int RadiusY = EllipseRectangle.GetHeight() / 2;

	TargetDevice->FillEllipse(
		D2D1_ELLIPSE
		{
			{
				static_cast<FLOAT>(EllipseRectangle.Left + RadiusX),
				static_cast<FLOAT>(EllipseRectangle.Top + RadiusY)
			},
			static_cast<FLOAT>(RadiusX), static_cast<FLOAT>(RadiusY)
		},
		PenBrush->GetDxBrush()
	);
}
void VCanvasPainter::SolidRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VSolidBrush* PenBrush) {
	TargetDevice->FillRoundedRectangle(D2D1::RoundedRect(
		D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top), static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
		static_cast<FLOAT>(Radius.X), static_cast<FLOAT>(Radius.Y)
	),
		PenBrush->GetDxBrush());
}

void VCanvasPainter::SolidRectangle(const VRect& Rectangle, VBitmapBrush* PenBrush) {
    TargetDevice->FillRectangle(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top), static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
                                PenBrush->GetDxBrush());
}
void VCanvasPainter::SolidEllipse(const VRect& EllipseRectangle, VBitmapBrush* PenBrush) {
    int RadiusX = EllipseRectangle.GetWidth() / 2;
    int RadiusY = EllipseRectangle.GetHeight() / 2;

    TargetDevice->FillEllipse(
            D2D1_ELLIPSE
                    {
                            {
                                    static_cast<FLOAT>(EllipseRectangle.Left + RadiusX),
                                    static_cast<FLOAT>(EllipseRectangle.Top + RadiusY)
                            },
                            static_cast<FLOAT>(RadiusX), static_cast<FLOAT>(RadiusY)
                    },
            PenBrush->GetDxBrush()
    );
}
void VCanvasPainter::SolidRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VBitmapBrush* PenBrush) {
    TargetDevice->FillRoundedRectangle(D2D1::RoundedRect(
                                               D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top), static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
                                               static_cast<FLOAT>(Radius.X), static_cast<FLOAT>(Radius.Y)
                                       ),
                                       PenBrush->GetDxBrush());
}

void VCanvasPainter::FillRectangle(const VRect& Rectangle, VPenBrush* BorderPen, VSolidBrush* FillBrush) {
	SolidRectangle(Rectangle, FillBrush);
	DrawEllipse(Rectangle, BorderPen);
}
void VCanvasPainter::FillEllipse(const VRect& EllipseRectangle, VPenBrush* BorderPen, VSolidBrush* FillBrush) {
	SolidEllipse(EllipseRectangle, FillBrush);
	DrawEllipse(EllipseRectangle, BorderPen);
}
void VCanvasPainter::FillRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VPenBrush* BorderPen, VSolidBrush* FillBrush) {
	SolidRoundedRectangle(Rectangle, Radius, FillBrush);
	DrawRoundedRectangle(Rectangle, Radius, BorderPen);
}
void VCanvasPainter::DrawString(const std::wstring& String, const VRect& StringRectangle, VFont* TargetFont, VSolidBrush* TextBrush) {
	D2D1_RECT_F D2DRECT = {
		static_cast<FLOAT>(StringRectangle.Left),
		static_cast<FLOAT>(StringRectangle.Top),
		static_cast<FLOAT>(StringRectangle.Right),
		static_cast<FLOAT>(StringRectangle.Bottom)
	};

#ifdef UNICODE
	TargetDevice->DrawTextW(String.c_str(), static_cast<UINT32>(String.length()), TargetFont->GetDXObject(), D2DRECT, TextBrush->GetDxBrush());
#else
    TargetDevice->DrawTextA(String.c_str(), static_cast<UINT32>(String.length()), TargetFont->GetDXObject(), D2DRECT, TextBrush->GetDxBrush());
#endif
}
void VCanvasPainter::DrawImage(const VRect& TargetRectangle, VImage* SourceImage, const VRect& SourceRectangle, const float& ImageOpacity) {
	TargetDevice->DrawBitmap(SourceImage->GetDirectXObject(),
		D2D1::RectF(TargetRectangle.Left, TargetRectangle.Top, TargetRectangle.Right, TargetRectangle.Bottom),
		ImageOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(SourceRectangle.Left, SourceRectangle.Top, SourceRectangle.Right, SourceRectangle.Bottom));
}
void VCanvasPainter::DrawCanvas(const VRect& TargetRectangle, VCanvasPainter* SourceImage, const VRect& SourceRectangle, const float& ImageOpacity) {
    ID2D1Bitmap* DirectXObject;

    HRESULT Result = SourceImage->GetDXObject()->GetBitmap(&DirectXObject);
    VLIB_CHECK_REPORT(FAILED(Result), L"Get bitmap from VCanvasPainter failed!");

    TargetDevice->DrawBitmap(DirectXObject,
                             D2D1::RectF(TargetRectangle.Left, TargetRectangle.Top, TargetRectangle.Right, TargetRectangle.Bottom),
                             ImageOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
                             D2D1::RectF(SourceRectangle.Left, SourceRectangle.Top, SourceRectangle.Right, SourceRectangle.Bottom));

    VDXObjectSafeFree(&DirectXObject);
}

VPainter::VPainter(ID2D1RenderTarget* RenderTarget) {
	VLIB_CHECK_REPORT(RenderTarget == nullptr, L"Render Target should not be nullptr!");
    RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	
	TargetDevice = RenderTarget;
}
void VPainter::Clear(const VColor& Color) {
	TargetDevice->Clear(Color.GetDxObject());
}
void VPainter::BeginDraw() {
	TargetDevice->BeginDraw();
}
void VPainter::EndDraw() {
	TargetDevice->EndDraw();
}

void VPainter::DrawLine(const VPoint& StartPoint, const VPoint& EndPoint, VPenBrush* PenBrush) {
	TargetDevice->DrawLine(D2D1::Point2F(static_cast<FLOAT>(StartPoint.X), static_cast<FLOAT>(StartPoint.Y)), D2D1::Point2F(static_cast<FLOAT>(EndPoint.X), static_cast<FLOAT>(EndPoint.Y)), 
		PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle());
}
void VPainter::DrawRectangle(const VRect& Rectangle, VPenBrush* PenBrush) {
	TargetDevice->DrawRectangle(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top), static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
		PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle());
}
void VPainter::DrawEllipse(const VRect& EllipseRectangle, VPenBrush* PenBrush) {
	int RadiusX = EllipseRectangle.GetWidth() / 2;
	int RadiusY = EllipseRectangle.GetHeight() / 2;

	TargetDevice->DrawEllipse(
		D2D1_ELLIPSE
		{
			{
				static_cast<FLOAT>(EllipseRectangle.Left + RadiusX),
				static_cast<FLOAT>(EllipseRectangle.Top + RadiusY)
			},
			static_cast<FLOAT>(RadiusX), static_cast<FLOAT>(RadiusY)
		},
		PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle()
	);
}
void VPainter::DrawRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VPenBrush* PenBrush) {
	TargetDevice->DrawRoundedRectangle(D2D1::RoundedRect(
		D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top), static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
		static_cast<FLOAT>(Radius.X), static_cast<FLOAT>(Radius.Y)
	),
	PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle());
}

void VPainter::SolidRectangle(const VRect& Rectangle, VSolidBrush* PenBrush) {
	TargetDevice->FillRectangle(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top), static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
		PenBrush->GetDxBrush());
}
void VPainter::SolidEllipse(const VRect& EllipseRectangle, VSolidBrush* PenBrush) {
	int RadiusX = EllipseRectangle.GetWidth() / 2;
	int RadiusY = EllipseRectangle.GetHeight() / 2;

	TargetDevice->FillEllipse(
		D2D1_ELLIPSE
		{
			{
				static_cast<FLOAT>(EllipseRectangle.Left + RadiusX),
				static_cast<FLOAT>(EllipseRectangle.Top + RadiusY)
			},
			static_cast<FLOAT>(RadiusX), static_cast<FLOAT>(RadiusY)
		},
		PenBrush->GetDxBrush()
	);
}
void VPainter::SolidRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VSolidBrush* PenBrush) {
	TargetDevice->FillRoundedRectangle(D2D1::RoundedRect(
		D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top), static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
		static_cast<FLOAT>(Radius.X), static_cast<FLOAT>(Radius.Y)
	),
		PenBrush->GetDxBrush());
}

void VPainter::SolidRectangle(const VRect& Rectangle, VBitmapBrush* PenBrush) {
    TargetDevice->FillRectangle(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top), static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
                                PenBrush->GetDxBrush());
}
void VPainter::SolidEllipse(const VRect& EllipseRectangle, VBitmapBrush* PenBrush) {
    int RadiusX = EllipseRectangle.GetWidth() / 2;
    int RadiusY = EllipseRectangle.GetHeight() / 2;

    TargetDevice->FillEllipse(
            D2D1_ELLIPSE
                    {
                            {
                                    static_cast<FLOAT>(EllipseRectangle.Left + RadiusX),
                                    static_cast<FLOAT>(EllipseRectangle.Top + RadiusY)
                            },
                            static_cast<FLOAT>(RadiusX), static_cast<FLOAT>(RadiusY)
                    },
            PenBrush->GetDxBrush()
    );
}
void VPainter::SolidRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VBitmapBrush* PenBrush) {
    TargetDevice->FillRoundedRectangle(D2D1::RoundedRect(
                                               D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top), static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
                                               static_cast<FLOAT>(Radius.X), static_cast<FLOAT>(Radius.Y)
                                       ),
                                       PenBrush->GetDxBrush());
}

void VPainter::FillRectangle(const VRect& Rectangle, VPenBrush* BorderPen, VSolidBrush* FillBrush) {
	SolidRectangle(Rectangle, FillBrush);
	DrawEllipse(Rectangle, BorderPen);
}
void VPainter::FillEllipse(const VRect& EllipseRectangle, VPenBrush* BorderPen, VSolidBrush* FillBrush) {
	SolidEllipse(EllipseRectangle, FillBrush);
	DrawEllipse(EllipseRectangle, BorderPen);
}
void VPainter::FillRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VPenBrush* BorderPen, VSolidBrush* FillBrush) {
	SolidRoundedRectangle(Rectangle, Radius, FillBrush);
	DrawRoundedRectangle(Rectangle, Radius, BorderPen);
}
void VPainter::DrawString(const std::wstring& String, const VRect& StringRectangle, VFont* TargetFont, VPenBrush* TextBrush) {
	D2D1_RECT_F D2DRECT = { 
		static_cast<FLOAT>(StringRectangle.Left), 
		static_cast<FLOAT>(StringRectangle.Top),
		static_cast<FLOAT>(StringRectangle.Right),
		static_cast<FLOAT>(StringRectangle.Bottom)
	};
#ifdef UNICODE
	TargetDevice->DrawTextW(String.c_str(), static_cast<UINT32>(String.length()), TargetFont->GetDXObject(), D2DRECT, TextBrush->GetDxBrush());
#else
    TargetDevice->DrawTextA(String.c_str(), static_cast<UINT32>(String.length()), TargetFont->GetDXObject(), D2DRECT, TextBrush->GetDxBrush());
#endif
}
void VPainter::DrawImage(const VRect& TargetRectangle, VImage* SourceImage, const VRect& SourceRectangle, const float& ImageOpacity) {
	TargetDevice->DrawBitmap(SourceImage->GetDirectXObject(),
		D2D1::RectF(TargetRectangle.Left, TargetRectangle.Top, TargetRectangle.Right, TargetRectangle.Bottom),
		ImageOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(SourceRectangle.Left, SourceRectangle.Top, SourceRectangle.Right, SourceRectangle.Bottom));
}
void VPainter::DrawCanvas(const VRect& TargetRectangle, VCanvasPainter* SourceImage, const VRect& SourceRectangle, const float& ImageOpacity) {
	ID2D1Bitmap* DirectXObject;

	HRESULT Result = SourceImage->GetDXObject()->GetBitmap(&DirectXObject);
	VLIB_CHECK_REPORT(FAILED(Result), L"Get bitmap from VCanvasPainter failed!");

	TargetDevice->DrawBitmap(DirectXObject,
		D2D1::RectF(TargetRectangle.Left, TargetRectangle.Top, TargetRectangle.Right, TargetRectangle.Bottom),
		ImageOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(SourceRectangle.Left, SourceRectangle.Top, SourceRectangle.Right, SourceRectangle.Bottom));

	VDXObjectSafeFree(&DirectXObject);
}

}

VLIB_END_NAMESPACE
