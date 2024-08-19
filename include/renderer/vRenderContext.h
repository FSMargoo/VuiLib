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
 * \file vRenderContext.h
 * \brief The render context wrapper of Skia
 */

#pragma once

#include <include/renderer/vRenderInterface.h>
#include <include/renderer/vRendererBase.h>

/**
 * The render context wrapper of Skia
 */
class VRenderContext : public SkRefCnt {
public:
	/**
	 * Construct the render context by interface
	 * @param Interface The interface's pointer
	 */
	explicit VRenderContext(const sk_sp<VRenderInterface> &Interface);
	~VRenderContext() = default;

public:
	/**
	 * Get the native object(Skia object) of render context
	 * @return The skia render context object
	 */
	auto &GetNativeContext() {
		return _context;
	}

private:
	sk_sp<GrDirectContext> _context;
};