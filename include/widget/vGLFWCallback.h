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
 * \file vGLFWCallback.h
 * \brief The callback function wrapper
 */

#pragma once

#include <include/renderer/vGLHeader.h>

/**
 * The base GLFW callback base class for GLFW callback function to be called
 */
class VGLFWWidget {
public:
	// "Repainting" dose not means "RearrangeLayout Rearranging" but "RearrangeLayout Rearranging" means "Repainting"
	virtual void OnGLFWLayoutRearranging(const int &Width, const int &Height) = 0;
	virtual void OnGLFWRepaint(const int &Width, const int &Height)			  = 0;
	virtual void OnGLFWMouseMove(const int &X, const int &Y)				  = 0;
	virtual void OnGLFWMouseClick(const int &X, const int &Y, const int &Button, const int &Action,
								  const int &Mods)							  = 0;
};

void VGLFWRegisterObject(VGLFWWidget *Widget, GLFWwindow *Window);
void VGLFWLayoutRearrangingCallback(GLFWwindow *Window, int Width, int Height);
void VGLFWFramebufferSizeCallback(GLFWwindow *Window, int Width, int Height);
void VGLFWMouseMoveCallback(GLFWwindow *Window, double X, double Y);
void VGLFWMouseClickCallback(GLFWwindow *Window, int Button, int Action, int Mods);