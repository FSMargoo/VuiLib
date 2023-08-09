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

template <class PointerType, class MemoryPool>
class VPointerDeleter {
public:
	using Pointer = typename PointerType;
	using Null	  = typename decltype(nullptr);

public:
	constexpr VPointerDeleter() = default;
	virtual void Delete(PointerType *Pointer, MemoryPool &Pool) {
		Pointer->~PointerType();
		Pool.Delete(Pointer);
	}
	virtual void DeleteArray(PointerType *Pointer, MemoryPool &Pool, const size_t &Size) {
		for (size_t Count = 0; Count < Size; ++Count) {
			(Pointer[Count]).~PointerType();
		}

		Pool.DeleteArray(Pointer, Size);
	}
};

template <class Type, class MemoryPool, class DeleterType = VPointerDeleter<Type, MemoryPool>>
class VUniquePointer : public DeleterType {
public:
	using Object   = typename VUniquePointer<Type, MemoryPool, DeleterType>;
	using NullType = typename DeleterType::Null;

public:
	explicit VUniquePointer(Type *&&Pointer, MemoryPool &PointerPool) noexcept : Pool(PointerPool), Ptr(Pointer) {
	}
	explicit VUniquePointer(NullType) noexcept : Ptr(nullptr) {
	}

	VUniquePointer(Object &&Object) noexcept : Pool(Object.Pool), Ptr(Object._ReleaseOwner()) {
	}

	VUniquePointer(const Type *Pointer)	 = delete;
	VUniquePointer(const Object &Object) = delete;

	Object &operator=(NullType) noexcept {
		Reset();

		return *this;
	}

	~VUniquePointer() noexcept {
		if (Ptr != nullptr) {
			DeleterType::Delete(Ptr, Pool);
		}
	}

public:
	constexpr bool Empty() {
		return _EmptyPtr();
	}
	Type *Release() {
		if (!_EmptyPtr()) {
			return _ReleaseOwner();
		} else {
			return nullptr;
		}
	}
	void Reset(Type *NewValue = nullptr) {
		Type *Pointer = _ReleaseOwner();
		if (Pointer != nullptr) {
			DeleterType::Delete(Pointer, Pool);
		}
		Ptr = NewValue;
	}
	Type *operator->() {
		return Ptr;
	}
	Type *Get() {
		return Ptr;
	}
	void Swap(Object &Instance) {
		_Swap(Instance.Ptr, Ptr);
	}

public:
	template <class... Argument>
	static Object MakeUnique(MemoryPool &Pool, Argument... Arg) {
		return Object(Pool.template Allocate<Type>(Arg...), Pool);
	}

private:
	Type *_ReleaseOwner() {
		return _ValueSwap(Ptr, nullptr);
	}
	bool _EmptyPtr() {
		return Ptr == nullptr;
	}

private:
	void _Swap(Type *&Left, Type *&Right) {
		Type *Temp = Right;
		Right	   = Left;
		Left	   = Temp;
	}
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