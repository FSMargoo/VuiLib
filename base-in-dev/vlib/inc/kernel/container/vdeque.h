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
 *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file vdeque.h
 * \brief This file contains the implementation of the VDeque data structure.
 */

#pragma once

#include <kernel/container/viterator.h>
#include <kernel/mem/vmemory.h>

/**
 * \struct VDequeNode
 * \brief A Node of deque
 */
template <class Type>
struct VDequeNode {
	VDequeNode() = default;

	Type Value; ///< The value stored in the node
};

/**
 * \class VDequeArray
 * \brief The VDequeArray class is a helper class for the VDeque data structure.
 * It provides the underlying array structure for the VDeque.
 * It includes methods for insertion and deletion at both ends of the array,
 * as well as methods for expanding the array when necessary.
 */
template <class NodeType, class AllocatorType>
class VDequeArray {
public:
	/* Only for type alias */
	using Node	   = VDequeNode<NodeType>;
	using NodePtr  = VDequeNode<NodeType> *;
	using DiffType = size_t;

public:
	/* Array can not be copied */
	VDequeArray(const VDequeArray &Object) = delete;

	/**
	 * Move constructor for VDequeArray.
	 * It takes ownership of the input object's resources, leaving it in a safe, empty state.
	 *
	 * @param Object The VDequeArray object to move from.
	 */
	VDequeArray(VDequeArray &&Object) noexcept {
		Head	   = Object.Head;
		Tail	   = Object.Tail;
		Array	   = Object.Array;
		ExpandSize = Object.ExpandSize;
		Allocator  = Object.Allocator;

		Object.Head	 = nullptr;
		Object.Tail	 = nullptr;
		Object.Array = nullptr;
		Object.Array = nullptr;
	}
	/**
	 * \brief Constructor that accepts an initial size and an allocator.
	 *
	 * This constructor creates a deque array with the provided initial size and allocator.
	 * The allocator is used for memory management in the deque array.
	 *
	 * @param InitSize The initial size of the deque array.
	 * @param ArrayAllocator The allocator to use for memory management in the deque array.
	 */
	VDequeArray(const size_t &InitSize, AllocatorType &ArrayAllocator) noexcept
		: Allocator(ArrayAllocator), ExpandSize(InitSize) {
		Array = Allocator.template AllocateArray<VDequeNode<NodeType>>(InitSize);
		Head  = Array + InitSize / 2;
		Tail  = Head;
	}

	/**
	 * \brief Check if the array is empty
	 * @return true if the array is empty, false otherwise
	 */
	constexpr bool Empty() {
		return Head == Tail;
	}

	/**
	 * \brief Insert a value at the head of the array
	 * @param Value The value to insert
	 */
	void HeadInsert(const NodeType &Value) {
		_CheckFrontField();

		Head->Value = Value;
		Head		= Head - 1;
	}

	/**
	 * \brief Insert a value at the head of the array
	 * @param Value The value to insert
	 */
	void HeadInsert(NodeType &&Value) {
		_CheckFrontField();

		Head->Value = Value;
		Head		= Head - 1;
	}

	/**
	 * \brief Insert a value at the tail of the array
	 * @param Value The value to insert
	 */
	void TailInsert(const NodeType &Value) {
		_CheckBackField();

		Tail->Value = Value;
		Tail		= Tail + 1;
	}

	/**
	 * \brief Insert a value at the tail of the array
	 * @param Value The value to insert
	 */
	void TailInsert(NodeType &&Value) {
		_CheckBackField();

		Tail		= Tail + 1;
		Tail->Value = Value;
	}

	/**
	 * \brief Delete the value at the head of the array
	 */
	void HeadDelete() {
		if (Head != Tail) {
			Head = Head + 1;
			Head->Value.~NodeType();
		}
	}

	/**
	 * \brief Delete the value at the tail of the array
	 */
	void TailDelete() {
		if (Tail != Array + ExpandSize) {
			Tail->Value.~NodeType();
			Tail = Tail - 1;
		}
	}

	/**
	 * \brief Get the size of the array
	 * @return The size of the array
	 */
	constexpr size_t Size() {
		return Tail - Head;
	}

