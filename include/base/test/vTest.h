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
 * \file vTest.h
 * \brief The test library
 */

#pragma once

#include <base/event/vEvent.h>
#include <base/vBase.h>

#include <functional>

#define VUnitTest(MODULE, X)  __VTest_MARCO_##MODULE##X()
#define VUnitTestM(MODULE, X) __VTest_MARCO_##MODULE##X

/***
 * The test task class
 */
class VTestTask {
public:
	VTestTask(std::function<bool()> Function, const std::string &TaskName);

public:
	/***
	 * Start test
	 * @return Whether the test task failed
	 */
	[[nodiscard]] bool Conduct() const {
		return _task();
	}

private:
	std::function<bool()> _task;

public:
	std::string TaskID;
};

/***
 * The test conductor class
 */
class VTestConductor {
public:
	VTestConductor() = default;

public:
	/***
	 * Add testing task
	 * @param Task The test task
	 */
	void AddTask(const VTestTask &Task);
	/***
	 * Start the test task
	 */
	void StartTasks();

private:
	std::vector<VTestTask> _taskList;
};