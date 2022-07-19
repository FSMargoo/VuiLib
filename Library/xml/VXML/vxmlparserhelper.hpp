/*
 * VXMLParserHelper.hpp
 *	@description  : Provide a Helper class for VXMLParser
*/

#pragma once

#include "../../VBase"

#include <codecvt>
#include <fstream>

VLIB_BEGIN_NAMESPACE

enum class VVMLDocumentEncoding {
	UTF8, UTF16
};

class VVMLParserHelper {
public:
	VVMLParserHelper() {

	}

	static std::wstring ReadFromFile(std::wstring FilePath, 
		VVMLDocumentEncoding DocumentEncoding = VVMLDocumentEncoding::UTF8) {
		std::ifstream FileStream(FilePath);
		
		std::string TempLine;
		std::string TotalFile;

		while (!FileStream.eof()) {
			std::getline(FileStream, TempLine);

			TotalFile += TempLine + "\n";
		}

		FileStream.close();

		switch (DocumentEncoding) {
		case VVMLDocumentEncoding::UTF8: {
			std::wstring_convert<std::codecvt_utf8<wchar_t>> Convertor;

			return Convertor.from_bytes(TotalFile);
		}
		case VVMLDocumentEncoding::UTF16: {
			std::wstring_convert<std::codecvt_utf16<wchar_t>> Convertor;

			return Convertor.from_bytes(TotalFile);
		}
		}

		return L"Unknown File";
	}
};

VLIB_END_NAMESPACE