	/**
	 * \brief Swap the contents of this array with another
	 * @param Object The other array to swap with
	 */
	void Swap(VDequeArray &Object) {
		NodePtr		   CHead	   = Object.Head;
		NodePtr		   CTail	   = Object.Tail;
		NodePtr		   CArray	   = Object.Array;
		AllocatorType &CAllocator  = Object.Allocator;
		DiffType	   CExpandSize = Object.ExpandSize;

		Object.Head		  = Head;
		Object.Tail		  = Tail;
		Object.Array	  = Array;
		Object.Allocator  = Allocator;
		Object.ExpandSize = ExpandSize;

		Head	   = CHead;
		Tail	   = CTail;
		Array	   = CArray;
		Allocator  = CAllocator;
		ExpandSize = CExpandSize;
	}

private:
	/**
	 * \brief Expands the array when necessary.
	 *
	 * This function is called when the array needs to be expanded. It creates a new array with double the size,
	 * copies the existing elements into the new array, and then deletes the old array. The head and tail pointers
	 * are updated to point to the correct positions in the new array.
	 */
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

	/**
	 * \brief Checks if the head of the array has reached the beginning of the array.
	 *
	 * This function is used to determine if the head of the array has reached the beginning of the array.
	 * It is used in conjunction with the _CheckFrontField function to determine if the array needs to be expanded.
	 *
	 * @return true if the head of the array has reached the beginning of the array, false otherwise.
	 */
	bool _ReachHead() {
		return Head == Array;
	}
	/**
	 * \brief Checks and expands the array if the head has reached the beginning.
	 *
	 * This function is used to check if the head of the array has reached the beginning of the array.
	 * If it has, the _ExpandArray function is called to expand the array.
	 */
	void _CheckFrontField() {
		if (_ReachHead()) {
			_ExpandArray();
		}
	}
	/**
	 * \brief Checks if the tail of the array has reached the end of the array.
	 *
	 * This function is used to determine if the tail of the array has reached the end of the array.
	 * It is used in conjunction with the _CheckBackField function to determine if the array needs to be expanded.
	 *
	 * @return true if the tail of the array has reached the end of the array, false otherwise.
	 */
	bool _ReachEnd() {
		return Tail == Array + ExpandSize;
	}
	/**
	 * \brief Checks and expands the array if the tail has reached the end.
	 *
	 * This function is used to check if the tail of the array has reached the end of the array.
	 * If it has, the _ExpandArray function is called to expand the array.
	 */
	void _CheckBackField() {
		if (_ReachEnd()) {
			_ExpandArray();
		}
	}

public:
	NodePtr		   Head;	   ///< Pointer to the head of the array
	NodePtr		   Tail;	   ///< Pointer to the tail of the array
	NodePtr		   Array;	   ///< Pointer to the array
	DiffType	   ExpandSize; ///< The size to expand the array by when necessary
	AllocatorType &Allocator;  ///< The allocator to use for the array
};

/**
 * \class VDequeIterator
 * \brief An random iterator for the VDeque data structure.
 */
template <class Type, class AllocatorType>
class VDequeIterator : public VRandomIterator<Type> {
public:
	explicit VDequeIterator(VDequeArray<Type, AllocatorType> *ArrayPointer, VDequeNode<Type> *Pointer) noexcept
		: Ptr(ArrayPointer), IteratorPtr(Pointer) {
	}
	Type &operator++() override {
		++IteratorPtr;
		++(this->Index);

		return IteratorPtr->Value;
	}
	Type &operator++(int) override {
		VDequeNode<Type> *Temp = IteratorPtr;
		++IteratorPtr;
		++(this->Index);

		return Temp->Value;
	}
	Type &operator--() override {
		--(this->Index);

		--IteratorPtr;

		return IteratorPtr->Value;
	}
	Type &operator[](const size_t &Position) override {
		if (Position >= Ptr->ExpandSize) {
			_vdebug_handle SubscriptOutOfRange;
			SubscriptOutOfRange.crash("Subscript out of valid range.");
		}
		return (Ptr->Head + Position)->Value;
	}
	Type &operator+=(const size_t &Delta) override {
		if (Delta + IteratorPtr > Ptr->Tail) {
			_vdebug_handle SubscriptOutOfRange;
			SubscriptOutOfRange.crash("Subscript out of valid range.");
		}
		IteratorPtr += Delta;

		return IteratorPtr->Value;
	}
	Type &operator-=(const size_t &Delta) override {
		if (IteratorPtr - Delta < Ptr->Head) {
			_vdebug_handle SubscriptOutOfRange;
			SubscriptOutOfRange.crash("Subscript out of valid range.");
		}
		IteratorPtr -= Delta;

		return IteratorPtr->Value;
	}
	Type &operator*() override {
		return IteratorPtr->Value;
	}
	bool operator==(const VDequeIterator<Type, AllocatorType> &Judge) {
		return Judge.IteratorPtr == IteratorPtr;
	}
	bool operator!=(const VDequeIterator<Type, AllocatorType> &Judge) {
		return Judge.IteratorPtr != IteratorPtr;
	}

private:
	VDequeArray<Type, AllocatorType> *Ptr;		   ///< Pointer to the array
	VDequeNode<Type>				 *IteratorPtr; ///< Pointer to the current node

