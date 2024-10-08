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
 * \file vGLFWException.h
 * \brief The type exception for GLFW
 */

#pragma once

#include <format>
#include <stdexcept>

class VGLFWFailure : public std::exception {
public:
	VGLFWFailure(const char *Message) : std::exception() {
		printf("VUILib Exception : GLFW Failure %s", Message);
	}
};
class VGLFWInstanceCreationFailed : public std::exception {
public:
	VGLFWInstanceCreationFailed(const char *InstanceType) : std::exception() {
		const auto message = std::format("Failed to create GLFW instance(type:{})", InstanceType);
		printf("VUILib Exception : GLFW Failure. %s", message.c_str());
	}
};
class VGLFWInvalidInstance : public std::exception {
public:
	VGLFWInvalidInstance(const char *InstanceType) : std::exception() {
		const auto message = std::format("Invalid instance(type:{})", InstanceType);
		printf("VUILib Exception : GLFW Failure. %s", message.c_str());
	}
};