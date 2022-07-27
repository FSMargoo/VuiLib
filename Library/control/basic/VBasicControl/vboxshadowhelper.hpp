/*
 * VBoxShadowHelper.hpp:
 *	@description  : The Box Shadow Control Helper
*/

#pragma once

#include "vuiobject.hpp"

VLIB_BEGIN_NAMESPACE

class VBoxShadowHelper {
public:
	static VRect GetShadowElementRect(int Width, int Height, int BoxShadowPixel) {
		return { BoxShadowPixel, BoxShadowPixel, BoxShadowPixel + Width, BoxShadowPixel + Height };
	}
	static VRect GetShadowRect(int Width, int Height, int BoxShadowPixel) {
		return { 0, 0, 
			Width + BoxShadowPixel * 2, Height + BoxShadowPixel * 2 };
	}
};

VLIB_END_NAMESPACE
