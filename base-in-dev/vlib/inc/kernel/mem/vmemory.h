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

#include "kernel/base/vbase.h"
#include "kernel/mem/vconstantpool.h"
#include "kernel/mem/vmemorypolicy.h"
#include "kernel/mem/vmemrbtree.h"

#include <mutex>

class VMemoryInterface {
public:
	virtual void OOMPanic() = 0;
};

template <size_t AllocateSize>
class VMemoryUnit {
public:
	explicit VMemoryUnit(VMemoryUnit<AllocateSize> *LastPtr, void *StartPtr)
		: Pointer(StartPtr), InUsed(false), Last(LastPtr), Next(nullptr), NextStorage(nullptr) {
	}

public:
	[[nodiscard]] constexpr size_t UnitSize() {
		return AllocateSize;
	}

public:
	[[nodiscard]] void *Allocate(const size_t &Size) {
		size_t					   Offset = AllocateSize;
		VMemoryUnit<AllocateSize> *Ptr	  = Next;

		InUsed = true;

		while (Offset <= Size) {
			Ptr->InUsed = true;
			Ptr			= Ptr->Next;

			Offset += AllocateSize;
		}

		return Pointer;
	}
	void *FetchPointer() {
		return Pointer;
	}

public:
	VMemoryUnit<AllocateSize> *Last;
	VMemoryUnit<AllocateSize> *Next;
	VMemoryUnit<AllocateSize> *NextStorage;

	bool InUsed;

private:
	void *Pointer;
};

class VMemoryPool;

using VMem4ByteUnit	  = VMemoryUnit<4>;
using VMem8ByteUnit	  = VMemoryUnit<8>;
using VMem16ByteUnit  = VMemoryUnit<16>;
using VMem32ByteUnit  = VMemoryUnit<32>;
using VMem64ByteUnit  = VMemoryUnit<64>;
using VMem128ByteUnit = VMemoryUnit<128>;
using VMem256ByteUnit = VMemoryUnit<256>;
using VMemoryConstant = VConstantExtractor<VMemoryPool>;

class VMemoryPool : public VMemoryInterface {
public:
	explicit VMemoryPool(VMemoryPolicy ManagerPolicy = VMemoryPolicy::Default());
	~VMemoryPool();

public:
	void OOMPanic() override;

public:
	template <class Type, class... Agrument>
	Type *Allocate(Agrument... BuiltAgrument) {
		_InitThreadLock();

		Type *Ptr = static_cast<Type *>(_AllocateMemory(sizeof(Type)));

		_ReleaseThreadLock();

		return new (Ptr) Type(BuiltAgrument...);
	}
	template <>
	const int *Allocate<const int, int>(int Agrument) {
		_InitThreadLock();

		const int *Ptr = static_cast<const int *>(IntPool->FindOrInsert(Agrument));

		_ReleaseThreadLock();

		return Ptr;
	}
	template <>
	const float *Allocate<const float, float>(float Agrument) {
		_InitThreadLock();

		const float *Ptr = static_cast<const float *>(FloatPool->FindOrInsert(Agrument));

		_ReleaseThreadLock();

		return Ptr;
	}
	template <>
	const double *Allocate<const double, double>(double Agrument) {
		_InitThreadLock();

		const double *Ptr = static_cast<const double *>(DoublePool->FindOrInsert(Agrument));

		_ReleaseThreadLock();

		return Ptr;
	}
	template <class Type>
	void Delete(Type *Ptr) {
		_InitThreadLock();

		_FreeMemory(Ptr, sizeof(Type));

		_ReleaseThreadLock();
	}

