/*
 * File name	: vwrite.h
 * Author		: Margoo
 * Date			: 11/14/2022
 * Description
 * : This head defines the writer
 */
#pragma once

#include "vdirectxfactory.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
/*
 * VFontAlignment (alias of DWRITE_TEXT_ALIGNMENT):
 *	Description : This object defines the alignment of text
 * in line
 */
using VFontAlignment = DWRITE_TEXT_ALIGNMENT;
/*
 * VFontParagraphAlignment (alias of DWRITE_PARAGRAPH_ALIGNMENT)
 * 	Description : This object defines the
 * alignment of text in paragraph
 */
using VFontParagraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT;

/*
 * VFont class:
 *	Description : This class describe a font object in surface
 */
class VFont
{
public:
	/*
   * FontStretch enum:
   *		Description : This enum defines the stretch preset style of font
   */
	enum FontStretch
	{
		STRETCH_UNDEFINED		= 0,
		STRETCH_ULTRA_CONDENSED = 1,
		STRETCH_EXTRA_CONDENSED = 2,
		STRETCH_CONDENSED		= 3,
		STRETCH_SEMI_CONDENSED	= 4,
		STRETCH_NORMAL			= 5,
		STRETCH_MEDIUM			= 5,
		STRETCH_SEMI_EXPANDED	= 6,
		STRETCH_EXPANDED		= 7,
		STRETCH_EXTRA_EXPANDED	= 8,
		STRETCH_ULTRA_EXPANDED	= 9
	};
	/*
   * FontStretch enum:
   *		Description : This enum defines the font preset style of font
   */
	enum FontStyle
	{
		STYLE_NORMAL  = ::DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
		STYLE_OBLIQUE = ::DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_OBLIQUE,
		STYLE_ITALIC  = ::DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_ITALIC
	};
	/*
   * FontStretch enum:
   *		Description : This enum defines the weight preset style of font
   */
	enum FontWeight
	{
		WEIGHT_THIN		   = 100,
		WEIGHT_EXTRA_LIGHT = 200,
		WEIGHT_ULTRA_LIGHT = 200,
		WEIGHT_LIGHT	   = 300,
		WEIGHT_SEMI_LIGHT  = 350,
		WEIGHT_NORMAL	   = 400,
		WEIGHT_REGULAR	   = 400,
		WEIGHT_MEDIUM	   = 500,
		WEIGHT_DEMI_BOLD   = 600,
		WEIGHT_SEMI_BOLD   = 600,
		WEIGHT_BOLD		   = 700,
		WEIGHT_EXTRA_BOLD  = 800,
		WEIGHT_ULTRA_BOLD  = 800,
		WEIGHT_BLACK	   = 900,
		WEIGHT_HEAVY	   = 900,
		WEIGHT_EXTRA_BLACK = 950,
		WEIGHT_ULTRA_BLACK = 950
	};

public:
	/*
   * VImage constructor:
   *		Description : The constructor of class VFont
   *		Values
	 * Tip	: {
   *			[const VFont&] FontObject : The specified font
   *		}
   */
	VFont(const VFont &FontObject);
	/*
   * VImage constructor:
   *		Description : The constructor of class VFont
   *		Values
	 * Tip	: {
   *			[const std::wstring&] FamilyName  : The font family name
   *of this font
	 * [const FontWeight&  ] TextWidget  : The weight of this font
   *[const FontStyle&   ] TextStyle   : The style
	 * of this font [const
   *FontStretch& ] TextStretch : The stretch style of this font [const float&
   *]
	 * TextSize	  : The size of this font (pixel) [const std::wstring&] TextSize
   *: The local of this font

	 * *		}
   */
	VFont(const std::wstring &FamilyName, const FontWeight &TextWidget, const FontStyle &TextStyle,
		  const FontStretch &TextStretch, const float &TextSize, const std::wstring &Local);
	~VFont();

public:
	/*
   * SetLineAlignment function£º
   *		Description : Set the line alignment mode of this font

	 */
	void SetLineAlignment(const VFontAlignment &Alignment);
	/*
   * SetParagraphAlignment function£º
   *		Description : Set the paragraph alignment mode of this
	 * font
   */
	void SetParagraphAlignment(const VFontParagraphAlignment &Alignment);
	/*
   * SetFamilyName function£º
   *		Description : Set the font family name of this font
   */
	void SetFamilyName(const std::wstring &FontFamilyName);
	/*
   * SetTextSize function£º
   *		Description : Set the text size of this font
   */
	void SetTextSize(const int &Size);
	/*
   * SetTextWidget function£º
   *		Description : Set the text weight of this font
   */
	void SetTextWidget(const int &Widget);

public:
	/*
   * GetFamilyName function£º
   *		Description : Get the family name of this font
   */
	std::wstring GetFamilyName() const;
	/*
   * GetLocaleName function£º
   *		Description : Get the locale name of this font
   */
	std::wstring GetLocaleName() const;
	/*
   * GetTextWeightStyle function£º
   *		Description : Get the font weight name of this font
 */
	FontWeight GetTextWeightStyle() const;
	/*
   * GetTextFontStyle function£º
   *		Description : Get the text font of this font
   */
	FontStyle GetTextFontStyle() const;
	/*
   * GetFontStretchStyle function£º
   *		Description : Get the font stretch of this font
   */
	FontStretch GetFontStretchStyle() const;
	/*
   * GetTextSize function£º
   *		Description : Get the text size of this font
   */
	int GetTextSize() const;

public:
	/*
   * GetDXObject function:
   *		Description : Get the pointer of DirectX2D object
   *
	 * Tips		: The pointer which returned is unchangeable
   */
	IDWriteTextFormat *GetDXObject() VRENDER_HELPER const
	{
		return TextFormat.Get();
	}

private:
	Microsoft::WRL::ComPtr<IDWriteTextFormat> TextFormat;
};
} // namespace Core

VLIB_END_NAMESPACE