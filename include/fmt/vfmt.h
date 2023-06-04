/*
 * VFmt.h (2023/5.20)
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
#pragma once

#include "../core/stl-ex/vstring.h"

#include <fstream>
#include <tchar.h>

VLIB_BEGIN_NAMESPACE

namespace Core
{
/**
 * @brief : The FMT output device base class
 */
class VFMTDevice
{
public:
	VFMTDevice();

public:
	/**
	 * @brief			: Print string
	 * @param String	: The string
	 */
	virtual void Output(const VString &String) = 0;
};

/**
 * @brief : The file output stream
 */
class VFileStream : public VFMTDevice
{
public:
	VFileStream(const VString &Path);

	void Output(const VString &String) override;

public:
	/**
	 * @brief		: Open a file
	 * @param Path	: The file path
	 */
	void Open(const VString &Path);
	/**
	 * @brief : Close the stream
	 */
	void Close();

private:
	std::basic_ofstream<VChar, std::char_traits<VChar>> Stream;
};

using VFMTStream = VFMTDevice;

class VFmt
{
public:
	static void Print(const VString &String)
	{
		_tprintf(VStr("%s"), String.CStyleString());
	}
	template <class Input, class... Types> static void Print(const VString &String, Input _Input, Types... Args)
	{
		Print(String.Args(_Input), Args...);
	}
	/**
	 * @brief			: The format print (on std stream)
	 * @tparam ...Types	: The agrument type
	 * @param String	: The string
	 * @param ...Args	: The agrument
	 */
	template <class... Types> static void Print(const VString &String, Types... Args)
	{
		Print(String, Args...);
	}

	static void PrintDevice(VFMTStream *Stream, const VString &String)
	{
		Stream->Output(String.CStyleString());
	}
	template <class Input, class... Types>
	static void PrintDevice(VFMTStream *Stream, const VString &String, Input _Input, Types... Args)
	{
		PrintDevice(Stream, String.Args(_Input), Args...);
	}
	/**
	 * @brief			: The format print (on specifed device)
	 * @tparam ...Types	: The agrument type
	 * @param String	: The string
	 * @param ...Args	: The agrument
	 */
	template <class... Types> static void PrintDevice(VFMTStream *Stream, const VString &String, Types... Args)
	{
		PrintDevice(Stream, String, Args...);
	}

	static VString Format(const VString &String)
	{
		return String;
	}
	template <class Input, class... Types> static VString Format(const VString &String, Input _Input, Types... Args)
	{
		return Format(String.Args(_Input), Args...);
	}
	/**
	 * @brief			: Format string
	 * @tparam ...Types : The agrument type
	 * @param String	: The format string
	 * @param ...Args	: The agrument
	 * @return			: The formated string
	 */
	template <class... Types> static VString Format(const VString &String, Types... Args)
	{
		return Format(String, Args...);
	}
};
} // namespace Core

VLIB_END_NAMESPACE