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
 * \file main.cpp
 * \brief A demo in VUILib for just a simple button creation
 */

#include <include/widget/vMainWindow.h>
#include <include/control/button/vPushButton.h>

int main() {
	VApplication application;
	VMainWindow  window(&application, 640, 480, "Hello, VUILib");

	VPushButton button(&window, 400, 400, "Hello World!");
	button.SetRichText(R"(<h2><font color="#23d96e" face="Times New Roman"><italic><h1>Hello</h1></italic>&nbsp;<bold>World</bold>&nbsp;<bold-italic>in</bold-italic>&nbsp;Rich&nbsp;Text!</font></h2>)");

	window.Show();

	return application.Run();
}