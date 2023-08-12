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

template <class Type>
class VInitList {
public:
	VInitList() noexcept : First(nullptr), Tail(nullptr) {
	}
	VInitList(const Type *FirstPtr, const Type *EndPtr) : First(FirstPtr), Tail(EndPtr) {
	}

public:
	constexpr const Type *Begin() noexcept {
		return First;
	}
	constexpr const Type *End() noexcept {
		return Tail;
	}
	constexpr size_t Size() noexcept {
		return Tail - First;
	}

public:
	constexpr const Type *begin() noexcept {
		return First;
	}
	constexpr const Type *end() noexcept {
		return Tail;
	}

private:
	const Type *First;
	const Type *Tail;
};
