/*
 * VRenderBasic.hpp
 *	@description : The Basic Model in VRender
 *	@birth		 : 2022/6.3
*/

#pragma once

#include "../../VBase"

VLIB_BEGIN_NAMESPACE

/*
 * VGdiplusHelper class:
 *	@description : The Gdiplus Auto Init & Free Tool Class
*/
class VGdiplusHelper {
private:
	ULONG_PTR _GdiplusToken;

public:
	/*
	 * Init Gdiplus
	*/
	VGdiplusHelper() {
		VGdiplus::GdiplusStartup(&_GdiplusToken, nullptr, nullptr);
	}
	/*
	 * Gdiplus Shutdown
	*/
	~VGdiplusHelper() {
		VGdiplus::GdiplusShutdown(_GdiplusToken);
	}
};

/* Helper Application */
VGdiplusHelper _VGdiplusHelper;

VLIB_END_NAMESPACE