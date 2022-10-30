// License(MIT)
// Athuor: Margoo
// The painter class
#pragma once

#include "vpainterdevice.h"
#include "vbrush.h"
#include "vimage.h"
#include "vwrite.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

class VCanvasPainter {
 private:
	 ID2D1BitmapRenderTarget* TargetDevice{};

 public:
	 VCanvasPainter(const int& Width, const int& Height, ID2D1RenderTarget* SourceDevice);
	 ~VCanvasPainter();

 public:
	 ID2D1BitmapRenderTarget* GetDXObject() {
		 return TargetDevice;
	 }

 public:
	 void BeginDraw();
	 void EndDraw();

 public:
	 void Clear(const VColor& FlushColor);

	 void DrawLine(const VPoint& StartPoint, const VPoint& EndPoint, VPenBrush* PenBrush);
	 void DrawRectangle(const VRect& Rectangle, VPenBrush* PenBrush);
	 void DrawEllipse(const VRect& EllipseRectangle, VPenBrush* PenBrush);
	 void DrawRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VPenBrush* PenBrush);

	 void SolidRectangle(const VRect& Rectangle, VSolidBrush* PenBrush);
	 void SolidEllipse(const VRect& EllipseRectangle, VSolidBrush* PenBrush);
	 void SolidRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VSolidBrush* PenBrush);
     void SolidRectangle(const VRect& Rectangle, VBitmapBrush* PenBrush);
     void SolidEllipse(const VRect& EllipseRectangle, VBitmapBrush* PenBrush);
     void SolidRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VBitmapBrush* PenBrush);

	 void FillRectangle(const VRect& Rectangle, VPenBrush* BorderPen, VSolidBrush* FillBrush);
	 void FillEllipse(const VRect& EllipseRectangle, VPenBrush* BorderPen, VSolidBrush* FillBrush);
	 void FillRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VPenBrush* BorderPen, VSolidBrush* FillBrush);

	 void DrawString(const std::wstring& String, const VRect& StringRectangle, VFont* TargetFont, VSolidBrush* TextBrush);

	 void DrawImage(const VRect& TargetRectangle, VImage* SourceImage, const VRect& SourceRectangle, const float& ImageOpacity);
     void DrawCanvas(const VRect& TargetRectangle, VCanvasPainter* SourceImage, const VRect& SourceRectangle, const float& ImageOpacity);
};

class VPainter {
 private:
	 ID2D1RenderTarget* TargetDevice;

 public:
	 explicit VPainter(ID2D1RenderTarget* RenderTarget);
	 ~VPainter();

 public:
	 void BeginDraw();
	 void EndDraw();

 public:
	 void Clear(const VColor& FlushColor);

	 void DrawLine(const VPoint& StartPoint, const VPoint& EndPoint, VPenBrush* PenBrush);
	 void DrawRectangle(const VRect& Rectangle, VPenBrush* PenBrush);
	 void DrawEllipse(const VRect& EllipseRectangle, VPenBrush* PenBrush);
	 void DrawRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VPenBrush* PenBrush);

	 void SolidRectangle(const VRect& Rectangle, VSolidBrush* PenBrush);
	 void SolidEllipse(const VRect& EllipseRectangle, VSolidBrush* PenBrush);
	 void SolidRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VSolidBrush* PenBrush);
     void SolidRectangle(const VRect& Rectangle, VBitmapBrush* PenBrush);
     void SolidEllipse(const VRect& EllipseRectangle, VBitmapBrush* PenBrush);
     void SolidRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VBitmapBrush* PenBrush);

	 void FillRectangle(const VRect& Rectangle, VPenBrush* BorderPen, VSolidBrush* FillBrush);
	 void FillEllipse(const VRect& EllipseRectangle, VPenBrush* BorderPen, VSolidBrush* FillBrush);
	 void FillRoundedRectangle(const VRect& Rectangle, const VPoint& Radius, VPenBrush* BorderPen, VSolidBrush* FillBrush);

	 void DrawString(const std::wstring& String, const VRect& StringRectangle, VFont* TargetFont, VPenBrush* TextBrush);

	 void DrawImage(const VRect& TargetRectangle, VImage* SourceImage, const VRect& SourceRectangle, const float& ImageOpacity);
	 void DrawCanvas(const VRect& TargetRectangle, VCanvasPainter* SourceImage, const VRect& SourceRectangle, const float& ImageOpacity);
};

}

VLIB_END_NAMESPACE
