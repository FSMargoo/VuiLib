/*
 * VCanvas.hpp
 *	@description : The Canvas Defition Of VRender
 *	@birth		 : 2022/6.6
*/

#pragma once

#include "vimage.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VCanvas class:
 *	@description : Provide a Canvas Class
*/
class VCanvas : public VImage {
public:
	/*
	 * Build up Functional
	*/

	VCanvas(int Width, int Height)
		: VImage(Width, Height) {

	}

	/*
	 * PaintCanvas Functional:
	 *	@description  : Paint a Canvas Into This Canvas
	*/
	void PaintCanvas(int X, int Y, VCanvas* Canvas) {
		VGdiplus::Graphics Graphics(GetNativeImage());

		Graphics.DrawImage(Canvas->GetNativeImage(), 
			{ X, Y, Canvas->GetWidth(), Canvas->GetHeight() }, 
			0, 0, Canvas->GetWidth(), Canvas->GetHeight(), VGdiplus::UnitPixel, Canvas->GetNativeAttributes());
	}
};

VLIB_END_NAMESPACE