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

#include "kernel/mem/vconstantpool.h"
#include "kernel/mem/vmemory.h"
#include "kernel/mem/vmemrbtree.h"
#include "kernel/mem/vpointer.h"

class LifeTestClass {
public:
	LifeTestClass() {
		printf("Class Here!\n");

		value = 114;
	}
	~LifeTestClass() {
		printf("Class bye~\n");
	}

public:
	int value;
};

int mem_test() {
	VMemoryPool Pool;
	{
		VUniquePointer<LifeTestClass, VMemoryPool> Pointer =
			VUniquePointer<LifeTestClass, VMemoryPool>::MakeUnique(Pool);
		printf("%d\n", Pointer->value);
	}

	return 0;
}

int main() {
	return mem_test();
}