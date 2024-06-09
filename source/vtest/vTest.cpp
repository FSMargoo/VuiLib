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
 * \file vTest.cpp
 * \brief The test library
 */

#include <base/test/vTest.h>
#include <chrono>
#include <ctime>
#include <format>
#include <thread>
#include <utility>

VTestTask::VTestTask(std::function<bool()> Function, std::string TaskName)
	: _task(std::move(Function)), TaskID(TaskName) {
}
void VTestConductor::AddTask(const VTestTask &Task) {
	_taskList.push_back(Task);
}
void VTestConductor::StartTasks() {
	auto const time = std::chrono::current_zone()
						  ->to_local(std::chrono::system_clock::now());
	printf("%s", std::format("Test start at time {:%Y-%m-%d %X}\n", time).c_str());

	bool globalFlag = false;
	for (VTestTask task : _taskList) {
		printf("%s", std::format("Task [{}] ", task.TaskID).c_str());

		bool		flag = false;
		std::thread taskThread([task, &flag]() mutable -> void { flag = task.Conduct(); });

		taskThread.join();

		if (flag) {
			printf("passed! \t[√]\n");
		} else {
			globalFlag = true;
			printf("failed! \t[×]\n");
		}
	}

	if (globalFlag) {
		printf("[WARNING] : You may using a VUILib which IS NOT STABLE, this VUILib version CAN NOT PASS UNIT TEST on your PC.\n"
			   "If it happened on a release version of VUILib please contact us with GitHub issue or the email <margoo@margoo.icu>.\n"
			   "Please provide us with above information.B");
	}
	else {
		printf("[INFO] : You are using a stable version of VUILib which passed all unit tests");
	}
}