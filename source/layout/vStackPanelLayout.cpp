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
 * \file vStackPanelLayout.cpp
 * \brief The stack panel, every element in stack panel will be arranged from top to bottom
 */

#include <include/layout/vStackPanelLayout.h>

VStackPanel::VStackPanel(VObject *Parent) : VObject(Parent) {
	Resize(Parent->GetWidth(), Parent->GetHeight());
}
VStackPanel::VStackPanel(VObject *Parent, const int &Width, const int &Height) : VObject(Parent) {
	Resize(Width, Height);

	_userSpecifiedSize->_value = true;
}
void VStackPanel::OnLayoutRearrange() {
	if (!_userSpecifiedSize->_value && (GetWidth() != _parent->GetWidth() || GetHeight() != _parent->GetHeight())) {
		Resize(_parent->GetWidth(), _parent->GetHeight());
	}

	Arrange();
}
VRect VStackPanel::SizeMeasure() {
	int count = 0;
	for (auto &object : _childList) {
		count += object->GetHeight();
		if (count >= GetHeight()) {
			count = GetHeight();

			break;
		}
	}

	return {0, 0, GetWidth(), count};
}
void VStackPanel::Arrange() {
	int count = 0;
	for (auto &object : _childList) {
		auto width				  = object->GetWidth();
		auto height				  = object->GetHeight();
		auto layoutSizingProperty = object->GetPropertyValue<VBooleanProperty>(PN_UserSpecifiedSize);

		bool hasBottomMargin = false;
		int	 bottomMargin	 = 0;
		if (object->HasProperty("stack.margin")) {
			auto &value = object->GetProperty("stack.margin")._value;
			if (value->_type == VPropertyType::Int) {
				auto margin = value->Cast<VIntProperty>()->GetValue();
				count += margin;
				bottomMargin	= margin;
				hasBottomMargin = true;
			}
		} else {
			if (object->HasProperty("stack.margin.top")) {
				auto &value = object->GetProperty("stack.margin.top")._value;
				if (value->_type == VPropertyType::Int) {
					auto margin = value->Cast<VIntProperty>()->GetValue();
					count += margin;
				}
			}
			if (object->HasProperty("stack.margin.bottom")) {
				auto &value = object->GetProperty("stack.margin.bottom")._value;
				if (value->_type == VPropertyType::Int) {
					auto margin		= value->Cast<VIntProperty>()->GetValue();
					bottomMargin	= margin;
					hasBottomMargin = true;
				}
			}
		}
		if (object->HasProperty("stack.align")) {
			auto &value = object->GetProperty("stack.align")._value;
			if (value->_type == VPropertyType::String) {
				auto align = value->Cast<VStringProperty>()->GetValue();
				if (align == "left") {
					width = GetWidth();
					if (!layoutSizingProperty->_value) {
						object->Resize(width, height);
					}

					object->Move(0, count);
				}
				if (align == "right") {
					width = GetWidth();
					if (!layoutSizingProperty->_value) {
						object->Resize(width, height);
					}

					object->Move(GetWidth() - object->GetWidth(), count);
				} else {
					width = GetWidth();
					if (!layoutSizingProperty->_value) {
						object->Resize(width, height);
					}

					object->Move(GetWidth() / 2 - object->GetWidth() / 2, count);
				}
			}
		} else {
			width = GetWidth();
			if (!layoutSizingProperty->_value) {
				object->Resize(width, height);
			}

			object->Move(GetWidth() / 2 - object->GetWidth() / 2, count);
		}

		count += object->GetHeight();
		if (hasBottomMargin) {
			count += bottomMargin;
		}
	}
}