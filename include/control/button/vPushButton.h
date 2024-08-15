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
 * \file vPushButton.h
 * \brief The push button class in VUILib
 */

#pragma once

#include <include/control/button/vAbstractButton.h>

/**
 * The push button in the VUILib, this button provide a property "text"
 * as the display text on the button
 */
class VPushButton : public VAbstractButton {
public:
	/**
	 * Create the button in default size with the specified parent
	 * @param Parent The parent of the button object
	 * @param Text The text on the button
	 */
	VPushButton(VObject *Parent, const OString &Text);
	/**
	 * Create the button in default size with the specified parent and
	 * geometry size
	 * @param Parent The parent of the button object
	 * @param Width The width of the button object
	 * @param Height The height of the button object
	 * @param Text The text on the button
	 */
	VPushButton(VObject *Parent, const int &Width, const int &Height, const OString &Text);

public:
	/**
	 * Set the plain text of the button, which does not support the rich text
	 * @param Text The text in plain form
	 */
	void SetPlainText(const OString &Text);
	/**
	 * Get the plain text of the button, which will format every rich text control
	 * symbol
	 * @return The text in plain form
	 */
	[[nodiscard]] OString GetPlainText() const;

private:
	/**
	 * Init the property of the button
	 * @param Text The text display on the button
	 */
	void InitProperty(const OString &Text);

private:
	VStringProperty *_text;
};