/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file vRichTextRenderer.cpp
 * \brief The HTML AST based rich text renderer in VUILib
 */

#include <include/renderer/vColorFactory.h>
#include <include/write/vRichTextRenderer.h>

#include <iostream>

VRichTextRenderer::VRichTextRenderer(VHTMLAST *AST) : _ast(AST) {
	auto root = _ast->GetRoot();

	_lineSpacing = 5;
	_wordSpacing = 0;

	InitFontSet();
}
void VRichTextRenderer::InitFontSet() {
	_otherTypeFace	 = SkTypeface::MakeFromName("Microsoft YaHei UI", SkFontStyle());
	_englishTypeFace = SkTypeface::MakeDefault();

	_englishFont.setTypeface(_englishTypeFace);
	_otherFont.setTypeface(_otherTypeFace);

	_englishFont.setSize(18);
	_otherFont.setSize(18);

	_englishFont.setEdging(SkFont::Edging::kAntiAlias);
	_otherFont.setEdging(SkFont::Edging::kAntiAlias);
}
std::tuple<SkScalar, std::optional<int>> VRichTextRenderer::CalculatingRendering(VHTMLASTNode *Node, const VRect &Bound,
																				 const SkScalar &X, const SkScalar &Y,
																				 const SkScalar &Size,
																				 SkFont &EnglishFont, SkFont &OtherFont,
																				 const SkScalar &MaxHeight) {
	SkScalar		  x				= X;
	SkScalar		  y				= Y;
	SkScalar		  size			= Size;
	SkFont			  ENGFont		= EnglishFont;
	SkFont			  otherFont		= OtherFont;
	sk_sp<SkTypeface> ENGTypeFace	= ENGFont.refTypeface();
	sk_sp<SkTypeface> otherTypeFace = otherFont.refTypeface();

	if (Node->GetId() == "h1") {
		size = _H1Size;
	}
	if (Node->GetId() == "h2") {
		size = _H2Size;
	}
	if (Node->GetId() == "h3") {
		size = _H3Size;
	}
	if (Node->GetId() == "h4") {
		size = _H4Size;
	}
	if (Node->GetId() == "br") {
		y += Size + _lineSpacing;
		_lineWidths.push_back(x - Bound.GetLeft());
		_lineHeights.push_back(MaxHeight);
		x = Bound.GetLeft();
	}
	if (Node->GetId() == "bold") {
		SkString familyName;
		SkString otherFamilyName;
		ENGTypeFace->getFamilyName(&familyName);
		otherTypeFace->getFamilyName(&otherFamilyName);
		SkFontStyle fontStyle =
			SkFontStyle(SkFontStyle::kBold_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kUpright_Slant);

		ENGTypeFace	  = SkTypeface::MakeFromName(familyName.c_str(), fontStyle);
		otherTypeFace = SkTypeface::MakeFromName(otherFamilyName.c_str(), fontStyle);
	}
	if (Node->GetId() == "italic") {
		SkString familyName;
		SkString otherFamilyName;
		ENGTypeFace->getFamilyName(&familyName);
		otherTypeFace->getFamilyName(&otherFamilyName);
		SkFontStyle fontStyle =
			SkFontStyle(SkFontStyle::kNormal_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kItalic_Slant);

		ENGTypeFace	  = SkTypeface::MakeFromName(familyName.c_str(), fontStyle);
		otherTypeFace = SkTypeface::MakeFromName(otherFamilyName.c_str(), fontStyle);

		ENGFont.setTypeface(ENGTypeFace);
		otherFont.setTypeface(otherTypeFace);
	}
	if (Node->GetId() == "bold-italic") {
		SkString familyName;
		SkString otherFamilyName;
		ENGTypeFace->getFamilyName(&familyName);
		otherTypeFace->getFamilyName(&otherFamilyName);
		SkFontStyle fontStyle =
			SkFontStyle(SkFontStyle::kBold_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kItalic_Slant);

		ENGTypeFace	  = SkTypeface::MakeFromName(familyName.c_str(), fontStyle);
		otherTypeFace = SkTypeface::MakeFromName(otherFamilyName.c_str(), fontStyle);
	}
	if (Node->GetId() == "font") {
		auto newSize		= Node->Property.find("size");
		auto newFace		= Node->Property.find("face");
		auto newEnglishFace = Node->Property.find("en-face");
		auto newOtherFace	= Node->Property.find("otr-face");

		auto ENGFontStyle	= ENGTypeFace->fontStyle();
		auto otherFontStyle = ENGTypeFace->fontStyle();
		if (newSize != Node->Property.end()) {
			size = atoi(newSize->second.c_str());
		}
		if (newFace != Node->Property.end()) {
			ENGTypeFace	  = SkTypeface::MakeFromName(newFace->second.c_str(), ENGFontStyle);
			otherTypeFace = SkTypeface::MakeFromName(newFace->second.c_str(), otherFontStyle);

			ENGFont.setTypeface(ENGTypeFace);
			otherFont.setTypeface(otherTypeFace);
		}
		if (newEnglishFace != Node->Property.end()) {
			ENGTypeFace = SkTypeface::MakeFromName(newEnglishFace->second.c_str(), ENGFontStyle);
			ENGFont.setTypeface(ENGTypeFace);
		}
		if (newOtherFace != Node->Property.end()) {
			otherTypeFace = SkTypeface::MakeFromName(newOtherFace->second.c_str(), otherFontStyle);
			otherFont.setTypeface(otherTypeFace);
		}
	}

	auto maxSize = std::max(size, MaxHeight);

	ENGFont.setSize(size);
	otherFont.setSize(size);

	for (auto &node : *Node) {
		if (node->GetId() == "$context") {
			for (auto character : node->Context) {
				if (y >= Bound.GetBottom()) {
					_lineWidths.push_back(x - Bound.GetLeft());
					_lineHeights.push_back(maxSize);

					return {};
				}

				auto codePoint = (char32_t)character.get_codepoint();
				if (codePoint == U'\0') {
					break;
				}
				if (codePoint == U'\n' || codePoint == U'\t') {
					y += Size;
					_lineWidths.push_back(x - Bound.GetLeft());
					_lineHeights.push_back(maxSize);
					x = Bound.GetLeft();

					continue;
				}

				SkFont font;
				if ((codePoint >= U'a' && codePoint <= U'z') || (codePoint >= U'A' && codePoint <= U'Z')) {
					font = ENGFont;
				} else {
					font = otherFont;
				}

				auto marchX = x + font.measureText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32);
				if (marchX >= Bound.GetRight()) {
					_lineWidths.push_back(x - Bound.GetLeft());
					_lineHeights.push_back(maxSize);
					maxSize = size;

					x = Bound.GetLeft();
					y += Size + _lineSpacing;
				}

				x += font.measureText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32) + _wordSpacing;
			}

			continue;
		}

		auto value = CalculatingRendering(node, Bound, x, y, size, ENGFont, otherFont, maxSize);
		if (!std::get<1>(value).has_value()) {
			return {};
		}
		x = std::get<1>(value).value();

		maxSize = std::max(std::get<0>(value), maxSize);
	}

	return {maxSize, x};
}
void VRichTextRenderer::Render(SkCanvas *Canvas, const VRect &Bound) {
	auto value =
		CalculatingRendering(_ast->GetRoot(), Bound, Bound.GetLeft(), Bound.GetTop(), 18, _englishFont, _otherFont, 0);
	if (std::get<1>(value).has_value()) {
		_lineWidths.push_back(std::get<1>(value).value());
		_lineHeights.push_back(std::get<0>(value));
	}
	RenderNode(_ast->GetRoot(), Canvas, Bound, Bound.GetLeft(), Bound.GetTop(), 18, _englishFont, _otherFont,
			   SkColorSetARGB(255, 255, 255, 255), 0);
}
std::tuple<SkScalar, SkScalar, int> VRichTextRenderer::RenderNode(VHTMLASTNode *Node, SkCanvas *Canvas, const VRect &Bound,
															 const SkScalar &X, const SkScalar &Y, const SkScalar &Size,
															 SkFont &EnglishFont, SkFont &OtherFont,
															 const SkColor &Color, const int &LineCount) {
	SkScalar		  x				= X;
	SkScalar		  y				= Y;
	SkScalar		  size			= Size;
	SkColor			  color			= Color;
	SkFont			  ENGFont		= EnglishFont;
	SkFont			  otherFont		= OtherFont;
	int				  lineCount		= LineCount;
	sk_sp<SkTypeface> ENGTypeFace	= ENGFont.refTypeface();
	sk_sp<SkTypeface> otherTypeFace = otherFont.refTypeface();

	auto baseHeight = _lineHeights[lineCount];

	SkPaint textColor;
	textColor.setColor(Color);
	textColor.setAntiAlias(true);
	if (Node->GetId() == "h1") {
		size = _H1Size;
	}
	if (Node->GetId() == "h2") {
		size = _H2Size;
	}
	if (Node->GetId() == "h3") {
		size = _H3Size;
	}
	if (Node->GetId() == "h4") {
		size = _H4Size;
	}
	if (Node->GetId() == "br") {
		y += baseHeight + _lineSpacing;
		x = Bound.GetLeft();
		++lineCount;
	}
	if (Node->GetId() == "bold") {
		SkString familyName;
		SkString otherFamilyName;
		ENGTypeFace->getFamilyName(&familyName);
		otherTypeFace->getFamilyName(&otherFamilyName);
		SkFontStyle fontStyle =
			SkFontStyle(SkFontStyle::kBold_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kUpright_Slant);

		ENGTypeFace	  = SkTypeface::MakeFromName(familyName.c_str(), fontStyle);
		otherTypeFace = SkTypeface::MakeFromName(otherFamilyName.c_str(), fontStyle);
	}
	if (Node->GetId() == "italic") {
		SkString familyName;
		SkString otherFamilyName;
		ENGTypeFace->getFamilyName(&familyName);
		otherTypeFace->getFamilyName(&otherFamilyName);
		SkFontStyle fontStyle =
			SkFontStyle(SkFontStyle::kNormal_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kItalic_Slant);

		ENGTypeFace	  = SkTypeface::MakeFromName(familyName.c_str(), fontStyle);
		otherTypeFace = SkTypeface::MakeFromName(otherFamilyName.c_str(), fontStyle);

		ENGFont.setTypeface(ENGTypeFace);
		otherFont.setTypeface(otherTypeFace);
	}
	if (Node->GetId() == "bold-italic") {
		SkString familyName;
		SkString otherFamilyName;
		ENGTypeFace->getFamilyName(&familyName);
		otherTypeFace->getFamilyName(&otherFamilyName);
		SkFontStyle fontStyle =
			SkFontStyle(SkFontStyle::kBold_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kItalic_Slant);

		ENGTypeFace	  = SkTypeface::MakeFromName(familyName.c_str(), fontStyle);
		otherTypeFace = SkTypeface::MakeFromName(otherFamilyName.c_str(), fontStyle);
	}
	if (Node->GetId() == "font") {
		auto newSize		= Node->Property.find("size");
		auto newColor		= Node->Property.find("color");
		auto newFace		= Node->Property.find("face");
		auto newEnglishFace = Node->Property.find("en-face");
		auto newOtherFace	= Node->Property.find("otr-face");

		auto ENGFontStyle	= ENGTypeFace->fontStyle();
		auto otherFontStyle = ENGTypeFace->fontStyle();
		if (newSize != Node->Property.end()) {
			size = atoi(newSize->second.c_str());
		}
		if (newColor != Node->Property.end()) {
			color = VColorFactory::MakeFromHexString(newColor->second);
			textColor.setColor(color);
		}
		if (newFace != Node->Property.end()) {
			ENGTypeFace	  = SkTypeface::MakeFromName(newFace->second.c_str(), ENGFontStyle);
			otherTypeFace = SkTypeface::MakeFromName(newFace->second.c_str(), otherFontStyle);

			ENGFont.setTypeface(ENGTypeFace);
			otherFont.setTypeface(otherTypeFace);
		}
		if (newEnglishFace != Node->Property.end()) {
			ENGTypeFace = SkTypeface::MakeFromName(newEnglishFace->second.c_str(), ENGFontStyle);
			ENGFont.setTypeface(ENGTypeFace);
		}
		if (newOtherFace != Node->Property.end()) {
			otherTypeFace = SkTypeface::MakeFromName(newOtherFace->second.c_str(), otherFontStyle);
			otherFont.setTypeface(otherTypeFace);
		}
	}
	if (Node->GetId() == "vcenter") {
		int heightSum = 0;
		for (auto count = lineCount; count < _lineWidths.size(); ++count) {
			heightSum += _lineHeights[count] + _lineSpacing;
		}

		y = y +  (Bound.GetBottom() - y) / 2 - heightSum / 2;
	}
	if (Node->GetId() == "center") {
		x = Bound.GetWidth() / 2 - _lineWidths[lineCount] / 2;
	}

	ENGFont.setSize(size);
	otherFont.setSize(size);

	for (auto &node : *Node) {
		if (node->GetId() == "$context") {
			for (auto character : node->Context) {
				if (y >= Bound.GetBottom()) {
					break;
				}

				auto codePoint = (char32_t)character.get_codepoint();
				if (codePoint == U'\0') {
					break;
				}
				if (codePoint == U'\n' || codePoint == U'\t') {
					y += baseHeight + _lineSpacing;
					++lineCount;
					continue;
				}

				SkFont font;
				if ((codePoint >= U'a' && codePoint <= U'z') || (codePoint >= U'A' && codePoint <= U'Z')) {
					font = ENGFont;
				} else {
					font = otherFont;
				}

				auto testX = x + font.measureText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32) + _wordSpacing;
				if (testX >= Bound.GetRight()) {
					x = Bound.GetLeft();
					y += baseHeight + _lineSpacing;
					++lineCount;
				}

				Canvas->drawSimpleText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32, x, y + baseHeight,
									   font, textColor);
				x += font.measureText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32) + _wordSpacing;
			}

			continue;
		}

		std::tie(x, y, lineCount) = RenderNode(node, Canvas, Bound, x, y, size, ENGFont, otherFont, color, lineCount);
	}

	return {x, y, lineCount};
}