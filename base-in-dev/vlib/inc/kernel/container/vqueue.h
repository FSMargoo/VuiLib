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
 * \file vqueue.h
 * \brief This file contains the definition of the VQueue class.
 */

#pragma once

#include <kernel/container/vdeque.h>

/**
 * \class VQueue
 * \brief The VQueue class is a simple queue data structure.
 * It is implemented using the VDeque class.
 */
template <class Type, class AllocatorType, class TypeExtractor = VTypeExtractor<Type>>
class VQueue {
public:
	/**
	 * Constructor taking an allocator.
	 * Initializes the underlying deque with the provided allocator.
	 *
	 * @param QueueAllocator The allocator to use for the queue.
	 */
	explicit VQueue(AllocatorType &QueueAllocator) : NativeDeque(QueueAllocator) {
	}

	/**
	 * Constructor taking an initializer list and an allocator.
	 * Initializes the underlying deque with the elements from the initializer list and the provided allocator.
	 *
	 * @param InitList The initializer list to initialize the queue with.
	 * @param QueueAllocator The allocator to use for the queue.
	 */
	VQueue(std::initializer_list<Type> InitList, AllocatorType &QueueAllocator)
		: NativeDeque(InitList, QueueAllocator) {
	}

public:
	/**
	 * Pushes a value to the front of the queue.
	 *
	 * @param Value The value to push.
	 */
	void Push(const Type &Value) {
		NativeDeque.PushFront(Value);
	}

	/**
	 * Pushes a value to the front of the queue.
	 * This is the move version, which is more efficient for large objects.
	 *
	 * @param Value The value to push.
	 */
	void Push(Type &&Value) {
		NativeDeque.PushFront(std::move(Value));
	}

	/**
	 * Pops a value from the back of the queue.
	 */
	void Pop() {
		NativeDeque.PopBack();
	}

	/**
	 * Returns a reference to the front of the queue.
	 *
	 * @return A reference to the front of the queue.
	 */
	Type &GetFront() {
		return NativeDeque.GetFront();
	}

	/**
	 * Returns a reference to the back of the queue.
	 *
	 * @return A reference to the back of the queue.
	 */
	Type &GetBack() {
		return NativeDeque.GetBack();
	}

	/**
	 * Returns the size of the queue.
	 *
	 * @return The size of the queue.
	 */
	const size_t &GetSize() {
		return NativeDeque.GetSize();
	}

private:
	VDeque<Type, AllocatorType, TypeExtractor> NativeDeque; ///< The underlying deque for the queue.
};