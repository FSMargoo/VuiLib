/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any personbe be  obtaining a copy
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
 * \file vWidget.h
 * \brief A OOP wrapper of GLFW API for VUILib native window
 */

#pragma once

#include <include/widget/vGLFWCallback.h>
#include <include/base/object/vObject.h>
#include <include/base/event/vEvent.h>
#include <include/app/vApplication.h>
#include <include/widget/vMonitor.h>
#include <include/mvvm/vViewBase.h>

#include <include/renderer/vGLHeader.h>

#include <queue>

/**
 * This class is a wrapper of GLFW monitor API, which will
 * maintain a GLFWwindow pointer, and convert the GLFW
 * event into VUILib VEvent
 */
class VWidget : public VObject, VGLFWWidget {
public:
	/**
	 * Construct the widget with only geometry information
	 * @param Application The application instance of the widget
	 * @param Width The width of the widget
	 * @param Height The height of the widget
	 * @param Title The title of the widget
	 */
	VWidget(VApplication *Application, const int &Width, const int &Height, const std::string &Title);
	/**
	 * Create a widget with geometry information on a specified monitor,
	 * it will create a full-screen window by the specified geometry information.
	 * @param Application The application instance of the widget
	 * @param Width The width of the widget
	 * @param Height The height of the widget
	 * @param Title The title of the widget
	 * @param Monitor The specified monitor where window to be shown
	 */
	VWidget(VApplication *Application, const int &Width, const int &Height, const std::string &Title, VMonitor &Monitor);
	~VWidget() override;

public:
	/**
	 * Display the window on the monitor
	 */
	void Show() override;
	/**
	 * Hide the window on the monitor
	 */
	void Hide() override;
	/**
	 * Adjust the window size with the geometry information
	 * @param Width The width of the window
	 * @param Height The height of the window
	 */
	void Resize(const int &Width, const int &Height) override;

public:
	/**
	 * Get the title of the widget
	 * @return The title of the widget
	 */
	[[nodiscard]] std::string GetTitle() const;
	/**
	 * Set the title of the object
	 * @param Title The new title of the object
	 */
	void SetTitle(const std::string &Title);

public:
	/**
	 * Execute the loop message for the widget. With this function,
	 * you can run the widget without the application. When the
	 * widget is closed, this function will stop
	 */
	void Execute();

public:
	void OnGLFWRepaint(const int &Width, const int &Height) override;
	void OnGLFWMouseMove(const int &X, const int &Y) override;
	void OnGLFWMouseClick(const int &X, const int &Y, const int &Button, const int &Action, const int &Mods) override;

protected:
	void RaiseUpAsFocus(VObject *Object) override;
	void LockFocus() override;
	void UnlockFocus() override;
	bool GetFocus() override;

private:
	/**
	 * When the widget needs to be repainted, this function will be called
	 * @param Message The repaint message
	 */
	void OnWidgetRepaint(VRepaintMessage *Message);

private:
	void OnFinalMessage(VBaseMessage *Message) override;
	void OnPaint(sk_sp<VSurface> &Surface) override;

private:
	/**
	 * Process the message
	 * @param Message The message to be processed
	 */
	void ProcessMessage(VBaseMessage *Message);

private:
	/**
	 * Initialize the widget object information
	 * @param Width The width of the widget
	 * @param Height The height of the widget
	 * @param Title The title of the widget
	 */
	void InitWidgetObject(const int &Width, const int &Height, const std::string &Title);

private:
	/**
	 * Flush the widget's context, equaling send a repaint message
	 * in the full widget
	 */
	void FlushWidget();

protected:
	GLFWwindow *_glfwWindow;

protected:
	VStringProperty *_title;

private:
	bool             _focusLocking;
	VObject         *_focusingObject;
	VApplication    *_application;

private:
	sk_sp<VRenderInterface> GLInterface;

private:
	VColorProperty *_windowBackgroundColor;
};