	template <class, class>
	friend class VDequeIterator;
};

/**
 * \class VDeque
 * \brief The VDeque class is a double-ended queue implemented with an array.
 * It provides efficient insertion and deletion at both its ends.
 */
template <class Type, class AllocatorType = VMemoryPool, class TypeExtractor = VTypeExtractor<Type>>
class VDeque {
public:
	using Iterator = VDequeIterator<Type, AllocatorType>;

public:
	/**
	 * \brief Default constructor that accepts an allocator.
	 *
	 * This constructor creates a deque with the provided allocator.
	 * The allocator is used for memory management in the deque.
	 *
	 * @param DequeAllocator The allocator to use for memory management in the deque.
	 */
	explicit VDeque(AllocatorType &DequeAllocator) noexcept : Array(8192, Allocator), Allocator(DequeAllocator) {
	}
	/**
	 * \brief Constructor that accepts an initializer list and an allocator.
	 *
	 * This constructor creates a deque with the elements in the initializer list.
	 * The allocator is used for memory management in the deque.
	 *
	 * @param InitList The initializer list to create the deque with.
	 * @param DequeAllocator The allocator to use for memory management in the deque.
	 */
	VDeque(std::initializer_list<Type> InitList, AllocatorType &DequeAllocator) noexcept
		: Array(8192, Allocator), Allocator(DequeAllocator) {
		for (auto &Value : InitList) {
			PushBack(Value);
		}
	}

public:
	/**
	 * \brief Inserts a value at the front of the deque.
	 *
	 * This function is used to insert a value at the front of the deque.
	 * It uses the HeadInsert function of the underlying array to perform the insertion.
	 *
	 * @param Value The value to insert at the front of the deque.
	 */
	void PushFront(Type &&Value) {
		Array.HeadInsert(std::forward<Type &&>(Value));
	}
	/**
	 * \brief Inserts a value at the front of the deque.
	 *
	 * This function is used to insert a value at the front of the deque.
	 * It uses the HeadInsert function of the underlying array to perform the insertion.
	 *
	 * @param Value The value to insert at the front of the deque.
	 */
	void PushFront(const Type &Value) {
		Array.HeadInsert(Value);
	}
	/**
	 * \brief Inserts a value at the back of the deque.
	 *
	 * This function is used to insert a value at the back of the deque.
	 * It uses the TailInsert function of the underlying array to perform the insertion.
	 *
	 * @param Value The value to insert at the back of the deque.
	 */
	void PushBack(Type &&Value) {
		Array.TailInsert(std::forward<Type &&>(Value));
	}
	/**
	 * \brief Inserts a value at the back of the deque.
	 *
	 * This function is used to insert a value at the back of the deque.
	 * It uses the TailInsert function of the underlying array to perform the insertion.
	 *
	 * @param Value The value to insert at the back of the deque.
	 */
	void PushBack(const Type &Value) {
		Array.TailInsert(Value);
	}
	/**
	 * \brief Removes a value from the front of the deque.
	 *
	 * This function is used to remove a value from the front of the deque.
	 * It uses the HeadDelete function of the underlying array to perform the deletion.
	 */
	void PopFront() {
		Array.HeadDelete();
	}
	/**
	 * \brief Removes a value from the back of the deque.
	 *
	 * This function is used to remove a value from the back of the deque.
	 * It uses the TailDelete function of the underlying array to perform the deletion.
	 */
	void PopBack() {
		Array.TailDelete();
	}
	/**
	 * \brief Returns a reference to the front element of the deque.
	 *
	 * This function is used to access the front element (i.e., the element that would be popped by PopFront) of the
	 * deque. It does not check whether the deque is empty before accessing the element, and calling this function on an
	 * empty deque results in undefined behavior.
	 *
	 * @return A reference to the front element of the deque.
	 */
	Type &GetFront() {
		return (Array.Head + 1)->Value;
	}
	/**
	 * \brief Returns a reference to the back element of the deque.
	 *
	 * This function is used to access the back element (i.e., the element that would be popped by PopBack) of the
	 * deque. It does not check whether the deque is empty before accessing the element, and calling this function on an
	 * empty deque results in undefined behavior.
	 *
	 * @return A reference to the back element of the deque.
	 */
	Type &GetBack() {
		return Array.Tail->Value;
	}

public:
	/**
	 * \brief Swaps the contents of this deque with another.
	 *
	 * This function is used to swap the contents of this deque with another. It swaps the underlying arrays and
	 * allocators of the two deque.
	 *
	 * @param Object The other deque to swap with.
	 */
	void Swap(const VDeque<Type, AllocatorType, TypeExtractor> &Object) {
		AllocatorType &CAllocator = Allocator;
		Array.Swap(Object.Array);
		Allocator		 = Object.Allocator;
		Object.Allocator = CAllocator;
	}

public:
	/**
	 * \brief Returns an iterator pointing to the first element in the deque.
	 *
	 * This function is used to get an iterator pointing to the first element in the deque.
	 * It uses the VDequeIterator constructor to create the iterator.
	 *
	 * @return An iterator pointing to the first element in the deque.
	 */
	Iterator Begin() {
		return VDequeIterator<Type, AllocatorType>(&Array, Array.Head);
	}
	/**
	 * \brief Returns an iterator pointing to the end of the deque.
	 *
	 * This function is used to get an iterator pointing to the end of the deque.
	 * It uses the VDequeIterator constructor to create the iterator.
	 *
	 * @return An iterator pointing to the end of the deque.
	 */
	Iterator End() {
		return Iterator(&Array, Array.Tail + 1);
	}

public:
	/**
	 * \brief Returns an iterator pointing to the first element in the deque.
	 *
	 * This function is used to get an iterator pointing to the first element in the deque.
	 * It uses the VDequeIterator constructor to create the iterator.
	 *
	 * @return An iterator pointing to the first element in the deque.
	 */
	Iterator begin() {
		return Iterator(&Array, Array.Head);
	}
	/**
	 * \brief Returns an iterator pointing to the end of the deque.
	 *
	 * This function is used to get an iterator pointing to the end of the deque.
	 * It uses the VDequeIterator constructor to create the iterator.
	 *
	 * @return An iterator pointing to the end of the deque.
	 */
	Iterator end() {
		return Iterator(&Array, Array.Tail);
	}

public:
	/**
	 * \brief Returns the size of the deque.
	 *
	 * This function is used to get the size of the deque.
	 * It uses the Size function of the underlying array to get the size.
	 *
	 * @return The size of the deque.
	 */
	constexpr size_t GetSize() {
		return Array.Size();
	}
	/**
	 * \brief Checks if the deque is empty.
	 *
	 * This function is used to check if the deque is empty.
	 * It uses the Empty function of the underlying array to perform the check.
	 *
	 * @return true if the deque is empty, false otherwise.
	 */
	constexpr bool IsEmpty() {
		return Array.Empty();
	}

public:
	/**
	 * \brief Accesses the element at a specific position in the deque.
	 *
	 * This function is used to access the element at a specific position in the deque.
	 * It uses the operator[] of the underlying array to perform the access.
	 * If the position is out of range, it will crash with a "VContainer Error : Target position out of range." message.
	 *
	 * @param Position The position of the element to access.
	 * @return A reference to the element at the specified position in the deque.
	 */
	Type &operator[](const size_t &Position) {
		if (Position >= GetSize()) {
			_vdebug_handle TargetPositionOutOfRange;
			TargetPositionOutOfRange.crash("VContainer Error : Target position out of range.");
		}

		return (Array.Head + Position)->Value;
	}

private:
	AllocatorType					&Allocator; ///< The allocator to use for the deque
	VDequeArray<Type, AllocatorType> Array;		///< The underlying array for the deque
};