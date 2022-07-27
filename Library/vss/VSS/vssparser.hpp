/*
 * VSSParser.hpp
 *	@description : Provide a VSS Parser
*/

#pragma once

#include "../../VBase"
#include "../../xml/VXML/vxmlparserhelper.hpp"
#include "vsscolorhelper.hpp"

#include "VSSParser/vss.seal.lexical.hpp"

#include <vector>
#include <map>

VLIB_BEGIN_NAMESPACE

enum class VVVSParserParseMode {
	FromFile, FromString
};
enum class VVVSParserStatus {
	Error, Ok
};
enum class 
	   VVSSPropertyType {
	IntValue, StringValue, BooleanValue, DoubleValue, ColorValue
};

struct VVSSPropertyValue {
	VVSSPropertyType Type            = VVSSPropertyType::IntValue;

	VColor			 PropertyAsColorValue;
	int				 PropertyAsInt    = 0;
	std::wstring     PropertyAsString = L"";
	double           PropertyAsDouble = 0.f;
	bool             PropertyAsBool   = false;
};

struct VVSSSelectorProperty {
	std::wstring				                PropertyTag;
	std::vector<VVSSPropertyValue>				ValueList;
};

enum class VVSSSelectorType {
	UnkownSelector, ElementSelector, IDSelector, ClassSelector, FakeClassSelector, ClassWithFakeClassSelector,
	GenericSelector
};

class  VVSSBasicSelector {
public:
	std::map<std::wstring, VVSSSelectorProperty> SelectorProperty;

	virtual VVSSSelectorType GetType() {
		return VVSSSelectorType::UnkownSelector;
	}

	VVSSBasicSelector() {  }
};
class  VVSSElementSelector : public VVSSBasicSelector {
public:
	std::wstring ElementTag;

	VVSSSelectorType GetType() override {
		return VVSSSelectorType::ElementSelector;
	}
};
class  VVSSIDSelector : public VVSSBasicSelector {
public:
	std::wstring IDTag;

	VVSSSelectorType GetType() override {
		return VVSSSelectorType::IDSelector;
	}
};
class  VVSSClassSelector : public VVSSBasicSelector {
public:
	std::wstring ClassTag;
	std::wstring TargetElement;

	VVSSSelectorType GetType() override {
		return VVSSSelectorType::ClassSelector;
	}
};
class  VVSSFakeClassSelector : public VVSSBasicSelector {
public:
	std::wstring ElementTag;
	std::wstring ClassTag;

	VVSSSelectorType GetType() override {
		return VVSSSelectorType::FakeClassSelector;
	}
};
class  VVSSClassWithFakeClassSelector : public VVSSBasicSelector {
public:
	std::wstring TargetElement;
	std::wstring ClassTag;
	std::wstring FakeClassTag;

	VVSSSelectorType GetType() override {
		return VVSSSelectorType::ClassWithFakeClassSelector;
	}
};
class  VVSSGenericSelector : public VVSSBasicSelector {
public:
	VVSSSelectorType GetType() override {
		return VVSSSelectorType::GenericSelector;
	}
};

struct VSSParserError {
	std::wstring ErrorInformation;
	unsigned int ErrorLine = 0;
};

struct VSSParserResult {
	std::vector<VVSSBasicSelector*> SelectorSet;

	VSSParserError                  ErrorInfo;
	VVVSParserStatus			    ParserStatus = VVVSParserStatus::Ok;
};

class  VSSParser {
private:
	SealForVSS::seal_lexical* ParserLexical;

