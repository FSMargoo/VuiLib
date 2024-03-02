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
 * \file vlinq.h
 * \brief A simple LINQ library
 */

#pragma once

#include <functional>
#include <type_traits>
#include <valarray>

namespace VBase {

/**
 * Concept for judge iterable object range
 * @tparam Begin
 * @tparam End
 */
template <class Begin, class End>
concept VCIterableRange = requires(Begin _Begin, End _End) {
	{ ++_Begin };
	{ *_Begin };
	requires !std::is_void_v<decltype(*_Begin)>;
	{ _Begin != _End };
};
/**
 * Concept for iterable object judgement
 * @tparam Type Object to be judged
 */
template <class Type>
concept VCIterable = std::is_array_v<Type> || requires(Type Value) {
	{ Value.begin() };
	{ Value.end() };
	requires VCIterableRange<decltype(Value.begin()), decltype(Value.end())>;
} || requires(Type Value) {
	{ std::begin(Value) };
	{ std::end(Value) };
	requires VCIterableRange<decltype(std::begin(Value)), decltype(std::end(Value))>;
};

/**
 * The chain call object by from function
 * @tparam Type The container type
 */
template <VCIterable Type>
class VLINQObject final {
public:
	explicit VLINQObject(Type &value) : _object(value) {
	}

public:
	/**
	 * Filter the specified element with lambda function
	 * @param lambda Lambda function
	 * @return The new LINQ object
	 */
	auto Select(const std::function<bool(typename Type::value_type)> &lambda) -> VLINQObject<Type> {
		return _cycleFetch(lambda);
	}
	/**
	 * Delete element from set with lambda function
	 * @param lambda Lambda function
	 * @return The new LINQ object
	 */
	auto Delete(const std::function<bool(typename Type::value_type)> &lambda) -> VLINQObject<Type> {
		return _cycleFetch([lambda](typename Type::value_type &x) -> bool { return !lambda(x); });
	}
	/**
	 * Delete intersect
	 * @param Object The data source object
	 */
	auto Intersect(const Type &object) -> VLINQObject<Type> {
		return _cycleFetch([object](typename Type::value_type &x) -> bool {
			for (auto &Instance : object) {
				if (Instance == x) {
					return false;
				}
			}

			return true;
		});
	}
	/**
	 * Apply operation on each element
	 * @param lambda Lambda function
	 * @return The new LINQ object
	 */
	auto X(const std::function<void(typename Type::value_type &)> &lambda) -> VLINQObject<Type> {
		return _cycleFetch([lambda](typename Type::value_type &x) -> bool {
			lambda(x);

			return true;
		});
	}
	/**
	 * Get the object
	 * @return The object
	 */
	auto Get() -> Type & {
		return _object;
	}

private:
	/**
	 * All of the LINQ operation's base function
	 * @param lambda Lambda function
	 * @return I dont care, it is priavte function
	 */
	inline auto _cycleFetch(const std::function<bool(typename Type::value_type &)> &lambda) -> VLINQObject<Type> {
		auto Result = Type();
		for (auto &Value : _object) {
			if (lambda(Value)) {
				Result.push_back(Value);
			}
		}

		return VLINQObject<Type>(Result);
	}

private:
	Type _object;
};

/**
 * Create a VLINQObject
 * @tparam Type The target object type
 * @return A LINQ Object
 */
template <VCIterable Type>
auto VLINQFrom(Type &Value) -> VLINQObject<Type> {
	return VLINQObject<Type>(Value);
}

} // namespace VBase