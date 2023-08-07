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

#include <stdlib.h>
#include <typeinfo>

#include "kernel/debug/vdebug.h"

enum class VCompareOperation {
	Lt,
	Gt
};

template <class Left, class Right>
constexpr bool VCompare(const Left &LeftValue, const Right &RightValue, const VCompareOperation &Op) noexcept {
	if constexpr (std::is_signed_v<Left> == std::is_signed_v<> Right) {
		if (Op == VCompareOperation::Lt) {
			return LeftValue < RightValue;
		} else {
			return LeftValue > RightValue;
		}
	} else if constexpr (std::is_signed_v<Left>) {
		return Left < 0 ? true : std::make_unsigned_t<Left>(LeftValue) < RightValue;
	} else {
		return Right < 0 ? true : LeftValue < std::make_unsigned_t<Left>(RightValue);
	}

	return false;
}