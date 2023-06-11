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

/**
 * @brief : The wrapper of the STL string
 */
class VString : public VProxyString
{
public:
	/**
	 * @brief : The iterator
	 */
	using Iterator = VProxyString::iterator;
	/**
	 * @brief : The const iterator
	 */
	using ConstIterator = VProxyString::const_iterator;
	/**
	 * @brief : The Reverse iterator
	 */
	using ReverseIterator = VProxyString::reverse_iterator;

public:
	/**
	 * @brief : If the IndexOf function dosen't find anything, return this
	 */
	static constexpr auto NoPosition{static_cast<size_type>(-1)};

public:
	VString();
	VString(const std::wstring &String);
	VString(const std::string &String);
	VString(const wchar_t *String);
	VString(wchar_t *String);
	VString(const char *String);
	VString(char *String);

public:
	/**
	 * @brief				: Build from the number
	 * @param NumberConvert : The number
	 * @return				: A string that was converted from the number
	 */
	static VString FromNumber(const int &NumberConvert);
	/**
	 * @brief				: Build from the number
	 * @param NumberConvert : The number
	 * @return				: A string that was converted from the number
	 */
	static VString FromNumber(const long long &NumberConvert);
	/**
	 * @brief				: Build from the number
	 * @param NumberConvert : The number
	 * @return				: A string that was converted from the number
	 */
	static VString FromNumber(const long &NumberConvert);
	/**
	 * @brief				: Build from the number
	 * @param NumberConvert : The number
	 * @return				: A string that was converted from the number
	 */
	static VString FromNumber(const unsigned int &NumberConvert);
	/**
	 * @brief				: Build from the number
	 * @param NumberConvert : The number
	 * @return				: A string that was converted from the number
	 */
	static VString FromNumber(const unsigned long &NumberConvert);
	/**
	 * @brief				: Build from the number
	 * @param NumberConvert : The number
	 * @return				: A string that was converted from the number
	 */
	static VString FromNumber(const unsigned long long &NumberConvert);
	/**
	 * @brief		 : Build from the low byte string
	 * @param String : The string
	 * @return		 : A string that was converted from the low byte string
	 */
	static VString FromString(const std::string &String);
	/**
	 * @brief		 : Build from the wide byte string
	 * @param String : The string
	 * @return		 : A string that was converted from the wide byte string
	 */
	static VString FromWideString(const std::wstring &String);

public:
	/**
	 * @brief			 : Split the string
	 * @param Begin		 : The begin of the split string
	 * @param SplitCount : The characters count of the split string
	 * @return			 : The string was splited
	 */
	VString Split(const size_t &Begin, const size_t &SplitCount);
	/**
	 * @brief			 : Split the string
	 * @param Begin		 : The begin of the split string
	 * @param SplitCount : The characters count of the split string
	 * @return			 : The string was splited
	 */
	VString Split(const size_t &Begin, const size_t &SplitCount) const;
	/**
	 * @brief			 : Split the string
	 * @param Begin		 : The begin of the split string
	 * @param SplitCount : The end of the split string
	 * @return			 : The string was splited
	 */
	VString SplitRange(const size_t &Begin, const size_t &End);

	/**
	 * @brief				: Append string
	 * @param AppendString	: The string be appended
	 */
	void Append(const VString &AppendString);

	/**
	 * @brief				: Is the string start with the specified string
	 * @param JudgeString	: The string will be judged
	 * @param StartOn		: Where to start
	 * @return				: If this string start with the specified string, return true, nor return false
	 */
	bool StartWith(const VString &JudgeString, const size_t &StartOn = 0);
	/**
	 * @brief	: Is the string end with the specified string
	 * @return	: If this string end with the specified string, return true, nor return false
	 */
	bool EndWith(const VString &JudgeString);

	/**
	 * @brief			: Get the characeter from specified position
	 * @param Position	: The position
	 * @return			: The characeter
	 */
	inline VChar &At(const size_t &Position);

	/**
	 * @brief	: Is the string empty
	 * @return	: If it is empty, return true
	 */
	bool IsEmpty();

