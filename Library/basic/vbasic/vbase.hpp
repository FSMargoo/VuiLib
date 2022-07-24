/*
 * vbase.hpp
 *	@description : The defition of basic object (VRect e.t.c)
 *	@birth		 : 2022/6.3
*/
#pragma once

#undef  WINVER
#define WINVER 0x550

#pragma warning(disable : 26415)

#include <graphics.h>

/* #ifndef _WIN64
#	error This version of VLib only supports the x64 platform (English)
#	error 该版本的 VLib 只支持 x64 平台 (简体中文)
#	error 該版本的 VLib 只支持 x64 平臺 (繁體中文)
#endif */

#pragma comment(lib, "gdiplus.lib")

/*
 * VLIB_BEGIN_NAMESPACE & VLIB_END_NAMESPACE Marco:
 *		If needed, user can def Marco "VLIB_ENABLE_NAMESPACE"
 *		To include vlib into a namespace
*/
#ifdef VLIB_ENABLE_NAMESPACE
#	define VLIB_BEGIN_NAMESPACE namespace VLib {
#	define VLIB_END_NAMESPACE   }
#else
#	define VLIB_BEGIN_NAMESPACE
#	define VLIB_END_NAMESPACE
#endif

#define RGBA(r, g, b, a)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))|(((DWORD)(BYTE)(a))<<24))

/*
 * VAssertDigibox
*/
#ifdef _DEBUG
#	define VAssertBox(_Message) _RPTW0(_CRT_ASSERT, _Message)
#else
#	define VAssertBox(_Message)
#endif

VLIB_BEGIN_NAMESPACE

#include <string>

/* Gdiplus defiiton in VLib */
namespace VGdiplus {
#define GDIPVER 0x0110
#include <gdiplus.h>
	using namespace Gdiplus;
}

/*
 * _VRect<_Type> class:
 *	@description : The Basic Rect Class (Template)
 *	@allow type  : digital type
*/
template<class _Type>
class _VRect {
public:
	/*
	 * Geomtery Information
	*/

	_Type left = 0;
	_Type top = 0;
	_Type right = 0;
	_Type bottom = 0;

public:
	bool operator==(_VRect Rect) {
		return Rect.left == left && Rect.top == top && Rect.right == right && Rect.bottom == bottom;
	}
	bool operator!=(_VRect Rect) {
		return !(Rect.left == left && Rect.top == top && Rect.right == right && Rect.bottom == bottom);
	}

	void operator=(_VRect Rect) {
		left = Rect.left;
		right = Rect.right;
		top = Rect.top;
		bottom = Rect.bottom;
	}

public:
	_VRect(const _Type _Left, const _Type _Top, const _Type _Right, const _Type _Bottom)
		: left(_Left), right(_Right), top(_Top), bottom(_Bottom) {

	}
	_VRect() {

	}

	/*
	 * Geomtery Functional (GetWidth & GetHeight)
	*/

	_Type GetWidth() {
		return right - left;
	}
	_Type GetHeight() {
		return bottom - top;
	}

	/*
	 * Clone Functional:
	 *	@description : Clone a object from this object
	*/
	_VRect Clone() {
		return _VRect{ left, top, right, bottom };
	}
	/*
	 * Offset Functional:
	 *	@description  : Make The Rect Offset
	 *	@return value : The New Rect Offseted
	*/
	_VRect Offset(const _Type LeftOffset,
		const _Type TopOffset,
		const _Type RightOffset,
		const _Type BottomOffset) {
		left += LeftOffset;
		top += TopOffset;
		right += RightOffset;
		bottom += BottomOffset;

		return Clone();
	}
	/*
	 * Offset Functional:
	 *	@description  : Move the Rectangle
	*/
	inline void Offset(const _Type NewX, const _Type NewY) {
		Move(NewX, NewY);
	}
	/*
	 * Move Functioinal:
	 *	@description  : Move the rect to a new position
	 *	@return value : Nothing
	*/
	void Move(const _Type Left, const _Type Top) {
		auto width = GetWidth();
		auto height = GetHeight();

		left = Left;
		top = Top;
		right = left + width;
		bottom = top + height;
	}
	/*
	 * Resize Functional:
	 *	@description  : Resize the Rectangle Area
	*/
	void Resize(int Width, int Height) {
		right  = left + Width;
		bottom = top  + Height;
	}

	/*
	 * Overlap Functional
	 *	@description  : Is the Two Rects Area Have Any Overlap
	 *	@return value : Yes or No
	*/
	bool Overlap(_VRect Rect) {
		return (max(left, Rect.left) < min(right, Rect.right)) &&
			(max(top, Rect.top) < min(bottom, Rect.bottom));
	}
	/*
	 * Include Functional:
	 *	@description  : Is the Rect In this Rect ( ALL AERA )
	 *	@return value : Yes or No
	*/
	bool Include(_VRect Rect) {
		return left <= Rect.left && top <= Rect.top &&
			bottom >= Rect.bottom && right >= Rect.right;
	}

