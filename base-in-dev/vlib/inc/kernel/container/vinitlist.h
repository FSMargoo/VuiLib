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
 * \file vinitlist.h
 * \brief This file contains the definition of the VInitList class.
 */

#pragma once

/**
 * \class VInitList
 * \brief The VInitList class is a simple wrapper around a pair of pointers, representing a range of elements.
 * It is used to provide range-based for loop support for initializer lists.
 */
template <class Type>
class VInitList {
public:
	/**
	 * Default constructor.
	 * Initializes the pointers to nullptr.
	 */
	VInitList() noexcept : First(nullptr), Tail(nullptr) {
	}

	/**
	 * Constructor taking a pair of pointers.
	 * Initializes the pointers to the provided values.
	 *
	 * @param FirstPtr Pointer to the first element in the range.
	 * @param EndPtr Pointer to one past the last element in the range.
	 */
	VInitList(const Type *FirstPtr, const Type *EndPtr) : First(FirstPtr), Tail(EndPtr) {
	}

public:
	/**
	 * Returns a pointer to the first element in the range.
	 *
	 * @return Pointer to the first element.
	 */
	constexpr const Type *Begin() noexcept {
		return First;
	}

	/**
	 * Returns a pointer to one past the last element in the range.
	 *
	 * @return Pointer to one past the last element.
	 */
	constexpr const Type *End() noexcept {
		return Tail;
	}

	/**
	 * Returns the number of elements in the range.
	 *
	 * @return Number of elements in the range.
	 */
	constexpr size_t Size() noexcept {
		return Tail - First;
	}

public:
	/**
	 * Returns a pointer to the first element in the range.
	 * This method is provided for compatibility with the C++ Standard Library.
	 *
	 * @return Pointer to the first element.
	 */
	constexpr const Type *begin() noexcept {
		return First;
	}

	/**
	 * Returns a pointer to one past the last element in the range.
	 * This method is provided for compatibility with the C++ Standard Library.
	 *
	 * @return Pointer to one past the last element.
	 */
	constexpr const Type *end() noexcept {
		return Tail;
	}

private:
	const Type *First; ///< Pointer to the first element in the range.
	const Type *Tail;  ///< Pointer to one past the last element in the range.
};
