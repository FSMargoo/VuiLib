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

VWidget::VWidget(VApplication *Application, const int &Width, const int &Height, const std::string& Title)
    : VObject(nullptr), _application(Application) {
	_glfwWindow = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);

	InitWidgetObject(Width, Height, Title);
}
VWidget::VWidget(VApplication *Application, const int &Width, const int &Height, const std::string &Title, VMonitor& Monitor)
	: VObject(nullptr), _application(Application) {
	/**
	 * If the monitor is disconnected, it will be marked as invalid
	 */
	if (Monitor._status == VMonitorStatus::Invalid) {
		throw VGLFWInvalidInstance("VMonitor");
	}

	_glfwWindow = glfwCreateWindow(Width, Height, Title.c_str(), Monitor._glfwMonitor, nullptr);

	InitWidgetObject(Width, Height, Title);
}
void VWidget::FlushWidget() {
	auto flushMessage = new VRepaintMessage(_glfwWindow, VRect{ 0, 0, _bound->_value.GetWidth(), _bound->_value.GetHeight() });
	_messageQueue.push(flushMessage);
}
void VWidget::InitWidgetObject(const int &Width, const int &Height, const std::string &Title) {
	VObject::Resize(Width, Height);
	auto title  = std::make_unique<VStringProperty>(Title);

	RegisterProperty("title", std::move(title));
	_title = GetPropertyValue<VStringProperty>("title");

	_windowBackgroundColor = _application->GetThemePropertyValue<VColorProperty>("window.background.color");

	GLInterface = sk_make_sp<VRenderInterface>();

	VGLFWRegisterObject(this, _glfwWindow);

	glfwSetFramebufferSizeCallback(_glfwWindow, VGLFWFramebufferSizeCallback);

	FlushWidget();
	ProcessMessageQueue();
}
void VWidget::Show() {
	glfwSetWindowAttrib(_glfwWindow, GLFW_VISIBLE, GLFW_TRUE);

	VObject::Show();
}
void VWidget::Hide() {
	glfwSetWindowAttrib(_glfwWindow, GLFW_VISIBLE, GLFW_FALSE);

	VObject::Hide();
}
void VWidget::Resize(const int &Width, const int &Height) {
	glfwSetWindowSize(_glfwWindow, Width, Height);

	VObject::Resize(Width, Height);
}
std::string VWidget::GetTitle() const {
	return _title->_value;
}
void VWidget::SetTitle(const std::string &Title) {
	_title->_value = Title;

	glfwSetWindowTitle(_glfwWindow, Title.c_str());
}
void VWidget::OnWidgetRepaint(VRepaintMessage *Message) {
	glfwMakeContextCurrent(_glfwWindow);

	int width  = static_cast<int>(_bound->_value.GetWidth());
	int height = static_cast<int>(_bound->_value.GetHeight());

	sk_sp<VRenderTarget> glRenderTarget =
	        sk_make_sp<VRenderTarget, VRenderTargetViewport>({.Width = width, .Height = height, .X = 0, .Y = 0});
	sk_sp<VRenderContext> glContext = sk_make_sp<VRenderContext, const sk_sp<VRenderInterface> &>(GLInterface);
	sk_sp<VSurface>		  glSurface =
	        sk_make_sp<VSurface, const sk_sp<VRenderTarget> &, const sk_sp<VRenderContext> &>(glRenderTarget, glContext);
	OnPaint(glSurface);
	for (auto& object : _childList) {
		auto accessibleObject = dynamic_cast<VWidget*>(object);
		accessibleObject->OnMessage(Message, glSurface);
	}

	auto canvas = glSurface->GetNativeSurface()->getCanvas();
	glSurface->GetNativeSurface()->draw(canvas, 0, 0);
	glContext->GetNativeContext()->flushAndSubmit();

	glfwSwapBuffers(_glfwWindow);
}
void VWidget::OnPaint(sk_sp<VSurface> &Surface) {
	auto canvas = Surface->GetNativeSurface()->getCanvas();

	canvas->clear(_windowBackgroundColor->_value);
	canvas->flush();
}
void VWidget::OnGLFWRepaint(const int &Width, const int &Height) {
	if (_bound->_value.GetWidth() != Width || _bound->_value.GetHeight() != Height) {
		// VObject::Resize(Width, Height);
	}

	auto repaintMessage = std::make_unique<VRepaintMessage>(_glfwWindow, VRect{ 0, 0, _bound->_value.GetWidth(), _bound->_value.GetHeight() });
	OnWidgetRepaint(repaintMessage.get());
}
void VWidget::ProcessMessageQueue() {
	sk_sp<VSurface> nullSurface = nullptr;
	while (!_messageQueue.empty()) {
		if (_messageQueue.front()->GetType() != VMessageType::Repaint) {
			OnMessage(_messageQueue.front(), nullSurface);
		}
		else {
			OnWidgetRepaint(_messageQueue.front()->Cast<VRepaintMessage>());
		}

		delete _messageQueue.front();

		_messageQueue.pop();
	}
}
void VWidget::OnFinalMessage(VBaseMessage *Message) {
	_messageQueue.push(Message);

	if (Message->GetType() == VMessageType::Repaint) {
		auto repaintMessage = Message->Cast<VRepaintMessage>();
		/**
		 * We only need to fusion the dirty rectangle with the
		 * next level of the children objects
		 */
		for (auto &object : _childList) {
			// To access the bound of the object
			auto accessibleObject = static_cast<VWidget*>(object);
			if (repaintMessage->DirtyRectangle.IsOverlap(accessibleObject->_bound->_value)) {
				repaintMessage->DirtyRectangle.FusionWith(accessibleObject->_bound->_value);
			}
		}
	}
}