/*
 * File name	: vimage.h
 * Author		: Margoo
 * Date			: 11/12/2022
 * Description
 * : This head provide a image object for vuilib's renderer
 */
#pragma once

#include "vcolor.h"
#include "vdirectxfactory.h"
#include <d2d1_1.h>
#include <d2d1effects.h>

#pragma comment(lib, "dxguid.lib")

VLIB_BEGIN_NAMESPACE

namespace Core
{
/*
 * VImage class:
 *	Description : This class describe a image surface
 */
class VImage
{
public:
	/*
	 * VImage constructor:
	 *		Description : The constructor of class VImage
	 *		Values
	 * Tip	: {
	 *			[const VImage&] Image : The specified image
	 *		}
	 */
	VImage(const VImage &Image);
	/*
* VImage constructor:
*		Description : The constructor of class VImage
*		Values
	 * Tip	: {
*			[const int&			 ] Width		: The width
*of image surface

	 * * 		[const int&			 ] Height		: The height
*of image surface [const
	 * VRenderHandle&] RenderHandle : The height of image
*surface
*		}
*/
	VImage(const int &Width, const int &Height, const VRenderHandle &RenderHandle);
	/*
	 * VImage constructor:
	 *		Description : The constructor of class VImage
	 *		Values
	 * Tip	: {
	 *			[const int&				 ] Width		: The width
	 *of image
	 * surface
	 * 		[const int&				 ] Height		: The height
	 *of image surface
	 * [const VSurfaceAlphaMode&] AlphaMode    : The alpha mode of
	 *surface [const VRenderHandle&	 ]
	 * RenderHandle : The height of image
	 *surface
	 *		}
	 */
	VImage(const int &Width, const int &Height, const VSurfaceAlphaMode &AlphaMode, const VRenderHandle &RenderHandle);
	/*
	 * VImage constructor:
	 *		Description : The constructor of class VImage
	 *		Values
	 * Tip	: {
	 *			[const VString&] Width		: The width of
	 *image surface [const
	 * VRenderHandle&	 ] RenderHandle : The height of image
	 *surface
	 *		}
	 */
	VImage(const VString &FilePath, const VRenderHandle &RenderHandle);
	/*
	 * VImage constructor:
	 *		Description : The constructor of class VImage
	 *		Values
	 * Tip	: {
	 *			[ID2D1Bitmap*] IBitmap : The DirectX Object
	 *		}
	 *		Tips
	 * : In general, we do not recommend that users use
	 *this constructor to construct a VBitmapBrush directly
	 * for the API
	 *Compatibility
	 */
	VImage(ID2D1Bitmap *IBitmap) VRENDER_HELPER;

	~VImage();

	/*
	 * GetWidth function:
	 *		Description : Get the width of the image surface
	 */
	int GetWidth() const;
	/*
	 * GetHeight function:
	 *		Description : Get the height of the image surface
	 */
	int GetHeight() const;

	/*
* ApplyGassBlur function:
*		Description : Apply the gaussian blur effect on image surface

	 */
	void ApplyGassBlur(const int &Radius, const VRenderHandle &RenderHandle);
	/*
* ApplyShadowEffect function:
*		Description : Apply the shadow effect on image surface

	 */
	void ApplyShadowEffect(const float &ShadowRadius, const VColor &ShadowColor, const VRenderHandle &RenderHandle,
						   VPoint *Offset = nullptr);

	/*
	 * GetPixel function:
	 *		Description : This function will get the target pixel's color
	 *data and
	 * return as the VColor object Tips		: The X, Y parameter
	 *should be a valid parameter shouldn't be a
	 * invalid value like negative
	 */
	VColor GetPixel(const int &X, const int &Y, const VRenderHandle &RenderHandle) const;

	/*
* IsValidBitmapFile function:
*		Description : This function will judge the target file is a

	 * *valid picture, if the function is valid, return true, nor return false Tips
*: The function's way to
	 * judge the file's legality is try to load the file,
*if there exist any wrong while loading, the function
	 * will return false,
*therefore, it function may return false even though the file is valid
*
*/
	static const bool IsValidBitmapFile(const VString &FilePath);

public:
	/*
	 * GetDirectXObject function:
	 *		Description : Get the direct2d object
	 *		Tips
	 * : The pointer which returned is unchangeable
	 */
	ID2D1Bitmap *GetDirectXObject() VRENDER_HELPER const
	{
		return DirectXBitmap.Get();
	}

private:
	VKits::VCOMPointer<ID2D1Bitmap> DirectXBitmap;
};
}; // namespace Core

VLIB_END_NAMESPACE
