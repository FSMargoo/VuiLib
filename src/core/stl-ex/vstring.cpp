#include "../../../include/core/stl-ex/vstring.h"

VLIB_BEGIN_NAMESPACE

#ifdef _UNICODE
const wchar_t *operator"" vs(const wchar_t *OriginString, size_t StringLength)
{
	return OriginString;
}
const wchar_t *operator"" vs(const char *OriginString, size_t StringLength)
{
	_bstr_t ConvertedString(OriginString);

	return (const wchar_t *)ConvertedString;
}

const wchar_t *vstring_convert(const wchar_t *OriginString)
{
	return OriginString;
}
const wchar_t *vstring_convert(const char *OriginString)
{
	_bstr_t ConvertedString(OriginString);

	return (const wchar_t *)ConvertedString;
}
#else
const char *operator"" vs(const wchar_t *OriginString, size_t StringLength)
{
	_bstr_t ConvertedString(OriginString);

	return (const char *)ConvertedString;
}
const char *operator"" vs(const char *OriginString, size_t StringLength)
{
	return OriginString;
}

const wchar_t *vstring_convert(const wchar_t *OriginString)
{
	_bstr_t ConvertedString(OriginString);

	return (const char *)ConvertedString;
}
const wchar_t *vstring_convert(const char *OriginString)
{
	return OriginString;
}
#endif

VString::VString() : VProxyString()
{
}
VString::VString(const VProxyString &String) : VProxyString(String)
{
}
VString::VString(const VChar *String) : VProxyString(String)
{
}
VString::VString(VChar *String) : VProxyString(String)
{
}

VString VString::Split(const size_t &Begin, const size_t &SplitCount)
{
	return substr(Begin, SplitCount);
}
VString VString::SplitRange(const size_t &Begin, const size_t &End)
{
	return substr(Begin, End - Begin);
}
void VString::Append(const VString &AppendString)
{
	this->operator+=(AppendString);
}
size_t VString::Size() const
{
	return size();
}
bool VString::StartWith(const VString &JudgeString, const size_t &StartOn)
{
	for (auto Count = StartOn; Count < StartOn + JudgeString.Size(); ++Count)
	{
		if (at(Count) != JudgeString[Count - StartOn])
		{
			return false;
		}
	}

	return true;
}
bool VString::EndWith(const VString &JudgeString)
{
	return StartWith(JudgeString, size() - JudgeString.size());
}
VChar &VString::At(const size_t &Position)
{
	return at(Position);
}

bool VString::IsEmpty()
{
	return empty();
}

void VString::Fill(const VChar &Character, size_t FillSize)
{
	if (FillSize == 0)
	{
		FillSize = size();
	}

	for (auto Count = 0; Count < FillSize; ++Count)
	{
		At(Count) = Character;
	}
}

size_t VString::IndexOf(const VString &String, const size_t &StartAt) const
{
	return find(String, StartAt);
}
size_t VString::IndexLastOf(const VString &String, const size_t &StartAt)
{
	return find_last_of(String, StartAt);
}
VString::Iterator VString::Begin()
{
	return begin();
}
VString::Iterator VString::End()
{
	return end();
}
VString::ReverseIterator VString::ReverseBegin()
{
	return rbegin();
}
VString::ReverseIterator VString::ReverseEnd()
{
	return rend();
}
void VString::Set(const VString &String)
{
	*this = String;
}
void VString::Erase(const size_t &Begin, const size_t &Count)
{
	erase(Begin, Count);
}
void VString::Erase(ConstIterator Iterator)
{
	erase(Iterator);
}
void VString::EraseRange(const size_t &Begin, const size_t &End)
{
	erase(Begin, End - Begin);
}
void VString::Insert(const size_t &Position, const VString &String)
{
	insert(Position, String);
}
void VString::Insert(const size_t &Position, const VString &String, const size_t &Count)
{
	insert(Position, String, Count);
}
void VString::Insert(ConstIterator Iterator, const VChar &Character)
{
	insert(Iterator, Character);
}
VString VString::FromNumber(const int &NumberConvert)
{
	return VSTCStr(NumberConvert);
}
VString VString::FromNumber(const long long &NumberConvert)
{
	return VSTCStr(NumberConvert);
}
VString VString::FromNumber(const long &NumberConvert)
{
	return VSTCStr(NumberConvert);
}
VString VString::FromNumber(const unsigned int &NumberConvert)
{
	return VSTCStr(NumberConvert);
}
VString VString::FromNumber(const unsigned long &NumberConvert)
{
	return VSTCStr(NumberConvert);
}
VString VString::FromNumber(const unsigned long long &NumberConvert)
{
	return VSTCStr(NumberConvert);
}
VString VString::Args(VString FormatInstance)
{
	auto Index = IndexOf(VStr("%s"));
	if (Index != VString::NoPosition)
	{
		EraseRange(Index, Index + 2);
		Insert(Index, FormatInstance);

		return VString(*this);
	}

	return VString(FormatInstance);
}
VString VString::Args(int IntFormat)
{
	auto Index = IndexOf(VStr("%d"));
	if (Index != VString::NoPosition)
	{
		EraseRange(Index, Index + 2);
		Insert(Index, FromNumber(IntFormat));
	}

	return VString(*this);
}
VString VString::Args(const long IntFormat)
{
	auto Index = IndexOf(VStr("%ld"));
	if (Index != VString::NoPosition)
	{
		EraseRange(Index, Index + 2);
		Insert(Index, FromNumber(IntFormat));
	}

	return VString(*this);
}
VString VString::Args(const long long IntFormat)
{
	auto Index = IndexOf(VStr("%lld"));
	if (Index != VString::NoPosition)
	{
		EraseRange(Index, Index + 4);
		Insert(Index, FromNumber(IntFormat));
	}

	return VString(*this);
}
VString VString::Args(const unsigned int IntFormat)
{
	auto Index = IndexOf(VStr("%ud"));
	if (Index != VString::NoPosition)
	{
		EraseRange(Index, Index + 2);
		Insert(Index, FromNumber(IntFormat));
	}

	return VString(*this);
}
VString VString::Args(const unsigned long IntFormat)
{
	auto Index = IndexOf(VStr("%uld"));
	if (Index != VString::NoPosition)
	{
		EraseRange(Index, Index + 4);
		Insert(Index, FromNumber(IntFormat));
	}

	return VString(*this);
}
VString VString::Args(const unsigned long long IntFormat)
{
	auto Index = IndexOf(VStr("%ulld"));
	if (Index != VString::NoPosition)
	{
		EraseRange(Index, Index + 5);
		Insert(Index, FromNumber(IntFormat));
	}

	return VString(*this);
}
size_t VString::Length() const
{
	return length();
}
const VChar *VString::CStyleString() const
{
	return (const VChar *)c_str();
}

VLIB_END_NAMESPACE