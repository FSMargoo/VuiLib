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
 * \file vBindingType.h
 * \brief A wrapper type for two-way binding
 */

#pragma once

#include <compare>

/**
 * This is a class for two-way binding, the control define property with
 * this data class. This class will maintain an instance of the type and
 * a pointer referred to the type.
 * @tparam Type The data type
 * @tparam Parameter The constructor parameter for the data type
 */
template <class Type, class... Parameter>
class VBindingType {
public:
	/**
	 * To call this constructor, a default constructor of the data type is required
	 */
	VBindingType() : _pointer(nullptr), _instance() {
	}
	explicit VBindingType(Parameter... Construct) : _pointer(nullptr), _instance(Construct...) {
	}

public:
	/**
	 * Bind a pointer, VBindingType assumes that the pointer is always available.
	 * @param Pointer The pointer to be bound.
	 */
	void Bind(Type *Pointer) {
		_pointer = Pointer;
	}
	/**
	 * Unbind, discard the pointer. Binding object will not copy the value into the
	 * cache of binding object.
	 */
	void Unbind() {
		_pointer = nullptr;
	}

public:
	/**
	 * Spaceship operator
	 * @param Value The value for judgement
	 */
	std::strong_ordering  friend operator<=>(const VBindingType& Object, const Type& Value) {
		if (Object._pointer != nullptr) {
			return *(Object._pointer) <=> Value;
		}

		return Object._instance <=> Value;
	}
	/**
	 * Spaceship operator
	 * @param Value The value for judgement
	 */
	std::strong_ordering  friend operator<=>(const Type& Value, const VBindingType& Object) {
		if (Object._pointer != nullptr) {
			return Value <=> *(Object._pointer);
		}

		return Value <=> Object._instance;
	}
	bool operator==(const Type& Value) {
		if (_pointer != nullptr) {
			return Value == *_pointer;
		}

		return Value == _instance;
	}

public:
	/**
	 * Get the pointer of instance
	 * @return The pointer of data type
	 */
	Type *Pointer() noexcept {
		/*
		 * If the _pointer != nullptr, return the pointer.
		 * However, nobody can sure whether the _pointer object
		 * has been released or not.
		 *
		 * TODO: Add a VMemoryLogger to help the memory manage.
		 */
		if (_pointer != nullptr) {
			return _pointer;
		} else {
			return &_instance;
		}
	}
	/**
	 * Get the reference of instance
	 * @return The reference of data type
	 */
	Type &operator*() noexcept {
		/*
		 * If the _pointer != nullptr, return the pointer.
		 * However, nobody can sure whether the _pointer object
		 * has been released or not.
		 *
		 * TODO: Add a VMemoryLogger to help the memory manage.
		 */
		if (_pointer != nullptr) {
			return *_pointer;
		} else {
			return _instance;
		}
	}

private:
	Type *_pointer;
	Type  _instance;
};
