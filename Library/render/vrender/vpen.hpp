/*
 * VPen.hpp
 *	@description : The Pen Defition in VRender ( Gdiplus Wrapper )
 *	@brith		 : 2022/6.3
*/

#pragma once

#include "vrenderbasic.hpp"
#include "vcolor.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VPenStyle enum class:
 *	@description : The Style Defition Of VPen
*/
enum class VPenStyle {
	SolidStyle, DashStyle, DotStyle, DashDotStyle, DashDotDotStyle, CustomStyle
};

/*
 * VPen class:
 *	@description : The Pen Defition
*/
class VPen {
private:
	/* The Gdiplus Native Pen */
	VMemoryPtr<VGdiplus::Pen> NativePen;

private:
	/*
	 * ConvertStyle Functional:
	 *	@description : Convert Gdiplus Dash Style Into VPen Dash Style
	*/
	VPenStyle           ConvertStyle(VGdiplus::DashStyle GdiplusStyle) {
		using namespace VGdiplus;

		switch (GdiplusStyle) {
		case DashStyle::DashStyleSolid: {
			return VPenStyle::SolidStyle;
		}
		case DashStyle::DashStyleDash: {
			return VPenStyle::DashStyle;
		}
		case DashStyle::DashStyleDot: {
			return VPenStyle::DotStyle;
		}
		case DashStyle::DashStyleDashDot: {
			return VPenStyle::DashDotStyle;
		}
		case DashStyle::DashStyleDashDotDot: {
			return VPenStyle::DashDotDotStyle;
		}
		case DashStyle::DashStyleCustom: {
			return VPenStyle::CustomStyle;
		}
		}

		return VPenStyle::CustomStyle;
	}
	/*
	 * ConvertStyle Functional:
	 *	@description : Convert VPen Dash Style Into Gdiplus Dash Style
	*/
	VGdiplus::DashStyle ConvertStyle(VPenStyle PenStyle) {
		using namespace VGdiplus;

		switch (PenStyle) {
		case VPenStyle::SolidStyle: {
			return DashStyle::DashStyleSolid;
		}
		case VPenStyle::DashStyle: {
			return DashStyle::DashStyleDash;
		}
		case VPenStyle::DotStyle: {
			return DashStyle::DashStyleDot;
		}
		case VPenStyle::DashDotStyle: {
			return DashStyle::DashStyleDashDot;
		}
		case VPenStyle::DashDotDotStyle: {
			return DashStyle::DashStyleDashDotDot;
		}
		case VPenStyle::CustomStyle: {
			return DashStyle::DashStyleCustom;
		}
		}

		return DashStyle::DashStyleCustom;
	}

public:
	/*
	 * GetNativePen Functional
	 *	@description : Get the Gdiplus Native Pen
	*/
	VGdiplus::Pen* GetNativePen() {
		return NativePen.get();
	}

	/*
	 * opeartor= Functional
	*/
	void operator=(const VPen& pen) {
		NativePen.reset(pen.NativePen->Clone());
	}

public:
	/*
	 * VPen Build Up Functional
	*/

	VPen(const VPen&   ObjectBuild) {
		NativePen.reset(ObjectBuild.NativePen->Clone());
	}
	VPen(const VColor& PenColor, const float Thinkness = 1.0f) {
		NativePen.reset(
			new VGdiplus::Pen(PenColor.GetNativeObject(), Thinkness)
		);
	}

	/*
	 * SetThinkness Functional :
	 *	@description : Set the Pen's Thinkness
	*/
	void SetThinkness(const float Thinkness = 1.0f) {
		NativePen->SetWidth(Thinkness);
	}
	/*
	 * GetThinkness Functional:
	 *	@description  : Get the Pen's Thinkness
	 *	@return value : The Thinkness Of Pen
	*/
	auto GetThinkness() -> decltype(NativePen->GetWidth()) {
		return NativePen->GetWidth();
	}

	/*
	 * SetColor Functional:
	 *	@description  : Set the Color Of Pen
	*/
	void SetColor(VColor Color) {
		NativePen->SetColor(Color.GetNativeObject());
	}
	/*
	 * GetColor Functional:
	 *	@description  : Get the Color Of Pen
	 *	@return value : The Color Of Pen
	*/
	VColor GetColor() {
		VGdiplus::Color NativeColor;
		NativePen->GetColor(&NativeColor);

		return NativeColor;
	}

	/*
	 * SetStyle Functional:
	 *	@description  : Set the Pen Style
	*/
	void SetStyle(VPenStyle Style) {
		NativePen->SetDashStyle(ConvertStyle(Style));
	}
	/*
	 * GetStyle Functional:
	 *	@description  : Get the Style of Pen
	 *	@return value : The Style Of Pen
	*/
	VPenStyle GetStyle() {
		return ConvertStyle(NativePen->GetDashStyle());
	}
};

VLIB_END_NAMESPACE