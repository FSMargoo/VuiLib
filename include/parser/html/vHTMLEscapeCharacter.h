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
 * \file vHTMLEscapeCharacter.h
 * \brief The escape character in the HTML language for the VUILib HTML parser
 */

#pragma once

#include <include/base/vBase.h>

class VHTMLInvalidEscapeCharacter {
public:
	VHTMLInvalidEscapeCharacter(const OString &Character);

public:
	const char* what() const {
		return reinterpret_cast<const char*>(_info.c_str());
	}

private:
	OString _info;
};

/**
 * The helper class for escape character mapping
 */
class VHTMLEscapeMap {
public:
	/**
	 * Convert the escape character into the specified
	 * @param Character The escape character with & control symbol and ; end symbol
	 * @return The converted string from escape symbol
	 */
	static OString EscapeConvert(const OString &Character);
};