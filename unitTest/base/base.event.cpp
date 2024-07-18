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
 * \file base.event.cpp
 * \brief The base event test
 */

#include <unitTest/base/base.event.h>
bool __VTest_Flag = false;

void __VTest_Base_Event_Recall() {
	__VTest_Flag = true;
}

bool VUnitTest(BaseEvent, Recall) {
	__VTest_Flag = false;

	VEvent<> recallEvent;
	recallEvent.Connect(__VTest_Base_Event_Recall);
	recallEvent.Emit();

	return __VTest_Flag;
}
bool VUnitTest(BaseEvent, Block) {
	__VTest_Flag = false;

	VEvent<> recallEvent;
	recallEvent.Connect(__VTest_Base_Event_Recall);
	recallEvent.Block(__VTest_Base_Event_Recall);
	recallEvent.Emit();

	return !__VTest_Flag;
}
bool VUnitTest(BaseEvent, Remove) {
	__VTest_Flag = false;

	VEvent<> recallEvent;
	recallEvent.Connect(__VTest_Base_Event_Recall);
	recallEvent.Disconnect(__VTest_Base_Event_Recall);
	recallEvent.Emit();

	return !__VTest_Flag;
}

bool VUnitTest(BaseEventClass, Recall) {
	__VEventTestClass testClass;

	VEvent<> recallEvent;
	recallEvent.Connect(&testClass, &__VEventTestClass::RecallFunction);
	recallEvent.Emit();

	return testClass.Flag;
}
bool VUnitTest(BaseEventClass, Block) {
	__VEventTestClass testClass;

	VEvent<> recallEvent;
	recallEvent.Connect(&testClass, &__VEventTestClass::RecallFunction);
	recallEvent.Block(&testClass, &__VEventTestClass::RecallFunction);
	recallEvent.Emit();

	return !testClass.Flag;
}
bool VUnitTest(BaseEventClass, Remove) {
	__VEventTestClass testClass;

	VEvent<> recallEvent;
	recallEvent.Connect(&testClass, &__VEventTestClass::RecallFunction);
	recallEvent.Disconnect(&testClass, &__VEventTestClass::RecallFunction);
	recallEvent.Emit();

	return !testClass.Flag;
}