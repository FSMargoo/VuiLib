/*
 * File name	: vbrush.h
 * Author		: Margoo
 * Date			: 11/11/2022
 * Description
 * : This head defines the brush class in vuilib's renderer
 */
#pragma once

#include "vcolor.h"
#include "vdirectxfactory.h"
#include "vimage.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
/*
 * VPenBrush class:
 *	Description : This brush is used to draw border color
 */
class VPenBrush
{
public:
	/*
   * VPenBrush constructor:
   *		Description : The constructor of class VPenBrush
   *
	 * Tips		: If a invalid render handle is specified, the
   *function
   *will throw a CRT debug exception
	 * Values Tip	: { [const VColor&       ] Color
   *: The border color [const VRenderHandle&] RenderHandle  :
	 * The render handle
   *of renderer [const float&		 ] LineThickness : The border thickness
   *<-
	 * optional
   *		}
   */
	VPenBrush(const VColor &Color, const VRenderHandle &RenderHandle, const float &LineThickness = 1.f);
	~VPenBrush();

public:
	/*
   * GetDxBrush function:
   *		Description : Get the pointer of DirectX2D object
   *
	 * Tips		: The pointer which returned is unchangeable
   */
	ID2D1SolidColorBrush *GetDxBrush() VRENDER_HELPER const;
	/*
   * GetStrokeStyle function:
   *	Description : Get the stroke style as DirectX2D object
   */
	ID2D1StrokeStyle *GetStrokeStyle() VRENDER_HELPER const;
	/*
   * GetThickness function:
   *	Description : Get the thickness of this brush
   */
	float GetThickness() const;

public:
	/*
   * SetThickness function:
   *	Description : Set the border thickness of this brush
   *	Tips
	 * : If a negative value or zero value is specified, the
   *function will throw a CRT debug exception
   */
	void SetThickness(const float &TargetThickness);

private:
	VKits::VCOMPointer<ID2D1SolidColorBrush> Brush;
	VKits::VCOMPointer<ID2D1StrokeStyle>	 StrokeStyle;

	float Thickness;
};

/*
 * VSolidBrush class:
 *	Description : This brush is used to draw fill color in a geometry shape
 */
class VSolidBrush
{
public:
	/*
   * VSolidBrush constructor:
   *		Description : The constructor of class VSolidBrush
   *
	 * Tips		: If a invalid render handle is specified, the
   *function
   *will throw a CRT debug exception
	 * Values Tip	: { [const VColor&       ] Color
   *: The fill color [const VRenderHandle&] RenderHandle  : The
	 * render handle of
   *renderer
   *		}
   */
	VSolidBrush(const VColor &Color, const VRenderHandle &RenderHandle);
	~VSolidBrush();

public:
	/*
   * GetDxBrush function:
   *		Description : Get the pointer of DirectX2D object
   *
	 * Tips		: The pointer which returned is unchangeable
   */
	ID2D1SolidColorBrush *GetDxBrush() VRENDER_HELPER const;

private:
	VKits::VCOMPointer<ID2D1SolidColorBrush> Brush;
};

/*
 * VBitmapBrush class:
 *	Description
 */
class VBitmapBrush
{
public:
	/*
   * VBitmapBrush constructor:
   *		Description : The constructor of class VBitmapBrush
   *
	 * Tips		: If a invalid render handle is specified, the
   *function will throw a CRT debug exception
	 * Values Tip	: { [VImage*
   *] Image		 : A image which is specified to fill [const

	 * *VRenderHandle&] RenderHandle  : A render handle of renderer
   *		}
   */
	VBitmapBrush(VImage *Image, const VRenderHandle &RenderHandle);
	/*
   * VBitmapBrush constructor:
   *		Description : The constructor of class VBitmapBrush
   *
	 * Tips		: 1> If a invalid render handle is specified,
   *the function will throw a CRT debug exception
	 * 2> If a VBitmapBrush is
   *constructed from this constructor, this means that this is a helper class 3>
 *In
	 * general, we do not recommend that users use this constructor to construct
   *a VBitmapBrush directly for the
	 * API Compatibility Values Tip	: {
   *[ID2D1Bitmap*		 ] Image		 : A image which is

	 * *specified to fill [const VRenderHandle&] RenderHandle  : A render handle of
   *renderer
   *		}

	 */
	VBitmapBrush(ID2D1Bitmap *Image, const VRenderHandle &RenderHandle) VRENDER_HELPER;
	~VBitmapBrush();

public:
	/*
   * GetDxBrush function:
   *		Description : Get the pointer of DirectX2D object
   *
	 * Tips		: The pointer which returned is unchangeable
   */
	ID2D1BitmapBrush *GetDxBrush() VRENDER_HELPER const;

private:
	VKits::VCOMPointer<ID2D1BitmapBrush> Brush;
};
} // namespace Core

VLIB_END_NAMESPACE