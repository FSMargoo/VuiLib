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
 * \file vApplication.h
 * \brief The application class in the VUILib
 */

#pragma once

#include <include/base/object/vObject.h>
#include <include/style/vRadixStyle.h>

/**
 * The application class in VUILib
 */
class VApplication {
public:
	/**
	 * Create the application in default mode; By default, it will
	 * create a theme in light mode with the blue theme color
	 */
	VApplication();

public:
	/**
	 * Get the theme property by the specified name
	 * @param Name The id of the theme property
	 * @return The reference of the property instance
	 */
	VStyleProperty& GetThemeProperty(const std::string &Name);
	/**
	 * Get the value of the specified name in specified type
	 * @tparam Type The type of the property
	 * @param Name The name of the property
	 * @return The value of the property in the specified name
	 */
	template<class Type>
	Type* GetThemePropertyValue(const std::string &Name) {
		return _style.GetPropertyValue<Type>(Name);
	}

public:
	/**
	 * Call this function to run the application
	 * @return The return for main function
	 */
	int Run();

private:
	friend class VMainWindow;

protected:
	GLFWwindow     *_mainWindow;
	VRadixStyleType _styleType;
	VStyleFunction  _style;
};
