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
		return { BoxShadowPixel / 2, BoxShadowPixel / 2 - 4, BoxShadowPixel / 2 + Width, BoxShadowPixel / 2 + Height };
	}
	static VRect GetShadowRect(int Width, int Height, int BoxShadowPixel) {
		return { 0, 0, 
			Width + BoxShadowPixel, Height + BoxShadowPixel };
	}
};

VLIB_END_NAMESPACE
