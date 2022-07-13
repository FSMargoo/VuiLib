/*
 * VColor.hpp
 *	@description : The Color Defition in VRender ( Gdiplus Wrapper )
 *	@brith		 : 2022/6.3
*/

#pragma once

#include "vrenderbasic.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VColor class:
 *	@description : Use VColor to Describe a Color
*/
class VColor {
private:
	VGdiplus::Color ColorObject;

public:
	/*
	 * GetNativeObject Functional :
	 *	@description : Get the Native Color Object in Gdiplus
	*/
	VGdiplus::Color GetNativeObject() const {
		return ColorObject;
	}

public:
	/*
	 * operator= Functional (symbol operator) :
	 *	@description : See in Code
	*/
	void operator=(const VColor& Object) {
		ColorObject = Object.ColorObject;
	}

	/*
	 * Build up Functional
	*/

	VColor(const VGdiplus::Color Object) {
		ColorObject = Object;
	}
	VColor(const VColor& Object) {
		ColorObject = Object.ColorObject;
	}
	VColor(BYTE r, BYTE g, BYTE b, BYTE a) {
		ColorObject = VGdiplus::Color(a, r, g, b);
	}
	VColor(BYTE r, BYTE g, BYTE b) {
		ColorObject = VGdiplus::Color(r, g, b);
	}
	VColor() {
		ColorObject = VGdiplus::Color(0, 0, 0);
	}

	/*
	 * Color Get Functional Group
	*/

	inline BYTE GetRed() const {
		return ColorObject.GetR();
	}
	inline BYTE GetGreen() const {
		return ColorObject.GetG();
	}
	inline BYTE GetBlue() const {
		return ColorObject.GetB();
	}
	inline BYTE GetAlpha() const {
		return ColorObject.GetA();
	}

	/*
	 * Color Set Functional Group
	*/
	inline void SetAlpha(const BYTE Value) {
		ColorObject = VGdiplus::Color(Value, GetRed(), GetGreen(), GetBlue());
	}
	inline void SetRed(const BYTE Value) {
		ColorObject = VGdiplus::Color(GetAlpha(), Value, GetGreen(), GetBlue());
	}
	inline void SetGreen(const BYTE Value) {
		ColorObject = VGdiplus::Color(GetAlpha(), GetRed(), Value, GetBlue());
	}
	inline void SetBlue(const BYTE Value) {
		ColorObject = VGdiplus::Color(GetAlpha(), GetRed(), GetGreen(), Value);
	}

	/*
	 * Reset Functional:
	 *	@description : Reset the Color Of the Object
	*/
	void Reset(BYTE r, BYTE g, BYTE b) {
		ColorObject = VGdiplus::Color(r, g, b);
	}
	void Reset(BYTE r, BYTE g, BYTE b, BYTE a) {
		ColorObject = VGdiplus::Color(a, r, g, b);
	}
};

VLIB_END_NAMESPACE