	/**
	 * @brief			: Fill the string
	 * @param Character : The specified characeter
	 * @param FillSize	: The count of the characters
	 */
	VString Fill(const VChar &Character, size_t FillSize = 0);
	/**
	 * @brief			: Set the string with specified string
	 * @param String	: The string
	 */
	void Set(const VString &String);
	/**
	 * @brief		: Delete the characeters from the
	 * @param Begin : The begin position
	 * @param Count : The end position
	 */
	void Erase(const size_t &Begin, const size_t &Count);
	/**
	 * @brief			: Erase the character by iterator
	 * @param Iterator	: The iterator
	 */
	void Erase(ConstIterator Iterator);
	/**
	 * @brief		: Erase by range
	 * @param Begin	: Range begin
	 * @param End	: Range end
	 */
	void EraseRange(const size_t &Begin, const size_t &End);
	/**
	 * @brief			: Insert a specified string
	 * @param Position	: The position
	 * @param String	: The string
	 */
	void Insert(const size_t &Position, const VString &String);
	/**
	 * @brief			: Insert a specified string by the specified times
	 * @param Position	: The position
	 * @param String	: The string
	 * @param Count		: Count of the string
	 */
	void Insert(const size_t &Position, const VString &String, const size_t &Count);
	/**
	 * @brief			: Insert a specified character by the iterator
	 * @param Iterator	: The iterator
	 * @param Character	: The character
	 */
	void Insert(ConstIterator Iterator, const VChar &Character);

	/**
	 * @brief			: Find the specified string
	 * @param String	: The string
	 * @param StartAt	: Where to start
	 * @return			: If there exists the string, return the position of the string, nor return npos
	 */
	size_t IndexOf(const VString &String, const size_t &StartAt = 0) const;
	/**
	 * @brief			: Find the one last of the specified string in this string
	 * @param String	: The string
	 * @param StartAt	: Where to start
	 * @return			: If there exists the string, return the position of the string, nor return npos
	 */
	size_t IndexLastOf(const VString &String, const size_t &StartAt = 0);

	/**
	 * @brief	: Get the begin iterator
	 * @return	: The iterator
	 */
	Iterator Begin();
	/**
	 * @brief	: Get the end iterator
	 * @return	: The iterator
	 */
	Iterator End();
	/**
	 * @brief	: Get the begin of the reverse iterator
	 * @return	: The iterator
	 */
	ReverseIterator ReverseBegin();
	/**
	 * @brief	: Get the end of the reverse iterator
	 * @return	: The iterator
	 */
	ReverseIterator ReverseEnd();

	/**
	 * @brief					: Args format with string
	 * @param FormatInstance	: Format agrument
	 * @return					: Formated string
	 */
	VString Args(VString FormatInstance) const;
	/**
	 * @brief			: Args format with int
	 * @param IntFormat	: Format agrument
	 * @return			: Formated string
	 */
	VString Args(int IntFormat) const;
	/**
	 * @brief			: Args format with long long
	 * @param IntFormat	: Format agrument
	 * @return			: Formated string
	 */
	VString Args(const long long IntFormat) const;
	/**
	 * @brief			: Args format with long
	 * @param IntFormat	: Format agrument
	 * @return			: Formated string
	 */
	VString Args(const long IntFormat) const;
	/**
	 * @brief			: Args format with unsigned int
	 * @param IntFormat	: Format agrument
	 * @return			: Formated string
	 */
	VString Args(const unsigned int IntFormat) const;
	/**
	 * @brief			: Args format with unsgiend long
	 * @param IntFormat	: Format agrument
	 * @return			: Formated string
	 */
	VString Args(const unsigned long IntFormat) const;
	/**
	 * @brief			: Args format with unsigned long long
	 * @param IntFormat	: Format agrument
	 * @return			: Formated string
	 */
	VString Args(const unsigned long long IntFormat) const;
	/**
	 * @brief			: Args format with VPoint
	 * @param IntFormat	: Format agrument
	 * @return			: Formated string
	 */
	VString Args(const Core::VPoint Format) const;
	/**
	 * @brief			: Args format with VRect
	 * @param IntFormat	: Format agrument
	 * @return			: Formated string
	 */
	VString Args(const Core::VRect Format) const;

	/**
	 * @brief			: Get the C Style String
	 * @return			: The C Style String
	 */
	const VChar *CStyleString() const;

	/**
	 * @brief  : Get the string size
	 * @return : The size of the string
	 */
	size_t Size() const;
	/**
	 * @brief  : Get the length of the string
	 * @return : The string's length
	 */
	size_t Length() const;
};

VLIB_END_NAMESPACE
