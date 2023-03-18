/*
 * File name	: vrenderbasic.h
 * Author		: Margoo
 * Date			: 11/11/2022
 * Description
 * : This head defines the basic function of vuilib's renderer
 */
#pragma once

#include "../../kits/vallocator.h"
#include "../stl-ex/vstring.h"
#include "../vbasiccore.h"

VLIB_BEGIN_NAMESPACE

#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

/*
 * VRENDER_HELPER Marco:
 *	Description : This is a empty Marco which often mark a helper class
 *(sometimes it can
 * be any object), if a class or any object is marked by this
 *Marco, in generally, we do not recommend do any
 * operation with the class (or
 *any object) directly for the API Compatibility
 */
#define VRENDER_HELPER

namespace Core
{
/*
 * VDXObjectSafeFree function:
 *	Description : This function provide a safely way to release DirectX object
 * Tips		:	1>	Since the function will makes a security judgments
 *					with null value, it's safe to let the Object parameter as null value
 *				2>	Since the function is marked with VRENDER_HELPER Marco, we don't recommend you to
 *					call this function directly in your code
 *				3>	The function can ONLY RELEASE the COM object, it's illegal to set the Object
 *					parameter as a pointer which isn't refer to a COM object, it will make the
 *					function crash
 */
template <class Type> void VDXObjectSafeFree(Type **Object) VRENDER_HELPER
{
	if (*Object)
	{
		(*Object)->Release();

		*Object = NULL;
	}
}

/*
 * VSurfaceAlphaMode enum:
 *	Description : This enum describe how to deal with alpha on a surface
 */
enum VSurfaceAlphaMode
{
	/*
   * Alpha mode should be determined implicitly. Some target surfaces do not
   * supply or imply this
	   information in which case alpha must be specified
   */
	VSF_ALPHA_MODE_UNKNOWN = 0,

	/*
	 * Treat the alpha as premultipled
	 */
	VSF_ALPHA_MODE_PREMULTIPLIED = 1,

	/*
	 * Opacity is in the 'A' component only
	 */
	VSF_ALPHA_MODE_STRAIGHT = 2,

	/*
	 * Ignore any alpha channel information
	 */
	VSF_ALPHA_MODE_IGNORE = 3
};

/*
 * VSurfaceAlphaMode2DXMode function:
 *	Description : This function convert the VSurfaceAlphaMode instance into

 * *D2D1_ALPHA_MODE, Tips		: Since the function is marked with
 *VRENDER_HELPER Marco, we don't recommend you to
 * call this function directly in
 *your code
 */
const D2D1_ALPHA_MODE VSurfaceAlphaMode2DXMode(const VSurfaceAlphaMode &AlphaMode);

/*
 * DXAlphaMode Marco:
 *	Description : This Marco is a alias of function
 *VSurfaceAlphaMode2DXMode, for more,
 * see in VSurfaceAlphaMode2DXMode
 *	Tips		: Since the Marco is marked with VRENDER_HELPER Marco,
 *we
 * don't recommend you to call this Marco directly in your code
 */
#define DXAlphaMode(Value) VSurfaceAlphaMode2DXMode(Value) VRENDER_HELPER
} // namespace Core

VLIB_END_NAMESPACE