	unsigned int              BaseLine = 0;

private:
	std::map<std::wstring, VVSSSelectorProperty>
			ParserProperty(std::wstring PropertyString, VSSParserResult* Result) {
		SealForVSS::seal_lexical PropertyLexical(PropertyString);
		SealForVSS::seal_lexical_type_info::seal_lexical_token
								 Token;

		std::map<std::wstring, VVSSSelectorProperty>
								 ReturnValue;

		while (!PropertyLexical.is_eof()) {
			VVSSSelectorProperty Property;

			Token = PropertyLexical.get_token();

			if (Token.cache_token == SealForVSS::EOF_TOKEN) {
				break;
			}
			if (Token.cache_token != SealForVSS::UNKNOW_TOKEN) {
				ThrowError(Result, L"Unknown Character + \"" + Token.token_string + L"\"");

				return ReturnValue;
			}

			Property.PropertyTag = Token.token_string;

			Token = PropertyLexical.get_token();
			if (Token.token_string != L":") {
				ThrowError(Result, L"Unknown Character + \"" + Token.token_string + L"\"");

				return ReturnValue;
			}

			bool EndTokenFound = false;

			while (!PropertyLexical.is_eof()) {
				Token = PropertyLexical.get_token();
				
				// End Of Line
				if (Token.token_string == L";") {
					EndTokenFound = true;

					break;
				}

				// Hex Color
				if (Token.token_string == L"#") {
					VVSSPropertyValue PropertyValue;

					std::wstring HexString;

					while (true) {
						Token = PropertyLexical.get_token();

						if (Token.cache_token == SealForVSS::EOF_TOKEN) {
							break;
						}

						HexString += Token.token_string;

						if (HexString.size() >= 6) {
							break;
						}
					}

					if (VSSColorHelper::IsValidHex(L"#" + HexString) == false) {
						if (Token.token_string.empty() == false) {
							ThrowError(Result, L"Unknown Hex Color \"" + Token.token_string + L"\"");
						}
						else {
							ThrowError(Result, L"The Hex Color is Necessary");
						}

						return ReturnValue;
					}
					else {
						PropertyValue.Type                 = VVSSPropertyType::ColorValue;
						PropertyValue.PropertyAsColorValue = VSSColorHelper::Hex2Color(L"#" + HexString);

						Property.ValueList.push_back(PropertyValue);
					}

					continue;
				}
				// RGB Color
				if (Token.token_string == L"rgb") {
					VVSSPropertyValue PropertyValue;

					Token = PropertyLexical.get_token();
					if (Token.token_string != L"(") {
						ThrowError(Result, L"Unknown Character + \"" + Token.token_string + L"\"");

						return ReturnValue;
					}

					int ColorArrary[3] = { 0, 0, 0 };

					for (int Count = 0; Count < 3; ++Count) {
						Token = PropertyLexical.get_token();
						if (Token.cache_token != SealForVSS::CONST_NUMBER) {
							ThrowError(Result, L"Unknown Character + \"" + Token.token_string + L"\" (It Should Be a Number)");

							return ReturnValue;
						}

						ColorArrary[Count] = _wtoi(Token.token_string.c_str());

						Token = PropertyLexical.get_token();
						
						if (Count < 2) {
							if (Token.token_string != L",") {
								ThrowError(Result, L"Unknown Character + \"" + Token.token_string + L"\" (Do you mean \',\'?)");

								return ReturnValue;
							}
						}
						else {
							if (Token.token_string != L")") {
								ThrowError(Result, L"Unknown Character + \"" + Token.token_string + L"\" (Do you mean \')\'?)");

								return ReturnValue;
							}
						}
					}

					PropertyValue.Type = VVSSPropertyType::ColorValue;
					PropertyValue.PropertyAsColorValue
									   = VColor(ColorArrary[0], ColorArrary[1], ColorArrary[2]);

					Property.ValueList.push_back(PropertyValue);

					continue;
				}
				// RGBA Color
				if (Token.token_string == L"rgba") {
					VVSSPropertyValue PropertyValue;

					Token = PropertyLexical.get_token();
					if (Token.token_string != L"(") {
						ThrowError(Result, L"Unknown Character + \"" + Token.token_string + L"\"");

						return ReturnValue;
					}

					int    ColorArrary[3] = { 0, 0, 0 };
					double Alpha = 0.f;

					for (int Count = 0; Count < 4; ++Count) {
						Token = PropertyLexical.get_token();
						if (Token.cache_token != SealForVSS::CONST_NUMBER) {
							ThrowError(Result, L"Unknown Character + \"" + Token.token_string + L"\" (It Should Be a Number)");

							return ReturnValue;
						}

						if (Count < 3) {
							ColorArrary[Count] = _wtoi(Token.token_string.c_str());
						}
						else {
							Alpha = _wtof(Token.token_string.c_str());
						}

						Token = PropertyLexical.get_token();

						if (Count < 3) {
							if (Token.token_string != L",") {
								ThrowError(Result, L"Unknown Character + \"" + Token.token_string + L"\" (Do you mean \',\'?)");

								return ReturnValue;
							}
						}
						else {
							if (Token.token_string != L")") {
								ThrowError(Result, L"Unknown Character + \"" + Token.token_string + L"\" (Do you mean \')\'?)");

								return ReturnValue;
							}
						}
					}

					PropertyValue.Type = VVSSPropertyType::ColorValue;
					PropertyValue.PropertyAsColorValue
						= VColor(ColorArrary[0], ColorArrary[1], ColorArrary[2], 255 * Alpha);

					Property.ValueList.push_back(PropertyValue);

					continue;
				}

				// Number Property
				if (Token.cache_token == SealForVSS::CONST_NUMBER) {
					VVSSPropertyValue NumberProperty;

					std::wstring NumberCache = Token.token_string;
					Token                    = PropertyLexical.get_token();

					if (Token.token_string == L"px") {
						if (NumberCache.find(L".") != Token.token_string.npos) {
							NumberProperty.Type = VVSSPropertyType::DoubleValue;
							NumberProperty.PropertyAsDouble = _wtof(NumberCache.c_str());
						}
						else {
							NumberProperty.Type = VVSSPropertyType::IntValue;
							NumberProperty.PropertyAsInt = _wtoi(NumberCache.c_str());
						}
					}
					else if (Token.token_string == L"%") {
						if (NumberCache.find(L".") != Token.token_string.npos) {
							NumberProperty.Type = VVSSPropertyType::DoubleValue;
							NumberProperty.PropertyAsDouble = _wtof(NumberCache.c_str()) / 100.f;
						}
						else {
							NumberProperty.Type             = VVSSPropertyType::DoubleValue;
							NumberProperty.PropertyAsDouble = double(_wtoi(NumberCache.c_str())) / 100.f;
						}
					}
					else {
						ThrowError(Result, L"Unknown Unit \"" + Token.token_string + L"\"");

						return ReturnValue;
					}

					Property.ValueList.push_back(NumberProperty);

					continue;
				}
				// Boolean Property
				if (Token.cache_token == SealForVSS::CONST_BOOLEAN) {
					VVSSPropertyValue BooleanProperty;

					if (Token.token_string == L"true") {
						BooleanProperty.Type             = VVSSPropertyType::BooleanValue;
						BooleanProperty.PropertyAsBool   = true;
					}
					else {
						BooleanProperty.Type           = VVSSPropertyType::BooleanValue;
						BooleanProperty.PropertyAsBool = false;
					}

					Property.ValueList.push_back(BooleanProperty);

					continue;
				}
				// String Property
				if (Token.cache_token == SealForVSS::CONST_STRING) {
					VVSSPropertyValue StringProperty;

					StringProperty.Type             = VVSSPropertyType::StringValue;
					StringProperty.PropertyAsString = Token.token_string.substr(1, Token.token_string.size() - 2);

					Property.ValueList.push_back(StringProperty);

					continue;
				}
				// Const Property
				if (Token.cache_token == SealForVSS::UNKNOW_TOKEN) {
					VVSSPropertyValue StringProperty;

					StringProperty.Type = VVSSPropertyType::StringValue;
					StringProperty.PropertyAsString = Token.token_string;

					Property.ValueList.push_back(StringProperty);

					continue;
				}
			}

			if (EndTokenFound == false) {
				ThrowError(Result, L"Couldn't Found the Match Symbol of \"{\"");

				return ReturnValue;
			}

			ReturnValue.insert(std::pair<std::wstring, VVSSSelectorProperty>(Property.PropertyTag, Property));
		}

		return ReturnValue;
	}

private:
	void ThrowError(VSSParserResult* Result, std::wstring ErrorString) {
		(*Result).ParserStatus = VVVSParserStatus::Error;
		(*Result).ErrorInfo    = { ErrorString, BaseLine + ParserLexical->get_line() + 1 };
		}

private:
	std::wstring GetPropertyString(VSSParserResult* Result) {
		std::wstring PropertyString;
		bool         EndFlag = false;
		SealForVSS::seal_lexical_type_info::seal_lexical_token
					 Token;

		while (!ParserLexical->is_eof()) {
			Token = ParserLexical->get_token();

			if (Token.token_string == L"}") {
				EndFlag = true;

				break;
			}

			PropertyString.append(Token.token_string + L" ");
		}

		if (EndFlag == false) {
			ThrowError(Result, L"Couldn't Found the Match Symbol Of \"{\"");

			return PropertyString;
		}

		return PropertyString;
	}

public:
	VSSParser(std::wstring VVSString, VVVSParserParseMode VVSParserMode = VVVSParserParseMode::FromString, int Line = 0) {
		switch (VVSParserMode) {
		case VVVSParserParseMode::FromString: {
			ParserLexical = new SealForVSS::seal_lexical(VVSString);
			BaseLine = Line;

			break;
		}
		case VVVSParserParseMode::FromFile: {
			if (VVMLParserHelper::FileExist(VVSString) == true) {
				ParserLexical = new SealForVSS::seal_lexical(VVMLParserHelper::ReadFromFile(VVSString));
				BaseLine = Line;
			}
			else {
				ParserLexical = nullptr;
			}

			break;
		}
		default: {
			ParserLexical = new SealForVSS::seal_lexical(VVSString);
			BaseLine = Line;

			break;
		}
		}
	}
	~VSSParser() {
		delete ParserLexical;
	}

