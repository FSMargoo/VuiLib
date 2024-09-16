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
 * \file vPushButton.cpp
 * \brief The push button class in VUILib
 */

#include <include/control/button/vPushButton.h>

VPushButton::VPushButton(VObject *Parent, const OString &Text) : VAbstractButton() {
	InitProperty(Text);

	SetParent(Parent);
}
VPushButton::VPushButton(VObject *Parent, const int &Width, const int &Height, const OString &Text)
	: VAbstractButton(Width, Height) {
	InitProperty(Text);

	SetParent(Parent);
}
void VPushButton::SetRichText(const OString &Text) {
	_text->_value = ostr::format("<html><vcenter><center>{}</center></vcenter></html>", Text);
	delete _AST;
	_AST = new VHTMLAST(_text->_value);

	RepaintOnSelf();
}
void VPushButton::SetPlainText(const OString &Text) {
	_text->_value = ostr::format("<html><vcenter><center>{}</center></vcenter></html>", VHTMLLexer::ConvertToPlainText(Text));
	delete _AST;
	_AST = new VHTMLAST(_text->_value);

	RepaintOnSelf();
}
OString VPushButton::GetPlainText() const {
	return _text->_value;
}
void VPushButton::InitProperty(const OString &Text) {
	auto text = std::make_unique<VStringProperty>(Text);
	RegisterProperty("text", std::move(text));
	_text = GetPropertyValue<VStringProperty>("text");

	_text->_value = ostr::format("<html><vcenter><center>{}</center></vcenter></html>", VHTMLLexer::ConvertToPlainText(Text));
	_AST		  = new VHTMLAST(_text->_value);
}
void VPushButton::OnPaint(sk_sp<SkSurface> &Surface) {
	VRect bound{0, 0, GetWidth(), GetHeight()};

	auto			  canvas = Surface->getCanvas();
	VRichTextRenderer renderer(_AST);

	renderer.Render(canvas, bound);

	canvas->flush();
}