	template <class Type>
	Type *AllocateArray(const size_t &ArraySize) {
		_InitThreadLock();

		Type *Ptr = static_cast<Type *>(_AllocateMemory(ArraySize * sizeof(Type)));

		_ReleaseThreadLock();

		return Ptr;
	}
	template <class Type>
	void DeletArray(Type *Ptr, const size_t &ArraySize) {
		_InitThreadLock();

		_FreeMemory(Ptr, ArraySize * sizeof(Type));

		_ReleaseThreadLock();
	}

protected:
	__forceinline void _InitMemoryPool();
	template <size_t ByteSize>
	__forceinline void _InitMemoryBlock(void *Block, VMemoryUnit<ByteSize> *ListHead, size_t TotalSize) {
		size_t				   Offset = ByteSize;
		VMemoryUnit<ByteSize> *Last	  = ListHead;
		VMemoryUnit<ByteSize> *Ptr	  = ListHead;
		while (Offset < TotalSize) {
			Ptr->Next = new VMemoryUnit<ByteSize>(Last, ((char *&)(Block)) + Offset);

			Last = Ptr->Next;
			Ptr	 = Ptr->Next;

			Offset += ByteSize;
		}
	}
	__forceinline bool _CheckPolicyLegit();
	short			   _EvaluateParticle(const size_t &Size);

protected:
	[[nodiscard]] void *_AllocateMemory(const size_t &Size);
	template <size_t ByteSize>
	bool _FreeBlock(void *Ptr, VMemoryUnit<ByteSize> *Proxy, const size_t &Size) {
		VMemoryUnit<ByteSize> *Pointer = Proxy;
		bool				   Flag	   = false;

		while (Pointer != nullptr) {
			if (Pointer->FetchPointer() == Ptr) {
				size_t SearchSize = Size / ByteSize;
				if (Size % ByteSize == 0) {
					--SearchSize;
				}

				Pointer->InUsed = false;
				while (SearchSize >= 1) {
					Pointer			= Pointer->Next;
					Pointer->InUsed = false;

					--SearchSize;
				}

				Flag = true;
				break;
			}

			if (Pointer->NextStorage != nullptr) {
				Pointer = Pointer->NextStorage;
				continue;
			}
			Pointer = Pointer->Next;
		}

		return Flag;
	}
	void _FreeMemory(void *Ptr, const size_t &Size);
	template <size_t ByteSize>
	void _DealyOOM(VMemoryUnit<ByteSize> *Proxy, const size_t &Reference) {
		if (Proxy == nullptr) {
			_vdebug_handle InvalidProxy;
			InvalidProxy.crash("Invalid proxy was specified!");
			return;
		}
		VMemoryUnit<ByteSize> *Ptr	= Proxy->Next;
		VMemoryUnit<ByteSize> *Prev = Proxy;
		while (true) {
			if (Ptr == nullptr && Prev->NextStorage != nullptr) {
				Prev = Prev->NextStorage;
				Ptr	 = Prev->Next;
			}
			if (Ptr == nullptr && Prev->NextStorage == nullptr) {
				break;
			}
			if (Ptr == nullptr) {
				break;
			}

			Prev = Ptr;
			Ptr	 = Ptr->Next;
		}

		size_t ExpandSize = 0;
		switch (Policy.ExpandMode) {
		case VMemoryExpandMode::UserDefine: {
			ExpandSize = Policy.UserFunction();
			if (ExpandSize == 0) {
				_vdebug_handle ZeroSizeExpand;
				ZeroSizeExpand.crash("Can not expand 0 size.");
			}

			break;
		}
		case VMemoryExpandMode::Constant: {
			ExpandSize = Policy.ExpandSize;

			break;
		}
		case VMemoryExpandMode::Double: {
			ExpandSize = Policy.InitSize * 2;

			break;
		}
		case VMemoryExpandMode::Add: {
			ExpandSize = Policy.InitSize + Policy.ExpandSize;

			break;
		}
		}
		if (Reference > ExpandSize) {
			size_t Ex  = ExpandSize;
			ExpandSize = Reference;
			while (ExpandSize % ByteSize != 0) {
				ExpandSize += 1;
			}
			ExpandSize += Ex;
		}

		void				  *Memory	  = malloc(ExpandSize);
		VMemoryUnit<ByteSize> *NewStorage = new VMemoryUnit<ByteSize>(nullptr, Memory);
		Prev->NextStorage				  = NewStorage;
		_InitMemoryBlock<ByteSize>(Memory, NewStorage, ExpandSize);
	}
	template <size_t ByteSize>
	[[nodiscard]] VMemoryUnit<ByteSize> *_SearchValidAllocator(VMemoryUnit<ByteSize> *Head, const size_t &TargetSize) {
		VMemoryUnit<ByteSize> *Ptr = Head;
		size_t				   Count;

		while (true) {
			Count							 = ByteSize;
			VMemoryUnit<ByteSize> *BlockHead = Ptr;
			while (!Ptr->InUsed) {
				if (Count >= TargetSize) {
					return BlockHead;
				}
				if (Ptr->Next == nullptr && Ptr->NextStorage != nullptr) {
					Count	  = ByteSize;
					Ptr		  = Ptr->NextStorage;
					BlockHead = Ptr;

					continue;
				} else {
					Ptr = Ptr->Next;
				}
				if (Ptr == nullptr) {
					return nullptr;
				}
				Count += ByteSize;
			}

			if (Ptr->Next == nullptr && Ptr->NextStorage != nullptr) {
				Ptr = Ptr->NextStorage;
			} else {
				Ptr = Ptr->Next;
				if (Ptr == nullptr) {
					return nullptr;
				}
			}
		}

		// OOM Error
		return nullptr;
	}

protected:
	void _InitThreadLock();
	void _ReleaseThreadLock();

protected:
	VMem4ByteUnit	*_4ByteProxy;
	VMem8ByteUnit	*_8ByteProxy;
	VMem16ByteUnit	*_16ByteProxy;
	VMem32ByteUnit	*_32ByteProxy;
	VMem64ByteUnit	*_64ByteProxy;
	VMem128ByteUnit *_128ByteProxy;
	VMem256ByteUnit *_256ByteProxy;

protected:
	VMemoryPolicy Policy;

protected:
	void *_4Byte;
	void *_8Byte;
	void *_16Byte;
	void *_32Byte;
	void *_64Byte;
	void *_128Byte;
	void *_256Byte;
	bool  OOM;

protected:
	VMemoryConstant::IntPool	*IntPool;
	VMemoryConstant::FloatPool	*FloatPool;
	VMemoryConstant::DoublePool *DoublePool;

private:
	std::mutex MutexLock;
};

