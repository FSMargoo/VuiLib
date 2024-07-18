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
 * \brief This is a demo which test the Skia wrapper of VUILib
 */

#include <include/renderer/vSurface.h>

#include <glfw/glfw3.h>

sk_sp<VRenderInterface> GLInterface;

#define WIDTH  640
#define HEIGHT 480

sk_sp<SkTextBlob> TextBlob;
SkPaint			 *Paint;
SkPaint			 *BlurPaint;

GLFWwindow *GLWindow;

/**
 * Init OpenGL interface object
 */
void InitGLInterface();
/**
 * Create GLFW window
 */
void InitWindow();
/**
 * Init Skia resource
 */
void InitResource();
/**
 * The frame buffer call back function
 */
void FrameBufferCallBack(GLFWwindow *Window, int Width, int Height);
/**
 * The error call back function of GLFW
 * @param Error The error code
 * @param Description The description string of the error
 */
void ErrorCallBack(int Error, const char *Description);
/**
 * Call Skia API to draw context
 */
void Draw(int Width, int Height);

int main() {
	InitWindow();
	InitResource();
	InitGLInterface();

	Draw(WIDTH, HEIGHT);
	while (!glfwWindowShouldClose(GLWindow)) {
		glfwPollEvents();
	}

	return 0;
}

void InitGLInterface() {
	GLInterface = sk_make_sp<VRenderInterface>();
}
void InitWindow() {
	glfwSetErrorCallback(ErrorCallBack);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	GLWindow = glfwCreateWindow(WIDTH, HEIGHT, "Skia Tester", nullptr, nullptr);
	if (!GLWindow) {
		printf("Failed to create GLFW window!");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetFramebufferSizeCallback(GLWindow, FrameBufferCallBack);

	glfwMakeContextCurrent(GLWindow);
}
void InitResource() {
	const SkScalar sigma	 = 3.65f;
	const SkScalar textSize	 = 40.0f;
	const uint8_t  blurAlpha = 127;
	auto		   face		 = SkTypeface::MakeFromName(
		   "Times New Roman", {SkFontStyle::kNormal_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kItalic_Slant});
	auto font = SkFont(face, textSize);
	TextBlob = SkTextBlob::MakeFromString("Hello VUILib, hello Skia,\r\n hello GLFW", SkFont(face, textSize), SkTextEncoding::kUTF8);
	Paint	 = new SkPaint();
	Paint->setAntiAlias(true);

	BlurPaint = new SkPaint(*Paint);

	BlurPaint->setAlpha(blurAlpha);
	BlurPaint->setMaskFilter(SkMaskFilter::MakeBlur(kNormal_SkBlurStyle, sigma, 0));
}
void FrameBufferCallBack(GLFWwindow *Window, int Width, int Height) {
	Draw(Width, Height);
}
void Draw(int Width, int Height) {
	sk_sp<VRenderTarget> glRenderTarget =
		sk_make_sp<VRenderTarget, VRenderTargetViewport>({.Width = Width, .Height = Height, .X = 0, .Y = 0});
	sk_sp<VRenderContext> glContext = sk_make_sp<VRenderContext, const sk_sp<VRenderInterface> &>(GLInterface);
	sk_sp<VSurface>		  glSurface =
		sk_make_sp<VSurface, const sk_sp<VRenderTarget> &, const sk_sp<VRenderContext> &>(glRenderTarget, glContext);

	auto canvas = glSurface->GetNativeSurface()->getCanvas();

	const SkScalar x = 8.0f;
	const SkScalar y = 52.0f;

	canvas->drawColor(SK_ColorWHITE);
	canvas->drawTextBlob(TextBlob.get(), x, y, *BlurPaint);
	canvas->drawTextBlob(TextBlob.get(), x, y, *Paint);

	canvas->flush();
	glContext->GetNativeContext()->flushAndSubmit();

	glfwSwapBuffers(GLWindow);
}
void ErrorCallBack(int Error, const char *Description) {
	fputs(Description, stderr);
}