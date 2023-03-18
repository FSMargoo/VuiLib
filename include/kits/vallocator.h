/*
 * File name	: vallocator.h
 * Author		: Margoo
 * Date			: 11/25/2022
 * Description
 * : The allocator in vuilib, that can able to manage pointer
 * centrally
 */
#pragma once

#include "../core/signal/vsignal.h"

VLIB_BEGIN_NAMESPACE

#define VMEM_UNKNOWN_DELETER 0
#define VMEM_DEFAULT_DELETER 1
#define VMEM_COM_DELETER	 2

namespace VKits
{
/*
 * VDefaultAllocator class:
 *	Description : The default allocator
 */
class VDefaultAllocator
{
public:
	template <class Type> Type *Malloc()
	{
		return new Type;
	}
};
/*
 * VCOMAllocator class:
 *	Description : The allocator for com object
 */
class VCOMAllocator
{
public:
	template <class Type> Type *Malloc()
	{
		return nullptr;
	}
};

class VUnknownDeleter
{
public:
	virtual unsigned int DeleterType()
	{
		return VMEM_UNKNOWN_DELETER;
	}
};

/*
 * VDefaultDeleter class:
 *	Description : The default deleter
 */
class VDefaultDeleter : public VUnknownDeleter
{
public:
	template <class Type> static void Free(Type Pointer)
	{
		delete Pointer;

		Pointer = nullptr;
	}

	unsigned int DeleterType() override;
};
/*
 * VCOMDeleter class:
 *	Description : The deleter for com object
 */
class VCOMDeleter : public VUnknownDeleter
{
public:
	template <class Type> static void Free(Type **Pointer)
	{
		delete Pointer;

		Pointer = nullptr;
	}
	template <> static void Free(IUnknown **Pointer)
	{
		if (*Pointer)
		{
			(*Pointer) = NULL;
		}
	}

	unsigned int DeleterType() override;
};

/*
 * VPointerWrapper class:
 *	Description : The wrapper to wrap a pointer
 */
struct VPointerWrapper
{
	VUnknownDeleter *Deleter;
	void			*Pointer;
};

/*
 * VAllocator class:
 *	Description : The allocator to manage pointer centrally
 */
class VAllocator
{
public:
	VAllocator(VAllocator *Parent = nullptr);
	~VAllocator();

	/*
	 * Malloc function:
	 *	Description : Allocate a memory with target type
	 */
	template <class Type, class Allocator = VDefaultAllocator, class Deleter = VDefaultDeleter> Type *Malloc()
	{
		Allocator AllocatorInstance;
		Type	 *Ptr = AllocatorInstance.Malloc<Type>();

		AllocatorMemory.push_back(VPointerWrapper{new Deleter, Ptr});

		return Ptr;
	}

private:
	/*
	 * FreeAllocator function:
	 *	Description : Release the allocator
	 */
	void FreeAllocator();

private:
	Core::VSignal<> OnDelete;

public:
	std::vector<VPointerWrapper> AllocatorMemory;
	VAllocator					*ParentAllocator;
};

/*
 * VCOMPointer class:
 *	Description : The pointer for COM object
 */
template <class COMObjectType> class VCOMPointer
{
public:
	VCOMPointer(const VCOMPointer<COMObjectType> &Pointer, VAllocator *TargetAllocator)
	{
		Object = Pointer.Object;

		if (Allocator)
		{
			Allocator->AllocatorMemory.push_back({new VCOMDeleter, (void *)this});
		}

		Allocator = TargetAllocator;
	}
	VCOMPointer(VAllocator *TargetAllocator)
	{
		Object = NULL;

		if (TargetAllocator)
		{
			TargetAllocator->AllocatorMemory.push_back({new VCOMDeleter, (void *)this});
		}

		Allocator = TargetAllocator;
	}
	VCOMPointer(COMObjectType *Ptr)
	{
		Object = Ptr;
	}
	~VCOMPointer()
	{
		if (Allocator)
		{
			for (auto Iterator = Allocator->AllocatorMemory.begin(); Iterator != Allocator->AllocatorMemory.end();
				 ++Iterator)
			{
				if ((void *)(*Iterator).Pointer == (void *)this)
				{
					VCOMDeleter *Deleter = static_cast<VCOMDeleter *>((*Iterator).Deleter);

					if (Object)
					{
						Object->Release();

						Object = NULL;
					}

					delete Deleter;

					Allocator->AllocatorMemory.erase(Iterator);

					break;
				}
			}
		}
	}

	COMObjectType *operator->()
	{
		return Object;
	}

	COMObjectType *Get() const
	{
		return Object;
	}

public:
	COMObjectType *Object;

public:
	VAllocator *Allocator;
};
} // namespace VKits

VLIB_END_NAMESPACE