class VThreadCache : public VMemoryPool {
public:
	VThreadCache(VMemoryPool &ParentPool, const size_t &InitSize);

public:
	template <class Type, class... Agrument>
	Type *Allocate(Agrument... BuiltAgrument) {
		Type *Ptr = static_cast<Type *>(_AllocateMemory(sizeof(Type)));

		return new (Ptr) Type(BuiltAgrument...);
	}
	template <>
	const int *Allocate<const int, int>(int Agrument) {
		return static_cast<const int *>(IntPool->FindOrInsert(Agrument));
	}
	template <>
	const float *Allocate<const float, float>(float Agrument) {
		return static_cast<const float *>(FloatPool->FindOrInsert(Agrument));
	}
	template <>
	const double *Allocate<const double, double>(double Agrument) {
		return static_cast<const double *>(DoublePool->FindOrInsert(Agrument));
	}
	template <class Type>
	void Delete(Type *Ptr) {
		_FreeMemory(Ptr, sizeof(Type));
	}

	template <class Type>
	Type *AllocateArray(const size_t &ArraySize) {
		return static_cast<Type *>(_AllocateMemory(ArraySize * sizeof(Type)));
	}
	template <class Type>
	void DeletArray(Type *Ptr, const size_t &ArraySize) {
		_FreeMemory(Ptr, ArraySize * sizeof(Type));
	}

protected:
	[[nodiscard]] void *_AllocateMemory(const size_t &Size);

private:
	VMemoryPool &Pool;
};

template <class Type>
class VSTLAllocator : public std::allocator<Type> {
public:
	typedef Type		value_type;
	typedef Type	   *pointer;
	typedef const Type *const_pointer;
	typedef Type	   &reference;
	typedef const Type &const_reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;

	template <class U>
	struct rebind {
		typedef VSTLAllocator<U> other;
	};

public:
	VSTLAllocator(VSTLAllocator &&Allocator) {
		Pool		   = Allocator.Pool;
		Allocator.Pool = nullptr;
	}
	VSTLAllocator(const VSTLAllocator &Allocator) {
		Pool = Allocator.Pool;
	}
	VSTLAllocator(VMemoryPool *AllocatorPool) : Pool(AllocatorPool) {
		if (AllocatorPool == nullptr) {
			_vdebug_handle InvalidAllocator;
			InvalidAllocator.crash("Invalid allocator was specified!");

			Pool = new VMemoryPool();
		}
	}
	template <class Other>
	VSTLAllocator(const VSTLAllocator<Other> &Allocator) {
		Pool = Allocator.Pool;
	}

	void construct(pointer Ptr, const Type &Value) {
		Ptr = ::new (Ptr) Type(Value);
	}
	void destroy(pointer Ptr) {
		Ptr->~Type();
	}

	pointer address(reference Ref) {
		return (pointer)&Ref;
	}

	const_pointer address(const_reference CRef) {
		return (const_pointer)&CRef;
	}

	size_type max_size() const {
		return size_type(UINT_MAX / sizeof(Type));
	}
	void deallocate(Type *const Ptr, const size_t Count) {
		if (Ptr == nullptr || Count == 0) {
			_vdebug_handle InvalidProvidedInfo;
			InvalidProvidedInfo.crash("Invalid pointer or count was specified!");

			return;
		}
		Pool->DeletArray(Ptr, Count);
	}
	constexpr Type *allocate(const size_t Count) {
		return Pool->AllocateArray<Type>(Count);
	}

public:
	VMemoryPool *GetPool() {
		return Pool;
	}

public:
	VMemoryPool *Pool;
};