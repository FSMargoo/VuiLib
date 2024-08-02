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
 * \file base.object.property.h
 * \brief The unit test module for object property module
 */

#pragma once

#include <include/base/object/vObjectProperty.h>
#include <include/base/object/vObject.h>
#include <include/base/test/vTest.h>

class __VObjectTestObject : public VObject {
public:
	__VObjectTestObject() {
		intDefaultPtr = std::make_unique<VIntProperty>();
		intValuePtr = std::make_unique<VIntProperty>(1);

		RegisterProperty<VIntProperty>("intDefault", intDefaultPtr);
		RegisterProperty<VIntProperty>("intValue", intValuePtr);
	}

public:
	void OnPaint(const sk_sp<VSurface> &Surface) override {

	}

private:
	std::unique_ptr<VIntProperty> intDefaultPtr;
	std::unique_ptr<VIntProperty> intValuePtr;
};

bool VUnitTest(ObjectProperty, Create);
bool VUnitTest(ObjectProperty, Read);
