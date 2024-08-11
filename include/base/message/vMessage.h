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
 * \file vMessage.h
 * \brief The message type in the VUILib
 */

#pragma once

#include <include/base/geometry/vPoint.h>
#include <include/base/geometry/vRect.h>

#include <include/renderer/vGLHeader.h>

/**
 * The type of the message
 */
enum class VMessageType {
	MouseMove, MouseClicked, Repaint
};
/**
 * The button type of the mouse
 */
enum class VMouseButton {
	Right, Left, Middle
};
/**
 * The click method type of the button
 */
enum class VClickType {
	Release, Press, Repeat
};

/**
 * The base message class in the VUILib message system
 */
class VBaseMessage {
public:
	/**
	 * The alias for GLFW API
	 */
	using Window = GLFWwindow*;

public:
	/**
	 * Construct the base message with the message trigger window
	 * @param Trigger The trigger window, if the pointer is nullptr, meaning
	 * this message is spread in only one window which will not join the message
	 * broadcast
	 */
	explicit VBaseMessage(Window Trigger);
	virtual ~VBaseMessage() = default;

public:
	/**
	 * Cast this pointer into the specified type pointer
	 * @tparam Type The target type of the pointer
	 * @return The pointer in specified type format
	 */
	template<class Type>
		requires std::is_base_of_v<VBaseMessage, Type>
	Type* Cast() {
		return static_cast<Type*>(this);
	}

public:
	/**
	 * The pure virtual function for subclass to override
	 * @return The type of the message
	 */
	virtual VMessageType GetType() = 0;

public:
	/**
	 * Where window the message was triggered
	 */
	Window Target;
};

/**
 * When the user's mouse moved, this message will be triggered
 */
class VMouseMovedMessage : public VBaseMessage {
public:
	/**
	 * Construct the message with the message trigger window
	 * @param Trigger The trigger window, if the pointer is nullptr, meaning
	 * this message is spread in only one window which will not join the message
	 * broadcast
	 * @param IX The X of the mouse
	 * @param IY The Y of the mouse
	 */
	VMouseMovedMessage(Window Trigger, const int &IX, const int &IY);

public:
	VMessageType GetType() override {
		return VMessageType::MouseMove;
	}

public:
	VPoint Point;
};

/**
 * When the user's mouse clicked, the message wil be triggered
 */
class VMouseClickedMessage : public VBaseMessage {
public:
	/**
	 * Construct the message with the message trigger window
	 * @param Trigger The trigger window, if the pointer is nullptr, meaning
	 * this message is spread in only one window which will not join the message
	 * broadcast
	 * @param IX The X of the mouse
	 * @param IY The Y of the mouse
	 */
	VMouseClickedMessage(Window Trigger, const int &IX, const int &IY, const VMouseButton &IButton, const VClickType &IClick);

public:
	VMessageType GetType() override {
		return VMessageType::MouseClicked;
	}

public:
	VPoint       Point;
	VMouseButton Button;
	VClickType   Click;
};

/**
 * When the UI needs to be repainted, this message will be emitted
 */
class VRepaintMessage : public VBaseMessage {
public:
	/**
	 * Construct the message with the message trigger window
	 * @param Trigger The trigger window, if the pointer is nullptr, meaning
	 * this message is spread in only one window which will not join the message
	 * broadcast
	 * @param Rectangle The dirty rectangle of the repaint
	 */
	VRepaintMessage(Window Trigger, const VRect &Rectangle);

public:
	VMessageType GetType() override {
		return VMessageType::Repaint;
	}

public:
	/**
	 * The dirty rectangle to be repainted
	 */
	VRect DirtyRectangle;
};