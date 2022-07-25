/*
 * VPainter.hpp
 *	@description : The Painter Of VRender
 *	@birth		 : 2022/6.11
*/

#pragma once

#include "vrenderbasic.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VFontFamily class:
 *	@description  : The Font Family
*/
class VFontFamily {
private:
	VMemoryPtr<VGdiplus::FontFamily> NativeFamily;

public:
	/*
	 * GetNativeFamily Functional:
	 *	@description  : Get the Native Gdiplus Family
	*/
	VGdiplus::FontFamily* GetNativeFamily() {
		return NativeFamily.get();
	}

public:
	VFontFamily(const VFontFamily& Family) {
		NativeFamily.reset(Family.NativeFamily->Clone());
	}
	VFontFamily(std::wstring FontName) {
		NativeFamily.reset(new VGdiplus::FontFamily(FontName.c_str()));
	}

	std::wstring GetFamilyName() {
		WCHAR FamilyName[1024];
		NativeFamily->GetFamilyName(FamilyName);

		return FamilyName;
	}
};

/*
 * VStringAlignment enum class:
 *	@description  : The String Alignment
*/
enum class VStringAlignment {
	AlignmentNear, AlignmentCenter, AlignmentFar
};

/*
 * VFontFormat class:
 *	@description  : The Font Format
*/
class VFontFormat {
private:
	std::unique_ptr<VGdiplus::StringFormat> NativeFormat;

public:
	/*
	 * GetNativeFontFormat Functional:
	 *	@description  : Get the Gdiplus StringFormat
	*/
	VGdiplus::StringFormat* GetNativeFontFormat() {
		return NativeFormat.get();
	}

private:
	/*
	 * ConvertAlignmentStyle Functional:
	 *	@description  : Convert VStringAlignment Into Gdiplus String Style
	*/
	VGdiplus::StringAlignment ConvertAlignmentStyle(VStringAlignment Alignment) {
		switch (Alignment)
		{
		case VStringAlignment::AlignmentNear:
			return VGdiplus::StringAlignment::StringAlignmentNear;
		case VStringAlignment::AlignmentCenter:
			return VGdiplus::StringAlignment::StringAlignmentCenter;
		case VStringAlignment::AlignmentFar: {
			return VGdiplus::StringAlignment::StringAlignmentFar;
		}
		}

		return VGdiplus::StringAlignment::StringAlignmentNear;
	}
	/*
	 * ConvertAlignmentStyle Functional:
	 *	@description  : Convert Gdiplus String Style Into VStringAlignment
	*/
	VStringAlignment ConvertAlignmentStyle(VGdiplus::StringAlignment Alignment) {
		switch (Alignment) {
		case VGdiplus::StringAlignment::StringAlignmentNear:
			return VStringAlignment::AlignmentNear;
		case VGdiplus::StringAlignment::StringAlignmentCenter:
			return VStringAlignment::AlignmentCenter;
		case VGdiplus::StringAlignment::StringAlignmentFar: {
			return VStringAlignment::AlignmentFar;
		}
		}

		return VStringAlignment::AlignmentNear;
	}

public:
	VFontFormat() {
		NativeFormat.reset(new VGdiplus::StringFormat());
	}
	VFontFormat(VFontFormat& Format) {
		NativeFormat.reset(Format.NativeFormat->Clone());
	}

public:
	/*
	 * SetAlignment Functional:
	 *	@description  : Set the Format Alignment
	*/
	void SetAlignment(VStringAlignment Alignment) {
		NativeFormat->SetAlignment(ConvertAlignmentStyle(Alignment));
	}
	/*
	 * GetAlignment Functional:
	 *	@description  : Get the Format Alignment
	*/
	VStringAlignment GetAlignment() {
		return ConvertAlignmentStyle(NativeFormat->GetAlignment());
	}

	/*
	 * SetLineAlignment Functional:
	 *	@description  : Set the Line Alignment
	*/
	void SetLineAlignment(VStringAlignment Alignment) {
		NativeFormat->SetLineAlignment(ConvertAlignmentStyle(Alignment));
	}
	/*
	 * GetLineAlignment Functional:
	 *	@description  : Get the Line Alignment
	*/
	VStringAlignment GetLineAlignment() {
		return ConvertAlignmentStyle(NativeFormat->GetLineAlignment());
	}
};

/*
 * VFont class:
 *	@description  : The Font in VRender
*/
class VFont {
private:
	VMemoryPtr<VGdiplus::Font> NativeFont;

public:
	VGdiplus::Font* GetNativeFont() {
		return NativeFont.get();
	}

public:
	/*
	 * Build up Functional
	*/

	VFont(VFontFamily* Family, float TextSize) {
		NativeFont.reset(new VGdiplus::Font(Family->GetNativeFamily(), TextSize, 0, VGdiplus::Gdiplus::Unit::UnitPixel));
	}

	/*
	 * GetHeight Functional:
	 *	@description  : Get the Height of Target Format Type With Target String
	*/
	int GetHeight(std::wstring String, VFontFormat* Format) {
		VGdiplus::FontFamily   FontFamily;
		VGdiplus::GraphicsPath StringPath;
		VGdiplus::RectF        StringRect;

		NativeFont->GetFamily(&FontFamily);

		StringPath.AddString(String.c_str(), -1, &FontFamily, NativeFont->GetStyle(), NativeFont->GetSize(),
			VGdiplus::PointF(0.f, 0.f), Format->GetNativeFontFormat());
		StringPath.GetBounds(&StringRect);

		return static_cast<int>(StringRect.Y + StringRect.Height);
	}
	int GetHeight(std::wstring String, VFontFormat* Format, VRect OutRect) {
		VGdiplus::FontFamily   FontFamily;
		VGdiplus::GraphicsPath StringPath;
		VGdiplus::RectF        StringRect;

		NativeFont->GetFamily(&FontFamily);

		StringPath.AddString(String.c_str(), -1, &FontFamily, NativeFont->GetStyle(), NativeFont->GetSize(),
			OutRect.ToGdiplusRect(), Format->GetNativeFontFormat());
		StringPath.GetBounds(&StringRect);

		return static_cast<int>(StringRect.Y + StringRect.Height);
	}
	/*
	 * GetWidth Functional:
	 *	@description  : Get the Width of Target Format Type With Target String
	*/
	int GetWidth (std::wstring String, VFontFormat* Format) {
		VGdiplus::FontFamily   FontFamily;
		VGdiplus::GraphicsPath StringPath;
		VGdiplus::RectF        StringRect;

		NativeFont->GetFamily(&FontFamily);

		StringPath.AddString(String.c_str(), -1, &FontFamily, NativeFont->GetStyle(), NativeFont->GetSize(),
			VGdiplus::PointF(0.f, 0.f), Format->GetNativeFontFormat());
		StringPath.GetBounds(&StringRect);

		return static_cast<int>(StringRect.Width + StringRect.X);
	}
	int GetWidth(std::wstring String, VFontFormat* Format, VRect OutRect) {
		VGdiplus::FontFamily   FontFamily;
		VGdiplus::GraphicsPath StringPath;
		VGdiplus::RectF        StringRect;

		NativeFont->GetFamily(&FontFamily);

		StringPath.AddString(String.c_str(), -1, &FontFamily, NativeFont->GetStyle(), NativeFont->GetSize(),
			OutRect.ToGdiplusRect(), Format->GetNativeFontFormat());
		StringPath.GetBounds(&StringRect);

		return static_cast<int>(StringRect.Width + StringRect.X);
	}
};

VLIB_END_NAMESPACE