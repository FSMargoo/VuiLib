#include "../../../include/core/uibasic/vnativeicon.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
VCanvasPainter *VNativeIcon::CheckImage(const VColor &Color, const int &Width, const int &Height,
										const VRenderHandle &Handle, const int &Thickness)
{
	VCanvasPainter *Painter = new VCanvasPainter(Width, Height, Handle);
	VPenBrush	   *Brush	= new VPenBrush(Color, Handle, Thickness);

	ID2D1StrokeStyle *StrokeStyle;
	VDirectXD2DFactory.GetInstance()->CreateStrokeStyle(
		D2D1::StrokeStyleProperties(D2D1_CAP_STYLE::D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE::D2D1_CAP_STYLE_ROUND),
		nullptr, 0, &StrokeStyle);

	Brush->SetStrokeStyle(StrokeStyle);

	VPoint Point1 = VPoint(Width * 0.26f, Height * 0.50f);
	VPoint Point2 = VPoint(Width * 0.49f, Height * 0.71f);
	VPoint Point3 = VPoint(Width * 0.75f, Height * 0.35f);

	Painter->BeginDraw();

	Painter->DrawLine(Point1, Point2, Brush);
	Painter->DrawLine(Point2, Point3, Brush);

	Painter->EndDraw();

	delete Brush;

	return Painter;
}
VCanvasPainter *VNativeIcon::DropDownUpIcon(const VColor &Color, const int &Width, const int &Height,
											const VRenderHandle &Handle, const int &Thickness)
{

	VCanvasPainter *Painter = new VCanvasPainter(Width, Height, Handle);
	VPenBrush	   *Brush	= new VPenBrush(Color, Handle, Thickness);

	ID2D1StrokeStyle *StrokeStyle;
	VDirectXD2DFactory.GetInstance()->CreateStrokeStyle(
		D2D1::StrokeStyleProperties(D2D1_CAP_STYLE::D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE::D2D1_CAP_STYLE_ROUND),
		nullptr, 0, &StrokeStyle);

	Brush->SetStrokeStyle(StrokeStyle);

	VPoint Point1 = VPoint(Width * 0.1f, Height * 0.50f);
	VPoint Point2 = VPoint(Width * 0.5f, Height * 0.91f);
	VPoint Point3 = VPoint(Width * 0.9f, Height * 0.5f);

	Painter->BeginDraw();

	Painter->DrawLine(Point1, Point2, Brush);
	Painter->DrawLine(Point2, Point3, Brush);

	Painter->EndDraw();

	delete Brush;

	return Painter;
}
VCanvasPainter *VNativeIcon::DropDownDownIcon(const VColor &Color, const int &Width, const int &Height,
											  const VRenderHandle &Handle, const int &Thickness)
{
	VCanvasPainter *Painter = new VCanvasPainter(Width, Height, Handle);
	VPenBrush	   *Brush	= new VPenBrush(Color, Handle, Thickness);

	ID2D1StrokeStyle *StrokeStyle;
	VDirectXD2DFactory.GetInstance()->CreateStrokeStyle(
		D2D1::StrokeStyleProperties(D2D1_CAP_STYLE::D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE::D2D1_CAP_STYLE_ROUND),
		nullptr, 0, &StrokeStyle);

	Brush->SetStrokeStyle(StrokeStyle);

	VPoint Point1 = VPoint(Width * 0.1f, Height * 0.50f);
	VPoint Point2 = VPoint(Width * 0.5f, Height * 0.10f);
	VPoint Point3 = VPoint(Width * 0.9f, Height * 0.50f);

	Painter->BeginDraw();

	Painter->DrawLine(Point1, Point2, Brush);
	Painter->DrawLine(Point2, Point3, Brush);

	Painter->EndDraw();

	delete Brush;

	return Painter;
}
} // namespace Core

VLIB_END_NAMESPACE