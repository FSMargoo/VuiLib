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
 * \file vAbstractButton.h
 * \brief The abstract button of the button control in VUILib
 */

#pragma once

#include <include/base/object/vObject.h>

/**
 * The abstract button in the VUILib, which provide a set of the
 * signals of the button which is generally used
 */
class VAbstractButton : public VObject {
public:
	/**
	 * Construct the button with default size (generally is 80x20), with the
	 * specified parent object
	 */
	explicit VAbstractButton();
	/**
	 * Construct the button with specified size, with the specified parent object
	 * @param Parent The parent object pointer
	 * @param Width The specified width size
	 * @param Height The specified height size
	 */
	VAbstractButton(const int &Width, const int &Height);

public:
	/**
	 * When the button was clicked, the event will be triggered
	 */
	VEvent<> OnClicked;
	/**
	 * When the button was hovered by mouse, this event will be triggered
	 */
	VEvent<> OnHover;
	/**
	 * When the button was left from the button, this event will be triggered
	 */
	VEvent<> OnLeft;

private:
	/**
	 * Init the button object
	 * @param Parent The specified parent of the button object
	 * @param Width The width of the button
	 * @param Height The height of the button
	 */
	void InitAbstractButton(const int &Width, const int &Height);
};