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

VRichTextRenderer::VRichTextRenderer(VHTMLAST *AST) : _ast(AST) {
	auto root = _ast->GetRoot();

	_lineSpace = 5;
	_wordSpace = 0;

	_color = SK_ColorWHITE;

	InitLabels();
	InitFontSet();
}
void VRichTextRenderer::Render(SkCanvas *Canvas, const VRect &Bound) {
	VRichTextRendererContext context(_lineWidths, _lineHeights);
	context.X			= Bound.GetLeft();
	context.Y			= Bound.GetTop();
	context.Bound		= Bound;
	context.Size		= _size;
	context.WordSpace	= _wordSpace;
	context.LineSpace	= _lineSpace;
	context.EnglishFont = _englishFont;
	context.OtherFont	= _otherFont;
	context.MaxHeight	= 0;
	context.Color		= _color;

	auto root = _ast->GetRoot();
	for (auto &node : *root) {
		if (_interfaceManager.contains(node->GetId())) {
			context	   = _interfaceManager[node->GetId()]->ContextMeasure(node, context);
			auto value = _interfaceManager[node->GetId()]->WidthMeasure(node, context, _interfaceManager);
			if (value.has_value()) {
				context = value.value();
			}
		} else {
			context	   = _interfaceManager["$Default"]->ContextMeasure(node, context);
			auto value = _interfaceManager["$Default"]->WidthMeasure(node, context, _interfaceManager);
			if (value.has_value()) {
				context = value.value();
				context.WidthCollections.push_back(context.X - context.Bound.GetLeft());
				context.HeightCollections.push_back(context.MaxHeight);
			}
		}
	}
	context.X			= Bound.GetLeft();
	context.Y			= Bound.GetTop();
	context.Size		= _size;
	context.WordSpace	= _wordSpace;
	context.LineSpace	= _lineSpace;
	context.EnglishFont = _englishFont;
	context.LineCount	= 0;
	context.OtherFont	= _otherFont;
	context.MaxHeight	= 0;
	context.Color		= _color;
	for (auto &node : *root) {
		if (_interfaceManager.contains(node->GetId())) {
			context = _interfaceManager[node->GetId()]->ContextMeasure(node, context);
			context = _interfaceManager[node->GetId()]->Render(node, Canvas, context, _interfaceManager);
		} else {
			context = _interfaceManager["$Default"]->ContextMeasure(node, context);
			context = _interfaceManager["$Default"]->Render(node, Canvas, context, _interfaceManager);
		}
	}
}
void VRichTextRenderer::InitFontSet() {
	_size = 18;

	_otherTypeFace	 = SkTypeface::MakeFromName("Microsoft YaHei UI", SkFontStyle());
	_englishTypeFace = SkTypeface::MakeDefault();

	_englishFont.setTypeface(_englishTypeFace);
	_otherFont.setTypeface(_otherTypeFace);

	_englishFont.setSize(_size);
	_otherFont.setSize(_size);

	_englishFont.setEdging(SkFont::Edging::kAntiAlias);
	_otherFont.setEdging(SkFont::Edging::kAntiAlias);
}
void VRichTextRenderer::InitLabels() {
	RegisterLabel<VRichTextH1Label>();
	RegisterLabel<VRichTextH2Label>();
	RegisterLabel<VRichTextH3Label>();
	RegisterLabel<VRichTextH4Label>();
	RegisterLabel<VRichTextBRLabel>();
	RegisterLabel<VRichDefaultLabel>();
	RegisterLabel<VRichTextItalicLabel>();
	RegisterLabel<VRichTextBoldLabel>();
	RegisterLabel<VRichTextBoldItalicLabel>();
	RegisterLabel<VRichTextFontLabel>();
	RegisterLabel<VRichTextULabel>();
	RegisterLabel<VRichTextStrikeLabel>();
	RegisterLabel<VRichTextCenterLabel>();
	RegisterLabel<VRichTextLeftLabel>();
	RegisterLabel<VRichTextRightLabel>();
	RegisterLabel<VRichTextVCenterLabel>();
}