	/*
	 * ToGdiplusRect Functional:
	 *	@description  : Convert This Rect Into Gdiplus Rect
	 *	@return value : Converted Value
	*/
	VGdiplus::Rect ToGdiplusRect() {
		return VGdiplus::Rect(static_cast<INT>(left), static_cast<INT>(top), static_cast<INT>(right - left), static_cast<INT>(bottom - top));
	}
	/*
	 * ToGdiplusRectF Functional:
	 *	@description  : Convert This Rect Into Gdiplus RectF
	 *	@return value : Converted Value
	*/
	VGdiplus::RectF ToGdiplusRectF() {
		return VGdiplus::RectF(static_cast<float>(left), static_cast<float>(top), static_cast<float>(right - left), static_cast<float>(bottom - top));
	}

	/*
	 * FusionRect Functional:
	 *	@description  : Fusion Rectangles With This Rectangle
	*/
	void FusionRect(_VRect Rect) {
		left = min(Rect.left, left);
		right = max(Rect.right, right);
		top = min(Rect.top, top);
		bottom = max(Rect.bottom, bottom);
	}

	/*
	 * OffsetRV Functional:
	 *	@description  : Offset but Return
	*/
	_VRect* OffsetRV(_Type OffsetX, _Type OffsetY) {
		Offset(OffsetX, OffsetY);

		return this;
	}
};

/*
 * _VPoint<_Type> class:
 *	@description : The Basic Point Class (Template)
 *	@allow type	 : digitial type
*/
template<class _Type>
class _VPoint {
public:
	/*
	 * Geomtery Information
	*/

	_Type x = 0;
	_Type y = 0;

public:
	bool operator==(_VPoint Point) {
		return x == Point.x && y == Point.y;
	}
	bool operator!=(_VPoint Point) {
		return !(x == Point.x && y == Point.y);
	}

public:
	_VPoint(const _Type _X, const _Type _Y)
		: x(_X), y(_Y) {

	}
	_VPoint(const _VPoint& Point) = default;
	_VPoint() {

	}

	/*
	 * ToGdiplusPoint Functional:
	 *	@description : Convert this Point Data Into Gdiplus Point
	*/
	VGdiplus::Point ToGdiplusPoint() {
		return VGdiplus::Point(static_cast<int>(x), static_cast<int>(y));
	}
	/*
	 * ToGdiplusPointF Functional:
	 *	@description : Convert this Point Data Into Gdiplus PointF
	*/
	VGdiplus::PointF ToGdiplusPointF() {
		return VGdiplus::PointF(static_cast<float>(x), static_cast<float>(y));
	}

	/*
	 * Clone Functional:
	 *	@description : Clone a object from this object
	*/
	_VPoint Clone() {
		return _VPoint{ x, y };
	}

	/*
	 * Move Functional:
	 *	@description  : Move The Point Into A Position
	*/
	void Move(int _X, int _Y) {
		x = _X;
		y = _Y;
	}
	/*
	 * Overlap Functional:
	 *	@description  : Is Two Points Overlap
	 *	@return value : Yes or not
	*/
	bool Overlap(_VPoint Point) {
		return Point.x == x && Point.y == Point.y;
	}
	/*
	 * InsideRect Functional:
	 *	@description  : Is the Point Inside a Rectangle
	*/
	bool InsideRect(_VRect<_Type> Rect) {
		return Rect.left <= x && Rect.top <= y &&
			Rect.right >= x && Rect.bottom >= y;
	}

	/*
	 * Offset Functional:
	 *	@description  : Offset
	*/
	void Offset(_Type OffsetX, _Type OffsetY) {
		x += OffsetX;
		y += OffsetY;
	}
};

/*
 * VRect & VRectF:
 *	@description : The Instantiation Of _VRect
*/

using VRect = _VRect<int>;
using VRectF = _VRect<float>;


/*
 * VPoint & VPointF:
 *	@description  : The Instantiation Of _VPoint
*/

using VPoint = _VPoint<int>;
using VPointF = _VPoint<float>;

/*
 * VSize & VsizeF:
 *	@description  : The Instantiation Of _VPoint (But Used for Size)
*/

using VSize = _VPoint<int>;
using VsizeF = _VPoint<float>;

/*
 * Marco for if&else stream branch
 */
#ifndef _MSC_VER
#	define VLikely(Exp)   __builtin_expect(!!(Exp), 1) 
#	define VUnlikely(Exp) __builtin_expect(!!(Exp), 0)
#else
#	define VLikely(Exp)   (Exp)
#	define VUnlikely(Exp) (Exp)
#endif

 /*
  * EasyXWindowResize Functional:
  *	@description  : Resize EasyX Window
 */
void EasyXWindowResize(int Width, int Height) {
	Resize(NULL, Width, Height);
}

VLIB_END_NAMESPACE
