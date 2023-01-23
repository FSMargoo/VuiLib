/*
 * File name	: vpainter.h
 * Author		: Margoo
 * Date			: 11/13/2022
 * Description	: This head define the painter class to paint on surface
 */
#pragma once

#include "vbrush.h"
#include "vimage.h"
#include "vpainterdevice.h"
#include "vwrite.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
/*
 * VCanvasPainter class:
 *	Description : This class provide a painter to paint on canvas surface which is
 *				  created by this class
 */
class VCanvasPainter
{
public:
	/*
	 * VCanvasPainter constructor:
	 *		Description : The VCanvasPainter of class VImage
	 *		Values Tip	: {
	 *			[const int&			 ] Width		: The width of canvas
	 *			[const int&			 ] Height		: The height of canvas
	 *			[const VRenderHandle&] RenderHandle : The render handle of renderer environment
	 *		}
	 */
	VCanvasPainter(const int &Width, const int &Height, const VRenderHandle &RenderHandle);
	~VCanvasPainter();

public:
	void BeginDraw();
	void EndDraw();

public:
	void Clear(const VColor &FlushColor);

	void DrawLine(const VPoint &StartPoint, const VPoint &EndPoint, VPenBrush *PenBrush);
	void DrawRectangle(const VRect &Rectangle, VPenBrush *PenBrush);
	void DrawEllipse(const VRect &EllipseRectangle, VPenBrush *PenBrush);
	void DrawRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VPenBrush *PenBrush);

	void SolidRectangle(const VRect &Rectangle, VSolidBrush *PenBrush);
	void SolidEllipse(const VRect &EllipseRectangle, VSolidBrush *PenBrush);
	void SolidRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VSolidBrush *PenBrush);
	void SolidRectangle(const VRect &Rectangle, VBitmapBrush *PenBrush);
	void SolidEllipse(const VRect &EllipseRectangle, VBitmapBrush *PenBrush);
	void SolidRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VBitmapBrush *PenBrush);

	void FillRectangle(const VRect &Rectangle, VPenBrush *BorderPen, VSolidBrush *FillBrush);
	void FillEllipse(const VRect &EllipseRectangle, VPenBrush *BorderPen, VSolidBrush *FillBrush);
	void FillRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VPenBrush *BorderPen,
							  VSolidBrush *FillBrush);

	void DrawString(const std::wstring &String, const VRect &StringRectangle, VFont *TargetFont,
					VSolidBrush *TextBrush);

	void DrawImage(const VRect &TargetRectangle, VImage *SourceImage, const VRect &SourceRectangle,
				   const float &ImageOpacity);
	void DrawCanvas(const VRect &TargetRectangle, VCanvasPainter *SourceImage, const VRect &SourceRectangle,
					const float &ImageOpacity);

public:
	ID2D1BitmapRenderTarget *GetDXObject() VRENDER_HELPER
	{
		return TargetDevice.Get();
	}

private:
	VKits::VCOMPointer<ID2D1BitmapRenderTarget> TargetDevice;
};

/*
 * VPainter class:
 *	Description : This class provide a painter on a surface, but this class
 *				  won't create a surface through render handle
 */
class VPainter
{
public:
	/*
	 * VPainter constructor:
	 *		Description : The VPainter of class VImage
	 *		Values Tip	: {
	 *			[const VRenderHandle&] RenderHandle : The render handle of renderer environment
	 *		}
	 */
	explicit VPainter(const VRenderHandle &RenderHandle);
	~VPainter();

public:
	void BeginDraw();
	void EndDraw();

public:
	void Clear(const VColor &FlushColor);

	void DrawLine(const VPoint &StartPoint, const VPoint &EndPoint, VPenBrush *PenBrush);
	void DrawRectangle(const VRect &Rectangle, VPenBrush *PenBrush);
	void DrawEllipse(const VRect &EllipseRectangle, VPenBrush *PenBrush);
	void DrawRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VPenBrush *PenBrush);

	void SolidRectangle(const VRect &Rectangle, VSolidBrush *PenBrush);
	void SolidEllipse(const VRect &EllipseRectangle, VSolidBrush *PenBrush);
	void SolidRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VSolidBrush *PenBrush);
	void SolidRectangle(const VRect &Rectangle, VBitmapBrush *PenBrush);
	void SolidEllipse(const VRect &EllipseRectangle, VBitmapBrush *PenBrush);
	void SolidRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VBitmapBrush *PenBrush);

	void FillRectangle(const VRect &Rectangle, VPenBrush *BorderPen, VSolidBrush *FillBrush);
	void FillEllipse(const VRect &EllipseRectangle, VPenBrush *BorderPen, VSolidBrush *FillBrush);
	void FillRoundedRectangle(const VRect &Rectangle, const VPoint &Radius, VPenBrush *BorderPen,
							  VSolidBrush *FillBrush);

	void DrawString(const std::wstring &String, const VRect &StringRectangle, VFont *TargetFont, VPenBrush *TextBrush);

	void DrawImage(const VRect &TargetRectangle, VImage *SourceImage, const VRect &SourceRectangle,
				   const float &ImageOpacity);
	void DrawCanvas(const VRect &TargetRectangle, VCanvasPainter *SourceImage, const VRect &SourceRectangle,
					const float &ImageOpacity);

private:
	ID2D1RenderTarget *TargetDevice;
};
} // namespace Core

VLIB_END_NAMESPACE
