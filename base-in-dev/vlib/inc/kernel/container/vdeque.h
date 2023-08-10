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

#pragma once

#include <kernel/container/viterator.h>
#include <kernel/mem/vmemory.h>

template <class Type>
struct VDequeNode {
	VDequeNode() = default;

	Type Value;
};

template <class NodeType, class AllocatorType>
class VDequeArray {
public:
	using Node	   = VDequeNode<NodeType>;
	using NodePtr  = VDequeNode<NodeType> *;
	using DiffType = size_t;

public:
	VDequeArray(const size_t &InitSize, AllocatorType &ArrayAllocator) noexcept
		: Allocator(ArrayAllocator), ExpandSize(InitSize) {
		Array = Allocator.template AllocateArray<VDequeNode<NodeType>>(InitSize);
		Head  = Array + InitSize / 2;
		Tail  = Head;
	}

	constexpr bool Empty() {
		return Head == Tail;
	}
	void HeadInsert(const NodeType &Value) {
		_CheckFrontField();

		Head->Value = Value;
		Head		= Head - 1;
	}
	void HeadInsert(NodeType &&Value) {
		_CheckFrontField();

		Head->Value = Value;
		Head		= Head - 1;
	}
	void TailInsert(const NodeType &Value) {
		_CheckBackField();

		Tail->Value = Value;
		Tail		= Tail + 1;
	}
	void TailInsert(NodeType &&Value) {
		_CheckBackField();

		Tail		= Tail + 1;
		Tail->Value = Value;
	}
	void HeadDelete() {
		if (Head != Tail) {
			Head = Head + 1;
			Head->Value.~NodeType();
		}
	}
	void TailDelete() {
		if (Tail != Array + ExpandSize) {
			Tail->Value.~NodeType();
			Tail = Tail - 1;
		}
	}
	constexpr size_t Size() {
		return Tail - Head;
	}

private:
	void _ExpandArray() {
		NodePtr	 NewArea	  = Allocator.template AllocateArray<VDequeNode<NodeType>>(ExpandSize * 2);
		DiffType PtrDiff	  = Tail - Head;
		DiffType HeadPosition = ExpandSize - (PtrDiff / 2);
		for (size_t Count = 0; Count < PtrDiff; ++Count) {
			*(NewArea + Count + HeadPosition) = *(Head + Count);
		}

		Allocator.template DeleteArray<VDequeNode<NodeType>>(Array, ExpandSize);

		Array = NewArea;
		Head  = Array + HeadPosition;
		Tail  = Array + HeadPosition + PtrDiff;

		ExpandSize *= 2;
	}

	bool _ReachHead() {
		return Head == Array;
	}
	void _CheckFrontField() {
		if (_ReachHead()) {
			_ExpandArray();
		}
	}
	bool _ReachEnd() {
		return Tail == Array + ExpandSize;
	}
	void _CheckBackField() {
		if (_ReachEnd()) {
			_ExpandArray();
		}
	}

public:
	NodePtr		   Head;
	NodePtr		   Tail;
	NodePtr		   Array;
	DiffType	   ExpandSize;
	AllocatorType &Allocator;
};

template <class Type, class AllocatorType = VMemoryPool, class TypeExtractor = VTypeExtractor<Type>>
class VDeque {
public:
	explicit VDeque(AllocatorType &DequeAllocator) noexcept : Array(8192, Allocator), Allocator(DequeAllocator) {
	}
	void PushFront(Type &&Value) {
		Array.HeadInsert(std::forward<Type &&>(Value));
	}
	void PushFront(const Type &Value) {
		Array.HeadInsert(Value);
	}
	void PushBack(Type &&Value) {
		Array.TailInsert(std::forward<Type &&>(Value));
	}
	void PushBack(const Type &Value) {
		Array.TailInsert(Value);
	}
	void PopFront() {
		Array.HeadDelete();
	}
	void PopBack() {
		Array.TailDelete();
	}
	Type &GetFront() {
		return (Array.Head + 1)->Value;
	}
	Type &GetBack() {
		return Array.Tail->Value;
	}

public:
	constexpr size_t GetSize() {
		return Array.Size();
	}
	constexpr bool IsEmpty() {
		return Array.Empty();
	}

public:
	Type &operator[](const size_t &Position) {
		if (Position >= GetSize()) {
			_vdebug_handle TargetPositionOutOfRange;
			TargetPositionOutOfRange.crash("VContainer Error : Target position out of range.");
		}

		return (Array.Head + Position)->Value;
	}

private:
	AllocatorType					&Allocator;
	VDequeArray<Type, AllocatorType> Array;
};