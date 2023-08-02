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

#include "kernel/mem/vmemory.h"

VMemoryPool::VMemoryPool(VMemoryPolicy ManagerPolicy = VMemoryPolicy::Default()) : Policy(ManagerPolicy) {
	_InitMemoryPool();
	if (!_CheckPolicyLegit()) {
		ManagerPolicy = VMemoryPolicy::Default();
	}
}
void VMemoryPool::_InitMemoryPool() {
	OOM = false;

	_4Bit  = malloc(Policy.InitSize);
	_8Bit  = malloc(Policy.InitSize);
	_16Bit = malloc(Policy.InitSize);
	_32Bit = malloc(Policy.InitSize);

	_4BitProxy	= new VMem4BitUnit(nullptr, _4Bit);
	_8BitProxy	= new VMem8BitUnit(nullptr, _8Bit);
	_16BitProxy = new VMem16BitUnit(nullptr, _16Bit);
	_32BitProxy = new VMem32BitUnit(nullptr, _32Bit);

	_InitMemoryBlock<4>(_4Bit, _4BitProxy, Policy.InitSize);
	_InitMemoryBlock<8>(_8Bit, _8BitProxy, Policy.InitSize);
	_InitMemoryBlock<16>(_16Bit, _16BitProxy, Policy.InitSize);
	_InitMemoryBlock<32>(_32Bit, _32BitProxy, Policy.InitSize);
}
bool VMemoryPool::_CheckPolicyLegit() {
	if (Policy.ExpandSize == 0 || Policy.UserFunction == NULL || Policy.InitSize == 0) {
		_vdebug_handle PolicyInvalid;
		PolicyInvalid.crash("Invalid policy setting!");

		return false;
	}

	return true;
}
void VMemoryPool::OOMPanic() {
	OOM = true;
}
void *VMemoryPool::_AllocateMemory(const size_t &Size) {
	OOM = false;

	/*
	 * Choose the smallest memory unit
	 */
	size_t Min		= Size % 4;
	short  BitCount = 4;
	if (Size % 8 < Min) {
		BitCount = 8;
	}
	if (Size % 16 < Min) {
		BitCount = 16;
	}
	if (Size % 32 < Min) {
		BitCount = 32;
	}

	void *Ptr = nullptr;
LAllocate:
	switch (BitCount) {
	case 4: {
		VMem4BitUnit *Unit = _SearchValidAllocator<4>(_4BitProxy, Size);
		if (Unit != nullptr) {
			Ptr = Unit->Allocate(Size);
		} else {
			OOM = true;
			break;
		}

		break;
	}
	case 8: {
		VMem8BitUnit *Unit = _SearchValidAllocator<8>(_8BitProxy, Size);
		if (Unit != nullptr) {
			Ptr = Unit->Allocate(Size);
		} else {
			OOM = true;
			break;
		}

		break;
	}
	case 16: {
		VMem16BitUnit *Unit = _SearchValidAllocator<16>(_16BitProxy, Size);
		if (Unit != nullptr) {
			Ptr = Unit->Allocate(Size);
		} else {
			OOM = true;
			break;
		}

		break;
	}
	case 32: {
		VMem32BitUnit *Unit = _SearchValidAllocator<32>(_32BitProxy, Size);
		if (Unit != nullptr) {
			Ptr = Unit->Allocate(Size);
		} else {
			OOM = true;
			break;
		}

		break;
	}
	}

	// If out of memory (Not enough memory)
	if (OOM) {
		if (Policy.ExpandMode == VMemoryExpandMode::Constant) {
			_vdebug_handle().crash("Memory pool out of memory. (However, in constant mode allocator can't expand)");
		} else {
			switch (BitCount) {
			case 4: {
				_DealyOOM<4>(_4BitProxy);

				break;
			}
			case 8: {
				_DealyOOM<8>(_8BitProxy);

				break;
			}
			case 16: {
				_DealyOOM<16>(_16BitProxy);

				break;
			}
			case 32: {
				_DealyOOM<32>(_32BitProxy);

				break;
			}
			}

			goto LAllocate;
		}
	}

	return Ptr;
}