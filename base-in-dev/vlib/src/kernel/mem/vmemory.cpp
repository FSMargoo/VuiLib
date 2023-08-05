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

VMemoryPool::VMemoryPool(VMemoryPolicy ManagerPolicy) : Policy(ManagerPolicy) {
	_InitMemoryPool();
	if (!_CheckPolicyLegit()) {
		ManagerPolicy = VMemoryPolicy::Default();
	}
}
void VMemoryPool::_InitMemoryPool() {
	OOM = false;

	_4Byte	 = malloc(Policy.InitSize);
	_8Byte	 = malloc(Policy.InitSize);
	_16Byte	 = malloc(Policy.InitSize);
	_32Byte	 = malloc(Policy.InitSize);
	_64Byte	 = malloc(Policy.InitSize);
	_128Byte = malloc(Policy.InitSize);
	_256Byte = malloc(Policy.InitSize);

	_4ByteProxy	  = new VMem4ByteUnit(nullptr, _4Byte);
	_8ByteProxy	  = new VMem8ByteUnit(nullptr, _8Byte);
	_16ByteProxy  = new VMem16ByteUnit(nullptr, _16Byte);
	_32ByteProxy  = new VMem32ByteUnit(nullptr, _32Byte);
	_64ByteProxy  = new VMem64ByteUnit(nullptr, _32Byte);
	_128ByteProxy = new VMem128ByteUnit(nullptr, _32Byte);
	_256ByteProxy = new VMem256ByteUnit(nullptr, _32Byte);

	_InitMemoryBlock<4>(_4Byte, _4ByteProxy, Policy.InitSize);
	_InitMemoryBlock<8>(_8Byte, _8ByteProxy, Policy.InitSize);
	_InitMemoryBlock<16>(_16Byte, _16ByteProxy, Policy.InitSize);
	_InitMemoryBlock<32>(_32Byte, _32ByteProxy, Policy.InitSize);
	_InitMemoryBlock<64>(_64Byte, _64ByteProxy, Policy.InitSize);
	_InitMemoryBlock<128>(_128Byte, _128ByteProxy, Policy.InitSize);
	_InitMemoryBlock<256>(_256Byte, _256ByteProxy, Policy.InitSize);
}
bool VMemoryPool::_CheckPolicyLegit() {
	if (Policy.ExpandSize == 0 || (Policy.ExpandMode == VMemoryExpandMode::UserDefine && Policy.UserFunction == NULL) ||
		Policy.InitSize == 0) {
		_vdebug_handle PolicyInvalid;
		PolicyInvalid.crash("Invalid policy setting!");

		return false;
	}

	return true;
}
void VMemoryPool::OOMPanic() {
	OOM = true;
}
short VMemoryPool::_EvaluateParticle(const size_t &Size) {
	if (Size == 4 || Size == 8 || Size == 16 || Size == 32 || Size == 64 || Size == 128 || Size == 256) {
		return static_cast<short>(Size);
	}

	short  Byte = 32;
	double Min	= std::round(Size / 32) * 0.7 + (Size % 32) * 0.3;

	double _4ByteCoefficient = std::round(Size / 4) * 0.7 + (Size % 4) * 0.3;
	if (Min > _4ByteCoefficient) {
		Byte = 4;
	}
	double _8ByteCoefficient = std::round(Size / 8) * 0.7 + (Size % 8) * 0.3;
	if (Min > _8ByteCoefficient) {
		Byte = 8;
	}
	double _16ByteCoefficient = std::round(Size / 16) * 0.7 + (Size % 16) * 0.3;
	if (Min > _16ByteCoefficient) {
		Byte = 16;
	}
	double _64ByteCoefficient = std::round(Size / 64) * 0.7 + (Size % 64) * 0.3;
	if (Min > _64ByteCoefficient) {
		Byte = 64;
	}
	double _128ByteCoefficient = std::round(Size / 128) * 0.7 + (Size % 128) * 0.3;
	if (Min > _128ByteCoefficient) {
		Byte = 1298;
	}
	double _256ByteCoefficient = std::round(Size / 256) * 0.7 + (Size % 256) * 0.3;
	if (Min > _256ByteCoefficient) {
		Byte = 256;
	}

	return Byte;
}
void *VMemoryPool::_AllocateMemory(const size_t &Size) {
	OOM = false;

	short ByteCount = _EvaluateParticle(Size);

	void *Ptr = nullptr;

LAllocate:
	switch (ByteCount) {
	case 4: {
		VMem4ByteUnit *Unit = _SearchValidAllocator<4>(_4ByteProxy, Size);
		if (Unit != nullptr) {
			Ptr = Unit->Allocate(Size);
			OOM = false;
		} else {
			OOM = true;
			break;
		}

		break;
	}
	case 8: {
		VMem8ByteUnit *Unit = _SearchValidAllocator<8>(_8ByteProxy, Size);
		if (Unit != nullptr) {
			Ptr = Unit->Allocate(Size);
			OOM = false;
		} else {
			OOM = true;
			break;
		}

		break;
	}
	case 16: {
		VMem16ByteUnit *Unit = _SearchValidAllocator<16>(_16ByteProxy, Size);
		if (Unit != nullptr) {
			Ptr = Unit->Allocate(Size);
			OOM = false;
		} else {
			OOM = true;
			break;
		}

		break;
	}
	case 32: {
		VMem32ByteUnit *Unit = _SearchValidAllocator<32>(_32ByteProxy, Size);
		if (Unit != nullptr) {
			Ptr = Unit->Allocate(Size);
			OOM = false;
		} else {
			OOM = true;
			break;
		}

		break;
	}
	case 64: {
		VMem64ByteUnit *Unit = _SearchValidAllocator<64>(_64ByteProxy, Size);
		if (Unit != nullptr) {
			Ptr = Unit->Allocate(Size);
			OOM = false;
		} else {
			OOM = true;
			break;
		}

		break;
	}
	case 128: {
		VMem128ByteUnit *Unit = _SearchValidAllocator<128>(_128ByteProxy, Size);
		if (Unit != nullptr) {
			Ptr = Unit->Allocate(Size);
			OOM = false;
		} else {
			OOM = true;
			break;
		}

		break;
	}
	case 256: {
		VMem256ByteUnit *Unit = _SearchValidAllocator<256>(_256ByteProxy, Size);
		if (Unit != nullptr) {
			Ptr = Unit->Allocate(Size);
			OOM = false;
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
			switch (ByteCount) {
			case 4: {
				_DealyOOM<4>(_4ByteProxy, Size);

				break;
			}
			case 8: {
				_DealyOOM<8>(_8ByteProxy, Size);

				break;
			}
			case 16: {
				_DealyOOM<16>(_16ByteProxy, Size);

				break;
			}
			case 32: {
				_DealyOOM<32>(_32ByteProxy, Size);

				break;
			}
			case 64: {
				_DealyOOM<64>(_64ByteProxy, Size);

				break;
			}
			case 128: {
				_DealyOOM<128>(_128ByteProxy, Size);

				break;
			}
			case 256: {
				_DealyOOM<256>(_256ByteProxy, Size);

				break;
			}
			}

			goto LAllocate;
		}
	}

	return Ptr;
}
void VMemoryPool::_FreeMemory(void *Ptr, const size_t &Size) {
	bool Flag = false;

	Flag = _FreeBlock<4>(Ptr, _4ByteProxy, Size);
	if (Flag) {
		return;
	}
	Flag = _FreeBlock<8>(Ptr, _8ByteProxy, Size);
	if (Flag) {
		return;
	}
	Flag = _FreeBlock<16>(Ptr, _16ByteProxy, Size);
	if (Flag) {
		return;
	}
	Flag = _FreeBlock<32>(Ptr, _32ByteProxy, Size);
	if (Flag) {
		return;
	}
	Flag = _FreeBlock<64>(Ptr, _64ByteProxy, Size);
	if (Flag) {
		return;
	}
	Flag = _FreeBlock<128>(Ptr, _128ByteProxy, Size);
	if (Flag) {
		return;
	}

	Flag = _FreeBlock<256>(Ptr, _256ByteProxy, Size);

	if (!Flag) {
		_vdebug_handle UnknownMemoryBlock;
		UnknownMemoryBlock.crash("Unknown memory block was specified!");
	}

	return;
}