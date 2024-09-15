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

#include <include/write/vRichTextRenderer.h>
#include <include/renderer/vColorFactory.h>

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
void VRichTextRenderer::Render(SkCanvas *Canvas, const VRect &Bound) {
	RenderNode(_ast->GetRoot(), Canvas, Bound, Bound.GetLeft(), Bound.GetTop(), 18, _englishFont, _otherFont,
			   SkColorSetARGB(255, 255, 255, 255));
}
std::tuple<SkScalar, SkScalar> VRichTextRenderer::RenderNode(VHTMLASTNode *Node, SkCanvas *Canvas,
															 const VRect &Bound, const SkScalar &X, const SkScalar &Y,
															 const SkScalar &Size, SkFont &EnglishFont,
															 SkFont &OtherFont, const SkColor &Color) {
	SkScalar x		   = X;
	SkScalar y		   = Y;
	SkScalar size	   = Size;
	SkColor	 color	   = Color;
	SkFont	 ENGFont   = EnglishFont;
	SkFont	 otherFont = OtherFont;
	sk_sp<SkTypeface> ENGTypeFace = ENGFont.refTypeface();
	sk_sp<SkTypeface> otherTypeFace = otherFont.refTypeface();

	bool inCentering = false;

	SkPaint	 textColor;
	textColor.setColor(Color);
	textColor.setAntiAlias(true);
	if (Node->GetId() == "h1") {
		size = 26;
	}
	if (Node->GetId() == "h2") {
		size = 20;
	}
	if (Node->GetId() == "h3") {
		size = 18;
	}
	if (Node->GetId() == "h4") {
		size = 16;
	}
	if (Node->GetId() == "br") {
		y += Size + _lineSpacing;
		x = Bound.GetLeft();
	}
	if (Node->GetId() == "bold") {
		SkString familyName;
		SkString otherFamilyName;
		ENGTypeFace->getFamilyName(&familyName);
		otherTypeFace->getFamilyName(&otherFamilyName);
		SkFontStyle fontStyle = SkFontStyle(SkFontStyle::kBold_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kUpright_Slant);

		ENGTypeFace = SkTypeface::MakeFromName(familyName.c_str(), fontStyle);
		otherTypeFace = SkTypeface::MakeFromName(otherFamilyName.c_str(), fontStyle);
	}
	if (Node->GetId() == "italic") {
		SkString familyName;
		SkString otherFamilyName;
		ENGTypeFace->getFamilyName(&familyName);
		otherTypeFace->getFamilyName(&otherFamilyName);
		SkFontStyle fontStyle = SkFontStyle(SkFontStyle::kNormal_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kItalic_Slant);

		ENGTypeFace = SkTypeface::MakeFromName(familyName.c_str(), fontStyle);
		otherTypeFace = SkTypeface::MakeFromName(otherFamilyName.c_str(), fontStyle);

		ENGFont.setTypeface(ENGTypeFace);
		otherFont.setTypeface(otherTypeFace);
	}
	if (Node->GetId() == "bold-italic") {
		SkString familyName;
		SkString otherFamilyName;
		ENGTypeFace->getFamilyName(&familyName);
		otherTypeFace->getFamilyName(&otherFamilyName);
		SkFontStyle fontStyle = SkFontStyle(SkFontStyle::kBold_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kItalic_Slant);

		ENGTypeFace = SkTypeface::MakeFromName(familyName.c_str(), fontStyle);
		otherTypeFace = SkTypeface::MakeFromName(otherFamilyName.c_str(), fontStyle);
	}
	if (Node->GetId() == "font") {
		auto newSize		= Node->Property.find("size");
		auto newColor		= Node->Property.find("color");
		auto newFace		= Node->Property.find("face");
		auto newEnglishFace = Node->Property.find("en-face");
		auto newOtherFace	= Node->Property.find("otr-face");

		auto ENGFontStyle = ENGTypeFace->fontStyle();
		auto otherFontStyle = ENGTypeFace->fontStyle();
		if (newSize != Node->Property.end()) {
			size = atoi(newSize->second.c_str());
		}
		if (newColor != Node->Property.end()) {
			color = VColorFactory::MakeFromHexString(newColor->second);
			textColor.setColor(color);
		}
		if (newFace != Node->Property.end()) {
			ENGTypeFace = SkTypeface::MakeFromName(newFace->second.c_str(), ENGFontStyle);
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
	if (Node->GetId() == "center") {
		inCentering = true;
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
					y += Size;
					continue;
				}

				SkFont font;
				if ((codePoint >= U'a' && codePoint <= U'z') || (codePoint >= U'A' && codePoint <= U'Z')) {
					font = ENGFont;
				} else {
					font = otherFont;
				}

				auto testX = x + font.measureText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32);
				if (testX >= Bound.GetRight()) {
					x = Bound.GetLeft();
					y += Size + _lineSpacing;
				}

				Canvas->drawSimpleText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32, x, y + font.getSize(), font, textColor);
				x += font.measureText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32) + _wordSpacing;
			}

			continue;
		}

		std::tie(x, y) = RenderNode(node, Canvas, Bound, x, y, size, ENGFont, otherFont, color);
	}

	return {x, y};
}