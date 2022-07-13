/*
 * VPainterDevice.hpp
 *	@description : The Painter Device Of VRender
 *	@birth		 : 2022/6.4
*/

#pragma once

#include "vrenderbasic.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VPaintbleType class:
 *	@description  : Describe the Type Of Paint-Allowed Object
*/
enum class VPaintbleType {
	HDCPainter, ImagePainter
};

/*
 * VPaintbleObject class:
 *	@description  : The Base Class For Every Paint-Allowed Object
*/
class VPaintbleObject {
private:
	VPaintbleType DeviceType;

public:
	/*
	 * Build up Functional
	*/

	VPaintbleObject(VPaintbleType TargetType)
		: DeviceType(TargetType) {

	}

	/*
	 * GetDeviceType Functional:
	 *	@description  : Get the Type Of Paint-Allowed Object
	*/
	VPaintbleType            GetDeviceType() {
		return DeviceType;
	}

	/*
	 * GetTargetHDC virtual Functional:
	 *	@description  : For Child Class Overide And Provide a HDC
	*/
	virtual HDC               GetTargetHDC()    { return NULL; }
	/*
	 * GetTargetBitmap virtual Functional:
	 *	@description  : For Child Class Overide And Provide a Gdiplus Bitmap
	*/
	virtual VGdiplus::Bitmap* GetTargetBitmap() { return nullptr; }
};

/*
 * VPaintbleHDC class <- public VPaintbleObject:
 *	@description  : Create A HDC Compatible Paint-Allowed Object
*/
class VPaintbleHDC : public VPaintbleObject {
private:
	HDC TargetDC;

public:
	/*
	 * Build up Functional
	*/

	VPaintbleHDC(HDC DC)
		: TargetDC(DC), VPaintbleObject(VPaintbleType::HDCPainter) {

	}

	/*
	 * GetTargetHDC override Functional:
	 *	@description  : Provide A HDC For Painter
	*/
	HDC GetTargetHDC() override { 
		return TargetDC;
	}
};

VLIB_END_NAMESPACE