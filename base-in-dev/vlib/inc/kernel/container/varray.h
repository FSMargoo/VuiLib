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
 * \file varray.h
 * \brief This file include a std::vector's implementation
 */

#pragma once

#include <kernel/container/viterator.h>
#include <kernel/container/vtypeextractor.h>
#include <kernel/mem/vmemory.h>

/**
 * \brief The memory type extract
 * \tparam Type The resource type
 */
template <class Type>
struct VArrayResourceManager {
	Type  *Pointer;
	Type  *Cursor;
	size_t Size;
};

/**
 * \brief The iterator of VArray class
 * \tparam Type The value type
 */
template <class Type>
class VArrayIterator : public VRandomIterator<Type> {
public:
	using Pointer  = Type *;
	using Iterator = VArrayIterator<Type>;

public:
	explicit VArrayIterator(Pointer IteratorPointer, Pointer Start, Pointer End, const size_t &ContainerSize) noexcept {
		Ptr	  = IteratorPointer;
		Head  = Start;
		Tail  = End;
		Index = 0;
		Size  = ContainerSize;
	}
	Type &operator++() override {
		++Ptr;
		++(this->Index);

		return *Ptr;
	}
	Type &operator++(int) override {
		Pointer Temp = Ptr;
		++Ptr;
		++(this->Index);

		return *Temp;
	}
	Type &operator--() override {
		--(this->Index);

		--Ptr;

		return *Ptr;
	}
	Type &operator[](const size_t &Position) override {
		if (Position >= Size) {
			_vdebug_handle SubscriptOutOfRange;
			SubscriptOutOfRange.crash("Subscript out of valid range.");
		}
		return *(Head + Position);
	}
	Type &operator+=(const size_t &Delta) override {
		if (Delta + Ptr > Tail) {
			_vdebug_handle SubscriptOutOfRange;
			SubscriptOutOfRange.crash("Subscript out of valid range.");
		}
		Ptr += Delta;

		return *Ptr;
	}
	Type &operator-=(const size_t &Delta) override {
		if (Ptr - Delta < Head) {
			_vdebug_handle SubscriptOutOfRange;
			SubscriptOutOfRange.crash("Subscript out of valid range.");
		}
		Ptr -= Delta;

		return *Ptr;
	}
	Type &operator*() override {
		return *Ptr;
	}
	bool operator==(const Iterator &Judge) {
		return Judge.Ptr == Ptr;
	}
	bool operator!=(const Iterator &Judge) {
		return Judge.Ptr != Ptr;
	}

private:
	Pointer Head;
	Pointer Ptr;
	Pointer Tail;
	size_t	Size;
	size_t	Index;
};

/**
 * \brief VArray provide a dynamic array and a sets of memory operation
 * \tparam Type The type of dynamic array
 * \tparam AllocatorType The allocator type
 * \tparam TypeExtractor The type extractor type
 */
template <class Type, class AllocatorType = VMemoryPool, class TypeExtractor = VTypeExtractor<Type>>
class VArray {
public:
	using ArrayType	 = VArray<Type, AllocatorType, TypeExtractor>;
	using Pointer	 = TypeExtractor::Pointer;
	using ConstRefer = TypeExtractor::ConstRefer;
	using CopyRefer	 = TypeExtractor::CopyRefer;
	using Iterator	 = VArrayIterator<Type>;

public:
	explicit VArray(AllocatorType &ArrayAllocator) noexcept : Allocator(ArrayAllocator) {
		_InitMemory();
	}
	VArray(std::initializer_list<Type> InitList, AllocatorType &ArrayAllocator) noexcept : Allocator(ArrayAllocator) {
		_InitMemory();
		for (auto &Element : InitList) {
			Push(Element);
		}
	}
	VArray(const ArrayType &Object) noexcept : Allocator(Object.Allocator) {
		_CopyConstructor(std::forward<ArrayType &>(Object));
	}
	VArray(ArrayType &&Object) noexcept : Allocator(Object.Allocator) {
		_MoveConstructor(std::forward<ArrayType &&>(Object));
	}

public:
	ArrayType &operator=(const ArrayType &Object) noexcept {
		_CheckAllocator(Object);
		_CopyConstructor(std::forward<ArrayType &>(Object));

		return *this;
	}
	ArrayType &operator=(ArrayType &&Object) noexcept {
		_CheckAllocator(Object);
		_MoveConstructor(std::forward<ArrayType &&>(Object));

		return *this;
	}

public:
	/**
	 * \brief Get the begging of iterator
	 */
	VArrayIterator<Type> Begin() {
		return VArrayIterator<Type>(Memory.Pointer, Memory.Pointer, Memory.Cursor, Memory.Size);
	}
	/**
	 * \brief Get the end of iterator
	 */
	VArrayIterator<Type> End() {
		return VArrayIterator<Type>(Memory.Cursor, Memory.Pointer, Memory.Cursor, Memory.Size);
	}

