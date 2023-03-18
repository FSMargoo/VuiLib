#include "../core/render/vcolor.h"

VLIB_BEGIN_NAMESPACE

namespace VKits
{
class VSSColorHelper
{
public:
	static short SingleHexLetterToNum(const wchar_t &Hex);

public:
	static short HexStringToNum(const VString &HexNumberString);
	static void	 HexStringLowerToUpper(VString &HexStringNumber);

public:
	static Core::VColor HexToColor(const VString &HexString);
	static bool			IsValidHex(const VString &HexString);
};
} // namespace VKits

VLIB_END_NAMESPACE