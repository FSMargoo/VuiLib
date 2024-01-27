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

int mem_test() {
	VMemoryPool	 TestMPool;
	VThreadCache Cache(TestMPool, 1024);
	const int	*A = Cache.Allocate<const int>((const int &)8);
	const int	*B = Cache.Allocate<const int>(9);
	const int	*C = Cache.Allocate<const int>(10);
	int			*D;
	for (int i = 0; i < 1145; ++i) {
		D = Cache.Allocate<int>(80);
	}

	printf("%d\n%d\n%d\n%d", *A, *B, *C, *D);

	return 0;
}

int main() {
	return mem_test();
}