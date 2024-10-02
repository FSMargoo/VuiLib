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
 * \file vPanelLayout.h
 * \brief The panel layout in VUILib, the basic panel base class
 */

#pragma once

#include <include/base/object/vObject.h>

/**
 * The default panel default class in VUILib, it provides one property for all child class:
 * 	panel.align : Allow "Left" or "Right", "Top", "Bottom"
 * The control will overlap
 */
class VPanel : public VObject {
public:
	explicit VPanel(VObject *Parent);
	VPanel(VObject *Parent, const int &Width, const int &Height);

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
};