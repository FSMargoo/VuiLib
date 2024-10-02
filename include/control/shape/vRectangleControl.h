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
 * \file vRectangleControl.h
 * \brief The basic shape control of a rectangle
 */

#pragma once

#include <include/base/object/vObject.h>

/**
 * The rectangle control, the rectangle control will display a rectangle on the screen
 */
class VRectangleControl : public VObject {
public:
	/**
	 * Init the rectangle control with specified geometry information
	 * @param Width The width of the rectangle control
	 * @param Height The height of the rectangle control
	 * @param Parent The parent of the rectangle control
	 */
	VRectangleControl(const int &Width, const int &Height, VObject *Parent);

public:
	/**
	 * Set the radius of the border
	 * @param Radius The border radius
	 */
	void SetBorderRadius(const float &Radius);
	/**
	 * Set the background color
	 * @param Color The color of the background
	 */
	void SetBackgroundColor(const SkColor &Color);
	/**
	 * Set the border color
	 * @param Color The color of the border
	 */
	void SetBorderColor(const SkColor &Color);

public:
	void OnPaint(sk_sp<SkSurface> &Surface) override;

private:
	/**
	 * Init the property by default with the color in white
	 */
	void InitProperty();

private:
	VFloatProperty *_borderRadius;
	VColorProperty *_backgroundColor;
	VColorProperty *_borderColor;
};