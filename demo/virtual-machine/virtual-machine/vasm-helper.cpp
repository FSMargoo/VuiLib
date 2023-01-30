#include "vasm-helper.h"

namespace VASM
{
byte GetRegRawByte(const std::wstring &RegName)
{
	byte Result = 0u;

	for (auto &Character : RegName)
	{
		Result += Character;
	}

	Result = Result > 17576 ? 0 : Result;

	return Result;
}
} // namespace VASM