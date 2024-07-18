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
 * \file vSurface.cpp
 * \brief The surface class of VRenderer
 */

#include <include/renderer/vSurface.h>

VSurface::VSurface(const int &Width, const int &Height) : _surface(SkSurface::MakeRasterN32Premul(Width, Height)) {
}
VSurface::VSurface(const sk_sp<VRenderTarget> &RenderTarget, const sk_sp<VRenderContext> &Context) {
	// By default, we enable MSAA flag.
	SkColorType	   colorType = kRGBA_8888_SkColorType;
	SkSurfaceProps property(SkSurfaceProps::Flags::kDynamicMSAA_Flag, SkPixelGeometry::kUnknown_SkPixelGeometry);

	// In fact, we don't need to proc the render target destroying
	_surface =
		SkSurface::MakeFromBackendRenderTarget(Context->GetNativeContext().get(), RenderTarget->GetNativeRenderTarget(),
											   kBottomLeft_GrSurfaceOrigin, colorType, nullptr, &property);
}