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
 * \file vmemory.h
 * \brief This file contains the definition of the VMemoryPool class and VThreadCache class.
 */

#pragma once

#include "kernel/base/vbase.h"
#include "kernel/mem/vconstantpool.h"
#include "kernel/mem/vmemorypolicy.h"
#include "kernel/mem/vmemrbtree.h"

#include <mutex>

/**
 * \brief The abstract of the memory interface
 */
class VMemoryInterface {
public:
	virtual void OOMPanic() = 0;
};

/**
 * \brief The memory unit define in memory pool, the memory unit is only a flag of a
 * 	memory space, it can't malloc or delete memory, actually. The memory block is a list
 * 	of series of memory units, each unit own its unique pointer to be the *base address*
 * 	of a unit. When the unit's memory was *delete* by memory pool, it only turns the InUsed
 * 	flag into false.
 * \tparam AllocateSize : The size of this unit
 */
template <size_t AllocateSize>
class VMemoryUnit {
public:
	/**
	 * \brief The constructor function of memory unit
	 * \param LastPtr
	 * \param StartPtr
	 */
	explicit VMemoryUnit(VMemoryUnit<AllocateSize> *LastPtr, void *StartPtr)
		: Pointer(StartPtr), InUsed(false), Last(LastPtr), Next(nullptr), NextStorage(nullptr) {
	}

public:
	/**
	 * \brief Get the size of this unit
	 * \return The size of the unit
	 */
	[[nodiscard]] constexpr size_t UnitSize() {
		return AllocateSize;
	}

public:
	/**
	 * \brief Allocate a memory since this unit
	 * \param Size The size that the memory need to be allocated
	 * \return The memory pointer
	 */
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
	VMemoryUnit<AllocateSize> *Last;		///< The prev node
	VMemoryUnit<AllocateSize> *Next;		///< The next node
	VMemoryUnit<AllocateSize> *NextStorage; ///< The next memory field

	bool InUsed; ///< Is this unit in used or not

private:
	void *Pointer; ///< The unit's pointer
};

class VMemoryPool;

/**
 * \brief The memory unit alias
 */
using VMem4ByteUnit = VMemoryUnit<4>;
/**
 * \brief The memory unit alias
 */
using VMem8ByteUnit = VMemoryUnit<8>;
/**
 * \brief The memory unit alias
 */
using VMem16ByteUnit = VMemoryUnit<16>;
/**
 * \brief The memory unit alias
 */
using VMem32ByteUnit = VMemoryUnit<32>;
/**
 * \brief The memory unit alias
 */
using VMem64ByteUnit = VMemoryUnit<64>;
/**
 * \brief The memory unit alias
 */
using VMem128ByteUnit = VMemoryUnit<128>;
/**
 * \brief The memory unit alias
 */
using VMem256ByteUnit = VMemoryUnit<256>;
/**
 * \brief The memory unit alias
 */
using VMemoryConstant = VConstantExtractor<VMemoryPool>;

/**
 * \brief A memory pool, this memory pool use different size of memory blocks to avoid
 * the memory fragmentation problem, for some constant value it will provide a constant pool to
 * collect some constant value, it uses std::mutex to make sure the thread security
 *
 *        +---------------------+
 *        |Specified memory size|
 *        +---------------------+
 * 		               |
 * 		               |
 * +-------------------v----------------------+
 * |Count a practical size of specified memory|
 * +------------------------------------------+
 * 		               |
 * 		               |
 * 		               |
 * 		               |
 * 		               v
 *        +--------------------------+    OOM Panic   +-----------------------+
 *        |Search in memory unit list+----------------+ Expand the memory unit|
 *        +--------------------------+                +-----------------------+
 *        		       |                                           |
 *        		       |                                           |
 *        		       |                                           |
 *        +------------v----------------+                          |
 *        |Count the offset of head list<--------------------------+
 *        +-----------------------------+

 */
