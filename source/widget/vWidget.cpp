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
    : VObject(nullptr), _application(Application), _focusingObject(nullptr), _focusLocking(false) {
	_glfwWindow = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);
	if (!_glfwWindow) {
		throw VGLFWFailure("Failed to create GLFW window!");
	}

	InitWidgetObject(Width, Height, Title);
}
VWidget::VWidget(VApplication *Application, const int &Width, const int &Height, const std::string &Title, VMonitor& Monitor)
	: VObject(nullptr), _application(Application), _focusingObject(nullptr), _focusLocking(false) {
	/**
	 * If the monitor is disconnected, it will be marked as invalid
	 */
	if (Monitor._status == VMonitorStatus::Invalid) {
		throw VGLFWInvalidInstance("VMonitor");
	}

	_glfwWindow = glfwCreateWindow(Width, Height, Title.c_str(), Monitor._glfwMonitor, nullptr);
	if (!_glfwWindow) {
		throw VGLFWFailure("Failed to create GLFW window!");
	}

	InitWidgetObject(Width, Height, Title);
}
VWidget::~VWidget() {
	if (!_glfwWindow) {
		glfwDestroyWindow(_glfwWindow);
	}
}
void VWidget::FlushWidget() {
	auto flushMessage = std::make_unique<VRepaintMessage>(_glfwWindow, VRect{ 0, 0, _bound->_value.GetWidth(), _bound->_value.GetHeight() });
	ProcessMessage(flushMessage.get());
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
	glfwSetCursorPosCallback(_glfwWindow, VGLFWMouseMoveCallback);

	FlushWidget();
}
void VWidget::Show() {
	glfwShowWindow(_glfwWindow);

	VObject::Show();
}
void VWidget::Hide() {
	glfwHideWindow(_glfwWindow);

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
void VWidget::Execute() {
	while (glfwWindowShouldClose(_glfwWindow)) {
		glfwPollEvents();
	}
}
void VWidget::RaiseUpAsFocus(VObject *Object) {
	if (!_focusLocking) {
		_focusingObject = Object;
	}
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
void VWidget::LockFocus() {
	_focusLocking = true;
}
void VWidget::UnlockFocus() {
	_focusLocking = false;
}
bool VWidget::GetFocusLocking() {
	return _focusLocking;
}
void VWidget::OnGLFWRepaint(const int &Width, const int &Height) {
	if (_bound->_value.GetWidth() != Width || _bound->_value.GetHeight() != Height) {
		VObject::Resize(Width, Height);
	}

	auto repaintMessage = std::make_unique<VRepaintMessage>(_glfwWindow, VRect{ 0, 0, _bound->_value.GetWidth(), _bound->_value.GetHeight() });
	OnWidgetRepaint(repaintMessage.get());
}
void VWidget::OnGLFWMouseMove(const int &X, const int &Y) {
	auto mouseMoveMessage = std::make_unique<VMouseMovedMessage>(_glfwWindow, X, Y);
	ProcessMessage(mouseMoveMessage.get());
}
void VWidget::OnGLFWMouseClick(const int &X, const int &Y, const int &Button, const int &Action, const int &Mods) {
	VMouseButton button;
	VClickType   clickType;
	VAdditionKey additionKey;
	switch (Button) {
		case GLFW_MOUSE_BUTTON_LEFT: {
			button = VMouseButton::Left;

			break;
		}
		case GLFW_MOUSE_BUTTON_RIGHT: {
			button = VMouseButton::Right;

			break;
		}
		case GLFW_MOUSE_BUTTON_MIDDLE: {
			button = VMouseButton::Middle;

			break;
		}
	}
	switch (Action) {
		case GLFW_RELEASE: {
			clickType = VClickType::Release;

			break;
		}
		case GLFW_REPEAT: {
			clickType = VClickType::Repeat;

			break;
		}
		case GLFW_PRESS: {
			clickType = VClickType::Press;

			break;
		}
	}
	switch (Mods) {
		case GLFW_MOD_ALT: {
			additionKey = VAdditionKey::Alt;
			break;
		}
		case GLFW_MOD_CONTROL: {
			additionKey = VAdditionKey::Ctrl;
			break;
		}
		case GLFW_MOD_CAPS_LOCK: {
			additionKey = VAdditionKey::CapsLock;
			break;
		}
		case GLFW_MOD_NUM_LOCK: {
			additionKey = VAdditionKey::NumberLock;
			break;
		}
		case GLFW_MOD_SHIFT: {
			additionKey = VAdditionKey::Shift;
			break;
		}
		case GLFW_MOD_SUPER: {
			additionKey = VAdditionKey::Super;
			break;
		}
	}

	auto mouseClickMessage = std::make_unique<VMouseClickedMessage>(_glfwWindow, X, Y, button, clickType);
	mouseClickMessage->AdditionKey = additionKey;
	ProcessMessage(mouseClickMessage.get());
}
void VWidget::ProcessMessage(VBaseMessage *Message) {
	sk_sp<VSurface> nullSurface = nullptr;
	if (Message->GetType() != VMessageType::Repaint) {
		OnMessage(Message, nullSurface);
	}
	else {
		OnWidgetRepaint(Message->Cast<VRepaintMessage>());
	}
}
void VWidget::OnFinalMessage(VBaseMessage *Message) {
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

	ProcessMessage(Message);
}