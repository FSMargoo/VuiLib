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
 * \file vGLFWCallback.cpp
 * \brief The callback function wrapper
 */

#include <include/widget/vGLFWCallback.h>

#include <unordered_map>

std::unordered_map<GLFWwindow *, VGLFWWidget *> ObjectMapping;

void VGLFWRegisterObject(VGLFWWidget *Widget, GLFWwindow *Window) {
	ObjectMapping.insert({Window, Widget});
}
void VGLFWLayoutRearrangingCallback(GLFWwindow *Window, int Width, int Height) {
	ObjectMapping[Window]->OnGLFWLayoutRearranging(Width, Height);
}
void VGLFWFramebufferSizeCallback(GLFWwindow *Window, int Width, int Height) {
	ObjectMapping[Window]->OnGLFWRepaint(Width, Height);
}
void VGLFWMouseMoveCallback(GLFWwindow *Window, double X, double Y) {
	ObjectMapping[Window]->OnGLFWMouseMove(static_cast<int>(X), static_cast<int>(Y));
}
void VGLFWMouseClickCallback(GLFWwindow *Window, int Button, int Action, int Mods) {
	int X;
	int Y;
	glfwGetWindowPos(Window, &X, &Y);
	ObjectMapping[Window]->OnGLFWMouseClick(X, Y, Button, Action, Mods);
}