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
 * \file vWidget.cpp
 * \brief A OOP wrapper of GLFW API for VUILib native window
 */

#include <include/widget/vWidget.h>

VWidget::VWidget(const int &Width, const int &Height, const std::string& Title)
    : VObject(nullptr) {
	_glfwWindow = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);

	auto width  = std::make_unique<VIntProperty>(Width);
	auto height = std::make_unique<VIntProperty>(Height);
	auto title  = std::make_unique<VStringProperty>(Title);

	RegisterProperty("width", std::move(width));
	RegisterProperty("height", std::move(height));
	RegisterProperty("title", std::move(title));
}
VWidget::VWidget(const int &Width, const int &Height, const std::string &Title, VMonitor& Monitor) {
	/**
	 * If the monitor is disconnected, it will be marked as invalid
	 */
	if (Monitor._status == VMonitorStatus::Invalid) {
		throw VGLFWInvalidInstance("VMonitor");
	}

	_glfwWindow = glfwCreateWindow(Width, Height, Title.c_str(), Monitor._glfwMonitor, nullptr);
}
void VWidget::Show() {
	glfwSetWindowAttrib(_glfwWindow, GLFW_VISIBLE, GLFW_TRUE);
}
void VWidget::Hide() {
	glfwSetWindowAttrib(_glfwWindow, GLFW_VISIBLE, GLFW_FALSE);
}
void VWidget::Resize(const int &Width, const int &Height) {
	glfwSetWindowSize(_glfwWindow, Width, Height);

	VObject::Resize(Width, Height);
}