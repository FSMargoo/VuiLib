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
 * \file vColorFactory.h
 * \brief The color factory class in VUILib renderer
 */

#pragma once

#include <include/renderer/vRendererBase.h>

#include <bitset>

/**
 * The color helper in VUILib renderer, all of
 */
class VColorFactory {
public:
	/**
	 * Create a color object from the hex string
	 * @param Color The hex color string, which can start with '#' or not
	 * @return The SkColor object formatted from the hex string
	 */
	static SkColor MakeFromHexString(const std::string &Color) {
		auto length = Color.size();
		if ((length < 6 || length > 7) || Color[0] == '#' && length == 6) {
			throw std::logic_error("Error hex string format!");
		}
		if (Color[0] != '#' && length == 7) {
			throw std::logic_error(std::format("Error hex string, do you mean #{}?", Color.substr(1, 6)));
		}
		auto hexString = length == 7 ? Color.substr(1, 6) : Color;

		auto rString = hexString.substr(0, 2);
		auto gString = hexString.substr(2, 2);
		auto bString = hexString.substr(4, 2);

		auto r = toDecimal(rString, 16);
		auto g = toDecimal(gString, 16);
		auto b = toDecimal(bString, 16);

		return VColorFactory::MakeRGB(r, g, b);
	}
	/**
	 * Make a SkColor object from the Red, Green, Blue value
	 * @param R The red value
	 * @param G The green value
	 * @param B The blue value
	 * @return A SkColor object with specified color
	 */
	static SkColor MakeRGB(const short &R, const short &G, const short &B) {
		return SkColorSetRGB(R, G, B);
	}
	/**
	 * Make a SkColor object from the Red, Green, Blue and alpha value
	 * @param A The alpha value
	 * @param R The red value
	 * @param G The green value
	 * @param B The blue value
	 * @return A SkColor object with specified color in target alpha value
	 */
	static SkColor MakeARGB(const short &A, const short &R, const short &G, const short &B) {
		return SkColorSetARGB(A, R, G, B);
	}
	/**
	 * Get the alpha value of the color
	 * @param Color The target color
	 * @return The alpha value of the color
	 */
	static short GetAValue(const SkColor &Color) {
		return SkColorGetA(Color);
	}
	/**
	 * Get the red value of the color
	 * @param Color The target color
	 * @return The red value of the color
	 */
	static short GetRValue(const SkColor &Color) {
		return SkColorGetR(Color);
	}
	/**
	 * Get the blue value of the color
	 * @param Color The target color
	 * @return The blue value of the color
	 */
	static short GetBValue(const SkColor &Color) {
		return SkColorGetB(Color);
	}
	/**
	 * Get the green value of the color
	 * @param Color The target color
	 * @return The green value of the color
	 */
	static short GetGValue(const SkColor &Color) {
		return SkColorGetG(Color);
	}

private:
	/**
	 * Convert the string from specified radix to the decimal number
	 * @param string The origin string
	 * @param radix The target radix
	 * @return The number in decimal number converted from the radix
	 */
	static int toDecimal(std::string string, int radix) {
		int result = 0;
		for (int count = 0; count < string.size(); count++) {
			char ch = string[count];

			if(ch >= '0' && ch <= '9') {
				result = result * radix + (ch - '0');
			}
			else {
				result = result * radix + (ch - 'a' + 10);
			}
		}

		return result;
	}
};