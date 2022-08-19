#include "../../include/kits/vcolorhelper.h"

VLIB_BEGIN_NAMESPACE

namespace VKits {
    short VSSColorHelper::SingleHexLetterToNum(const wchar_t& Hex) {
        if (Hex >= '0' && Hex <= '9') {
            return Hex - L'0';
        }

        return Hex - 87;
    }
    short  VSSColorHelper::HexStringToNum(const std::wstring& HexNumberString) {
        short   FirstCount  = SingleHexLetterToNum(HexNumberString[0]) * 16;
        short   SecondCount = SingleHexLetterToNum(HexNumberString[1]);

        return FirstCount + SecondCount;
    }
    Core::VColor VSSColorHelper::HexToColor(const std::wstring& HexString) {
        auto RawHexString = HexString.substr(1, HexString.size() - 1);
        short ColorArrary[3] = { 0, 0, 0 };

        for (short Count = 2; Count <= RawHexString.size(); Count += 2) {
            ColorArrary[Count / 2 - 1] = HexStringToNum(RawHexString.substr(Count - 2, 2));
        }

        return Core::VColor::FromBYTERGBA(ColorArrary[0], ColorArrary[1], ColorArrary[2]);
    }
    bool   VSSColorHelper::IsValidHex(const std::wstring& HexString) {
        if (HexString[0] != L'#') {
            return false;
        }
        if (HexString.size() != 7) {
            return false;
        }

        for (auto Letter = HexString.begin() + 1; Letter != HexString.end(); ++Letter) {
            if (!(*Letter >= L'0' && *Letter <= L'9' || *Letter >= L'a' && *Letter <= L'f')) {
                return false;
            }
        }

        return true;
    }
}

VLIB_END_NAMESPACE