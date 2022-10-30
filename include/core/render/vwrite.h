// License(MIT)
// Athuor: Margoo
// This is the wrapper of DWrite
#pragma once

#include "vdirectxfactory.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

using VFontAlignment           = DWRITE_TEXT_ALIGNMENT;
using VFontParagraphAlignment  = DWRITE_PARAGRAPH_ALIGNMENT;

class VFont {
 private:
	 IDWriteTextFormat* TextFormat{};

 public:
	 enum FontStretch {
		 STRETCH_UNDEFINED = 0,
		 STRETCH_ULTRA_CONDENSED = 1,
		 STRETCH_EXTRA_CONDENSED = 2,
		 STRETCH_CONDENSED = 3,
		 STRETCH_SEMI_CONDENSED = 4,
		 STRETCH_NORMAL = 5,
		 STRETCH_MEDIUM = 5,
		 STRETCH_SEMI_EXPANDED = 6,
		 STRETCH_EXPANDED = 7,
		 STRETCH_EXTRA_EXPANDED = 8,
		 STRETCH_ULTRA_EXPANDED = 9
	 };
	 enum FontStyle {
		 STYLE_NORMAL  = ::DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
		 STYLE_OBLIQUE = ::DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_OBLIQUE,
		 STYLE_ITALIC   = ::DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_ITALIC
	};
	 enum FontWeight {
		 WEIGHT_THIN = 100,
		 WEIGHT_EXTRA_LIGHT = 200,
		 WEIGHT_ULTRA_LIGHT = 200,
		 WEIGHT_LIGHT = 300,
		 WEIGHT_SEMI_LIGHT = 350,
		 WEIGHT_NORMAL = 400,
		 WEIGHT_REGULAR = 400,
		 WEIGHT_MEDIUM = 500,
		 WEIGHT_DEMI_BOLD = 600,
		 WEIGHT_SEMI_BOLD = 600,
		 WEIGHT_BOLD = 700,
		 WEIGHT_EXTRA_BOLD = 800,
		 WEIGHT_ULTRA_BOLD = 800,
		 WEIGHT_BLACK = 900,
		 WEIGHT_HEAVY = 900,
		 WEIGHT_EXTRA_BLACK = 950,
		 WEIGHT_ULTRA_BLACK = 950
	 };

 public:
	 IDWriteTextFormat* GetDXObject() {
		 return TextFormat;
	 }

 public:
    VFont(const VFont& FontObject);
	 VFont(const std::wstring& FamilyName, FontWeight TextWidget, FontStyle TextStyle, FontStretch TextStretch,
		 float TextSize, const std::wstring& Local);
	 ~VFont();

 public:
	 void SetLineAlignment(const VFontAlignment& Alignment);
	 void SetParagraphAlignment(const VFontParagraphAlignment& Alignment);
     void SetFamilyName(const std::wstring& FontFamilyName);
     void SetTextSize(const int& Size);

     std::wstring GetFamilyName() const;
     std::wstring GetLocaleName() const;
     FontWeight   GetTextWeightStyle() const;
     FontStyle    GetTextFontStyle() const;
     FontStretch  GetFontStretchStyle() const;
     int          GetTextSize() const;
};
}

VLIB_END_NAMESPACE