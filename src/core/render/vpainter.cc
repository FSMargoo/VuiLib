#pragma warning(disable : 4244)

#include "../../../include/core/render/vpainter.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
VCanvasPainter::VCanvasPainter(const int &Width, const int &Height, const VRenderHandle &RenderHandle)
	: TargetDevice(RenderHandle.Allocator)
{
	RenderHandle._IRenderTarget->CreateCompatibleRenderTarget(D2D1::SizeF(Width, Height), &TargetDevice.Object);
}
VCanvasPainter::~VCanvasPainter()
{
	TargetDevice.Object->Release();
	TargetDevice.Object = NULL;
}
void VCanvasPainter::Clear(const VColor &Color)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->Clear(Color.GetDxObject());
#endif
}
void VCanvasPainter::BeginDraw()
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->BeginDraw();
#endif
}
void VCanvasPainter::EndDraw()
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->EndDraw();
#endif
}
void VCanvasPainter::DrawLine(const VPoint &StartPoint, const VPoint &EndPoint, VPenBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->DrawLine(D2D1::Point2F(static_cast<FLOAT>(StartPoint.X), static_cast<FLOAT>(StartPoint.Y)),
						   D2D1::Point2F(static_cast<FLOAT>(EndPoint.X), static_cast<FLOAT>(EndPoint.Y)),
						   PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle());
#endif
}
void VCanvasPainter::DrawRectangle(const VRect &Rectangle, VPenBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->DrawRectangle(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top),
											static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
								PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle());
#endif
}
void VCanvasPainter::DrawEllipse(const VRect &EllipseRectangle, VPenBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	int RadiusX = EllipseRectangle.GetWidth() / 2;
	int RadiusY = EllipseRectangle.GetHeight() / 2;

	TargetDevice->DrawEllipse(D2D1_ELLIPSE{{static_cast<FLOAT>(EllipseRectangle.Left + RadiusX),
											static_cast<FLOAT>(EllipseRectangle.Top + RadiusY)},
										   static_cast<FLOAT>(RadiusX),
										   static_cast<FLOAT>(RadiusY)},
							  PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle());
#endif
}
void VCanvasPainter::DrawRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VPenBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->DrawRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top),
									  static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
						  static_cast<FLOAT>(Radius.X), static_cast<FLOAT>(Radius.Y)),
		PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle());
#endif
}

void VCanvasPainter::SolidRectangle(const VRect &Rectangle, VSolidBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->FillRectangle(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top),
											static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
								PenBrush->GetDxBrush());
#endif
}
void VCanvasPainter::SolidEllipse(const VRect &EllipseRectangle, VSolidBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	int RadiusX = EllipseRectangle.GetWidth() / 2;
	int RadiusY = EllipseRectangle.GetHeight() / 2;

	TargetDevice->FillEllipse(D2D1_ELLIPSE{{static_cast<FLOAT>(EllipseRectangle.Left + RadiusX),
											static_cast<FLOAT>(EllipseRectangle.Top + RadiusY)},
										   static_cast<FLOAT>(RadiusX),
										   static_cast<FLOAT>(RadiusY)},
							  PenBrush->GetDxBrush());
#endif
}
void VCanvasPainter::SolidRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VSolidBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->FillRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top),
									  static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
						  static_cast<FLOAT>(Radius.X), static_cast<FLOAT>(Radius.Y)),
		PenBrush->GetDxBrush());
#endif
}

void VCanvasPainter::SolidRectangle(const VRect &Rectangle, VBitmapBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->FillRectangle(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top),
											static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
								PenBrush->GetDxBrush());
#endif
}
void VCanvasPainter::SolidEllipse(const VRect &EllipseRectangle, VBitmapBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	int RadiusX = EllipseRectangle.GetWidth() / 2;
	int RadiusY = EllipseRectangle.GetHeight() / 2;

	TargetDevice->FillEllipse(D2D1_ELLIPSE{{static_cast<FLOAT>(EllipseRectangle.Left + RadiusX),
											static_cast<FLOAT>(EllipseRectangle.Top + RadiusY)},
										   static_cast<FLOAT>(RadiusX),
										   static_cast<FLOAT>(RadiusY)},
							  PenBrush->GetDxBrush());
#endif
}
void VCanvasPainter::SolidRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VBitmapBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->FillRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top),
									  static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
						  static_cast<FLOAT>(Radius.X), static_cast<FLOAT>(Radius.Y)),
		PenBrush->GetDxBrush());
#endif
}

void VCanvasPainter::FillRectangle(const VRect &Rectangle, VPenBrush *BorderPen, VSolidBrush *FillBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	SolidRectangle(Rectangle, FillBrush);
	DrawRectangle(Rectangle, BorderPen);
#endif
}
void VCanvasPainter::FillEllipse(const VRect &EllipseRectangle, VPenBrush *BorderPen, VSolidBrush *FillBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	SolidEllipse(EllipseRectangle, FillBrush);
#endif
}
void VCanvasPainter::FillRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VPenBrush *BorderPen,
										  VSolidBrush *FillBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	SolidRoundedRectangle(Rectangle, Radius, FillBrush);

	DrawRoundedRectangle(Rectangle, Radius, BorderPen);
