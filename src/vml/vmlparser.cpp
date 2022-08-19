#include "../../include/vml/vmlparser.h"

VLIB_BEGIN_NAMESPACE

namespace VML {
    VMLPropertyValue VMLNode::GetProperty(const std::wstring& Type) {
        VMLPropertyValue Value = NodeValue.find(Type)->second;

        return Value;
    }
    bool VMLNode::PropertyExsit(const std::wstring& Type) {
        if (NodeValue.find(Type) != NodeValue.end()) {
            return true;
        }

        return false;
    }

    void VMLParser::ThrowError(VMLParserResult* Result, const std::wstring& ErrorString) {
        (*Result).ParserStatus = VMLParserStatus::Error;
        (*Result).ErrorInfo.push_back({ ErrorString, BaseLine + ParserLexical->get_line() + 1 });
    }
    VMLPropertyValue VMLParser::ToPropertyValue(const std::wstring& String) {
        std::wstring ValueString = String.substr(1, String.size() - 2);

        if (ValueString == L"true") {
            VMLPropertyValue Value;
            Value.PropertyAsBool = true;
            Value.PropertyType = VMLPropertyType::BooleanValue;

            return Value;
        }
        if (ValueString == L"false") {
            VMLPropertyValue Value;
            Value.PropertyAsBool = false;
            Value.PropertyType = VMLPropertyType::BooleanValue;

            return Value;
        }

        VMLPropertyValue Value;
        VMLPropertyType  Type = VMLPropertyType::IntValue;

        bool DotExsit = false;

        for (auto& Character : ValueString) {
            if (!(Character >= L'0' && Character <= L'9')) {
                if (Character == L'.') {
                    if (!DotExsit) {
                        DotExsit = true;

                        Type = VMLPropertyType::DoubleValue;
                    }
                    else {
                        Type = VMLPropertyType::StringValue;
                    }
                }
                else {
                    Type = VMLPropertyType::StringValue;
                }
            }
        }

        Value.PropertyType = Type;

        switch (Type) {
            case VMLPropertyType::IntValue: {
                Value.PropertyAsInt = _wtoi(ValueString.c_str());
                break;
            }
            case VMLPropertyType::StringValue: {
                Value.PropertyAsString = ValueString;
                break;
            }
            case VMLPropertyType::DoubleValue: {
                Value.PropertyAsDouble = _wtof(ValueString.c_str());
                break;
            }
        }

        return Value;
    }
    VMLParser::VMLParser(const std::wstring& VMLString, VMLParserParseMode VMLParserMode, const int& Line) {
        switch (VMLParserMode) {
            case VMLParserParseMode::FromString: {
                ParserLexical = new VKits::seal_lexical(VMLString);
                BaseLine = Line;

                break;
            }
            case VMLParserParseMode::FromFile: {
                if (VKits::VParserHelper::FileExist(VMLString)) {
                    ParserLexical = new VKits::seal_lexical(VKits::VParserHelper::ReadFromFile(VMLString));
                    BaseLine = Line;
                }
                else {
                    FileExist = false;
                }

                break;
            }
            default: {
                ParserLexical = new VKits::seal_lexical(VMLString);
                BaseLine = Line;

                break;
            }
        }
    }
    VMLParserResult VMLParser::ParseVML() {
        VMLParserResult ParseResult;

        if (!FileExist) {
            ParseResult.ErrorInfo.push_back({ L"Target File Dosen't Exsits!", 0 });
            ParseResult.ParserStatus = VMLParserStatus::Failed;

            return ParseResult;
        }

        VKits::seal_lexical_type_info::_lexical_token_type Token;

        int ChildrenSequence = 0;

        while (!ParserLexical->is_eof()) {
            Token = ParserLexical->get_token();

            if (Token.cache_token == EOF_TOKEN) {
                break;
            }

            // If the Begin of the Code Isn't '<' That Certain Error
            if (Token.cache_token != LESS_THAN_TOKEN) {
                ThrowError(&ParseResult, L"Unknown Token : \"" + Token.token_string + L"\"");

                break;
            }

            Token = ParserLexical->get_token();

            bool BreakTokenTrigger = false;

            // Skip Comment
            if (Token.token_string == L"!") {
                bool EndFlag = false;

                // Parse Node Property
                while (!ParserLexical->is_eof()) {
                    Token = ParserLexical->get_token();

                    if (Token.token_string == L"--") {
                        if (!BreakTokenTrigger) {
                            BreakTokenTrigger = true;

                            continue;
                        }

                        Token = ParserLexical->get_token();

                        if (Token.token_string != L">") {
                            ThrowError(&ParseResult, L"Unknow Character \"" + Token.token_string + L"\"");
                        }

                        EndFlag = true;

                        break;
                    }
                }

                if (!EndFlag) {
                    ThrowError(&ParseResult, L"Could Not Find the Match End Block of the Comment");

                    return ParseResult;
                }

                continue;
            }

            if (Token.cache_token != UNKNOW_TOKEN) {
                ThrowError(&ParseResult, L"Unknown Token : \"" + Token.token_string + L"\"");

                break;
            }

            if (ParseResult.Nodes.find(Token.token_string) != ParseResult.Nodes.end()) {
                ThrowError(&ParseResult, L"The VML Node \"" + Token.token_string + L"\" Already Exists!");

                return ParseResult;
            }

            VMLNode NewNode;
            NewNode.NodeTag = Token.token_string;

            bool EndFlag = false;

            std::wstring PropertyName;

            // Parse Node Property
            while (!ParserLexical->is_eof()) {
                Token = ParserLexical->get_token();

                if (Token.token_string == L">") {
                    EndFlag = true;

                    break;
                }
                if (Token.cache_token != UNKNOW_TOKEN) {
                    ThrowError(&ParseResult, L"Unknown Token : \"" + Token.token_string + L"\"");

                    return ParseResult;
                }

                PropertyName = Token.token_string;

                // Skip White Space
                Token = ParserLexical->get_token();
                if (Token.cache_token != EQUAL_SIGN_TOKEN) {
                    ThrowError(&ParseResult, L"Unknown Token : \"" + Token.token_string + L"\"");

                    return ParseResult;
                }

                Token = ParserLexical->get_token();

                if (Token.cache_token != CONST_STRING) {
                    ThrowError(&ParseResult, L"Unknown Token : \"" + Token.token_string + L"\"");

                    return ParseResult;
                }

                auto Property = ToPropertyValue(Token.token_string);
                Property.PropertyName = PropertyName;

                NewNode.NodeValue.insert(std::pair<std::wstring, VMLPropertyValue>(PropertyName,
                                                                                   Property));
            }

            // Parse the Sub Nodes of This Node
            std::wstring  SubContent;

            unsigned long LeftBracketCount = 0;

            while (!ParserLexical->is_eof()) {
                Token = ParserLexical->get_token();

                if (Token.token_string == L"<") {
                    Token = ParserLexical->get_token();

                    bool BreakTokenTrigger = false;

                    // Skip Comment
                    if (Token.token_string == L"!") {
                        bool EndFlag = false;

                        // Parse Node Property
                        while (!ParserLexical->is_eof()) {
                            Token = ParserLexical->get_token();

                            if (Token.token_string == L"--") {
                                if (!BreakTokenTrigger) {
                                    BreakTokenTrigger = true;

                                    continue;
                                }

                                Token = ParserLexical->get_token();

                                if (Token.token_string != L">") {
                                    ThrowError(&ParseResult, L"Unknow Character \"" + Token.token_string + L"\"");
                                }

                                EndFlag = true;

                                break;
                            }
                        }

                        if (!EndFlag) {
                            ThrowError(&ParseResult, L"Could Not Find the Match End Block of the Comment");

                            return ParseResult;
                        }

                        continue;
                    }
                    if (Token.token_string == L"/") {
                        if (LeftBracketCount == 0) {
                            Token = ParserLexical->get_token();

                            if (Token.token_string != NewNode.NodeTag) {
                                ThrowError(&ParseResult, L"Could Not Find the Match End Block of \"" + Token.token_string + L"\"");

                                return ParseResult;
                            }

                            Token = ParserLexical->get_token();

                            if (Token.cache_token != MORE_THAN_TOKEN) {
                                ThrowError(&ParseResult, L"Unknown Token : \"" + Token.token_string + L"\"");

                                return ParseResult;
                            }

                            break;
                        }

                        SubContent.append(L"<");

                        --LeftBracketCount;
                    }
                    else {
                        SubContent.append(L"<");

                        ++LeftBracketCount;
                    }
                }

                SubContent.append(Token.token_string + L" ");
            }

            if (!SubContent.empty()) {
                VMLParser* SubContentParser = new VMLParser(SubContent, VMLParserParseMode::FromString, ParserLexical->get_line());
                auto Result = SubContentParser->ParseVML();

                if (Result.ParserStatus == VMLParserStatus::Error) {
                    ParseResult.ParserStatus = VMLParserStatus::Error;

                    for (auto& ErrorInfo : Result.ErrorInfo) {
                        ParseResult.ErrorInfo.push_back(ErrorInfo);
                    }

                    return ParseResult;
                }
                for (auto& ChildrenNode : Result.Nodes) {
                    NewNode.ChildrenNodes.insert(
                            std::pair<std::wstring, VMLNode>(ChildrenNode.first, ChildrenNode.second));
                }
            }

            NewNode.ChildrenSequence = ChildrenSequence;

            ++ChildrenSequence;

            ParseResult.Nodes.insert(std::pair<std::wstring, VMLNode>(NewNode.NodeTag, NewNode));

            if (!EndFlag) {
                ThrowError(&ParseResult, L"Could Not Found The Match Symbol Of \"<\"");

                break;
            }
        }

        return ParseResult;
    }
}

VLIB_END_NAMESPACE