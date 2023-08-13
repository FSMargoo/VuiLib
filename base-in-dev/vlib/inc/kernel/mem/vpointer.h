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
 * \file vpointer.h
 * \brief The smart pointer def
 */

#pragma once

/**
 * \brief A deleter class for VUniquePointer.
 *
 * \tparam PointerType The type of the pointer to be deleted.
 * \tparam MemoryPool The type of the memory pool used for deallocation.
 */
template <class PointerType, class MemoryPool>
class VPointerDeleter {
public:
	using Pointer = typename PointerType;
	using Null	  = typename decltype(nullptr);

public:
	/**
	 * \brief Default constructor for VPointerDeleter.
	 */
	constexpr VPointerDeleter() = default;

	/**
	 * \brief Deletes a single object pointed to by the given pointer.
	 *
	 * \param Pointer The pointer to the object to be deleted.
	 * \param Pool The memory pool used for deallocation.
	 */
	virtual void Delete(PointerType *Pointer, MemoryPool &Pool) {
		Pointer->~PointerType();
		Pool.Delete(Pointer);
	}

	/**
	 * \brief Deletes an array of objects pointed to by the given pointer.
	 *
	 * \param Pointer The pointer to the array of objects to be deleted.
	 * \param Pool The memory pool used for deallocation.
	 * \param Size The size of the array.
	 */
	virtual void DeleteArray(PointerType *Pointer, MemoryPool &Pool, const size_t &Size) {
		for (size_t Count = 0; Count < Size; ++Count) {
			(Pointer[Count]).~PointerType();
		}

		Pool.DeleteArray(Pointer, Size);
	}
};

/**
 * \brief A smart pointer class that manages the lifetime of an object.
 *
 * \tparam Type The type of the managed object.
 * \tparam MemoryPool The type of the memory pool used for allocation and deallocation.
 * \tparam DeleterType The type of the deleter used for object destruction.
 */
template <class Type, class MemoryPool, class DeleterType = VPointerDeleter<Type, MemoryPool>>
class VUniquePointer : public DeleterType {
public:
	using Object   = typename VUniquePointer<Type, MemoryPool, DeleterType>;
	using NullType = typename DeleterType::Null;

public:
	/**
	 * \brief Constructs a VUniquePointer from an rvalue reference to a pointer.
	 *
	 * \param Pointer The rvalue reference to the pointer.
	 * \param PointerPool The memory pool used for deallocation.
	 */
	explicit VUniquePointer(Type *&&Pointer, MemoryPool &PointerPool) noexcept : Pool(PointerPool), Ptr(Pointer) {
	}

	/**
	 * \brief Constructs a VUniquePointer from a null value.
	 *
	 * \param NullType The null value.
	 */
	explicit VUniquePointer(NullType) noexcept : Ptr(nullptr) {
	}

	/**
	 * \brief Constructs a VUniquePointer by moving the ownership from another VUniquePointer.
	 *
	 * \param Object The other VUniquePointer to move the ownership from.
	 */
	VUniquePointer(Object &&Object) noexcept : Pool(Object.Pool), Ptr(Object._ReleaseOwner()) {
	}

	/**
	 * \brief Deleted copy constructor.
	 *
	 * \param Pointer The pointer to be copied.
	 */
	VUniquePointer(const Type *Pointer) = delete;

	/**
	 * \brief Deleted copy constructor.
	 *
	 * \param Object The other VUniquePointer to be copied.
	 */
	VUniquePointer(const Object &Object) = delete;

	/**
	 * \brief Assignment operator for assigning a null value to the VUniquePointer.
	 *
	 * \param NullType The null value.
	 * \return Object& The reference to the VUniquePointer.
	 */
	Object &operator=(NullType) noexcept {
		Reset();

		return *this;
	}

	/**
	 * \brief Destructor for VUniquePointer.
	 */
	~VUniquePointer() noexcept {
		if (Ptr != nullptr) {
			DeleterType::Delete(Ptr, Pool);
		}
	}

public:
	/**
	 * \brief Checks if the VUniquePointer is empty.
	 *
	 * \return bool True if the VUniquePointer is empty, false otherwise.
	 */
	constexpr bool Empty() {
		return _EmptyPtr();
	}

	/**
	 * \brief Releases the ownership of the managed object.
	 *
	 * \return Type* The raw pointer to the managed object.
	 */
	Type *Release() {
		if (!_EmptyPtr()) {
			return _ReleaseOwner();
		} else {
			return nullptr;
		}
	}

	/**
	 * \brief Resets the VUniquePointer to manage a new object.
	 *
	 * \param NewValue The new object to be managed.
	 */
	void Reset(Type *NewValue = nullptr) {
		Type *Pointer = _ReleaseOwner();
		if (Pointer != nullptr) {
			DeleterType::Delete(Pointer, Pool);
		}
		Ptr = NewValue;
	}

	/**
	 * \brief Overloaded arrow operator to access the managed object's members.
	 *
	 * \return Type* The raw pointer to the managed object.
	 */
	Type *operator->() {
		return Ptr;
	}

	/**
	 * \brief Returns the raw pointer to the managed object.
	 *
	 * \return Type* The raw pointer to the managed object.
	 */
	Type *Get() {
		return Ptr;
	}

	/**
	 * \brief Swaps the ownership of the managed objects between two VUniquePointers.
	 *
	 * \param Instance The other VUniquePointer to swap ownership with.
	 */
	void Swap(Object &Instance) {
		_Swap(Instance.Ptr, Ptr);
	}

public:
	/**
	 * \brief Creates a new VUniquePointer and allocates memory for the managed object.
	 *
	 * \tparam Argument The types of the arguments to be passed to the constructor of the managed object.
	 * \param Pool The memory pool used for allocation.
	 * \param Arg The arguments to be passed to the constructor of the managed object.
	 * \return Object The newly created VUniquePointer.
	 */
	template <class... Argument>
	static Object MakeUnique(MemoryPool &Pool, Argument... Arg) {
		return Object(Pool.template Allocate<Type>(Arg...), Pool);
	}

private:
	/**
	 * \brief Releases the ownership of the managed object.
	 *
	 * \return Type* The raw pointer to the managed object.
	 */
	Type *_ReleaseOwner() {
		return _ValueSwap(Ptr, nullptr);
	}

	/**
	 * \brief Checks if the VUniquePointer is empty.
	 *
	 * \return bool True if the VUniquePointer is empty, false otherwise.
	 */
	bool _EmptyPtr() {
		return Ptr == nullptr;
	}

private:
	/**
	 * \brief Swaps the values of two pointers.
	 *
	 * \param Left The first pointer.
	 * \param Right The second pointer.
	 */
	void _Swap(Type *&Left, Type *&Right) {
		Type *Temp = Right;
		Right	   = Left;
		Left	   = Temp;
	}

	/**
	 * \brief Swaps the value of a pointer with a new value.
	 *
	 * \param Old The old value of the pointer.
	 * \param New The new value of the pointer.
	 * \return Type* The old value of the pointer.
	 */
	Type *_ValueSwap(Type *&Old, Type *&&New) {
		Type *Temp = Old;
		Old		   = New;

		return Temp;
	}

private:
	template <class, class, class>
	friend class VUniquePointer;

private:
	MemoryPool &Pool;
	Type	   *Ptr;
};