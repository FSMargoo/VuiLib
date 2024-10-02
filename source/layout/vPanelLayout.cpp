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
 * \file vPanelLayout.cpp
 * \brief The panel layout in VUILib, the basic panel base class
 */

#include <include/layout/vPanelLayout.h>

VPanel::VPanel(VObject *Parent) : VObject(Parent) {
	Resize(Parent->GetWidth(), Parent->GetHeight());
}
VPanel::VPanel(VObject *Parent, const int &Width, const int &Height) : VObject(Parent) {
	Resize(Width, Height);
}
void VPanel::OnLayoutRearrange() {
	Resize(_parent->GetWidth(), _parent->GetHeight());

	Arrange();
}
VRect VPanel::SizeMeasure() {
	return { 0, 0, GetWidth(), GetHeight() };
}
void VPanel::Arrange() {
	for (auto& object : _childList) {
		if (object->HasProperty("panel.align")) {
			auto& value = object->GetProperty("panel.align")._value;
			if (value->_type == VPropertyType::String) {
				auto align = value->Cast<VStringProperty>()->GetValue();
				if (align == "left") {
					object->Move(0, GetHeight() / 2 - object->GetHeight() / 2);
				}
				if (align == "right") {
					object->Move(GetWidth() - object->GetWidth(), GetHeight() / 2 - object->GetHeight() / 2);
				}
				if (align == "top") {
					object->Move(GetWidth() / 2 - object->GetWidth() / 2, 0);
				}
				if (align == "bottom") {
					object->Move(GetWidth() / 2 - object->GetWidth() / 2, GetHeight() - object->GetHeight());
				}
			}
		}
	}
}