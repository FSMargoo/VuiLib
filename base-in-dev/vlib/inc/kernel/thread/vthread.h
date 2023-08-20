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

#include "vmemory.h"

#include <thread>

template <class ReturnType, class... Argument>
class VThread {
public:
	explicit VThread(VMemoryPool &ParentMemPool, const std::function<ReturnType(Argument...)> &Function,
					 Argument... FunctionArgument)
		: Thread(Function, FunctionArgument...), Cache(ParentMemPool, 1024) {
	}
	void Detch() noexcept {
		Thread.detach();
	}
	void Join() noexcept {
		Thread.join();
	}
	constexpr bool Joinble() noexcept {
		return Thread.joinable();
	}
	std::thread::id GetId() noexcept {
		return Thread.get_id();
	}
	std::stop_source GetStopResource() noexcept {
		return Thread.get_stop_source();
	}
	std::stop_token GetStopToken() noexcept {
		return Thread.get_stop_token();
	}
	std::thread::native_handle_type GetNativeHandle() noexcept {
		return Thread.native_handle();
	}

private:
	std::jthread Thread;
	VThreadCache Cache;
};

class _VThreadMemoryPoolManager {
public:
	_VThreadMemoryPoolManager();

private:
	_VMem::VRBTree<std::thread::id, VThreadCache *, VMemoryPolicy> PoolMap;
};