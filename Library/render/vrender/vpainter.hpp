/*
 * VPainter.hpp
 *	@description : The Painter Of VRender
 *	@birth		 : 2022/6.4
*/

#pragma warning(disable : 4244)

#pragma once

#include "vpen.hpp"
#include "vbrush.hpp"
#include "vfont.hpp"
#include "vcanvas.hpp"
#include "vrenderbasic.hpp"
#include "vpainterdevice.hpp"
#include "vcolor.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VPainter class:
 *	@description  : The Painter in VRender
*/
class VPainterDevice {
private:
	VMemoryPtr<VGdiplus::Graphics> NativeGraphics;

public:
	VGdiplus::Graphics* GetNativeGraphics() {
		return NativeGraphics.get();
	}

public:
	/*
	 * Build up Functional
	*/

	VPainterDevice(VPaintbleObject* TargetDevice) {
		switch (TargetDevice->GetDeviceType()) {
		case VPaintbleType::HDCPainter: {
			NativeGraphics.reset(new VGdiplus::Graphics(TargetDevice->GetTargetHDC()));

			break;
		}
		case VPaintbleType::ImagePainter: {
			NativeGraphics.reset(new VGdiplus::Graphics(TargetDevice->GetTargetBitmap()));

			break;
		}
		}

		/* Set the Render Quality as High Quality */
		NativeGraphics->SetSmoothingMode(VGdiplus::SmoothingMode::SmoothingModeHighQuality);
		NativeGraphics->SetTextRenderingHint(VGdiplus::TextRenderingHint::TextRenderingHintAntiAliasGridFit);
		NativeGraphics->SetInterpolationMode(VGdiplus::InterpolationMode::InterpolationModeDefault);
	}

	/*
	 * Draw Functional
	*/

	void DrawRectangle(VPen* Pen, VRect        Rect) {
		NativeGraphics->DrawRectangle(Pen->GetNativePen(), Rect.ToGdiplusRect());
	}
	void FillRectangle(VPen* Pen, VBasicBrush* Brush, VRect  Rect) {
		NativeGraphics->FillRectangle(Brush->GetNativeBrush(), Rect.ToGdiplusRect());
		NativeGraphics->DrawRectangle(Pen->GetNativePen(), Rect.ToGdiplusRect());
	}
	void SolidRectangle(VBasicBrush* Brush, VRect        Rect) {
		NativeGraphics->FillRectangle(Brush->GetNativeBrush(), Rect.ToGdiplusRect());
	}

	void DrawEllipse(VPen* Pen, VRect        Rect) {
		NativeGraphics->DrawEllipse(Pen->GetNativePen(), Rect.ToGdiplusRect());
	}
	void FillEllipse(VPen* Pen, VBasicBrush* Brush, VRect  Rect) {
		NativeGraphics->FillEllipse(Brush->GetNativeBrush(), Rect.ToGdiplusRect());
		NativeGraphics->DrawEllipse(Pen->GetNativePen(), Rect.ToGdiplusRect());
	}
	void SolidEllipse(VBasicBrush* Brush, VRect        Rect) {
		NativeGraphics->FillEllipse(Brush->GetNativeBrush(), Rect.ToGdiplusRect());
	}