	VSSParserResult ParseVSS() {
		VSSParserResult Result;

		if (ParserLexical == nullptr) {
			ThrowError(&Result, L"File doesn't exist!");

			return Result;
		}

		SealForVSS::seal_lexical_type_info::seal_lexical_token
			Token;

		while (!ParserLexical->is_eof()) {
			VVSSBasicSelector* Selector;

			Token = ParserLexical->get_token();

			if (Token.cache_token == SealForVSS::EOF_TOKEN) {
				break;
			}

			std::wstring LevelOneTag;

			if (Token.cache_token == SealForVSS::UNKNOW_TOKEN) {
				LevelOneTag = Token.token_string;
				Token       = ParserLexical->get_token();

				// Element Selector
				if (Token.token_string == L"{") {
					Selector = new VVSSElementSelector;

					static_cast<VVSSElementSelector*>(Selector)->ElementTag = LevelOneTag;

					std::wstring PropertyString = GetPropertyString(&Result);
					if (Result.ParserStatus == VVVSParserStatus::Error) {
						return Result;
					}

					auto PropertySet = ParserProperty(PropertyString, &Result);
					if (Result.ParserStatus == VVVSParserStatus::Error) {
						return Result;
					}

					PropertyString.clear();

					Selector->SelectorProperty = PropertySet;

					Result.SelectorSet.push_back(Selector);
				}
				else if (Token.token_string == L".") {
					Token = ParserLexical->get_token();

					std::wstring LevelTwoTag = Token.token_string;

					if (Token.cache_token != SealForVSS::UNKNOW_TOKEN) {
						ThrowError(&Result, L"Unknown Character + \"" + Token.token_string + L"\"");

						return Result;
					}

					Token = ParserLexical->get_token();

					// Class Selector
					if (Token.token_string == L"{") {
						Selector = new VVSSClassSelector;

						static_cast<VVSSClassSelector*>(Selector)->ClassTag      = LevelTwoTag;
						static_cast<VVSSClassSelector*>(Selector)->TargetElement = LevelOneTag;

						std::wstring PropertyString = GetPropertyString(&Result);
						if (Result.ParserStatus == VVVSParserStatus::Error) {
							return Result;
						}

						auto PropertySet = ParserProperty(PropertyString, &Result);
						if (Result.ParserStatus == VVVSParserStatus::Error) {
							return Result;
						}

						PropertyString.clear();

						Selector->SelectorProperty = PropertySet;

						Result.SelectorSet.push_back(Selector);
					}
					// Fake Class with Class Selector
					else if (Token.token_string == L":") {
						Selector = new VVSSClassWithFakeClassSelector;

						Token = ParserLexical->get_token();
						if (Token.cache_token != SealForVSS::UNKNOW_TOKEN) {
							ThrowError(&Result, L"Unknown Character + \"" + Token.token_string + L"\"");

							return Result;
						}

						std::wstring LevelThreeTag = Token.token_string;

						static_cast<VVSSClassWithFakeClassSelector*>(Selector)->ClassTag      = LevelTwoTag;
						static_cast<VVSSClassWithFakeClassSelector*>(Selector)->TargetElement = LevelOneTag;
						static_cast<VVSSClassWithFakeClassSelector*>(Selector)->FakeClassTag  = LevelThreeTag;

						Token = ParserLexical->get_token();
						if (Token.token_string != L"{") {
							ThrowError(&Result, L"Unknown Character + \"" + Token.token_string + L"\"");

							return Result;
						}

						std::wstring PropertyString = GetPropertyString(&Result);
						if (Result.ParserStatus == VVVSParserStatus::Error) {
							return Result;
						}

						auto PropertySet = ParserProperty(PropertyString, &Result);
						if (Result.ParserStatus == VVVSParserStatus::Error) {
							return Result;
						}

						PropertyString.clear();

						Selector->SelectorProperty = PropertySet;

						Result.SelectorSet.push_back(Selector);
						
					}
				}
				else if (Token.token_string == L":") {
					// Fake Class
					// Class Selector
					Token = ParserLexical->get_token();

					if (Token.cache_token != SealForVSS::UNKNOW_TOKEN) {
						ThrowError(&Result, L"Unknown Character + \"" + Token.token_string + L"\"");

						return Result;
					}

					Selector = new VVSSFakeClassSelector;

					static_cast<VVSSFakeClassSelector*>(Selector)->ClassTag   = Token.token_string;
					static_cast<VVSSFakeClassSelector*>(Selector)->ElementTag = LevelOneTag;

					Token = ParserLexical->get_token();
					if (Token.token_string != L"{") {
						ThrowError(&Result, L"Unknown Character + \"" + Token.token_string + L"\"");

						return Result;
					}

					std::wstring PropertyString = GetPropertyString(&Result);
					if (Result.ParserStatus == VVVSParserStatus::Error) {
						return Result;
					}

					auto PropertySet = ParserProperty(PropertyString, &Result);
					if (Result.ParserStatus == VVVSParserStatus::Error) {
						return Result;
					}

					PropertyString.clear();

					Selector->SelectorProperty = PropertySet;

					Result.SelectorSet.push_back(Selector);
				}
				else {
					ThrowError(&Result, L"Unknown Character + \"" + Token.token_string + L"\"");

					return Result;
				}
			}
			// Class Selector
			else if (Token.cache_token == SealForVSS::DOT_TOKEN) {
				Token = ParserLexical->get_token();

				std::wstring LevelTwoTag = Token.token_string;

				if (Token.cache_token != SealForVSS::UNKNOW_TOKEN) {
					ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

					return Result;
				}

				Token = ParserLexical->get_token();

				if (Token.token_string == L"{") {
					Selector = new VVSSClassSelector;
					static_cast<VVSSClassSelector*>(Selector)->ClassTag = LevelTwoTag;

					std::wstring PropertyString = GetPropertyString(&Result);
					if (Result.ParserStatus == VVVSParserStatus::Error) {
						return Result;
					}

					auto PropertySet = ParserProperty(PropertyString, &Result);
					if (Result.ParserStatus == VVVSParserStatus::Error) {
						return Result;
					}

					PropertyString.clear();

					Selector->SelectorProperty = PropertySet;

					Result.SelectorSet.push_back(Selector);
				}
				else if (Token.token_string == L":") {
					Selector = new VVSSClassWithFakeClassSelector;

					Token = ParserLexical->get_token();
					if (Token.cache_token != SealForVSS::UNKNOW_TOKEN) {
						ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

						return Result;
					}

					std::wstring LevelThreeTag = Token.token_string;

					static_cast<VVSSClassWithFakeClassSelector*>(Selector)->ClassTag = LevelTwoTag;
					static_cast<VVSSClassWithFakeClassSelector*>(Selector)->TargetElement = LevelOneTag;
					static_cast<VVSSClassWithFakeClassSelector*>(Selector)->FakeClassTag  = LevelThreeTag;

					Token = ParserLexical->get_token();
					if (Token.token_string != L"{") {
						ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

						return Result;
					}

					std::wstring PropertyString = GetPropertyString(&Result);
					if (Result.ParserStatus == VVVSParserStatus::Error) {
						return Result;
					}

					auto PropertySet = ParserProperty(PropertyString, &Result);
					if (Result.ParserStatus == VVVSParserStatus::Error) {
						return Result;
					}

					PropertyString.clear();

					Selector->SelectorProperty = PropertySet;

					Result.SelectorSet.push_back(Selector);
				}
				else {
					ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

					return Result;
				}
			}
			// ID Selector
			else if (Token.token_string == L"#") {
				Token = ParserLexical->get_token();
				if (Token.cache_token != SealForVSS::UNKNOW_TOKEN) {
					ThrowError(&Result, L"Unknown Character + \"" + Token.token_string + L"\"");

					return Result;
				}

				Selector = new VVSSIDSelector;
				static_cast<VVSSIDSelector*>(Selector)->IDTag = Token.token_string;

				Token = ParserLexical->get_token();
				if (Token.token_string != L"{") {
					ThrowError(&Result, L"Unknown Character + \"" + Token.token_string + L"\"");

					return Result;
				}

				std::wstring PropertyString = GetPropertyString(&Result);
				if (Result.ParserStatus == VVVSParserStatus::Error) {
					return Result;
				}

				auto PropertySet = ParserProperty(PropertyString, &Result);
				if (Result.ParserStatus == VVVSParserStatus::Error) {
					return Result;
				}

				PropertyString.clear();

				Selector->SelectorProperty = PropertySet;

				Result.SelectorSet.push_back(Selector);
			}
			// Generic Selector
			else if (Token.token_string == L"*") {
				Selector = new VVSSGenericSelector;

				Token = ParserLexical->get_token();
				if (Token.token_string != L"{") {
					ThrowError(&Result, L"Unknown Character + \"" + Token.token_string + L"\"");

					return Result;
				}

				std::wstring PropertyString = GetPropertyString(&Result);
				if (Result.ParserStatus == VVVSParserStatus::Error) {
					return Result;
				}

				auto PropertySet = ParserProperty(PropertyString, &Result);
				if (Result.ParserStatus == VVVSParserStatus::Error) {
					return Result;
				}

				PropertyString.clear();

				Selector->SelectorProperty = PropertySet;

				Result.SelectorSet.push_back(Selector);
			}
			else {
				ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

				return Result;
			}
		}

		return Result;
	}
};

VLIB_END_NAMESPACE