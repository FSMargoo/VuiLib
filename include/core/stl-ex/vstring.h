#pragma once
#pragma warning(disable : 4455)

#include "../vbasiccore.h"

#include <comdef.h>

VLIB_BEGIN_NAMESPACE

#pragma comment(lib, "comsuppw.lib")
#ifdef _UNICODE
#define VStr(Str) L##Str
#define VSTCStr	  std::to_wstring

using VChar		   = wchar_t;
using VProxyString = std::wstring;

const wchar_t *operator"" vs(const wchar_t *OriginString, size_t StringLength);
const wchar_t *operator"" vs(const char *OriginString, size_t StringLength);

const wchar_t *vstring_convert(const wchar_t *OriginString);
const wchar_t *vstring_convert(const char *OriginString);
#else
#define VStr(Str) ##Str
#define VSTCStr	  std::to_string

#pragma comment(lib, "comsupp.lib")
using VChar		   = char;
using VProxyString = std : string;

const char *operator"" vs(const wchar_t *OriginString, size_t StringLength);
const char *operator"" vs(const char *OriginString, size_t StringLength);

const wchar_t *vstring_convert(const wchar_t *OriginString);
const wchar_t *vstring_convert(const char *OriginString);
#endif

class VString : public VProxyString
{
public:
	using Iterator		  = VProxyString::iterator;
	using ConstIterator	  = VProxyString::const_iterator;
	using ReverseIterator = VProxyString::reverse_iterator;

public:
	static constexpr auto NoPosition{static_cast<size_type>(-1)};

public:
	VString();
	VString(const std::wstring &String);
	VString(const std::string &String);
	VString(const VChar *String);
	VString(VChar *String);

public:
	static VString FromNumber(const int &NumberConvert);
	static VString FromNumber(const long long &NumberConvert);
	static VString FromNumber(const long &NumberConvert);
	static VString FromNumber(const unsigned int &NumberConvert);
	static VString FromNumber(const unsigned long &NumberConvert);
	static VString FromNumber(const unsigned long long &NumberConvert);
	static VString FromString(const std::string &String);
	static VString FromWideString(const std::wstring &String);

public:
	VString Split(const size_t &Begin, const size_t &SplitCount);
	VString Split(const size_t &Begin, const size_t &SplitCount) const;
	VString SplitRange(const size_t &Begin, const size_t &End);

	void Append(const VString &AppendString);

	bool StartWith(const VString &JudgeString, const size_t &StartOn = 0);
	bool EndWith(const VString &JudgeString);

	inline VChar &At(const size_t &Position);

	bool IsEmpty();

	void Fill(const VChar &Character, size_t FillSize = 0);
	void Set(const VString &String);
	void Erase(const size_t &Begin, const size_t &Count);
	void Erase(ConstIterator Iterator);
	void EraseRange(const size_t &Begin, const size_t &End);
	void Insert(const size_t &Position, const VString &String);
	void Insert(const size_t &Position, const VString &String, const size_t &Count);
	void Insert(ConstIterator Iterator, const VChar &Character);

	size_t IndexOf(const VString &String, const size_t &StartAt = 0) const;
	size_t IndexLastOf(const VString &String, const size_t &StartAt = 0);

	Iterator		Begin();
	Iterator		End();
	ReverseIterator ReverseBegin();
	ReverseIterator ReverseEnd();

	VString Args(VString FormatInstance);
	VString Args(int IntFormat);
	VString Args(const long long IntFormat);
	VString Args(const long IntFormat);
	VString Args(const unsigned int IntFormat);
	VString Args(const unsigned long IntFormat);
	VString Args(const unsigned long long IntFormat);

	const VChar *CStyleString() const;

	size_t Size() const;
	size_t Length() const;
};

VLIB_END_NAMESPACE
