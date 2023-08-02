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
#include "kernel/mem/vmemorypolicy.h"

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
		size_t					   Offset = 0;
		VMemoryUnit<AllocateSize> *Ptr	  = Next;

		while (Offset < Size) {
			Ptr->InUsed = true;
			Ptr			= Ptr->Next;

			++Offset;
		}

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

using VMem4BitUnit	= VMemoryUnit<4>;
using VMem8BitUnit	= VMemoryUnit<8>;
using VMem16BitUnit = VMemoryUnit<16>;
using VMem32BitUnit = VMemoryUnit<32>;

class VMemoryPool : public VMemoryInterface {
public:
	VMemoryPool(VMemoryPolicy ManagerPolicy = VMemoryPolicy::Default());

public:
	void OOMPanic() override;

private:
	__forceinline void _InitMemoryPool();
	template <size_t BitSize>
	__forceinline void _InitMemoryBlock(void *Block, VMemoryUnit<BitSize> *ListHead, size_t TotalSize) {
		size_t				  Offset = BitSize;
		VMemoryUnit<BitSize> *Last	 = ListHead;
		VMemoryUnit<BitSize> *Ptr	 = ListHead;
		while (Offset < TotalSize) {
			Ptr->Next = new VMemoryUnit<BitSize>(Last, ((char *&)(Block)) + Offset);

			Last = Ptr->Next;
			Ptr	 = Ptr->Next;

			Offset += BitSize;
		}
	}
	__forceinline bool _CheckPolicyLegit();

private:
	__forceinline [[nodiscard]] void *_AllocateMemory(const size_t &Size);
	template <size_t BitSize>
	void _DealyOOM(VMemoryUnit<BitSize> *Proxy) {
		VMemoryUnit<BitSize> *Ptr  = Proxy->Next;
		VMemoryUnit<BitSize> *Prev = Proxy;
		while (Ptr != nullptr) {
			Prev = Ptr;
			Ptr	 = Ptr->Next;
			if (Ptr == nullptr && Ptr->NextStorage != nullptr) {
				Prev = Ptr->NextStorage;
				Ptr	 = Ptr->NextStorage->Next;
			}
		}

		size_t ExpandSize = 0;
		switch (Policy.ExpandMode) {
		case VMemoryExpandMode::UserDefine: {
			ExpandSize = Policy.UserFunction();
			if (ExpandSize == 0) {
				_vdebug_handle ZeroSizeExpand;
				ZeroSizeExpand.crash("Can not expand 0 size.")
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
	}
	template <size_t BitSize>
	[[nodiscard]] VMemoryUnit<BitSize> *_SearchValidAllocator(VMemoryUnit<BitSize> *Head, const size_t &TargetSize) {
		VMemoryUnit<BitSize> *Ptr = Head;

		while (Ptr != nullptr) {
			while (Ptr->InUsed) {
				Ptr = Ptr->Next;
			}

			VMemoryUnit<BitSize> *Pointer	= Ptr;
			size_t				  RestCount = 1;
			while (Ptr != nullptr && !Ptr->InUsed) {
				Ptr = Ptr->Next;

				++RestCount;
				if (RestCount * BitSize >= TargetSize) {
					return Pointer;
				}
			}
		}

		// OOM Error
		return nullptr;
	}
	template <size_t BitSize>
	void _Reallocate() {
	}

private:
	VMem4BitUnit  *_4BitProxy;
	VMem8BitUnit  *_8BitProxy;
	VMem16BitUnit *_16BitProxy;
	VMem32BitUnit *_32BitProxy;

private:
	VMemoryPolicy Policy;

private:
	void *_4Bit;
	void *_8Bit;
	void *_16Bit;
	void *_32Bit;
	bool  OOM;
};