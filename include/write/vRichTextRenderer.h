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
 * \file vRichTextRenderer.h
 * \brief The HTML AST based rich text renderer in VUILib
 */

#pragma once

#include <include/write/vRichTextNativeLabel.h>

/**
 * The rich text renderer in VUILib
 */
class VRichTextRenderer {
public:
	/**
	 * Construct the rich text renderer with the HTML AST
	 * @param AST The HTML AST tree of the rich text
	 */
	explicit VRichTextRenderer(VHTMLAST *AST);

public:
	/**
	 * Render the rich text on the canvas
	 * @param Canvas The reference of the canvas
	 * @param Bound The bound of the text area
	 */
	void Render(SkCanvas *Canvas, const VRect &Bound);

public:
	/**
	 * Register a specified type label into the renderer
	 * @tparam Type The type of the label interface
	 */
	template <class Type>
		requires std::is_base_of_v<VRichTextLabelRendererInterface, Type>
	void RegisterLabel() {
		auto instance = new Type();
		if (!_interfaceManager.contains(instance->GetID())) {
			_interfaceManager.insert({instance->GetID(), instance});
		}
	}

private:
	/**
	 * Init the renderer default font set
	 */
	void InitFontSet();
	/**
	 * Init the native label interface
	 */
	void InitLabels();

private:
	VHTMLAST *_ast;

private:
	SkScalar _lineSpace;
	SkScalar _wordSpace;

private:
	std::vector<SkScalar> _lineWidths;
	std::vector<SkScalar> _lineHeights;

private:
	sk_sp<SkTypeface> _otherTypeFace;
	sk_sp<SkTypeface> _englishTypeFace;
	SkFont			  _englishFont;
	SkFont			  _otherFont;
	SkColor			  _color;
	SkScalar		  _size;

private:
	VRichTextLabelInterfaceManager _interfaceManager;
};