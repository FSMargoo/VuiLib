#pragma once

#include "../core/vbasiccore.h"
#include <codecvt>
#include <fstream>
#include <io.h>

VLIB_BEGIN_NAMESPACE

namespace VKits
{
enum class VDocumentEncoding
{
	UTF8,
	UTF16
};

class VParserHelper
{
public:
	VParserHelper()
	{
	}

	static std::wstring ReadFromFile(const std::wstring &FilePath,
									 VDocumentEncoding	 DocumentEncoding = VDocumentEncoding::UTF8);

	static bool FileExist(const std::wstring &FilePath);
};
} // namespace VKits

VLIB_END_NAMESPACE