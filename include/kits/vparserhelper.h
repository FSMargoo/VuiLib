#pragma once

#include "../core/stl-ex/vstring.h"
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

	static VString ReadFromFile(const VString &FilePath, VDocumentEncoding DocumentEncoding = VDocumentEncoding::UTF8);

	static bool FileExist(const VString &FilePath);
};
} // namespace VKits

VLIB_END_NAMESPACE