#include "../../include/vss/vssparser.h"

VLIB_BEGIN_NAMESPACE

namespace VSS {
    std::map<std::wstring, VSSSelectorProperty> VSSParser::ParserProperty(const std::wstring& PropertyString, VSSParserResult* Result) {
        VKits::seal_lexical PropertyLexical(PropertyString);
        VKits::seal_lexical_type_info::seal_lexical_token
                Token;

        std::map<std::wstring, VSSSelectorProperty>
                ReturnValue;

        while (!PropertyLexical.is_eof()) {
            VSSSelectorProperty Property;

            Token = PropertyLexical.get_token();

            if (Token.cache_token == EOF_TOKEN) {
                break;
            }
            if (Token.cache_token != UNKNOW_TOKEN) {
                ThrowError(Result, L"Unknown Character \"" + Token.token_string + L"\"");

                return ReturnValue;
            }

            Property.PropertyTag = Token.token_string;

            Token = PropertyLexical.get_token();
            if (Token.token_string != L":") {
                ThrowError(Result, L"Unknown Character \"" + Token.token_string + L"\"");

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
                    VSSPropertyValue PropertyValue;

                    std::wstring HexString;

                    while (true) {
                        Token = PropertyLexical.get_token();

                        if (Token.cache_token == EOF_TOKEN) {
                            break;
                        }

                        HexString += Token.token_string;

                        if (HexString.size() >= 6) {
                            break;
                        }
                    }

                    VKits::VSSColorHelper::HexStringLowerToUpper(HexString);

                    if (!VKits::VSSColorHelper::IsValidHex(L"#" + HexString)) {
                        if (!Token.token_string.empty()) {
                            ThrowError(Result, L"Unknown Hex Color \"" + Token.token_string + L"\"");
                        }
                        else {
                            ThrowError(Result, L"The Hex Color is Necessary");
                        }

                        return ReturnValue;
                    }
                    else {
                        PropertyValue.Type                 = VSSPropertyType::ColorValue;
                        PropertyValue.PropertyAsColorValue = VKits::VSSColorHelper::HexToColor(L"#" + HexString);

                        Property.ValueList.push_back(PropertyValue);
                    }

                    continue;
                }
                // RGB Color
                if (Token.token_string == L"rgb") {
                    VSSPropertyValue PropertyValue;

                    Token = PropertyLexical.get_token();
                    if (Token.token_string != L"(") {
                        ThrowError(Result, L"Unknown Character \"" + Token.token_string + L"\"");

                        return ReturnValue;
                    }

                    int ColorArrary[3] = { 0, 0, 0 };

                    for (int Count = 0; Count < 3; ++Count) {
                        Token = PropertyLexical.get_token();
                        if (Token.cache_token != CONST_NUMBER) {
                            ThrowError(Result, L"Unknown Character \"" + Token.token_string + L"\" (It Should Be a Number)");

                            return ReturnValue;
                        }

                        ColorArrary[Count] = _wtoi(Token.token_string.c_str());

                        Token = PropertyLexical.get_token();

                        if (Count < 2) {
                            if (Token.token_string != L",") {
                                ThrowError(Result, L"Unknown Character \"" + Token.token_string + L"\" (Do you mean \',\'?)");

                                return ReturnValue;
                            }
                        }
                        else {
                            if (Token.token_string != L")") {
                                ThrowError(Result, L"Unknown Character \"" + Token.token_string + L"\" (Do you mean \')\'?)");

                                return ReturnValue;
                            }
                        }
                    }

                    PropertyValue.Type = VSSPropertyType::ColorValue;
                    PropertyValue.PropertyAsColorValue
                            = Core::VColor::FromBYTERGBA(ColorArrary[0], ColorArrary[1], ColorArrary[2]);

                    Property.ValueList.push_back(PropertyValue);

                    continue;
                }
                // RGBA Color
                if (Token.token_string == L"rgba") {
                    VSSPropertyValue PropertyValue;

                    Token = PropertyLexical.get_token();
                    if (Token.token_string != L"(") {
                        ThrowError(Result, L"Unknown Character \"" + Token.token_string + L"\"");

                        return ReturnValue;
                    }

                    int    ColorArrary[3] = { 0, 0, 0 };
                    double Alpha = 0.f;

                    for (int Count = 0; Count < 4; ++Count) {
                        Token = PropertyLexical.get_token();
                        if (Token.cache_token != CONST_NUMBER) {
                            ThrowError(Result, L"Unknown Character \"" + Token.token_string + L"\" (It Should Be a Number)");

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
                                ThrowError(Result, L"Unknown Character \"" + Token.token_string + L"\" (Do you mean \',\'?)");

                                return ReturnValue;
                            }
                        }
                        else {
                            if (Token.token_string != L")") {
                                ThrowError(Result, L"Unknown Character \"" + Token.token_string + L"\" (Do you mean \')\'?)");

                                return ReturnValue;
                            }
                        }
                    }

                    PropertyValue.Type = VSSPropertyType::ColorValue;
                    PropertyValue.PropertyAsColorValue
                            = Core::VColor::FromBYTERGBA(ColorArrary[0], ColorArrary[1], ColorArrary[2], 255 * Alpha);

                    Property.ValueList.push_back(PropertyValue);

                    continue;
                }

                // Number Property
                if (Token.cache_token == CONST_NUMBER) {
                    VSSPropertyValue NumberProperty;

                    std::wstring NumberCache = Token.token_string;
                    Token                    = PropertyLexical.get_token();

                    if (Token.token_string == L"px") {
                        if (NumberCache.find(L".") != Token.token_string.npos) {
                            NumberProperty.Type = VSSPropertyType::DoubleValue;
                            NumberProperty.PropertyAsDouble = _wtof(NumberCache.c_str());
                        }
                        else {
                            NumberProperty.Type = VSSPropertyType::IntValue;
                            NumberProperty.PropertyAsInt = _wtoi(NumberCache.c_str());
                        }
                    }
                    else if (Token.token_string == L"%") {
                        if (NumberCache.find(L".") != Token.token_string.npos) {
                            NumberProperty.Type = VSSPropertyType::DoubleValue;
                            NumberProperty.PropertyAsDouble = _wtof(NumberCache.c_str()) / 100.f;
                        }
                        else {
                            NumberProperty.Type             = VSSPropertyType::DoubleValue;
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
                if (Token.cache_token == CONST_BOOLEAN) {
                    VSSPropertyValue BooleanProperty;

                    if (Token.token_string == L"true") {
                        BooleanProperty.Type             = VSSPropertyType::BooleanValue;
                        BooleanProperty.PropertyAsBool   = true;
                    }
                    else {
                        BooleanProperty.Type           = VSSPropertyType::BooleanValue;
                        BooleanProperty.PropertyAsBool = false;
                    }

                    Property.ValueList.push_back(BooleanProperty);

                    continue;
                }
                // String Property
                if (Token.cache_token == CONST_STRING) {
                    VSSPropertyValue StringProperty;

                    StringProperty.Type             = VSSPropertyType::StringValue;
                    StringProperty.PropertyAsString = Token.token_string.substr(1, Token.token_string.size() - 2);

                    Property.ValueList.push_back(StringProperty);

                    continue;
                }
                // Const Property
                if (Token.cache_token == UNKNOW_TOKEN) {
                    VSSPropertyValue StringProperty;

                    StringProperty.Type = VSSPropertyType::StringValue;
                    StringProperty.PropertyAsString = Token.token_string;

                    Property.ValueList.push_back(StringProperty);

                    continue;
                }
            }

            if (!EndTokenFound) {
                ThrowError(Result, L"Couldn't Found the Match Symbol of \"{\"");

                return ReturnValue;
            }

            ReturnValue.insert(std::pair<std::wstring, VSSSelectorProperty>(Property.PropertyTag, Property));
        }

        return ReturnValue;
    }
    VSSParser::VSSParser(const std::wstring& VSString, VVVSParserParseMode VVSParserMode, const int& Line) {
        switch (VVSParserMode) {
            case VVVSParserParseMode::FromString: {
                ParserLexical = new VKits::seal_lexical(VSString);
                BaseLine = Line;

                break;
            }
            case VVVSParserParseMode::FromFile: {
                if (VKits::VParserHelper::FileExist(VSString)) {
                    ParserLexical = new VKits::seal_lexical(VKits::VParserHelper::ReadFromFile(VSString));
                    BaseLine = Line;
                }
                else {
                    ParserLexical = nullptr;
                }

                break;
            }
            default: {
                ParserLexical = new VKits::seal_lexical(VSString);
                BaseLine = Line;

                break;
            }
        }
    }
    VSSParser::~VSSParser() {
        delete ParserLexical;
    }
    VSSParserResult VSSParser::ParseVSS() {
        VSSParserResult Result;

        if (ParserLexical == nullptr) {
            ThrowError(&Result, L"File doesn't exist!");

            return Result;
        }

        VKits::seal_lexical_type_info::seal_lexical_token
                Token;

        while (!ParserLexical->is_eof()) {
            VSSBasicSelector* Selector;

            Token = ParserLexical->get_token();

            if (Token.cache_token == EOF_TOKEN) {
                break;
            }

            std::wstring LevelOneTag;

            if (Token.cache_token == UNKNOW_TOKEN) {
                LevelOneTag = Token.token_string;
                Token       = ParserLexical->get_token();

                // Element Selector
                if (Token.token_string == L"{") {
                    Selector = new VSSElementSelector;

                    static_cast<VSSElementSelector*>(Selector)->ElementTag = LevelOneTag;

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

                    if (Token.cache_token != UNKNOW_TOKEN) {
                        ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

                        return Result;
                    }

                    Token = ParserLexical->get_token();

                    // Class Selector
                    if (Token.token_string == L"{") {
                        Selector = new VSSClassSelector;

                        static_cast<VSSClassSelector*>(Selector)->ClassTag      = LevelTwoTag;
                        static_cast<VSSClassSelector*>(Selector)->TargetElement = LevelOneTag;

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
                        Selector = new VSSClassWithFakeClassSelector;

                        Token = ParserLexical->get_token();
                        if (Token.cache_token != UNKNOW_TOKEN) {
                            ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

                            return Result;
                        }

                        std::wstring LevelThreeTag = Token.token_string;

                        static_cast<VSSClassWithFakeClassSelector*>(Selector)->ClassTag      = LevelTwoTag;
                        static_cast<VSSClassWithFakeClassSelector*>(Selector)->ElementTag    = LevelOneTag;
                        static_cast<VSSClassWithFakeClassSelector*>(Selector)->FakeClassTag  = LevelThreeTag;

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
                }
                else if (Token.token_string == L":") {
                    Token = ParserLexical->get_token();

                    auto PreToken = ParserLexical->view_token(2);

                    // Fake Class Selector
                    if (Token.cache_token == UNKNOW_TOKEN) {
                        Selector = new VSSFakeClassSelector;

                        static_cast<VSSFakeClassSelector *>(Selector)->ClassTag = Token.token_string;
                        static_cast<VSSFakeClassSelector *>(Selector)->ElementTag = LevelOneTag;

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
                    // Fake Element Selector
                    else if (Token.token_string == L":" &&
                             PreToken.token_string != L":") {
                        Selector = new VSSFakeElementSelector;

                        Token = ParserLexical->get_token();
                        if (Token.cache_token != UNKNOW_TOKEN) {
                            ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

                            return Result;
                        }

                        static_cast<VSSFakeElementSelector *>(Selector)->ElementTag = LevelOneTag;
                        static_cast<VSSFakeElementSelector *>(Selector)->FakeElementTag = Token.token_string;

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
                    // Fake Element Selector with Fake Class
                    else if (PreToken.token_string == L":") {
                        Selector = new VSSFakeElementWithClassSelector;

                        Token = ParserLexical->get_token();
                        if (Token.cache_token != UNKNOW_TOKEN) {
                            ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

                            return Result;
                        }

                        static_cast<VSSFakeElementWithClassSelector *>(Selector)->ElementTag = LevelOneTag;
                        static_cast<VSSFakeElementWithClassSelector *>(Selector)->FakeElementTag = Token.token_string;

                        Token = ParserLexical->get_token();
                        if (Token.token_string != L":") {
                            ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

                            return Result;
                        }
                        Token = ParserLexical->get_token();
                        if (Token.cache_token != UNKNOW_TOKEN) {
                            ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

                            return Result;
                        }

                        static_cast<VSSFakeElementWithClassSelector *>(Selector)->ClassTag = Token.token_string;

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
                else {
                    ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

                    return Result;
                }
            }
            // Class Selector
            else if (Token.cache_token == DOT_TOKEN) {
                Token = ParserLexical->get_token();

                std::wstring LevelTwoTag = Token.token_string;

                if (Token.cache_token != UNKNOW_TOKEN) {
                    ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

                    return Result;
                }

                Token = ParserLexical->get_token();

                if (Token.token_string == L"{") {
                    Selector = new VSSClassSelector;
                    static_cast<VSSClassSelector*>(Selector)->ClassTag = LevelTwoTag;

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
                    Token = ParserLexical->get_token();

                    auto PreToken = ParserLexical->view_token(2);

                    // Fake Class Selector
                    if (Token.cache_token == UNKNOW_TOKEN) {
                        Selector = new VSSClassWithFakeClassSelector;

                        static_cast<VSSClassWithFakeClassSelector *>(Selector)->FakeClassTag = Token.token_string;
                        static_cast<VSSClassWithFakeClassSelector *>(Selector)->ClassTag     = LevelTwoTag;

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
                        // Fake Element Selector
                    else if (Token.token_string == L":" &&
                             PreToken.token_string != L":") {
                        Selector = new VSSClassWithFakeElementSelector;

                        Token = ParserLexical->get_token();
                        if (Token.cache_token != UNKNOW_TOKEN) {
                            ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

                            return Result;
                        }

                        static_cast<VSSClassWithFakeElementSelector *>(Selector)->ClassTag = LevelTwoTag;
                        static_cast<VSSClassWithFakeElementSelector *>(Selector)->ElementTag = Token.token_string;

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
                        // Fake Element Selector with Fake Class
                    else if (PreToken.token_string == L":") {
                        Selector = new VSSFakeElementWithClassSelector;

                        Token = ParserLexical->get_token();
                        if (Token.cache_token != UNKNOW_TOKEN) {
                            ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

                            return Result;
                        }

                        static_cast<VSSClassWithFakeClassAndFakeElementSelector *>(Selector)->ClassTag       = LevelOneTag;
                        static_cast<VSSClassWithFakeClassAndFakeElementSelector *>(Selector)->FakeElementTag = Token.token_string;

                        Token = ParserLexical->get_token();
                        if (Token.token_string != L":") {
                            ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

                            return Result;
                        }
                        Token = ParserLexical->get_token();
                        if (Token.cache_token != UNKNOW_TOKEN) {
                            ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

                            return Result;
                        }

                        static_cast<VSSClassWithFakeClassAndFakeElementSelector *>(Selector)->FakeClassTag = Token.token_string;

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
                else {
                    ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

                    return Result;
                }
            }
                // ID Selector
            else if (Token.token_string == L"#") {
                Token = ParserLexical->get_token();
                if (Token.cache_token != UNKNOW_TOKEN) {
                    ThrowError(&Result, L"Unknown Character \"" + Token.token_string + L"\"");

                    return Result;
                }

                Selector = new VSSIDSelector;
                static_cast<VSSIDSelector*>(Selector)->IDTag = Token.token_string;

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
                // Generic Selector
            else if (Token.token_string == L"*") {
                Selector = new VSSGenericSelector;

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

        return Result;
    }
}

VLIB_END_NAMESPACE