class VMemoryPool : public VMemoryInterface {
public:
	/**
	 * \brief The constructor of the memory pool
	 * \param ManagerPolicy The policy that will be used in memory allocation
	 */
	explicit VMemoryPool(VMemoryPolicy ManagerPolicy = VMemoryPolicy::Default());
	/**
	 * \brief A deleted copy function
	 * \param Object None
	 */
	explicit VMemoryPool(VMemoryPool &Object) = delete;
	/**
	 * \brief A deleted copy function
	 * \param Object None
	 */
	explicit VMemoryPool(const VMemoryPool &Object) = delete;
	/**
	 * \brief The move constructor function
	 * \param Object
	 */
	VMemoryPool(VMemoryPool &&Object);
	/**
	 * \brief Destructor function
	 */
	~VMemoryPool();

public:
	/**
	 * \brief This API was designed to process the OOM panic at memory allocation,
	 * however, it has be discarded
	 */
	void OOMPanic() override;

public:
	/**
	 * \brief Allocate an object with init argument, it will use placement new to construct a object
	 * \tparam Type The Object type
	 * \tparam Argument The construct argument type
	 * \param BuiltArgument The construct argument
	 * \return The pointer to the object
	 */
	template <class Type, class... Argument>
	Type *Allocate(Argument... BuiltArgument) {
		_InitThreadLock();

		Type *Ptr = static_cast<Type *>(_AllocateMemory(sizeof(Type)));

		_ReleaseThreadLock();

		return new (Ptr) Type(std::forward<Argument>(BuiltArgument)...);
	}
	/**
	 * \brief Allocate a const value from constant pool
	 * \param Argument The construct argument
	 * \return The constant pointer to the constant pool
	 */
	template <>
	const int *Allocate<const int, int>(int Argument) {
		_InitThreadLock();

		const int *Ptr = static_cast<const int *>(IntPool->FindOrInsert(Argument));

		_ReleaseThreadLock();

		return Ptr;
	}
	/**
	 * \brief Allocate a const value from constant pool
	 * \param Argument The construct argument
	 * \return The constant pointer to the constant pool
	 */
	template <>
	const float *Allocate<const float, float>(float Argument) {
		_InitThreadLock();

		const float *Ptr = static_cast<const float *>(FloatPool->FindOrInsert(Argument));

		_ReleaseThreadLock();

		return Ptr;
	}
	/**
	 * \brief Allocate a const value from constant pool
	 * \param Argument The construct argument
	 * \return The constant pointer to the constant pool
	 */
	template <>
	const double *Allocate<const double, double>(double Argument) {
		_InitThreadLock();

		const double *Ptr = static_cast<const double *>(DoublePool->FindOrInsert(Argument));

		_ReleaseThreadLock();

		return Ptr;
	}
	/**
	 * \brief Free a memory (This function won't execute the deconstruct function of this object, please use carefully)
	 * \tparam Type	The type of the object
	 * \param Ptr The pointer reference to the object
	 */
	template <class Type>
	void Delete(Type *Ptr) {
		_InitThreadLock();

		_FreeMemory(Ptr, sizeof(Type));

		_ReleaseThreadLock();
	}
	/**
	 * \brief Allocate an array memory block (The return value of the block will not be init at any value)
	 * \tparam Type The type of the array
	 * \param ArraySize The size of the array
	 * \return The pointer reference to the array memory
	 */
	template <class Type>
	Type *AllocateArray(const size_t &ArraySize) {
		_InitThreadLock();

		Type *Ptr = static_cast<Type *>(_AllocateMemory(ArraySize * sizeof(Type)));

		_ReleaseThreadLock();

		return Ptr;
	}
	/**
	 * \brief Delete an array memory
	 * (This function won't execute the deconstruct function of this object, please use carefully)
	 * \tparam Type The memory type of the array
	 * \param Ptr The pointer reference to the pointer
	 * \param ArraySize The size of the array
	 */
	template <class Type>
	void DeleteArray(Type *Ptr, const size_t &ArraySize) {
		_InitThreadLock();

		_FreeMemory(Ptr, ArraySize * sizeof(Type));

		_ReleaseThreadLock();
	}

protected:
	/**
	 * \brief Init the memory block (The proxy and the byte pointer)
	 */
	__forceinline void _InitMemoryPool();
	/**
	 * \brief Init a memory block (It use offset to count each pointer of each proxy)
	 * \tparam ByteSize The unit size
	 * \param Block The start pointer
	 * \param ListHead The head of unit chain
	 * \param TotalSize The size of the memory block
	 */
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
	/**
	 * \brief Check whether a specified policy is legit or not
	 * \return If the policy is legit return true, nor return false
	 */
	__forceinline bool _CheckPolicyLegit();
	/**
	 * \brief Count a practical size of specified memory size
	 * \param Size The size of specified size
	 * \return The practical allocate size of the memory size
	 */
	short _EvaluateParticle(const size_t &Size);

protected:
	/**
	 * \brief Allocate a memory with specified size
	 * \param Size The size that it needs to be allocated
	 * \return The memory
	 */
	[[nodiscard]] void *_AllocateMemory(const size_t &Size);
	/**
	 * \brief Free the memory block with specified unit
	 * \tparam ByteSize The proxy unit size
	 * \param Ptr The pointer
	 * \param Proxy The unit proxy
	 * \param Size The size of the memory block
	 * \return If function can not find the pointer in specified proxy, return false
	 */
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
	/**
	 * \brief Release the memory
	 * \param Ptr
	 * \param Size
	 */
	void _FreeMemory(void *Ptr, const size_t &Size);
	/**
	 * \brief Process the OOM panic, it will expand the memory with the specified policy
	 * \tparam ByteSize
	 * \param Proxy
	 * \param Reference
	 */
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
		/* Make sure that allocate enough memory */
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
		if (NewStorage == nullptr) {
			_vdebug_handle NoRestOfMemory;
			NoRestOfMemory.crash("Not enough memory on system!");
		}

