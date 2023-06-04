/*
 * VQuarkFmt.cpp (2023/5.20)
 * 	 The fmt lib in the VQuark
 *
 *
 * Copyright (C) 2023~now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this softwareand associated
 * documentation files(the "Software"), to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and /or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following conditions :
 *
 * The above copyright noticeand this permission notice shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "../include/fmt/vfmt.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
VFMTDevice::VFMTDevice()
{
}
VFileStream::VFileStream(const VString &Path)
{
	Stream.open(Path);
}
void VFileStream::Open(const VString &Path)
{
	Stream.open(Path);
}
void VFileStream::Close()
{
	Stream.close();
}
void VFileStream::Output(const VString &String)
{
	Stream << VProxyString(String.CStyleString());
}
} // namespace Core

VLIB_END_NAMESPACE