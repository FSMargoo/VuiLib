/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * UserDefine:
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
 * \file : The constant pool def
 */

#pragma once

#include "kernel/mem/vmemrbtree.h"

/**
 * \brief The constant pool of one type
 * \tparam Type The type need to be constant pool collected
 * \tparam Allocator The allocator type
 */
template <class Type, class Allocator>
class VConstantPool {
public:
	explicit VConstantPool(Allocator &BindPool) : Pool(BindPool), Mapping(BindPool) {
	}
	void *FindOrInsert(const Type &Value) {
		if (Mapping.Exists(Value)) {
			return Mapping.Find(Value);
		}

		Mapping.Insert(Value, Pool.template Allocate<Type>(Value));
		return Mapping.Find(Value);
	}

private:
	Allocator							   &Pool;
	_VMem::VRBTree<Type, void *, Allocator> Mapping;
};

/**
 * \brief The constant pool extractor
 * \tparam Allocator The allocator type
 */
template <class Allocator>
struct VConstantExtractor {
	using IntPool	 = VConstantPool<int, Allocator>;
	using DoublePool = VConstantPool<double, Allocator>;
	using FloatPool	 = VConstantPool<float, Allocator>;
};