// License(MIT)
// Athuor: Margoo
// This is the basic render file in vuilib
#pragma once

#include "../vbasiccore.h"

VLIB_BEGIN_NAMESPACE

// Connect DirectX 2D Library
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

namespace Core {

template<class ObjectType>
void VDXObjectSafeFree(ObjectType** Object) {
	if (*Object) {
		(*Object)->Release();

		*Object = NULL;
	}
}

}

VLIB_END_NAMESPACE