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
#include <third/SimpleJson/sJSON.h>
#include <third/winToast/include/wintoastlib.h>

#include <fstream>
#include <chrono>
#include <ctime>
#include <format>
#include <thread>
#include <utility>

class CustomHandler : public WinToastLib::IWinToastHandler {
public:
	void toastActivated() const {
	}

	void toastActivated(int actionIndex) const {
	}

	void toastDismissed(WinToastDismissalReason state) const {
		switch (state) {
		case UserCanceled:
			exit(0);

			break;
		case TimedOut:
			exit(0);

			break;
		case ApplicationHidden:
			exit(0);

			break;
		default:
			break;
		}
	}

	void toastFailed() const {
	}
};

VTestTask::VTestTask(std::function<bool()> Function, const std::string &TaskName)
	: _task(std::move(Function)), TaskID(TaskName) {
}
void VTestConductor::AddTask(const VTestTask &Task) {
	_taskList.push_back(Task);
}
void VTestConductor::StartTasks() {
	using namespace WinToastLib;

	auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
	printf("%s", std::format("Test started at time {:%Y-%m-%d %X}\n", time).c_str());

	bool globalFlag	  = false;
	long successCount = 0;
	long failedCount  = 0;

	sJSONElementNode *elementNode = new sJSONElementNode;
	sJSONArray		 *arrayNode	  = new sJSONArray;

	for (VTestTask task : _taskList) {
		printf("%s", std::format("Task [{}] ", task.TaskID).c_str());

		bool flag = false;
		std::thread taskThread([task, &flag]() mutable -> void { flag = task.Conduct(); });

		taskThread.join();

		sJSONElementNode *subElement = new sJSONElementNode;
		subElement->InsertChildrenNode(new sJSONElementNode("taskID", new sJSONRealValue<std::string>(task.TaskID)));
		subElement->InsertChildrenNode(new sJSONElementNode("status", new sJSONRealValue<int>(flag ? 1 : 0)));
		arrayNode->ValueSet.push_back(subElement);

		if (flag) {
			printf("passed! \t[√]\n");
			++successCount;
		} else {
			globalFlag = true;
			printf("failed! \t[×]\n");
			++failedCount;
		}
	}

	elementNode->InsertChildrenNode(new sJSONElementNode("tests", arrayNode));
	sJSONRootNode root		  = sJSONRootNode(elementNode);
	std::string	  jsonContext = sJSONWriter::WriteJSON(root);

	printf("%s", std::format("All {} test(s) has been conducted. {} test(s) failed, {} test(s) passed\n",
							 _taskList.size(), failedCount, successCount)
					 .c_str());

	if (globalFlag) {
		printf("[WARNING] : You may using a VUILib which IS NOT STABLE, this VUILib version CAN NOT PASS UNIT TEST on "
			   "your PC.\n"
			   "If it happened on a release version of VUILib please contact us with GitHub issue or the email "
			   "<margoo@margoo.icu>.\n"
			   "Please provide us with above information.\n");
	} else {
		printf("[INFO] : You are using a stable version of VUILib which passed all unit tests\n");
	}

	printf("Creating testing result file (vtest.json)\n");

	std::ofstream stream("./vtest.json");
	stream << jsonContext;
	stream.close();

	printf("Successfully wrote testing result in vtest.json!\n");

	WinToastTemplate::AudioOption audioOption	 = WinToastTemplate::AudioOption::Default;
	std::wstring				  appName		 = L"VTest Conduct Result";
	std::wstring				  appUserModelID = L"VTest Notification";
	WinToast::instance()->setAppName(appName);
	WinToast::instance()->setAppUserModelId(appUserModelID);
	if (!WinToast::instance()->initialize()) {
		printf("Failed to create windows toast, the notification function will be unavailable!\n");
	}

	WinToastTemplate startupToast(WinToastTemplate::Text02);
	startupToast.setAudioOption(audioOption);
	if (globalFlag) {
		startupToast.setTextField(
			std::format(
				L"VTest tasks FAILED!\nWith {} test(s) has been conducted. {} test(s) failed, {} test(s) passed!\n"
				"You may using a VUILib which IS NOT STABLE, this VUILib version CAN NOT PASS UNIT TEST on your PC.\n"
				"If it happened on a release version of VUILib please contact us with GitHub issue or the email "
				"<margoo@margoo.icu>.\n"
				"Please provide us with the console information.",
				_taskList.size(), failedCount, successCount),
			WinToastTemplate::FirstLine);
		startupToast.setAttributionText(L"[FAILED!] Failed!");
	} else {
		startupToast.setTextField(
			std::format(
				L"VTest tasks finished!\nWith {} test(s) has been conducted. {} test(s) failed, {} test(s) passed!",
				_taskList.size(), failedCount, successCount),
			WinToastTemplate::FirstLine);
		startupToast.setAttributionText(L"[SUCCESS!] Finished!");
	}
	startupToast.setImagePath(L"");

	WinToast::instance()->showToast(startupToast, new CustomHandler());

	Sleep(5000);
}