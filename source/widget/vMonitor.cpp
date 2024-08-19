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
 * \file vMonitor.cpp
 * \brief The monitor wrapper of GLFW
 */

#include <include/widget/vMonitor.h>

#include <stdexcept>

VMonitor::VMonitor(const int &Index) {
	glfwSetMonitorCallback(&VMonitor::GLFWMonitorCallback);
	_monitorOnEvent.Connect(this, &VMonitor::OnMonitor);

	int	 monitorCount;
	auto monitorList = glfwGetMonitors(&monitorCount);

	if (Index >= monitorCount) {
		_glfwMonitor   = nullptr;
		_glfwVideoMode = nullptr;
		_status		   = VMonitorStatus::Invalid;

		throw std::out_of_range("Monitor count out of range.");
	} else {
		_glfwMonitor   = monitorList[Index];
		_glfwVideoMode = glfwGetVideoMode(_glfwMonitor);
		_status		   = VMonitorStatus::Valid;
	}
}
VMonitor::VMonitor() {
	glfwSetMonitorCallback(&VMonitor::GLFWMonitorCallback);
	_monitorOnEvent.Connect(this, &VMonitor::OnMonitor);

	_glfwMonitor   = glfwGetPrimaryMonitor();
	_glfwVideoMode = glfwGetVideoMode(_glfwMonitor);

	if (!_glfwMonitor) {
		_status = VMonitorStatus::Invalid;

		throw VGLFWInstanceCreationFailed("GLFWmonitor");
	}
}
void VMonitor::OnMonitor(GLFWmonitor *Monitor, MonitorEventType Event) {
	if (Monitor == _glfwMonitor) {
		_status = Event == GLFW_CONNECTED ? VMonitorStatus::Valid : VMonitorStatus::Invalid;
	}
}
int VMonitor::GetWidth() const {
	return _glfwVideoMode->width;
}
int VMonitor::GetHeight() const {
	return _glfwVideoMode->height;
}
[[nodiscard]] VMonitorStatus VMonitor::GetStatus() const {
	return _status;
}