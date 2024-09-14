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
 * \file vSurface.h
 * \brief The surface class of VRenderer
 */

#pragma once

#include <include/renderer/vRenderContext.h>
#include <include/renderer/vRenderTarget.h>
#include <include/renderer/vRendererBase.h>

/**
 * The surface wrapper of Skia surface class
 */
class VSurface : public SkRefCnt {
public:
	/**
	 * Allocates raster surface, in general, create a surface in memory.
	 * Pixel memory will be deleted when VSurface is deleted.
	 * @param Width The width of raster surface
	 * @param Height The height of raster surface
	 */
	VSurface(const int &Width, const int &Height);
	/**
	 * Create a new surface object with an existed skia surface pointer
	 * @param Surface The sk_sp pointer referred to the surface object
	 */
	VSurface(sk_sp<SkSurface> &Surface);
	/**
	 * Construct the surface by render target and render a context object.
	 * @param RenderTarget The render target object
	 * @param Context The render context object.
	 */
	explicit VSurface(const sk_sp<VRenderTarget> &RenderTarget, const sk_sp<VRenderContext> &Context);
	~VSurface() = default;

public:
	/**
	 * Get the native surface object
	 * @return The SkSurface object in a smart pointer
	 */
	auto &GetNativeSurface() {
		return _surface;
	}

private:
	sk_sp<SkSurface> _surface;
};