#include "../../include/kits/vallocator.h"

VLIB_BEGIN_NAMESPACE

namespace VKits {
	VAllocator::VAllocator(VAllocator* Parent) : ParentAllocator(Parent) {
		if (Parent) {
			Parent->OnDelete.Connect(this, &VAllocator::FreeAllocator);
		}
	}
	VAllocator::~VAllocator() {
		if (!ParentAllocator) {
			FreeAllocator();
		}
	}
	void VAllocator::FreeAllocator() {
		for (auto Object = AllocatorMemory.begin(); Object != AllocatorMemory.end(); ++Object) {
			if (Object->Deleter->DeleterType() == VMEM_DEFAULT_DELETER) {
				VDefaultDeleter* Deleter = static_cast<VDefaultDeleter*>(Object->Deleter);
				Deleter->Free(Object->Pointer);

				delete Deleter;

				continue;
			}
			if (Object->Deleter->DeleterType() == VMEM_COM_DELETER) {
				VCOMDeleter* Deleter = static_cast<VCOMDeleter*>(Object->Deleter);
				
				Deleter->Free(
					(IUnknown**)(
						&(
							(
								(VCOMPointer<IUnknown>*)
								Object->Pointer
							 )
							 ->Object
						 )
					));

				((VCOMPointer<IUnknown>*)Object->Pointer)->Allocator = nullptr;

				delete Object->Pointer;
				delete Deleter;

				continue;
			}
		}

		AllocatorMemory.clear();

		OnDelete.Emit();
	}

	unsigned int VDefaultDeleter::DeleterType() {
		return VMEM_DEFAULT_DELETER;
	}
	unsigned int VCOMDeleter::DeleterType() {
		return VMEM_COM_DELETER;
	}
}

VLIB_END_NAMESPACE