#endif
}
void VCanvasPainter::DrawString(const VString &String, const VRect &StringRectangle, VFont *TargetFont,
								VSolidBrush *TextBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	D2D1_RECT_F D2DRECT = {static_cast<FLOAT>(StringRectangle.Left), static_cast<FLOAT>(StringRectangle.Top),
						   static_cast<FLOAT>(StringRectangle.Right), static_cast<FLOAT>(StringRectangle.Bottom)};

#ifdef UNICODE
	TargetDevice->DrawTextW(String.CStyleString(), static_cast<UINT32>(String.Length()), TargetFont->GetDXObject(),
							D2DRECT, TextBrush->GetDxBrush());
#else
	TargetDevice->DrawTextA(String.CStyleString(), static_cast<UINT32>(String.Length()), TargetFont->GetDXObject(),
							D2DRECT, TextBrush->GetDxBrush());
#endif
#endif
}
void VCanvasPainter::DrawImage(const VRect &TargetRectangle, VImage *SourceImage, const VRect &SourceRectangle,
							   const float &ImageOpacity)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->DrawBitmap(
		SourceImage->GetDirectXObject(),
		D2D1::RectF(TargetRectangle.Left, TargetRectangle.Top, TargetRectangle.Right, TargetRectangle.Bottom),
		ImageOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(SourceRectangle.Left, SourceRectangle.Top, SourceRectangle.Right, SourceRectangle.Bottom));
#endif
}
void VCanvasPainter::DrawCanvas(const VRect &TargetRectangle, VCanvasPainter *SourceImage, const VRect &SourceRectangle,
								const float &ImageOpacity)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	ID2D1Bitmap *DirectXObject;

	HRESULT Result = SourceImage->GetDXObject()->GetBitmap(&DirectXObject);
	VLIB_CHECK_REPORT(FAILED(Result), L"Get bitmap from VCanvasPainter failed!");

	TargetDevice->DrawBitmap(
		DirectXObject,
		D2D1::RectF(TargetRectangle.Left, TargetRectangle.Top, TargetRectangle.Right, TargetRectangle.Bottom),
		ImageOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(SourceRectangle.Left, SourceRectangle.Top, SourceRectangle.Right, SourceRectangle.Bottom));

	VDXObjectSafeFree(&DirectXObject);
#endif
}

VPainter::VPainter(const VRenderHandle &RenderHandle)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	VLIB_CHECK_REPORT(RenderHandle._IRenderTarget == nullptr, L"Invalid render handle was specified");
	RenderHandle._IRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

	TargetDevice = RenderHandle._IRenderTarget;
#endif
}
VPainter::~VPainter()
{
	TargetDevice->Release();
	TargetDevice = NULL;
}
void VPainter::Clear(const VColor &Color)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->Clear(Color.GetDxObject());
#endif
}
void VPainter::BeginDraw()
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->BeginDraw();
#endif
}
void VPainter::EndDraw()
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->EndDraw();
#endif
}

void VPainter::DrawLine(const VPoint &StartPoint, const VPoint &EndPoint, VPenBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->DrawLine(D2D1::Point2F(static_cast<FLOAT>(StartPoint.X), static_cast<FLOAT>(StartPoint.Y)),
						   D2D1::Point2F(static_cast<FLOAT>(EndPoint.X), static_cast<FLOAT>(EndPoint.Y)),
						   PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle());
#endif
}
void VPainter::DrawRectangle(const VRect &Rectangle, VPenBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->DrawRectangle(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top),
											static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
								PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle());
#endif
}
void VPainter::DrawEllipse(const VRect &EllipseRectangle, VPenBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	int RadiusX = EllipseRectangle.GetWidth() / 2;
	int RadiusY = EllipseRectangle.GetHeight() / 2;

	TargetDevice->DrawEllipse(D2D1_ELLIPSE{{static_cast<FLOAT>(EllipseRectangle.Left + RadiusX),
											static_cast<FLOAT>(EllipseRectangle.Top + RadiusY)},
										   static_cast<FLOAT>(RadiusX),
										   static_cast<FLOAT>(RadiusY)},
							  PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle());
#endif
}
void VPainter::DrawRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VPenBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->DrawRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top),
									  static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
						  static_cast<FLOAT>(Radius.X), static_cast<FLOAT>(Radius.Y)),
		PenBrush->GetDxBrush(), PenBrush->GetThickness(), PenBrush->GetStrokeStyle());
#endif
}

void VPainter::SolidRectangle(const VRect &Rectangle, VSolidBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->FillRectangle(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top),
											static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
								PenBrush->GetDxBrush());
