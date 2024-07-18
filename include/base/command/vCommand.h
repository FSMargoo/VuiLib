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
 * \file vCommand.h
 * \brief The command class
 */

#pragma once

#include <base/event/vEvent.h>

/***
 * An empty structure for VCommand's placeholder
 */
struct VPlaceHolderObject {};

/***
 * The command class for VUILib. It just like a event which has not connection method
 * @tparam ObjectType The object type for class function, if not use the VPlaceHolderObject type
 * @tparam Parameter The parameter list of command
 */
template <class ObjectType, class... Parameter>
class VCommand {
public:
	using Delegate = VEvent<Parameter...>;

public:
	explicit VCommand(Delegate::FunctionPointer Pointer) {
		_commandEvent.Connect(Pointer);
	}
	VCommand(ObjectType *Object, Delegate::template ClassFunctionPointer<ObjectType> Pointer) {
		_commandEvent.template Connect<ObjectType>(Object, Pointer);
	}

public:
	/***
	 * Emit the command
	 * @param CallList The call parameter list
	 */
	void operator()(Parameter... CallList) {
		_commandEvent.Emit(CallList...);
	}
	/***
	 * Emit the command
	 * @param CallList The call parameter list
	 */
	void OnCommand(Parameter... CallList) {
		_commandEvent.Emit(CallList...);
	}

private:
	Delegate _commandEvent;
};