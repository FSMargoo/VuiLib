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
 * \file vViewBase.h
 * \brief The view layer base class in MVVM
 */

#pragma once

#include <mvvm/vViewModelBase.h>

/**
 * The class is the base class for view layer in MVVM for VUILib
 * @tparam Model The model layer class type, make sure the class can be directly free
 * @tparam Parameter The init parameter for the model class
 */
template<class ViewModel, class... Parameter>
	requires VIsViewModel<ViewModel>
class VViewBase : public VViewModel<ViewModel> {
public:
	/**
	 * Constructing the model by the parameter of model class
	 * @param ModelParameter The parameter to construct the model class
	 */
	explicit VViewBase(Parameter... ModelParameter) {
		InitModel(std::forward<Parameter...>(ModelParameter...));
	}
	/**
	 * Constructing the model by an exists model instance, the view model class
	 * will take over the life cycle of model pointer
	 * @param ModelPointer
	 */
	explicit VViewBase(ViewModel* ViewModelPointer) : _viewModelPointer(ViewModelPointer) {

	}
	~VViewBase() {
		// Make sure every model pointer are directly deletable.
		delete _viewModelPointer;
	}

private:
	/**
	 * Init the model base class
	 * @param ModelParameter The parameter for the base class
	 */
	inline void InitModel(Parameter... ModelParameter) {
		_viewModelPointer = new ViewModel;
	}

protected:
	ViewModel *_viewModelPointer;
};