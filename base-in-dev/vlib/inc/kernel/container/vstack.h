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
 * \file vstack.h
 * \brief This file contains the definition of the VStack class.
 */

#pragma once

#include <kernel/container/vdeque.h>

/**
 * \class VStack
 * \brief The VStack class is a simple stack data structure.
 * It is implemented using the VDeque class.
 */
template <class Type, class AllocatorType, class TypeExtractor = VTypeExtractor<Type>>
class VStack {
public:
	/**
	 * \brief Constructor taking an allocator.
	 * Initializes the underlying deque with the provided allocator.
	 *
	 * @param StackAllocator The allocator to use for the stack.
	 */
	explicit VStack(AllocatorType &StackAllocator) : NativeDeque(StackAllocator) {
	}

	/**
	 * Constructor taking an initializer list and an allocator.
	 * Initializes the underlying deque with the elements from the initializer list and the provided allocator.
	 *
	 * @param InitList The initializer list to initialize the stack with.
	 * @param StackAllocator The allocator to use for the stack.
	 */
	VStack(std::initializer_list<Type> InitList, AllocatorType &StackAllocator)
		: NativeDeque(InitList, StackAllocator) {
	}

public:
	/**
	 * \brief Pushes a value to the top of the stack.
	 *
	 * @param Value The value to push.
	 */
	void Push(const Type &Value) {
		NativeDeque.PushFront(Value);
	}

	/**
	 * \brief Pushes a value to the top of the stack.
	 * This is the move version, which is more efficient for large objects.
	 *
	 * @param Value The value to push.
	 */
	void Push(Type &&Value) {
		NativeDeque.PushFront(std::move(Value));
	}

	/**
	 * \brief Pops a value from the top of the stack.
	 */
	void Pop() {
		NativeDeque.PopFront();
	}

	/**
	 * \brief Returns a reference to the top of the stack.
	 *
	 * @return A reference to the top of the stack.
	 */
	Type &GetFront() {
		return NativeDeque.GetFront();
	}

	/**
	 * \brief Returns a reference to the bottom of the stack.
	 *
	 * @return A reference to the bottom of the stack.
	 */
	Type &GetBack() {
		return NativeDeque.GetBack();
	}

	/**
	 * \brief Returns the size of the stack.
	 *
	 * @return The size of the stack.
	 */
	const size_t &GetSize() {
		return NativeDeque.GetSize();
	}

private:
	VDeque<Type, AllocatorType, TypeExtractor> NativeDeque; ///< The underlying deque for the stack.
};