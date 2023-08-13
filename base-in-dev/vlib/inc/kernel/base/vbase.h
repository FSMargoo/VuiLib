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

/**
 * \file vbase.h
 * \brief This file contains the definition of the VCompareOperation enum and the VCompare function.
 */

#pragma once

/* Include necessary libraries */
#include <cstdlib>
#include <typeinfo>

/* Include debug header file */
#include <kernel/debug/vdebug.h>

/**
 * \enum VCompareOperation
 * \brief An enum class for comparison operations.
 */
enum class VCompareOperation {
	Lt, ///< Less than
	Gt	///< Greater than
};

/**
 * \brief A template function for comparing two values.
 * \param LeftValue The left value to compare.
 * \param RightValue The right value to compare.
 * \param Op The comparison operation to perform.
 * \return A boolean value indicating the result of the comparison.
 */
template <class Left, class Right>
constexpr bool VCompare(const Left &LeftValue, const Right &RightValue, const VCompareOperation &Op) noexcept {
	// If both values are of the same sign
	if constexpr (std::is_signed_v<Left> == std::is_signed_v<Right>) {
		// If operation is less than
		if (Op == VCompareOperation::Lt) {
			return LeftValue < RightValue;
		} else { // If operation is greater than
			return LeftValue > RightValue;
		}
	} else if constexpr (std::is_signed_v<Left>) { // If LeftValue is signed
		return LeftValue < 0 || std::make_unsigned_t<Left>(LeftValue) < RightValue;
	} else { // If RightValue is signed
		return RightValue < 0 || LeftValue < std::make_unsigned_t<Left>(RightValue);
	}

	// Default return value
	return false;
}