	/* For C++ foreach */
public:
	VArrayIterator<Type> begin() {
		return Begin();
	}
	VArrayIterator<Type> end() {
		return End();
	}

private:
	/**
	 * \brief Check the allocator between two array is same or not
	 */
	void _CheckAllocator(const ArrayType &Object) {
		if (&(Object.Allocator) != &Allocator) {
			_vdebug_handle().crash("The equal operation between two array should happen in same allocator!");
		}
	}
	/**
	 * \brief The copy constructor function's wrap
	 */
	void _CopyConstructor(const ArrayType &Object) {
		size_t Size = Object.GetSize();
		if (Memory.Size < Size) {
			Preset(Object.Memory.Size);
		}
		for (size_t Count = 0; Count < Size; ++Count) {
			Push(*(Object.Memory.Pointer + Count));
		}
	}
	/**
	 * \brief The move constructor function's wrap
	 */
	void _MoveConstructor(ArrayType &&Object) {
		Memory.Size	   = Object.Memory.Size;
		Memory.Pointer = Object.Memory.Pointer;
		Memory.Cursor  = Object.Memory.Cursor;

		Object.Memory.Size	  = 0;
		Object.Memory.Pointer = nullptr;
		Object.Memory.Cursor  = nullptr;
	}

public:
	/**
	 * \brief Push value from back
	 * \param Value The value that need to be pushed
	 */
	void Push(const ConstRefer Value) {
		_InsertFromBack(Value);
	}
	/**
	 * \brief Push value from back
	 * \param Value The value that need to be pushed
	 */
	void Push(CopyRefer Value) {
		_InsertFromBack(std::move(Value));
	}
	/**
	 * \brief Push init list from back
	 */
	void Push(std::initializer_list<Type> InitList) {
		for (auto &Element : InitList) {
			Push(Element);
		}
	}
	/**
	 * \brief Insert an element at specified position
	 */
	void Insert(const ConstRefer Value, const size_t &Where) {
		_InsertByPosition(Value, Where);
	}
	/**
	 * \brief Insert an element at specified position
	 */
	void Insert(CopyRefer Value, const size_t &Where) {
		_InsertByPosition(Value, Where);
	}
	/**
	 * \brief Insert init list at specified position
	 */
	void Insert(std::initializer_list<Type> InitList, const size_t &Where) {
		size_t Count = 0;
		for (auto &Element : InitList) {
			Insert(Element, Where + Count);
			++Count;
		}
	}

public:
	/**
	 * \brief Convert the array to init list
	 * \return
	 */
	std::initializer_list<Type> ToInitList() {
		return std::initializer_list<Type>(Memory.Pointer, Memory.Cursor);
	}

public:
	/**
	 * \brief Delete a element
	 * \param Where
	 */
	void Delete(const size_t &Where) {
		if (_GetSize() <= Where) {
			_vdebug_handle().crash("Invalid delete subscript!");
		}

		_EraseElement(Where);
	}
	void Delete(const VRange &Range) {
		if (!Range.ValidRange()) {
			_vdebug_handle().crash("Invalid range was specified!");
		}

		for (size_t Count = Range.Start; Count < Range.End; ++Count) {
			_EraseElement(Range.Start);
		}
	}

public:
	/**
	 * \brief Provide a function to do some operations of element (Do not operate the array object in the function)
	 */
	void Range(const std::function<void(ConstRefer)> &Function) {
		for (size_t Count = 0; Count < _GetSize(); ++Count) {
			Function(*(Memory.Pointer + Count));
		}
	}
	/**
	 * \brief Change the element value by the map of some specified value
	 * \param PairList
	 */
	void Map(std::initializer_list<VMapPair<Type, Type>> PairList) {
		size_t Size = _GetSize();
		for (size_t Count = 0; Count < Size; ++Count) {
			for (auto &Pair : PairList) {
				if (*(Memory.Pointer + Count) == Pair.OriginKey) {
					*(Memory.Pointer + Count) = Pair.MapValue;

					break;
				}
			}
		}
	}
	/**
	 * \brief Discard the specified value from the array
	 * \return A new array that is modified
	 */
	VArray<Type> Filter(std::initializer_list<Type> Value) {
		VArray<Type> Result(Allocator);
		size_t		 Size = _GetSize();
		for (size_t Count = 0; Count < Size; ++Count) {
			bool	   Flag		  = false;
			ConstRefer LocalValue = At(Count);
			for (auto &Element : Value) {
				if (LocalValue == Element) {
					Flag = true;
					break;
				}
			}

			if (!Flag) {
				Result.Push(LocalValue);
			}
		}

		return Result;
	}
	/**
	 * \brief Erase the end element of array
	 */
	void Pop() {
		Delete(_GetSize());
	}
	/**
	 * \brief Split an array from specified range
	 */
	ArrayType Split(VRange Range) {
		ArrayType Array(Allocator);
		if (!Range.ValidRange() || Range.Start >= _GetSize()) {
			_vdebug_handle().crash("Invalid split range");
		}
		for (size_t Count = Range.Start; Count <= Range.End; ++Count) {
			Array.Push(At(Count));
		}

		return Array;
	}

public:
	/**
	 * \brief Preset the memory to avoid the allocation of memory, if the size less than
	 * 			the origin size, it will discard old data
	 * \param Size The specified memory size
	 */
	void Preset(const size_t &Size) {
		if (Size > Memory.Size) {
			_ReallocateMemory(Size);
		} else {
			size_t ArraySize = _GetSize();
			Type  *OldMemory = Memory.Pointer;
			Type  *NewMemory = Allocator.template AllocateArray<Type>(Size);

			_MoveData(OldMemory, NewMemory, Memory.Size);

			Memory.Pointer = NewMemory;
			Memory.Cursor  = NewMemory + ArraySize;
			Memory.Size	   = Size;
		}
	}

public:
	/**
	 * \brief Return the element on specified position
	 * \param Position
	 * \return
	 */
	ConstRefer At(const size_t &Position) {
		if (Position >= _GetSize()) {
			_vdebug_handle().crash("Subscript out of range!");
		}

		return *(Memory.Pointer + Position);
	}
	/**
	 * \brief Get the size of the array
	 * \return
	 */
	[[nodiscard]] size_t GetSize() const {
		return _GetSize();
	}
	ConstRefer operator[](const size_t &Position) {
		return At(Position);
	}

private:
	/**
	 * \brief Init the memory resource manager
	 */
	void _InitMemory() {
		Memory.Size	   = 8192;
		Memory.Pointer = Allocator.template AllocateArray<Type>(Memory.Size);
		Memory.Cursor  = Memory.Pointer;
	}
	/**
	 * \brief Reallocate a memory
	 */
	void _ReallocateMemory(const size_t &NewSize) {
		size_t ArraySize = _GetSize();
		Type  *OldMemory = Memory.Pointer;
		Type  *NewMemory = Allocator.template AllocateArray<Type>(NewSize);

		_MoveData(OldMemory, NewMemory, Memory.Size);

		Allocator.template DeleteArray<Type>(OldMemory, Memory.Size);

		Memory.Pointer = NewMemory;
		Memory.Cursor  = NewMemory + ArraySize;
		Memory.Size	   = NewSize;
	}
	/**
	 * \brief Move the array data
	 */
	void _MoveData(Pointer Old, Pointer New, const size_t &Size) {
		for (size_t Count = 0; Count < Size; ++Count) {
			*(New + Count) = std::move(*(Old + Count));
		}
	}
	/**
	 * \brief Delete element at specified position
	 */
	void _EraseElement(const size_t &Where) {
		Type  *Element	= Memory.Pointer + Where;
		size_t RealSize = _GetSize() - 1;
		Element->~Type();
		for (size_t Count = Where; Count < RealSize; ++Count) {
			*(Memory.Pointer + Count) = std::move(*(Memory.Pointer + Count + 1));
		}
		--Memory.Cursor;
	}

private:
	/**
	 * \brief Insert element from back
	 */
	template <class ValueType>
	void _InsertFromBack(ValueType &&Value) {
		if (_GetSize() >= Memory.Size) {
			_ReallocateMemory(Memory.Size * 2);
		}
		*(Memory.Cursor) = std::move(Value);

		++Memory.Cursor;
	}
	/**
	 * \brief Insert element at specified position
	 */
	template <class ValueType>
	void _InsertByPosition(ValueType &&Value, const size_t &Position) {
		size_t Size = _GetSize();
		if (Size + 1 >= Memory.Size) {
			_ReallocateMemory(Memory.Size * 2);
		}
		if (Position > Size + 1) {
			_vdebug_handle().crash("Invalid insert subscript");
		}

		Pointer Cursor = Memory.Cursor;
		for (; Size > Position; --Size, --Cursor) {
			*(Cursor) = std::move(*(Cursor - 1));
		}
		*(Cursor) = std::move(Value);

		++Memory.Cursor;
	}
	/**
	 * \brief Count the size of the array
	 */
	[[nodiscard]] size_t _GetSize() const {
		return Memory.Cursor - Memory.Pointer;
	}

private:
	VArrayResourceManager<Type> Memory;
	AllocatorType			   &Allocator;
};