		Prev->NextStorage = NewStorage;
		_InitMemoryBlock<ByteSize>(Memory, NewStorage, ExpandSize);
	}
	/**
	 * \brief Search valid memory unit, if exists continuous memory list that not be marked as in used,
	 * it will return the head memory unit.
	 * \tparam ByteSize
	 * \param Head
	 * \param TargetSize
	 * \return
	 */
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

/**
 * \brief Thread cache is used for the mul thread processing, a thread will own a unique memory cache,
 * if the thread requires to allocate a memory block, it will send request to thread cache first, if
 * thread cache's rest memory block can't afford for the memory allocating, it will finally send request
 * to main allocator, this design can reduce the efficiency loss due to multi-threaded locks, all of the
 * API is same as the VMemory pool.
 * However, the creation of VThreadCache should provide a VMemoryPool object.
 */
class VThreadCache : public VMemoryPool {
public:
	VThreadCache(VMemoryPool &ParentPool, const size_t &InitSize);

public:
	template <class Type, class... Argument>
	Type *Allocate(Argument... BuiltArgument) {
		Type *Ptr = static_cast<Type *>(_AllocateMemory(sizeof(Type)));

		return new (Ptr) Type(BuiltArgument...);
	}
	template <>
	const int *Allocate<const int, int>(int Argument) {
		return static_cast<const int *>(IntPool->FindOrInsert(Argument));
	}
	template <>
	const float *Allocate<const float, float>(float Argument) {
		return static_cast<const float *>(FloatPool->FindOrInsert(Argument));
	}
	template <>
	const double *Allocate<const double, double>(double Argument) {
		return static_cast<const double *>(DoublePool->FindOrInsert(Argument));
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
	void DeleteArray(Type *Ptr, const size_t &ArraySize) {
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
		Pool->DeleteArray(Ptr, Count);
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