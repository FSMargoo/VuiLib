/*
 * VMLParser.hpp
 *	@description : Provide a VML Parser for VuiLib
 *	@birth		 : 2022/6.4
*/

#pragma once

#include "XMLParser/seal.lexical.hpp"

#include "vxmlparserhelper.hpp"
#include "../../vbase"

#include <corecrt_wstdlib.h>
#include <vector>

VLIB_BEGIN_NAMESPACE

enum class VVMLParserStatus {
	Ok, Error, Failed
};

struct VMLParserError {
	std::wstring ErrorString;
	int          Line;
};

enum class VVMLPropertyType {
	IntValue, StringValue, DoubleValue, BooleanValue
};

struct VVMLPropertyValue {
	std::wstring	 PropertyName = L"";

	VVMLPropertyType PropertyType = VVMLPropertyType::IntValue;

	int				 PropertyAsInt = 0;
	std::wstring     PropertyAsString = L"";
	double           PropertyAsDouble = 0.f;
	bool             PropertyAsBool = false;
};

struct VVMLNode {
	std::wstring							  NodeTag;
	std::map<std::wstring, VVMLPropertyValue> NodeValue;

	std::map<std::wstring, VVMLNode>		  ChildrenNodes;

	VVMLPropertyValue GetProperty(std::wstring Type) {
		VVMLPropertyValue Value = NodeValue.find(Type)->second;

		return Value;
	}
	bool PropertyExsit(std::wstring Type) {
		if (NodeValue.find(Type) != NodeValue.end()) {
			return true;
		}

		return false;
	}
};

struct VVMLParserResult {
	std::vector<VMLParserError>		 ErrorInfo;
	std::map<std::wstring, VVMLNode> Nodes;
	VVMLParserStatus				 ParserStatus = VVMLParserStatus::Ok;
};

enum class VVMLParserParseMode {
	FromFile, FromString
};

class VVMLParser {
private:
	seal_lexical* ParserLexical = nullptr;

private:
	void			  ThrowError(VVMLParserResult* Result, std::wstring ErrorString) {
		(*Result).ParserStatus = VVMLParserStatus::Error;
		(*Result).ErrorInfo.push_back({ ErrorString, BaseLine + ParserLexical->get_line() + 1 });
	}
	VVMLPropertyValue ToPropertyValue(std::wstring String) {
		String = String.substr(1, String.size() - 2);

		if (String == L"true") {
			VVMLPropertyValue Value;
			Value.PropertyAsBool = true;
			Value.PropertyType = VVMLPropertyType::BooleanValue;

			return Value;
		}
		if (String == L"false") {
			VVMLPropertyValue Value;
			Value.PropertyAsBool = false;
			Value.PropertyType = VVMLPropertyType::BooleanValue;

			return Value;
		}

		VVMLPropertyValue Value;
		VVMLPropertyType  Type = VVMLPropertyType::IntValue;

		bool DotExsit = false;

		for (auto& Character : String) {
			if (!(Character >= L'0' && Character <= L'9')) {
				if (Character == L'.') {
					if (DotExsit == false) {
						DotExsit = true;

						Type = VVMLPropertyType::DoubleValue;
					}
					else {
						Type = VVMLPropertyType::StringValue;
					}
				}
				else {
					Type = VVMLPropertyType::StringValue;
				}
			}
		}

		Value.PropertyType = Type;

		switch (Type) {
		case VVMLPropertyType::IntValue: {
			Value.PropertyAsInt = _wtoi(String.c_str());
			break;
		}
		case VVMLPropertyType::StringValue: {
			Value.PropertyAsString = String;
			break;
		}
		case VVMLPropertyType::DoubleValue: {
			Value.PropertyAsDouble = _wtof(String.c_str());
			break;
		}
		}

		return Value;
	}

	int  BaseLine = 0;

private:
	bool FileExist = true;

public:
	VVMLParser(std::wstring VMLString, VVMLParserParseMode VMLParserMode = VVMLParserParseMode::FromString, int Line = 0) {
		switch (VMLParserMode) {
		case VVMLParserParseMode::FromString: {
			ParserLexical = new seal_lexical(VMLString);
			BaseLine = Line;

			break;
		}
		case VVMLParserParseMode::FromFile: {
			if (VVMLParserHelper::FileExist(VMLString) == true) {
				ParserLexical = new seal_lexical(VVMLParserHelper::ReadFromFile(VMLString));
				BaseLine = Line;
			}
			else {
				FileExist = false;
			}

			break;
		}
		default: {
			ParserLexical = new seal_lexical(VMLString);
			BaseLine = Line;

			break;
		}
		}
	}

	VVMLParserResult ParseVML() {
		VVMLParserResult ParseResult;

		if (FileExist == false) {
			ParseResult.ErrorInfo.push_back({ L"Target File Dosen't Exsits!", 0 });
			ParseResult.ParserStatus = VVMLParserStatus::Failed;

			return ParseResult;
		}

		seal_lexical_type_info::_lexical_token_type Token;

		while (!ParserLexical->is_eof()) {
			Token = ParserLexical->get_token();

			if (Token.cache_token == EOF_TOKEN) {
				break;
			}

			// If the Begin of the Code Isn't '<' That Certain Error
			if (Token.cache_token != LESS_THAN_TOKEN) {
				ThrowError(&ParseResult, L"Unknown Token : \"" + Token.token_string + L"\"0");

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
						if (BreakTokenTrigger == false) {
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

				if (EndFlag == false) {
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

			VVMLNode NewNode;
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

				NewNode.NodeValue.insert(std::pair<std::wstring, VVMLPropertyValue>(PropertyName,
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
								if (BreakTokenTrigger == false) {
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

						if (EndFlag == false) {
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

			if (SubContent.empty() == false) {
				VVMLParser* SubContentParser = new VVMLParser(SubContent, VVMLParserParseMode::FromString, ParserLexical->get_line());
				auto Result = SubContentParser->ParseVML();

				if (Result.ParserStatus == VVMLParserStatus::Error) {
					ParseResult.ParserStatus = VVMLParserStatus::Error;

					for (auto& ErrorInfo : Result.ErrorInfo) {
						ParseResult.ErrorInfo.push_back(ErrorInfo);
					}

					return ParseResult;
				}
				for (auto& ChildrenNode : Result.Nodes) {
					NewNode.ChildrenNodes.insert(
						std::pair<std::wstring, VVMLNode>(ChildrenNode.first, ChildrenNode.second));
				}
			}

			ParseResult.Nodes.insert(std::pair<std::wstring, VVMLNode>(NewNode.NodeTag, NewNode));

			if (EndFlag == false) {
				ThrowError(&ParseResult, L"Could Not Found The Match Symbol Of \"<\"");

				break;
			}
		}

		return ParseResult;
	}
};

VLIB_END_NAMESPACE