#endif
}
void VPainter::SolidEllipse(const VRect &EllipseRectangle, VSolidBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	int RadiusX = EllipseRectangle.GetWidth() / 2;
	int RadiusY = EllipseRectangle.GetHeight() / 2;

	TargetDevice->FillEllipse(D2D1_ELLIPSE{{static_cast<FLOAT>(EllipseRectangle.Left + RadiusX),
											static_cast<FLOAT>(EllipseRectangle.Top + RadiusY)},
										   static_cast<FLOAT>(RadiusX),
										   static_cast<FLOAT>(RadiusY)},
							  PenBrush->GetDxBrush());
#endif
}
void VPainter::SolidRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VSolidBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->FillRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top),
									  static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
						  static_cast<FLOAT>(Radius.X), static_cast<FLOAT>(Radius.Y)),
		PenBrush->GetDxBrush());
#endif
}

void VPainter::SolidRectangle(const VRect &Rectangle, VBitmapBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->FillRectangle(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top),
											static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
								PenBrush->GetDxBrush());
#endif
}
void VPainter::SolidEllipse(const VRect &EllipseRectangle, VBitmapBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	int RadiusX = EllipseRectangle.GetWidth() / 2;
	int RadiusY = EllipseRectangle.GetHeight() / 2;

	TargetDevice->FillEllipse(D2D1_ELLIPSE{{static_cast<FLOAT>(EllipseRectangle.Left + RadiusX),
											static_cast<FLOAT>(EllipseRectangle.Top + RadiusY)},
										   static_cast<FLOAT>(RadiusX),
										   static_cast<FLOAT>(RadiusY)},
							  PenBrush->GetDxBrush());
#endif
}
void VPainter::SolidRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VBitmapBrush *PenBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->FillRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(static_cast<FLOAT>(Rectangle.Left), static_cast<FLOAT>(Rectangle.Top),
									  static_cast<FLOAT>(Rectangle.Right), static_cast<FLOAT>(Rectangle.Bottom)),
						  static_cast<FLOAT>(Radius.X), static_cast<FLOAT>(Radius.Y)),
		PenBrush->GetDxBrush());
#endif
}

void VPainter::FillRectangle(const VRect &Rectangle, VPenBrush *BorderPen, VSolidBrush *FillBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	SolidRectangle(Rectangle, FillBrush);
	DrawEllipse(Rectangle, BorderPen);
#endif
}
void VPainter::FillEllipse(const VRect &EllipseRectangle, VPenBrush *BorderPen, VSolidBrush *FillBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	SolidEllipse(EllipseRectangle, FillBrush);
	DrawEllipse(EllipseRectangle, BorderPen);
#endif
}
void VPainter::FillRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VPenBrush *BorderPen,
									VSolidBrush *FillBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	SolidRoundedRectangle(Rectangle, Radius, FillBrush);
	DrawRoundedRectangle(Rectangle, Radius, BorderPen);
#endif
}
void VPainter::DrawString(const VString &String, const VRect &StringRectangle, VFont *TargetFont, VPenBrush *TextBrush)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	D2D1_RECT_F D2DRECT = {static_cast<FLOAT>(StringRectangle.Left), static_cast<FLOAT>(StringRectangle.Top),
						   static_cast<FLOAT>(StringRectangle.Right), static_cast<FLOAT>(StringRectangle.Bottom)};
#ifdef UNICODE
	TargetDevice->DrawTextW(String.CStyleString(), static_cast<UINT32>(String.length()), TargetFont->GetDXObject(),
							D2DRECT, TextBrush->GetDxBrush());
#else
	TargetDevice->DrawTextA(String.CStyleString(), static_cast<UINT32>(String.length()), TargetFont->GetDXObject(),
							D2DRECT, TextBrush->GetDxBrush());
#endif
#endif
}
void VPainter::DrawImage(const VRect &TargetRectangle, VImage *SourceImage, const VRect &SourceRectangle,
						 const float &ImageOpacity)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	TargetDevice->DrawBitmap(
		SourceImage->GetDirectXObject(),
		D2D1::RectF(TargetRectangle.Left, TargetRectangle.Top, TargetRectangle.Right, TargetRectangle.Bottom),
		ImageOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(SourceRectangle.Left, SourceRectangle.Top, SourceRectangle.Right, SourceRectangle.Bottom));
#endif
}
void VPainter::DrawCanvas(const VRect &TargetRectangle, VCanvasPainter *SourceImage, const VRect &SourceRectangle,
						  const float &ImageOpacity)
{
#ifndef _VLIB_CANVAS_DEBUING_S
	ID2D1Bitmap *DirectXObject;

	HRESULT Result = SourceImage->GetDXObject()->GetBitmap(&DirectXObject);
	VLIB_CHECK_REPORT(FAILED(Result), L"Get bitmap from VCanvasPainter failed!");

	TargetDevice->DrawBitmap(
		DirectXObject,
		D2D1::RectF(TargetRectangle.Left, TargetRectangle.Top, TargetRectangle.Right, TargetRectangle.Bottom),
		ImageOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(SourceRectangle.Left, SourceRectangle.Top, SourceRectangle.Right, SourceRectangle.Bottom));

	VDXObjectSafeFree(&DirectXObject);
#endif
}
} // namespace Core

VLIB_END_NAMESPACE
