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
 * \file vMainWindow.h
 * \brief The major window in the VUILib, the lifetime of the main window
 * is also the lifetime of the program
 */

#pragma once

#include <include/widget/vWidget.h>

/**
 * The major window in the VUILib, the lifetime of the main window
 * is also the lifetime of the program, it will bind to the main window
 * of the application object which determined the main loop's lifetime
 */
class VMainWindow : public VWidget {
public:
	/**
	 * Construct the widget with only geometry information
	 * @param Application The application instance of the widget
	 * @param Width The width of the widget
	 * @param Height The height of the widget
	 * @param Title The title of the widget
	 */
	VMainWindow(VApplication *Application, const int &Width, const int &Height, const std::string &Title);
	/**
	 * Create a widget with geometry information on a specified monitor,
	 * it will create a full-screen window by the specified geometry information.
	 * @param Application The application instance of the widget
	 * @param Width The width of the widget
	 * @param Height The height of the widget
	 * @param Title The title of the widget
	 * @param Monitor The specified monitor where window to be shown
	 */
	VMainWindow(VApplication *Application, const int &Width, const int &Height, const std::string &Title, VMonitor &Monitor);
};