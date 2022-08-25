#pragma once

#include "../kits/seal.lexical.h"
#include "../kits/vparserhelper.h"
#include "../core/vbasiccore.h"

#include <map>
#include <vector>

VLIB_BEGIN_NAMESPACE

namespace VML {
    enum class VMLParserStatus {
        Ok, Error, Failed
    };

    struct VMLParserError {
        std::wstring ErrorString;
        int          Line;
    };

    enum class VMLPropertyType {
        IntValue, StringValue, DoubleValue, BooleanValue, NativeCall, ObjectCallParameter
    };

    struct VMLPropertyValue {
        std::wstring	 PropertyName = L"";

        VMLPropertyType  PropertyType = VMLPropertyType::IntValue;

        int				 PropertyAsInt = 0;
        std::wstring     PropertyAsString = L"";
        double           PropertyAsDouble = 0.f;
        bool             PropertyAsBool = false;

        std::vector<std::wstring>
                         PropertyAsObjectCallParameter;

        std::wstring     NativeCallMethodName = L"";
        std::vector<VMLPropertyValue> NativeCallParameter;
    };

    struct VMLNode {
        std::wstring							 NodeTag;
        std::map<std::wstring, VMLPropertyValue> NodeValue;

        std::map<std::wstring, VMLNode>		  ChildrenNodes;
        int                                       ChildrenSequence = 0;

        VMLPropertyValue GetProperty(const std::wstring& Type);
        bool PropertyExsit(const std::wstring& Type);
    };

    struct VMLParserResult {
        std::vector<VMLParserError>		ErrorInfo;
        std::map<std::wstring, VMLNode> Nodes;
        VMLParserStatus				 ParserStatus = VMLParserStatus::Ok;
    };

    enum class VMLParserParseMode {
        FromFile, FromString
    };

    class VMLParser {
    private:
        VKits::seal_lexical* ParserLexical = nullptr;

    private:
        void			 ThrowError(VMLParserResult* Result, const std::wstring& ErrorString);
        VMLPropertyValue ToPropertyValue(const std::wstring& String);
        int  BaseLine = 0;

    private:
        bool FileExist = true;

    public:
        VMLParser(const std::wstring& VMLString, VMLParserParseMode VMLParserMode = VMLParserParseMode::FromString, const int& Line = 0);

        VMLParserResult ParseVML();
    };
}

VLIB_END_NAMESPACE