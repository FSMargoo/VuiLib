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
 * \file vWindowView.h
 * \brief The extent of the view class in window form
 */

#pragma once

#include <include/mvvm/vViewBase.h>
#include <include/widget/vWidget.h>
#include <include/base/object/vObject.h>

/**
 * To inherit of the view base class in window form
 * @tparam ViewModel The view model class type
 * @tparam Parameter The construction parameter of the view mode class
 */
template<class ViewModel, class... Parameter>
    requires VIsViewModel<ViewModel>
class VWindowView : public VViewBase<ViewModel>, VWidget {
public:
	/**
	 * Constructing the window view by the parameter of view model class and
	 * the geometry information of the window
	 * @param ModelParameter The parameter to construct the model class
	 * @param Width The width of the widget
	 * @param Height The height of the widget
	 * @param Title The title of the widget
	 */
	VWindowView(const int &Width, const int &Height, const std::string &Title, Parameter... ModelParameter)
		: VViewBase<ViewModel>(ModelParameter...), VWidget(Width, Height, Title) {
	}
	/**
	 * Constructing the window view by the parameter of view model class and
	 * the geometry information of the window in the specified monitor
	 * @param ModelParameter The parameter to construct the model class
	 * @param Width The width of the widget
	 * @param Height The height of the widget
	 * @param Title The title of the widget
	 * @param Monitor The specified monitor where window to be shown
	 */
	VWindowView(const int &Width, const int &Height, const std::string &Title, VMonitor &Monitor,
	            Parameter... ModelParameter)
	    : VViewBase<ViewModel>(ModelParameter...) {

	}
	~VWindowView() override = default;
};