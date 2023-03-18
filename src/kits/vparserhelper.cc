#include "../../include/kits/vparserhelper.h"

VLIB_BEGIN_NAMESPACE

namespace VKits
{
VString VParserHelper::ReadFromFile(const VString &FilePath, VDocumentEncoding DocumentEncoding)
{
	std::ifstream FileStream(FilePath);

	std::string TempLine;
	std::string TotalFile;

	while (true)
	{
		std::getline(FileStream, TempLine);

		if (!FileStream.eof())
		{
			TotalFile += TempLine + "\n";
		}
		else
		{
			TotalFile += TempLine;

			break;
		}
	}

	FileStream.close();

	switch (DocumentEncoding)
	{
	case VDocumentEncoding::UTF8: {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> Convertor;

		return Convertor.from_bytes(TotalFile);
	}
	case VDocumentEncoding::UTF16: {
		std::wstring_convert<std::codecvt_utf16<wchar_t>> Convertor;

		return Convertor.from_bytes(TotalFile);
	}
	}

	return L"Unknown File";
}
bool VParserHelper::FileExist(const VString &FilePath)
{
	return (_waccess(FilePath.CStyleString(), 04) != -1);
}
} // namespace VKits

VLIB_END_NAMESPACE