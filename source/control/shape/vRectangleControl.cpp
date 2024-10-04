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
 * \file vRectangle.cpp
 * \brief The basic shape control of a rectangle
 */

#include <include/control/shape/vRectangleControl.h>

VRectangleControl::VRectangleControl(const int &Width, const int &Height, VObject *Parent) : VObject(Parent) {
	InitProperty();

	Resize(Width, Height);

	SetPropertyValue<VBooleanProperty, bool>(PN_UserSpecifiedSize, true);
}
VRectangleControl::VRectangleControl(VObject *Parent) : VObject(Parent) {
	InitProperty();

	Resize(100, 50);
}
void VRectangleControl::SetBorderRadius(const float &Radius) {
	_borderRadius->_value = Radius;
}
void VRectangleControl::SetBackgroundColor(const SkColor &Color) {
	_backgroundColor->_value = Color;
}
void VRectangleControl::SetBorderColor(const SkColor &Color) {
	_borderColor->_value = Color;
}
void VRectangleControl::OnPaint(sk_sp<SkSurface> &Surface) {
	SkRect bound(0, 0, GetWidth(), GetHeight());

	SkPaint paint;
	paint.setStyle(SkPaint::kFill_Style);
	paint.setColor(_backgroundColor->_value);
	SkPaint strokePaint;
	strokePaint.setStyle(SkPaint::kStroke_Style);
	strokePaint.setColor(_borderColor->_value);

	auto canvas = Surface->getCanvas();
	canvas->drawRoundRect(bound, _borderRadius->_value, _borderRadius->_value, paint);
	canvas->drawRoundRect(bound, _borderRadius->_value, _borderRadius->_value, strokePaint);

	canvas->flush();
}
void VRectangleControl::InitProperty() {
	auto borderRadius	 = std::make_unique<VFloatProperty>(0.f);
	auto backgroundColor = std::make_unique<VColorProperty>(SK_ColorWHITE);
	auto borderColor	 = std::make_unique<VColorProperty>(SK_ColorWHITE);

	RegisterProperty("border.radius", std::move(borderRadius));
	RegisterProperty("color.background", std::move(backgroundColor));
	RegisterProperty("color.border", std::move(borderColor));

	_borderRadius	 = GetPropertyValue<VFloatProperty>("border.radius");
	_backgroundColor = GetPropertyValue<VColorProperty>("color.background");
	_borderColor	 = GetPropertyValue<VColorProperty>("color.border");
}