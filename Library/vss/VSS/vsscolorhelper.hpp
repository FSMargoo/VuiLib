/*
 * VSSColorHelper.hpp
 *	@description : Provide a Color Helper for VSS
*/

#pragma once

#include "../../render/vrender/vcolor.hpp"

VLIB_BEGIN_NAMESPACE

class VSSColorHelper {
public:
	static short  SingleHexLetter2Num(wchar_t Hex) {
		if (Hex >= '0' && Hex <= '9') {
			return Hex - L'0';
		}

		return Hex - 87;
	}

public:
	static short  HexString2Num(std::wstring HexNumberString) {
		short   FirstCount  = SingleHexLetter2Num(HexNumberString[0]) * 16;
		short   SecondCount = SingleHexLetter2Num(HexNumberString[1]);

		return FirstCount + SecondCount;
	}

public:
	static VColor Hex2Color(std::wstring HexString) {
		HexString = HexString.substr(1, HexString.size() - 1);
		short ColorArrary[3] = { 0, 0, 0 };

		for (short Count = 2; Count <= HexString.size(); Count += 2) {
			ColorArrary[Count / 2 - 1] = HexString2Num(HexString.substr(Count - 2, 2));
		}

		return VColor(ColorArrary[0], ColorArrary[1], ColorArrary[2]);
	}
	static bool   IsValidHex(std::wstring HexString) {
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
};

VLIB_END_NAMESPACE