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
 * \file vevent.h
 * \brief The event instance
 */

#pragma once

#include <base/LINQ/vlinq.h>
#include <base/object/vobject.h>

#include <functional>
#include <list>

namespace VBase {


/**
 * The event class like C#
 * @tparam Type The event parameter type which handler will receive
 */
template <class... Type>
class VEvent {
public:
	VEvent()  = default;
	~VEvent() = default;

public:
	/**
	 * Subscribe handler to the event
	 * @param object The handler function
	 */
	auto Subscribe(const std::function<void(Type..., VObject*)> &object) -> void {
		_registerFunction(object);
	}
	/**
	 * Unsubscribe handler to the event
	 * @param object The handler function
	 */
	auto Unsubscribe(const std::function<void(Type..., VObject*)> &object) -> void {
		_unregisterFunction(object);
	}
	/**
	 * Do event broadcasting
	 */
	auto Send(Type... parameter, VObject* sender) -> void {
		_sendEvent(parameter..., sender);
	}

private:
	inline auto _registerFunction(const std::function<void(Type..., VObject*)> &object) -> void {
		if (object != NULL) {
			_handler.push_back(object);
		}
	}
	inline auto _unregisterFunction(const std::function<void(Type..., VObject*)> &object) -> void {
		if (object != NULL) {
			_handler =
				VLINQFrom(_handler).Delete([object](const std::function<void(Type..., VObject*)> x) -> bool { return object == x; });
		}
	}
	inline auto _sendEvent(Type... parameter, VObject* sender) {
		VLINQFrom(_handler).X([parameter..., sender](std::function<void(Type..., VObject*)>& function) -> void {
			function(parameter..., sender);
		});
	}

private:
	std::list<std::function<void(Type..., VObject*)>> _handler;
};
} // namespace VBase