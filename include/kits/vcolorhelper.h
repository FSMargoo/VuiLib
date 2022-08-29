#include "../core/render/vcolor.h"

VLIB_BEGIN_NAMESPACE

namespace VKits {
    class VSSColorHelper {
    public:
        static short        SingleHexLetterToNum(const wchar_t& Hex);

    public:
        static short        HexStringToNum(const std::wstring& HexNumberString);
        static void         HexStringLowerToUpper(std::wstring& HexStringNumber);

    public:
        static Core::VColor HexToColor(const std::wstring& HexString);
        static bool         IsValidHex(const std::wstring& HexString);
    };
}

VLIB_END_NAMESPACE