	void DrawRoundedRectangle(VPen* Pen, VRect        Rect, VPoint Radius) {
		/* Use Gdiplus Path */
		VGdiplus::GraphicsPath RectanglePath;

		if (Radius.x < Rect.right - Rect.left && Radius.y < Rect.bottom - Rect.top) {
			RectanglePath.AddArc(Rect.left, Rect.top, Radius.x, Radius.y, 180, 90);
			RectanglePath.AddLine(Rect.left + Radius.x, Rect.top, Rect.right - Radius.x, Rect.top);

			RectanglePath.AddArc(Rect.right - Radius.x, Rect.top, Radius.x, Radius.y, 270, 90);
			RectanglePath.AddLine(Rect.right, Rect.top + Radius.y, Rect.right, Rect.bottom - Radius.y);

			RectanglePath.AddArc(Rect.right - Radius.x, Rect.bottom - Radius.y, Radius.x, Radius.y, 0, 90);
			RectanglePath.AddLine(Rect.right - Radius.x, Rect.bottom, Rect.left + Radius.x, Rect.bottom);

			RectanglePath.AddArc(Rect.left, Rect.bottom - Radius.y, Radius.x, Radius.y, 90, 90);
			RectanglePath.AddLine(Rect.left, Rect.bottom - Radius.y, Rect.left, Rect.top + Radius.y);
		}
		else {
			RectanglePath.AddArc(Rect.left, Rect.top, Radius.x, Radius.y, 180, 90);
			RectanglePath.AddArc(Rect.right - Radius.x, Rect.top, Radius.x, Radius.y, 270, 90);
			RectanglePath.AddArc(Rect.right - Radius.x, Rect.bottom - Radius.y, Radius.x, Radius.y, 0, 90);
			RectanglePath.AddArc(Rect.left, Rect.bottom - Radius.y, Radius.x, Radius.y, 90, 90);
		}

		RectanglePath.CloseFigure();

		NativeGraphics->DrawPath(Pen->GetNativePen(), &RectanglePath);
	}
	void SolidRoundedRectangle(VBasicBrush* Brush, VRect        Rect, VPoint Radius) {
		VGdiplus::GraphicsPath FillRectanglePath;

		if (Radius.x < Rect.right - Rect.left && Radius.y < Rect.bottom - Rect.top) {
			FillRectanglePath.AddArc(Rect.left, Rect.top, Radius.x, Radius.y, 180, 90);
			FillRectanglePath.AddLine(Rect.left + Radius.x, Rect.top, Rect.right - Radius.x, Rect.top);

			FillRectanglePath.AddArc(Rect.right - Radius.x, Rect.top, Radius.x, Radius.y, 270, 90);
			FillRectanglePath.AddLine(Rect.right, Rect.top + Radius.y, Rect.right, Rect.bottom - Radius.y);

			FillRectanglePath.AddArc(Rect.right - Radius.x, Rect.bottom - Radius.y, Radius.x, Radius.y, 0, 90);
			FillRectanglePath.AddLine(Rect.right - Radius.x, Rect.bottom, Rect.left + Radius.x, Rect.bottom);

			FillRectanglePath.AddArc(Rect.left, Rect.bottom - Radius.y, Radius.x, Radius.y, 90, 90);
			FillRectanglePath.AddLine(Rect.left, Rect.bottom - Radius.y, Rect.left, Rect.top + Radius.y);
		}
		else {
			FillRectanglePath.AddArc(Rect.left, Rect.top, Radius.x, Radius.y, 180, 90);
			FillRectanglePath.AddArc(Rect.right - Radius.x, Rect.top, Radius.x, Radius.y, 270, 90);
			FillRectanglePath.AddArc(Rect.right - Radius.x, Rect.bottom - Radius.y, Radius.x, Radius.y, 0, 90);
			FillRectanglePath.AddArc(Rect.left, Rect.bottom - Radius.y, Radius.x, Radius.y, 90, 90);
		}

		FillRectanglePath.CloseFigure();

		NativeGraphics->FillPath(Brush->GetNativeBrush(), &FillRectanglePath);
	}
	void FillRoundedRectangle(VPen* Pen, VBasicBrush* Brush, VRect  Rect, VPoint Radius) {
		Rect.Offset(Pen->GetThinkness(), Pen->GetThinkness(), -Pen->GetThinkness(), -Pen->GetThinkness());

		SolidRoundedRectangle(Brush, Rect, Radius);

		DrawRoundedRectangle(Pen, Rect, Radius);
	}

	void DrawString(std::wstring String, VPoint Position, VFont* Font, VBasicBrush* Brush) {
		NativeGraphics->DrawString(String.c_str(), -1, Font->GetNativeFont(), Position.ToGdiplusPointF(), Brush->GetNativeBrush());
	}
	void DrawString(std::wstring String, VFont* Font, VBasicBrush* Brush, VFontFormat* Format, VRect Rect) {
		NativeGraphics->DrawString(String.c_str(), -1, Font->GetNativeFont(), Rect.ToGdiplusRectF(), Format->GetNativeFontFormat(),
			Brush->GetNativeBrush());
	}
	void DrawImage(VImage* Image, VRect  Rect) {
		NativeGraphics->DrawImage(Image->GetNativeImage(), Rect.ToGdiplusRect(), 
			0, 0, Image->GetWidth(), Image->GetHeight(), VGdiplus::UnitPixel, Image->GetNativeAttributes());
	}
	void DrawImage(VImage* Image, int X, int Y) {
		NativeGraphics->DrawImage(Image->GetNativeImage(), { X, Y, Image->GetWidth(), Image->GetHeight() },
			0, 0, Image->GetWidth(), Image->GetHeight(), VGdiplus::UnitPixel, Image->GetNativeAttributes());
	}
};

VLIB_END_NAMESPACE