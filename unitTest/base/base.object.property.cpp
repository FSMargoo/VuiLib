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
 * \file base.object.property.cpp
 * \brief The unit test module for object property module
 */

#include <unitTest/base/base.object.property.h>

bool VUnitTest(ObjectProperty, Create) {
	auto intDefaultPtr = std::make_unique<VIntProperty>();
	auto intValuePtr = std::make_unique<VIntProperty>(1);
	auto longDefaultPtr = std::make_unique<VLongProperty>();
	auto longValuePtr = std::make_unique<VLongProperty>(1);
	auto floatDefaultPtr = std::make_unique<VFloatProperty>();
	auto floatValuePtr = std::make_unique<VFloatProperty>(1);
	auto stringDefaultPtr = std::make_unique<VStringProperty>();
	auto stringValuePtr = std::make_unique<VStringProperty>("Hello, VUILib");
	auto rectDefaultPtr = std::make_unique<VRectProperty>();
	auto rectValuePtr = std::make_unique<VRectProperty>(VRect(0, 0, 100, 100));
	auto pointDefaultPtr = std::make_unique<VPointProperty>();
	auto pointValuePtr = std::make_unique<VPointProperty>(VPoint (0, 0));
	VPropertyList list;
	std::unique_ptr<VPropertyValueBase> ptrList[] = {
	        std::move(intDefaultPtr), std::move(intValuePtr), std::move(longDefaultPtr),
	        std::move(longValuePtr), std::move(floatDefaultPtr), std::move(floatValuePtr),
	        std::move(stringDefaultPtr), std::move(stringValuePtr), std::move(rectDefaultPtr),
	        std::move(rectValuePtr), std::move(pointDefaultPtr), std::move(pointValuePtr)
	};
	for (auto count : VRange<int>(0, 12)) {
		auto name = std::to_string(count);
		list.insert({name, VObjectProperty(name, std::move(ptrList[count]))});
	}

	return true;
}
bool VUnitTest(ObjectProperty, Read) {
	__VObjectTestObject object;
	auto& defaultProperty = object.GetProperty("intDefault");
	auto& valueProperty = object.GetProperty("intValue");
	return defaultProperty.GetValue()->Cast<VIntProperty>()->GetValue() == 0 && valueProperty.GetValue()->Cast<VIntProperty>()->GetValue() == 1;
}
bool VUnitTest(ObjectProperty, MemoryLeak) {
	std::unique_ptr<VObjectProperty> property;
	{
		auto intValue = std::make_unique<VIntProperty>(203);
		auto& value = reinterpret_cast<std::unique_ptr<VPropertyValueBase>&>(intValue);
		property = std::make_unique<VObjectProperty>("leakTest", std::move(value));
	}
	return property->GetValue()->Cast<VIntProperty>()->GetValue() == 203;
}