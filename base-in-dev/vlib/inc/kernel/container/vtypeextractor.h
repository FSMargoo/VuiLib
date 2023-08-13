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
 * \file vtypeextractor.h
 * \brief This file contains the definition of the VTypeExtractor struct.
 */

#pragma once

/**
 * \struct VTypeExtractor
 * \brief The VTypeExtractor struct is a template struct that extracts various type definitions from a given type.
 *
 * It provides typedefs for pointer, reference, copy reference, const type, const pointer, and const reference of the
 * given type.
 */
template <class Type>
struct VTypeExtractor {
	using Pointer	   = typename Type *;  ///< Pointer to Type
	using Refer		   = typename Type &;  ///< Reference to Type
	using CopyRef	   = typename Type &&; ///< Copy reference (rvalue reference) to Type
	using ConstType	   = const Type;	   ///< Const Type
	using ConstPointer = const Type *;	   ///< Pointer to const Type
	using ConstRefer   = const Type &;	   ///< Reference to const Type
};