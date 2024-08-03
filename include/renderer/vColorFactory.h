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

/**
 * The color helper in VUILib renderer, all of
 */
class VColorFactory {
public:
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
	static SkColor GetAValue(const SkColor &Color) {
		return SkColorGetA(Color);
	}
	/**
	 * Get the red value of the color
	 * @param Color The target color
	 * @return The red value of the color
	 */
	static SkColor GetRValue(const SkColor &Color) {
		return SkColorGetR(Color);
	}
	/**
	 * Get the blue value of the color
	 * @param Color The target color
	 * @return The blue value of the color
	 */
	static SkColor GetBValue(const SkColor &Color) {
		return SkColorGetB(Color);
	}
	/**
	 * Get the green value of the color
	 * @param Color The target color
	 * @return The green value of the color
	 */
	static SkColor GetGValue(const SkColor &Color) {
		return SkColorGetG(Color);
	}
};