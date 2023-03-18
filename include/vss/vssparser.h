#pragma once

#include "../core/render/vcolor.h"
#include "../kits/seal.lexical.h"
#include "../kits/vcolorhelper.h"
#include "../kits/vparserhelper.h"

VLIB_BEGIN_NAMESPACE

#include <map>
#include <vector>

namespace VSS
{
enum class VVVSParserParseMode
{
	FromFile,
	FromString
};
enum class VVVSParserStatus
{
	Error,
	Ok
};

enum class VSSPropertyType
{
	IntValue,
	StringValue,
	BooleanValue,
	DoubleValue,
	ColorValue
};

struct VSSPropertyValue
{
	VSSPropertyType Type = VSSPropertyType::IntValue;

	Core::VColor PropertyAsColorValue;
	int			 PropertyAsInt	  = 0;
	VString		 PropertyAsString = L"";
	double		 PropertyAsDouble = 0.f;
	bool		 PropertyAsBool	  = false;
};

struct VSSSelectorProperty
{
	VString						  PropertyTag;
	std::vector<VSSPropertyValue> ValueList;
};

enum class VSSSelectorType
{
	UnkownSelector,
	ElementSelector,
	IDSelector,
	ClassSelector,
	FakeClassSelector,
	ClassWithFakeClassSelector,
	GenericSelector,
	FakeElementSelector,
	FakeElementWithClassSelector,
	ClassWithFakeElementSelector,
	ClassWithFakeClassAndFakeElementSelector
};

class VSSBasicSelector
{
public:
	std::map<VString, VSSSelectorProperty> SelectorProperty;

	virtual VSSSelectorType GetType()
	{
		return VSSSelectorType::UnkownSelector;
	}

	VSSBasicSelector()
	{
	}
};
class VSSElementSelector : public VSSBasicSelector
{
public:
	VString ElementTag;

	VSSSelectorType GetType() override
	{
		return VSSSelectorType::ElementSelector;
	}
};
class VSSIDSelector : public VSSBasicSelector
{
public:
	VString IDTag;

	VSSSelectorType GetType() override
	{
		return VSSSelectorType::IDSelector;
	}
};
class VSSClassSelector : public VSSBasicSelector
{
public:
	VString ClassTag;
	VString TargetElement;

	VSSSelectorType GetType() override
	{
		return VSSSelectorType::ClassSelector;
	}
};
class VSSFakeClassSelector : public VSSBasicSelector
{
public:
	VString ElementTag;
	VString ClassTag;

	VSSSelectorType GetType() override
	{
		return VSSSelectorType::FakeClassSelector;
	}
};
class VSSClassWithFakeClassSelector : public VSSBasicSelector
{
public:
	VString ClassTag;
	VString ElementTag;
	VString FakeClassTag;

	VSSSelectorType GetType() override
	{
		return VSSSelectorType::ClassWithFakeClassSelector;
	}
};
class VSSClassWithFakeElementSelector : public VSSBasicSelector
{
public:
	VString ElementTag;
	VString ClassTag;

	VSSSelectorType GetType() override
	{
		return VSSSelectorType::ClassWithFakeElementSelector;
	}
};
class VSSClassWithFakeClassAndFakeElementSelector : public VSSBasicSelector
{
public:
	VString FakeElementTag;
	VString FakeClassTag;
	VString ClassTag;

	VSSSelectorType GetType() override
	{
		return VSSSelectorType::ClassWithFakeClassAndFakeElementSelector;
	}
};
class VSSGenericSelector : public VSSBasicSelector
{
public:
	VSSSelectorType GetType() override
	{
		return VSSSelectorType::GenericSelector;
	}
};
class VSSFakeElementSelector : public VSSBasicSelector
{
public:
	VString ElementTag;
	VString FakeElementTag;

public:
	VSSSelectorType GetType() override
	{
		return VSSSelectorType::FakeElementSelector;
	}
};
class VSSFakeElementWithClassSelector : public VSSBasicSelector
{
public:
	VString ElementTag;
	VString ClassTag;
	VString FakeElementTag;

public:
	VSSSelectorType GetType() override
	{
		return VSSSelectorType::FakeElementWithClassSelector;
	}
};

struct VSSParserError
{
	VString		 ErrorInformation;
	unsigned int ErrorLine = 0;
};

struct VSSParserResult
{
	std::vector<VSSBasicSelector *> SelectorSet;

	VSSParserError	 ErrorInfo;
	VVVSParserStatus ParserStatus = VVVSParserStatus::Ok;
};

class VSSParser
{
private:
	VKits::seal_lexical *ParserLexical;

	unsigned int BaseLine = 0;

private:
	std::map<VString, VSSSelectorProperty> ParserProperty(const VString &PropertyString, VSSParserResult *Result);

private:
	void ThrowError(VSSParserResult *Result, const VString &ErrorString)
	{
		(*Result).ParserStatus = VVVSParserStatus::Error;
		(*Result).ErrorInfo	   = {ErrorString, BaseLine + ParserLexical->get_line() + 1};
	}

private:
	VString GetPropertyString(VSSParserResult *Result)
	{
		VString											  PropertyString;
		bool											  EndFlag = false;
		VKits::seal_lexical_type_info::seal_lexical_token Token;

		while (!ParserLexical->is_eof())
		{
			Token = ParserLexical->get_token();

			if (Token.token_string == L"}")
			{
				EndFlag = true;

				break;
			}

			PropertyString.append(Token.token_string + L" ");
		}

		if (EndFlag == false)
		{
			ThrowError(Result, L"Couldn't Found the Match Symbol Of \"{\"");

			return PropertyString;
		}

		return PropertyString;
	}

public:
	VSSParser(const VString &VSString, VVVSParserParseMode VVSParserMode = VVVSParserParseMode::FromString,
			  const int &Line = 0);
	~VSSParser();

	VSSParserResult ParseVSS();
};
} // namespace VSS

VLIB_END_NAMESPACE