/*
 * VMemory.hpp
 *	@description : A Simple Memory Toolset
 *	@brith		 : 2022/6.3
*/

#pragma once

#include "vbase.hpp"

VLIB_BEGIN_NAMESPACE

#ifdef VLIB_DONT_USE_CPP_MEMORY
#include <utility>

/*
 * VMemoryDeleter class:
 *	@description : The Default Deleter In VMemoryPtr
*/
template<class _Type>
class VMemoryDeleter {
public:
	void do_delete(_Type* ptr) {
		delete ptr;
	}
};

/*
 * VMemoryPtr class:
 *	@description : The Complex Implementation Of std::unqiue_ptr
*/
template<class _Type, class _Deleter = VMemoryDeleter<_Type>>
class VMemoryPtr {
private:
	/* Memory Data */

	_Type* ptr;

public:
	VMemoryPtr(_Type* _Ptr) {
		ptr = _Ptr;
	}
	VMemoryPtr() {
		ptr = nullptr;
	}

	/*
	 * Operator -> :
	 *	@description : A way to access the Ptr
	*/
	_Type* operator->() {
		return get();
	}

	/*
	 * Reset Functional:
	 *	@description : Reset the Refer of Memory Ptr
	*/
	void Reset(_Type* _Ptr) {
		/* If the resource ptr is not a nullptr, delete it */
		if (ptr != nullptr) {
			_Deleter DeleterApplication;
			DeleterApplication.do_delete(_Ptr);
		}

		ptr = _Ptr;
	}

	/*
	 * Get Functional:
	 *	@description  : Get the Refer of Memory Ptr
	 *	@return value : The Memory Ptr
	*/
	_Type* Get() {
		return _Ptr;
	}

	/*
	 * The Compatible Interface
	*/

	void   reset(_Type* _Ptr) { Reset(std::forward(_Ptr)); }
	_Type* get() { return Get(); }
};
#else
#	include <memory>
#	define VMemoryPtr std::unique_ptr
#endif

VLIB_END_NAMESPACE