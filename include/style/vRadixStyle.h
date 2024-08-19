/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining af this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, subject to the following conditions:
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
 * \file vRadixStyle.h
 * \brief The default style factory in Radix theme
 */

#pragma once

#include <include/renderer/vRendererBase.h>
#include <include/style/vStyleFunction.h>

/**
 * This helper will create the theme color which determined by the
 * theme color specified by user
 */
class VRadixColorHelper {
public:
	/**
	 * Get the specified gradient color in specified level; Radix theme design a
	 * set of color format rules; Obey it
	 * @param Level The gradient color level
	 * @param BackgroundColor The background color
	 * @param ThemeColor The theme color
	 * @return The color in specified color level
	 */
	static SkColor GetGradientColor(const int &Level, const SkColor &BackgroundColor, const SkColor &ThemeColor) {
		if (Level > 12) {
			throw std::logic_error("Gradient level shouldn't greater than 12!");
		}
		auto alpha = Level / 12 * 255;
		auto r	   = (VColorFactory::GetR(ThemeColor) * alpha + VColorFactory::GetR(BackgroundColor) * 256) / 256;
		auto g	   = (VColorFactory::GetG(ThemeColor) * alpha + VColorFactory::GetG(BackgroundColor) * 256) / 256;
		auto b	   = (VColorFactory::GetB(ThemeColor) * alpha + VColorFactory::GetB(BackgroundColor) * 256) / 256;

		return VColorFactory::MakeRGB(r, g, b);
	}
};

/**
 * VUILib provide two types of style
 */
enum class VRadixStyleType {
	light,
	dark
};

/**
 * This class will product two style functions in dark or light format
 */
class VRadixStyleFactory {
public:
	/**
	 * Get the style function in specified theme format
	 * @param Style The theme format, light or dark
	 * @param ThemeColor The theme color
	 * @return The style function in specified theme style
	 */
	static VStyleFunction GetStyle(const VRadixStyleType &Style, const SkColor &ThemeColor) {
		switch (Style) {
		case VRadixStyleType::light: {
			auto backgroundColor = VColorFactory::MakeRGB(255, 255, 255);

			VStyleFunction style;
			auto		   windowBackgroundColor =
				std::make_unique<VColorProperty>(VRadixColorHelper::GetGradientColor(0, backgroundColor, ThemeColor));

			style.RegisterProperty("window.background.color", std::move(windowBackgroundColor));

			return style;
		}
		case VRadixStyleType::dark: {
			auto backgroundColor = VColorFactory::MakeRGB(11, 11, 11);

			VStyleFunction style;
			auto		   windowBackgroundColor =
				std::make_unique<VColorProperty>(VRadixColorHelper::GetGradientColor(0, backgroundColor, ThemeColor));

			style.RegisterProperty("window.background.color", std::move(windowBackgroundColor));

			return style;
		}
		}
	}
};