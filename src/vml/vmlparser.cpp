#include "../../include/vml/vmlparser.h"

VLIB_BEGIN_NAMESPACE

namespace VML
{
VMLPropertyValue VMLNode::GetProperty(const std::wstring &Type)
{
	VMLPropertyValue Value = NodeValue.find(Type)->second;

	return Value;
}
bool VMLNode::PropertyExsit(const std::wstring &Type)
{
	if (NodeValue.find(Type) != NodeValue.end())
	{
		return true;
	}

	return false;
}
bool VMLParser::CheckParameter(const std::vector<VMLPropertyValue>	 &CheckedObject,
							   std::initializer_list<VMLPropertyType> CheckedList)
{
	if (CheckedObject.size() != CheckedList.size())
	{
		return false;
	}

	auto Iterator = CheckedList.begin();
	for (int Count = 0; Count < CheckedObject.size(); ++Count)
	{
		if (CheckedObject[Count].PropertyType != *Iterator)
		{
			return false;
		}

		++Iterator;
	}

	return true;
}
void VMLParser::ThrowError(VMLParserResult *Result, const std::wstring &ErrorString)
{
	(*Result).ParserStatus = VMLParserStatus::Error;
	(*Result).ErrorInfo.push_back({ErrorString, static_cast<int>(BaseLine + ParserLexical->get_line() + 1)});
}
VMLPropertyValue VMLParser::ToPropertyValue(const std::wstring &String)
{
	std::wstring ValueString = String.substr(1, String.size() - 2);

	if (ValueString[0] == L'$')
	{
		VMLPropertyValue Value;
		Value.PropertyType = VMLPropertyType::VariableCall;

		VKits::seal_lexical *VariableParameterLexical = new VKits::seal_lexical(ValueString);

		// Skip $
		VariableParameterLexical->get_token();

		Value.NativeCallMethodName = VariableParameterLexical->get_token().token_string;

		auto GrammarCheckToken = VariableParameterLexical->get_token();

		if (GrammarCheckToken.token_string != L"(" || GrammarCheckToken.cache_token == EOF_TOKEN)
		{
			return Value;
		}

		Value.PropertyType = VMLPropertyType::VariableDefine;

		std::wstring PropertyValue;
		while (!VariableParameterLexical->is_eof())
		{
			auto CacheToken = VariableParameterLexical->get_token();

			if (CacheToken.cache_token != CONST_STRING)
			{
				CacheToken.token_string.insert(CacheToken.token_string.begin(), L'\"');
				CacheToken.token_string.insert(CacheToken.token_string.end(), L'\"');
			}

			PropertyValue	 = CacheToken.token_string;
			auto TokenString = VariableParameterLexical->get_token().token_string;

			if (TokenString == L".")
			{
				VMLPropertyValue ObjectCallValue;

				ObjectCallValue.PropertyType = VMLPropertyType::ObjectCallParameter;

				std::wstring PropertyValue;
				while (!VariableParameterLexical->is_eof())
				{
					ObjectCallValue.PropertyAsObjectCallParameter.push_back(CacheToken.token_string);

					CacheToken = VariableParameterLexical->get_token();
					if (CacheToken.token_string != L"." || CacheToken.token_string == L")")
					{
						break;
					}

					CacheToken = VariableParameterLexical->get_token();
				}

				Value.NativeCallParameter.push_back(ObjectCallValue);
			}
			else if (TokenString != L"," || TokenString == L")")
			{
				Value.NativeCallParameter.push_back(ToPropertyValue(PropertyValue));
				PropertyValue.clear();

				break;
			}
			else
			{
				Value.NativeCallParameter.push_back(ToPropertyValue(PropertyValue));
			}

			PropertyValue.clear();
		}

		if (CheckParameter(Value.VariableInitValue, {VMLPropertyType::IntValue}))
		{
			Value.VariableType = VMLVariableType::IntType;
		}
		if (CheckParameter(Value.VariableInitValue, {VMLPropertyType::StringValue}))
		{
			Value.VariableType = VMLVariableType::StringType;
		}
		if (CheckParameter(Value.VariableInitValue, {VMLPropertyType::DoubleValue}))
		{
			Value.VariableType = VMLVariableType::DoubleType;
		}
		if (CheckParameter(Value.VariableInitValue, {VMLPropertyType::BooleanValue}))
		{
			Value.VariableType = VMLVariableType::BooleanType;
		}

		return Value;
	}
	if (ValueString[0] == L'@')
	{
		VMLPropertyValue Value;
		Value.PropertyType = VMLPropertyType::NativeCall;

		VKits::seal_lexical *NativeCallLexical = new VKits::seal_lexical(ValueString);

		// Skip @
		NativeCallLexical->get_token();

		Value.NativeCallMethodName = NativeCallLexical->get_token().token_string;

		auto GrammarCheckToken = NativeCallLexical->get_token();

		if (GrammarCheckToken.token_string != L"(" || GrammarCheckToken.cache_token == EOF_TOKEN)
		{
			return Value;
		}

		std::wstring PropertyValue;
		while (!NativeCallLexical->is_eof())
		{
			auto CacheToken = NativeCallLexical->get_token();

			if (CacheToken.cache_token != CONST_STRING)
			{
				CacheToken.token_string.insert(CacheToken.token_string.begin(), L'\"');
				CacheToken.token_string.insert(CacheToken.token_string.end(), L'\"');
			}

			PropertyValue = CacheToken.token_string;

			auto TokenString = NativeCallLexical->get_token().token_string;

			if (TokenString == L".")
			{
				VMLPropertyValue ObjectCallValue;

				ObjectCallValue.PropertyType = VMLPropertyType::ObjectCallParameter;

				std::wstring PropertyValue;
				while (!NativeCallLexical->is_eof())
				{
					ObjectCallValue.PropertyAsObjectCallParameter.push_back(CacheToken.token_string);

					CacheToken = NativeCallLexical->get_token();
					if (CacheToken.token_string != L"." || CacheToken.token_string == L")")
					{
						break;
					}

					CacheToken = NativeCallLexical->get_token();
				}

				Value.NativeCallParameter.push_back(ObjectCallValue);
			}
			else if (TokenString != L"," || TokenString == L")")
			{
				Value.NativeCallParameter.push_back(ToPropertyValue(PropertyValue));
				PropertyValue.clear();

				break;
			}
			else
			{
				Value.NativeCallParameter.push_back(ToPropertyValue(PropertyValue));
			}

			PropertyValue.clear();
		}

		return Value;
	}
	if (ValueString[0] == L'-')
	{
		auto SpiltString = ValueString.substr(1, ValueString.size() - 1);

		bool DotExsit = false;

		VMLPropertyValue Value;
		VMLPropertyType	 Type = VMLPropertyType::IntValue;

		for (auto &Character : SpiltString)
		{
			if (!(Character >= L'0' && Character <= L'9'))
			{
				if (Character == L'.')
				{
					if (!DotExsit)
					{
						DotExsit = true;

						Type = VMLPropertyType::DoubleValue;
					}
					else
					{
						Type = VMLPropertyType::StringValue;
					}
				}
				else
				{
					Type = VMLPropertyType::StringValue;
				}
			}
		}

		Value.PropertyType = Type;

		switch (Type)
		{
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

	if (ValueString == L"true")
	{
		VMLPropertyValue Value;
		Value.PropertyAsBool = true;
		Value.PropertyType	 = VMLPropertyType::BooleanValue;

		return Value;
	}
	if (ValueString == L"false")
	{
		VMLPropertyValue Value;
		Value.PropertyAsBool = false;
		Value.PropertyType	 = VMLPropertyType::BooleanValue;

		return Value;
	}

	VMLPropertyValue Value;
	VMLPropertyType	 Type = VMLPropertyType::IntValue;

	bool DotExsit = false;

	for (auto &Character : ValueString)
	{
		if (!(Character >= L'0' && Character <= L'9'))
		{
			if (Character == L'.')
			{
				if (!DotExsit)
				{
					DotExsit = true;

					Type = VMLPropertyType::DoubleValue;
				}
				else
				{
					Type = VMLPropertyType::StringValue;
				}
			}
			else
			{
				DotExsit = true;
				Type	 = VMLPropertyType::StringValue;
			}
		}
	}

	Value.PropertyType = Type;

	switch (Type)
	{
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
VMLParser::VMLParser(const std::wstring &VMLString, VMLParserParseMode VMLParserMode, const int &Line)
{
	switch (VMLParserMode)
	{
	case VMLParserParseMode::FromString: {
		FilePath = VMLString;

		ParserLexical = new VKits::seal_lexical(VMLString);
		BaseLine	  = Line;

		break;
	}
	case VMLParserParseMode::FromFile: {
		if (VKits::VParserHelper::FileExist(VMLString))
		{
			ParserLexical = new VKits::seal_lexical(VKits::VParserHelper::ReadFromFile(VMLString));
			BaseLine	  = Line;
		}
		else
		{
			FileExist = false;
		}

		break;
	}
	default: {
		ParserLexical = new VKits::seal_lexical(VMLString);
		BaseLine	  = Line;

		break;
	}
	}
}
VMLParser::~VMLParser()
{
	delete ParserLexical;
}
VMLParserResult VMLParser::ParseVML()
{
	VMLParserResult ParseResult;

	if (FilePath.find(L"/") != -1)
	{
		ParseResult.FilePath = FilePath.substr(0, FilePath.find_last_of(L"/"));
	}
	else
	{
		ParseResult.FilePath = FilePath.substr(0, FilePath.find_last_of(L"\\"));
	}

	if (!FileExist)
	{
		ParseResult.ErrorInfo.push_back({L"Target file dosen't exists!", 0});
		ParseResult.ParserStatus = VMLParserStatus::Failed;

		return ParseResult;
	}

	VKits::seal_lexical_type_info::_lexical_token_type Token;

	int ChildrenSequence = 0;

	while (!ParserLexical->is_eof())
	{
		Token = ParserLexical->get_token();

		if (Token.cache_token == EOF_TOKEN)
		{
			break;
		}

		// If the Begin of the Code Isn't '<' That Certain Error
		if (Token.cache_token != LESS_THAN_TOKEN)
		{
			ThrowError(&ParseResult, L"Unknown Token : \"" + Token.token_string + L"\", did you mean \"<\"?");

			break;
		}

		Token = ParserLexical->get_token();

		bool BreakTokenTrigger = false;

		// Skip Comment
		if (Token.token_string == L"!")
		{
			bool EndFlag = false;

			// Parse Node Property
			while (!ParserLexical->is_eof())
			{
				Token = ParserLexical->get_token();

				if (Token.token_string == L"--")
				{
					if (!BreakTokenTrigger)
					{
						BreakTokenTrigger = true;

						continue;
					}

					Token = ParserLexical->get_token();

					if (Token.token_string != L">")
					{
						ThrowError(&ParseResult, L"Unknown Character \"" + Token.token_string + L"\"");
					}

					EndFlag = true;

					break;
				}
			}

			if (!EndFlag)
			{
				ThrowError(&ParseResult, L"Could Not Find the Match End Block of the Comment");

				return ParseResult;
			}

			continue;
		}

		if (Token.cache_token != UNKNOW_TOKEN)
		{
			ThrowError(&ParseResult, L"Unknown Token : \"" + Token.token_string + L"\".");

			break;
		}

		if (ParseResult.Nodes.find(Token.token_string) != ParseResult.Nodes.end())
		{
			ThrowError(&ParseResult, L"The VML node \"" + Token.token_string + L"\" already exists!");

			return ParseResult;
		}

		VMLNode NewNode;
		NewNode.NodeTag = Token.token_string;

		NewNode.BlockStart = ParserLexical->get_index();

		bool EndFlag	  = false;
		bool ScopeEndFlag = false;

		std::wstring PropertyName;

		// Parse Node Property
		while (!ParserLexical->is_eof())
		{
			Token = ParserLexical->get_token();

			if (Token.token_string == L"/")
			{
				Token = ParserLexical->get_token();

				if (Token.token_string != L">")
				{
					ThrowError(&ParseResult, L"Unknown token : \"" + Token.token_string + L"\", did you mean \">\"?");

					return ParseResult;
				}

				NewNode.ChildrenSequence = ChildrenSequence;

				++ChildrenSequence;

				ParseResult.Nodes.insert(std::pair<std::wstring, VMLNode>(NewNode.NodeTag, NewNode));

				ScopeEndFlag = true;
				EndFlag		 = true;

				break;
			}
			if (Token.token_string == L">")
			{
				EndFlag = true;

				break;
			}
			if (Token.cache_token != UNKNOW_TOKEN)
			{
				ThrowError(&ParseResult, L"Unknown token : \"" + Token.token_string + L"\".");

				return ParseResult;
			}

			PropertyName = Token.token_string;

			// Skip White Space
			Token = ParserLexical->get_token();
			if (Token.cache_token != EQUAL_SIGN_TOKEN)
			{
				ThrowError(&ParseResult, L"Unknown Token : \"" + Token.token_string + L"\", did you mean \"=\"?");

				return ParseResult;
			}

			Token = ParserLexical->get_token();

			if (Token.cache_token != CONST_STRING)
			{
				ThrowError(&ParseResult, L"Unknown Token : \"" + Token.token_string +
											 L"\" (All of the key value should be included by string).");

				return ParseResult;
			}

			auto Property		  = ToPropertyValue(Token.token_string);
			Property.PropertyName = PropertyName;

			NewNode.NodeValue.insert(std::pair<std::wstring, VMLPropertyValue>(PropertyName, Property));
		}

		if (!EndFlag)
		{
			ThrowError(&ParseResult,
					   L"The end symbol \">\" lost, maybe you need to add \">\" at the end of this scope.");

			return ParseResult;
		}

		if (ScopeEndFlag)
		{
			continue;
		}

		EndFlag = false;

		// Parse the Sub Nodes of This Node
		std::wstring SubContent;

		unsigned long LeftBracketCount = 0;

		auto StartPosition = ParserLexical->get_index();
		auto EndPosition   = 0;

		auto ChildStartLine = ParserLexical->get_line() + BaseLine;
		while (!ParserLexical->is_eof())
		{
			Token = ParserLexical->get_token();

			if (Token.token_string == L"/")
			{
				auto ViewToken = ParserLexical->view_token();
				if (ViewToken.token_string == L">")
				{
					--LeftBracketCount;
				}
			}
			if (Token.token_string == L"<")
			{
				NewNode.BlockEnd = ParserLexical->get_index();

				EndPosition = ParserLexical->get_index() - 1;
				Token		= ParserLexical->get_token();

				bool BreakTokenTrigger = false;

				// Skip Comment
				if (Token.token_string == L"!")
				{
					bool EndFlag = false;

					// Parse Node Property
					while (!ParserLexical->is_eof())
					{
						Token = ParserLexical->get_token();

						if (Token.token_string == L"--")
						{
							if (!BreakTokenTrigger)
							{
								BreakTokenTrigger = true;

								continue;
							}

							Token = ParserLexical->get_token();

							if (Token.token_string != L">")
							{
								ThrowError(&ParseResult, L"Unknown Character \"" + Token.token_string + L"\"");
							}

							EndFlag = true;

							break;
						}
					}

					if (!EndFlag)
					{
						ThrowError(&ParseResult, L"Could not find the match end block of the comment, "
												 L"maybe you need "
												 L"add \"-->\" at the end of the comment.");

						return ParseResult;
					}

					continue;
				}
				if (Token.token_string == L"/")
				{
					if (LeftBracketCount == 0)
					{
						Token = ParserLexical->get_token();
						if (Token.token_string != NewNode.NodeTag)
						{
							ThrowError(&ParseResult, L"Could not find the match end block of \"" + NewNode.NodeTag +
														 L"\", maybe you need to add \"</" + NewNode.NodeTag +
														 L"> at the end of this scope.");

							return ParseResult;
						}

						Token = ParserLexical->get_token();

						if (Token.cache_token != MORE_THAN_TOKEN)
						{
							ThrowError(&ParseResult,
									   L"Unknown Token : \"" + Token.token_string + L"\", did you mean \">\"?");

							return ParseResult;
						}

						EndFlag = true;

						break;
					}
					--LeftBracketCount;
				}
				else
				{
					++LeftBracketCount;
				}
			}
		}

		if (StartPosition != EndPosition)
		{
			SubContent = ParserLexical->get_buffer().substr(StartPosition, EndPosition - StartPosition - 1);
		}

		if (!SubContent.empty())
		{
			VMLParser *SubContentParser = new VMLParser(SubContent, VMLParserParseMode::FromString, ChildStartLine);
			auto	   Result			= SubContentParser->ParseVML();

			if (Result.ParserStatus == VMLParserStatus::Error)
			{
				ParseResult.ParserStatus = VMLParserStatus::Error;

				for (auto &ErrorInfo : Result.ErrorInfo)
				{
					ParseResult.ErrorInfo.push_back(ErrorInfo);
				}

				return ParseResult;
			}
			for (auto &ChildrenNode : Result.Nodes)
			{
				NewNode.ChildrenNodes.insert(std::pair<std::wstring, VMLNode>(ChildrenNode.first, ChildrenNode.second));
			}
		}

		NewNode.ChildrenSequence = ChildrenSequence;

		++ChildrenSequence;

		ParseResult.Nodes.insert(std::pair<std::wstring, VMLNode>(NewNode.NodeTag, NewNode));

		if (!EndFlag)
		{
			ThrowError(&ParseResult, L"The end symbol \"<\" lost, maybe you need to add \"</" + NewNode.NodeTag +
										 L">\" at the end of this scope.");

			break;
		}
	}

	return ParseResult;
}
} // namespace VML

VLIB_END_NAMESPACE