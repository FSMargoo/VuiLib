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
 * \file vWidget.h
 * \brief A OOP wrapper of GLFW API for VUILib native window
 */

#pragma once

#include <include/base/event/vEvent.h>
#include <include/mvvm/vViewBase.h>
#include <include/widget/vMonitor.h>

#include <glfw/glfw3.h>

/**
 * This class is a wrapper of GLFW monitor API, which will
 * maintain a GLFWwindow pointer, and converting the GLFW
 * event into VUILib VEvent
 */
class VWidget {
public:
	/**
	 * Construct the widget with only geometry information
	 * @param Width The width of the widget
	 * @param Height The height of the widget
	 * @param Title The title of the widget
	 */
	VWidget(const int &Width, const int &Height, const std::string &Title);
	/**
	 * Create a widget with geometry information on a specified monitor,
	 * it will create a full-screen window by the specified geometry information.
	 * @param Width The width of the widget
	 * @param Height The height of the widget
	 * @param Title The title of the widget
	 * @param Monitor The specified monitor where window to be shown
	 */
	VWidget(const int &Width, const int &Height, const std::string &Title, VMonitor &Monitor);

private:
	/**
	 * The friend class definition of the VWindowView, in VUILib,
	 * we directly hide the GLFW API detailed to user. In this case,
	 * we choose to use the friend class method
	 */
	template<class ViewModel, class... Parameter>
	    requires VIsViewModel<ViewModel>
	friend class VWindowView;

private:
	GLFWwindow *_glfwWindow;
};