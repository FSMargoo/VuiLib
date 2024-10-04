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
 * \file vStackPanelLayout.h
 * \brief The stack panel, every element in stack panel will be arranged from top to bottom
 */

#pragma once

#include <include/base/object/vObject.h>

/**
 * The stack panel, every element in stack panel will be arranged from top to bottom
 * it provides one property for all child class:
 * 	stack.align : Allow "left" or "right", "center"
 * 	stack.margin.top : Allow a number to set the margin related to the top
 * 	stack.margin.bottom : Allow a number to set the margin related to the bottom
 */
class VStackPanel : public VObject {
public:
	explicit VStackPanel(VObject *Parent);
	VStackPanel(VObject *Parent, const int &Width, const int &Height);

public:
	/**
	 * Measure the rectangle size of the panel, return the rectangle related to the origin point (0, 0)
	 * @return The size rectangle related to the origin point (0, 0)
	 */
	virtual VRect SizeMeasure();
	/**
	 * Arrange the panel child objects
	 */
	virtual void Arrange();

public:
	void OnLayoutRearrange() override;

public:
	void OnPaint(sk_sp<SkSurface> &Surface) override {

	}
};