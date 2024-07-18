/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file sJson.hpp
 * \brief The simple JSON library
 */

#pragma once

#include <map>
#include <string>
#include <typeinfo>
#include <vector>

class sJSONElementNode;

/**
 * sJSON 对象值的类型
 */
enum class sJSONValueType
{
	Value,
	Array,
	Object,
	Null
};
/**
 * sJSON Parser Token 的类型
 */
enum class sJSONTokenType
{
	BigLeft,
	BigRight,
	MiddleLeft,
	MiddleRight,
	String,
	Number,
	Float,
	Boolean,
	Comma,
	Colon,
	Unknown,
	End,
	Null
};

extern std::map<char, sJSONTokenType> sJSONTokenMap;
extern std::map<sJSONTokenType, char> sJSONTokenStringMap;
extern std::map<sJSONTokenType, sJSONTokenType> sJSONOppsiteTokenMap;

/**
 * sJSON 值的基类
 */
class sJSONValue
{
public:
	sJSONValue()  = default;
	~sJSONValue() = default;

public:
	virtual __forceinline const sJSONValueType GetType() const = 0;
	virtual __forceinline const size_t		   GetHash() const
	{
		return 0;
	}

	template <class Object> Object To()
	{
		return static_cast<Object>(this);
	}
};

/**
 * sJSON 中的空
 */
class sJSONNull : public sJSONValue
{
public:
	sJSONNull()	 = default;
	~sJSONNull() = default;

public:
	__forceinline const sJSONValueType GetType() const override
	{
		return sJSONValueType::Null;
	}
};

/**
 * sJSON 中的实值
 */
template <class Type> class sJSONRealValue : public sJSONValue
{
public:
	sJSONRealValue(const Type &SetValue) : Value(SetValue)
	{
	}
	~sJSONRealValue() = default;

	__forceinline sJSONValueType const GetType() const override
	{
		return sJSONValueType::Value;
	}

	__forceinline const size_t GetHash() const override
	{
		return typeid(Type).hash_code();
	}

	static constexpr bool Equal(const sJSONValue *Judgement)
	{
		if (Judgement->GetType() != sJSONValueType::Value)
		{
			return false;
		}
		else
		{
			return Judgement->GetHash() == typeid(Type).hash_code();
		}
	}
	Type operator*()
	{
		return Value;
	}

public:
	Type Value;
};

using sJSONString  = sJSONRealValue<std::string>;
using sJSONInt	   = sJSONRealValue<int>;
using sJSONDouble  = sJSONRealValue<double>;
using sJSONBoolean = sJSONRealValue<bool>;

/**
 * sJSON 的数组
 */
class sJSONArray : public sJSONValue
{
public:
	sJSONArray()  = default;
	~sJSONArray() = default;

public:
	__forceinline sJSONValueType const GetType() const override
	{
		return sJSONValueType::Array;
	}

public:
	std::vector<sJSONValue *> ValueSet;
};

/**
 * sJSON 树节点
 */
typedef class sJSONElementNode : public sJSONValue
{
public:
	sJSONElementNode()
	{
		Value = nullptr;
	}
	sJSONElementNode(const std::string &Tag, sJSONValue *Value)
	{
		this->Tag	= Tag;
		this->Value = Value;
	}
	sJSONElementNode(std::map<std::string, sJSONElementNode *> SetChildren) : Children(SetChildren), Value(nullptr)
	{
	}
	~sJSONElementNode()
	{
		if (Value != nullptr)
			delete Value;
	}

public:
	__forceinline sJSONValueType const GetType() const override
	{
		return sJSONValueType::Object;
	}

	template <class Type> Type To()
	{
		return Value->To<Type>();
	}

public:
	bool IsEmpty()
	{
		return Value->GetType() == sJSONValueType::Null;
	}
	const std::string GetTag() const
	{
		return Tag;
	}
	void InsertChildrenNode(sJSONElementNode *Node)
	{
		Children.emplace(Node->Tag, Node);
	}
	const sJSONValue *GetValue() const
	{
		return Value;
	}
	sJSONValue *operator*()
	{
		return Value;
	}

public:
	sJSONElementNode *operator[](const std::string &ChildrenTag)
	{
		return Children[ChildrenTag];
	}

public:
	sJSONValue								 *Value;
	std::string								  Tag;
	std::map<std::string, sJSONElementNode *> Children;
} sJSONObject;

/**
 * sJSON 中括号元素查询辅助类
 */
class sJSONElementFinder
{
public:
	sJSONElementFinder(sJSONElementNode *Ptr) : Value(Ptr)
	{
	}

	sJSONElementFinder operator[](const std::string &Finder)
	{
		return sJSONElementFinder(Value->operator[](Finder));
	}
	sJSONValue *GetValue()
	{
		return Value->Value;
	}
	bool IsEmpty()
	{
		return Value->Value->GetType() == sJSONValueType::Null;
	}
	bool IsArray()
	{
		return Value->Value->GetType() == sJSONValueType::Array;
	}
	bool IsObject()
	{
		return Value->Value->GetType() == sJSONValueType::Object;
	}
	bool IsRealValue()
	{
		return Value->Value->GetType() == sJSONValueType::Value;
	}
	bool Exsits(const std::string &Finder)
	{
		return Value->Children.find(Finder) != Value->Children.end();
	}

	template <class Type> Type To()
	{
		return Value->To<Type>();
	}

	std::map<std::string, sJSONElementNode *>::iterator begin()
	{
		Iterator = Value->Children.begin();

		return Iterator;
	}
	std::map<std::string, sJSONElementNode *>::iterator end()
	{
		return Value->Children.end();
	}
	auto operator++()
	{
		++Iterator;

		return *Iterator;
	}
	auto operator++(int)
	{
		auto Temp(*Iterator);
		++Iterator;

		return Temp;
	}

	std::vector<sJSONValue *>::iterator ArrayBegin()
	{
		if (Value->Value->GetType() == sJSONValueType::Array)
		{
			return Value->Value->To<sJSONArray *>()->ValueSet.begin();
		}

		return std::vector<sJSONValue *>::iterator();
	}
	std::vector<sJSONValue *>::iterator ArrayEnd()
	{
		if (Value->Value->GetType() == sJSONValueType::Array)
		{
			return Value->Value->To<sJSONArray *>()->ValueSet.end();
		}

		return std::vector<sJSONValue *>::iterator();
	}

private:
	std::map<std::string, sJSONElementNode *>::iterator Iterator;
	sJSONElementNode								   *Value;
};

/**
 * sJSON 树根
 */
class sJSONRootNode
{
public:
	sJSONRootNode(sJSONObject *RootObject) : Object(RootObject)
	{
	}

public:
	sJSONElementFinder operator[](const std::string &ChildrenTag)
	{
		return sJSONElementFinder(Object->operator[](ChildrenTag));
	}
	bool Exsits(const std::string &Finder)
	{
		return Object->Children.find(Finder) != Object->Children.end();
	}

	std::map<std::string, sJSONElementNode *>::iterator begin()
	{
		Iterator = Object->Children.begin();

		return Iterator;
	}
	std::map<std::string, sJSONElementNode *>::iterator end()
	{
		return Object->Children.end();
	}
	auto operator++()
	{
		++Iterator;

		return *Iterator;
	}
	auto operator++(int)
	{
		auto Temp(*Iterator);
		++Iterator;

		return Temp;
	}

private:
	std::map<std::string, sJSONElementNode *>::iterator Iterator;
	sJSONObject										   *Object;
};

/**
 * sJSON 解析器错误类型
 */
class sJSONParserStatus
{
public:
	using ErrorList = std::vector<std::string>;
	using Iterator	= std::vector<std::string>::iterator;

public:
	sJSONParserStatus() = default;

	__forceinline const bool ExsitsError() const
	{
		return !ErrorInfo.empty();
	}
	std::vector<std::string>::iterator begin()
	{
		NativeIterator = ErrorInfo.begin();
		return NativeIterator;
	}
	std::vector<std::string>::iterator end()
	{
		return ErrorInfo.end();
	}
	std::string &operator++()
	{
		++NativeIterator;

		return *NativeIterator;
	}
	std::string operator++(int)
	{
		std::string Temp(*NativeIterator);
		++NativeIterator;

		return Temp;
	}

public:
	ErrorList ErrorInfo;
	Iterator  NativeIterator;
};

/**
 * sJSON 词法分析器
 */
class sJSONLexer
{
public:
	sJSONLexer(const std::string &Code, sJSONParserStatus *Status) : sJSON(Code)
	{
		Line	  = 1;
		Iterator  = sJSON.begin();
		RawString = true;
	}

public:
	inline std::tuple<std::string, sJSONTokenType> operator()()
	{
		if (Iterator == sJSON.end() || *Iterator == '\0')
		{
			return {"", sJSONTokenType::End};
		}
		std::string Token;
		bool		ExsitsDot = false;
		bool		Number	  = false;
		while (Iterator != sJSON.end())
		{
			if (*Iterator == '\n' || *Iterator == '\r')
			{
				++Line;
			}
			else if (*Iterator >= 'a' && *Iterator <= 'z' && *Iterator >= 'A' && *Iterator <= 'Z' && *Iterator == '_' &&
					 !Number)
			{
				Token.push_back(*Iterator);
			}
			else if (((*Iterator >= '0' && *Iterator <= '9') || *Iterator == '.') && (Token.empty() || Number))
			{
				Number = true;
				if (*Iterator == '.')
				{
					if (ExsitsDot)
					{
						ExsitsDot = true;
						Token.push_back(*Iterator);
					}
					else
					{
						ParserStatus->ErrorInfo.push_back("<Bad number>");
						return {"", sJSONTokenType::Unknown};
					}
				}
				else
				{
					Token.push_back(*Iterator);
				}
			}
			else if (!Token.empty())
			{
				if (!Number)
				{
					if (Token == "true" || Token == "false")
					{
						return {Token, sJSONTokenType::Boolean};
					}
					if (Token == "null")
					{
						return {Token, sJSONTokenType::Null};
					}
					else
					{
						return {Token, sJSONTokenType::Unknown};
					}
				}
				else
				{
					if (!ExsitsDot)
					{
						return {Token, sJSONTokenType::Number};
					}
					else
					{
						return {Token, sJSONTokenType::Float};
					}
				}
			}
			else if (*Iterator == '\"')
			{
				if (!RawString)
				{
					Token.push_back(*Iterator);
				}
				++Iterator;
				while (Iterator != sJSON.end())
				{
					if (*Iterator == '\"')
					{
						if (!RawString)
						{
							Token.push_back(*Iterator);
						}
						++Iterator;
						return {Token, sJSONTokenType::String};
					}
					if (*Iterator == '\\')
					{
						if (Iterator + 1 != sJSON.end() && RawString)
						{
							++Iterator;
							if (*Iterator == '\"')
							{
								Token.push_back('\"');
							}
							if (*Iterator == 't')
							{
								Token.push_back('\t');
							}
							if (*Iterator == 'n')
							{
								Token.push_back('\n');
							}
							if (*Iterator == 'r')
							{
								Token.push_back('\r');
							}
						}
						else
						{
							ParserStatus->ErrorInfo.push_back("Not match \" of the begin of \"");
							return {"", sJSONTokenType::Unknown};
						}
					}
					else
					{
						Token.push_back(*Iterator);
					}

					++Iterator;
				}
			}
			else if (*Iterator != ' ' && *Iterator != '\t')
			{
				sJSONTokenType Type = sJSONTokenType::Unknown;
				if (sJSONTokenMap.find(*Iterator) != sJSONTokenMap.end())
				{
					Type = sJSONTokenMap[*Iterator];
				}

				Token.push_back(*Iterator);
				++Iterator;

				return {Token, Type};
			}

			++Iterator;
		}

		return {"", sJSONTokenType::End};
	}
	bool operator*()
	{
		return Iterator != sJSON.end();
	}
	const size_t GetLine() const
	{
		return Line;
	}

public:
	bool RawString;

private:
	sJSONParserStatus	 *ParserStatus;
	std::string::iterator Iterator;
	std::string			  sJSON;
	size_t				  Line;
};

/**
 * sJSON 解析器
 */
class sJSONParser
{
public:
	sJSONParser(std::string Code) : RootObject(new sJSONObject()), Lexer(Code, &Status)
	{
	}

public:
	sJSONRootNode Parse()
	{
		auto Token = Lexer();
		switch (std::get<1>(Token))
		{
		case sJSONTokenType::BigLeft: {
			sJSONParser *SubParser = new sJSONParser(FetchContext(sJSONTokenType::BigLeft, sJSONTokenType::BigRight));
			RootObject->Children   = SubParser->ParseSet();
			if (SubParser->Status.ExsitsError())
			{
				Status = SubParser->Status;
				return sJSONRootNode(RootObject);
			}

			break;
		}
		default: {
			Status.ErrorInfo.push_back("Unknown token at line " + std::to_string(Lexer.GetLine()) + ".");
			return sJSONRootNode(RootObject);
		}
		}

		return sJSONRootNode(RootObject);
	}

private:
	std::map<std::string, sJSONElementNode *> ParseSet()
	{
		std::map<std::string, sJSONElementNode *> Set;
		while (*Lexer)
		{
			auto Token = Lexer();
			switch (std::get<1>(Token))
			{
			case sJSONTokenType::String: {
				std::string Tag = std::get<0>(Token);
				Token			= Lexer();

				if (std::get<1>(Token) != sJSONTokenType::Colon)
				{
					Status.ErrorInfo.push_back("Unknown token at line " + std::to_string(Lexer.GetLine()) + ".");
					return std::map<std::string, sJSONElementNode *>();
				}

				auto Value = ParseValue();
				if (Status.ExsitsError())
				{
					return std::map<std::string, sJSONElementNode *>();
				}

				Set.emplace(Tag, new sJSONElementNode(Tag, Value));
				if (Value->GetType() == sJSONValueType::Object)
				{
					Set[Tag]->Children = static_cast<sJSONObject *>(Value)->Children;
				}

				Token = Lexer();
				if (std::get<1>(Token) != sJSONTokenType::Comma && std::get<1>(Token) != sJSONTokenType::End)
				{
					Status.ErrorInfo.push_back("Unknown token at line " + std::to_string(Lexer.GetLine()) + ".");
					return std::map<std::string, sJSONElementNode *>();
				}

				break;
			}
			default: {
				Status.ErrorInfo.push_back("Unknown token at line " + std::to_string(Lexer.GetLine()) + ".");
				return std::map<std::string, sJSONElementNode *>();
			}
			}
		}

		return Set;
	}
	sJSONArray *ParseArray()
	{
		sJSONArray *Array = new sJSONArray;
		while (*Lexer)
		{
			Array->ValueSet.push_back(ParseValue());
			if (Status.ExsitsError())
			{
				return nullptr;
			}
			auto Token = Lexer();

			if (std::get<1>(Token) != sJSONTokenType::End && std::get<1>(Token) != sJSONTokenType::Comma)
			{
				Status.ErrorInfo.push_back("Unknown token at line " + std::to_string(Lexer.GetLine()) + ".");
				return nullptr;
			}
		}

		return Array;
	}
	sJSONValue *ParseValue()
	{
		while (*Lexer)
		{
			auto Token = Lexer();
			switch (std::get<1>(Token))
			{
			case sJSONTokenType::String: {
				return new sJSONRealValue<std::string>(std::get<0>(Token));
			}
			case sJSONTokenType::Number: {
				return new sJSONRealValue<int>(atoi(std::get<0>(Token).c_str()));
			}
			case sJSONTokenType::Float: {
				return new sJSONRealValue<double>(atof(std::get<0>(Token).c_str()));
			}
			case sJSONTokenType::Null: {
				return new sJSONNull();
			}
			case sJSONTokenType::Boolean: {
				if (std::get<0>(Token) == "true")
				{
					return new sJSONRealValue<bool>(true);
				}
				else
				{
					return new sJSONRealValue<bool>(false);
				}
			}
			case sJSONTokenType::BigLeft: {
				sJSONParser *SubParser =
					new sJSONParser(FetchContext(std::get<1>(Token), sJSONOppsiteTokenMap[std::get<1>(Token)]));
				auto Value = new sJSONObject(SubParser->ParseSet());

				delete SubParser;

				return Value;
			}
			case sJSONTokenType::MiddleLeft: {
				sJSONParser *SubParser =
					new sJSONParser(FetchContext(std::get<1>(Token), sJSONOppsiteTokenMap[std::get<1>(Token)]));
				auto Array = SubParser->ParseArray();

				delete SubParser;

				return Array;
			}
			default: {
				Status.ErrorInfo.push_back("Unknown token at line " + std::to_string(Lexer.GetLine()) + ".");
				return nullptr;
			}
			}
		}

		Status.ErrorInfo.push_back("<Bad sJSON>");
		return nullptr;
	}

private:
	inline std::string FetchContext(const sJSONTokenType &Left, const sJSONTokenType &Right)
	{
		Lexer.RawString = false;
		std::string Context;
		int			DeepthLevel = 0;
		while (*Lexer)
		{
			auto Tuple = Lexer();
			if (std::get<1>(Tuple) == Left)
			{
				++DeepthLevel;
			}
			if (std::get<1>(Tuple) == Right)
			{
				--DeepthLevel;
				if (DeepthLevel == -1)
				{
					Lexer.RawString = true;
					return Context;
				}
			}

			Context.append(std::get<0>(Tuple) + " ");
		}

		Status.ErrorInfo.push_back("<Bad sJSON tree>");

		return Context;
	}

private:
	sJSONLexer		  Lexer;
	sJSONObject		 *RootObject;
	sJSONParserStatus Status;
};

#define sJSONHelperCountDown(Var) for (int Count = 0; Count < Var; ++Count)

/**
 * sJSON 序列化器
 */
class sJSONWriter
{
public:
	static std::string WriteJSON(sJSONRootNode &Root, bool Format = true, bool Brackets = true, size_t Level = 1)
	{
		std::string sJSON;
		if (Brackets)
		{
			if (Format)
				sJSONHelperCountDown(Level)
				{
					sJSON.push_back('\t');
				}
			sJSON = "{";
			if (Format)
			{
				sJSON.push_back('\n');
			}
		}
		for (auto &Node : Root)
		{
			if (Format)
				sJSONHelperCountDown(Level)
				{
					sJSON.push_back('\t');
				}

			sJSON.append("\"" + Node.first + "\"");
			sJSON.append(":");
			sJSON.append(WriteValue(Node.second->Value, Format, Brackets, Level));

			sJSON.push_back(',');
			if (Format)
			{
				sJSON.push_back('\n');
			}
		}

		sJSON.pop_back();
		if (Format)
		{
			sJSON.pop_back();
			sJSON.push_back('\n');
		}
		if (Brackets)
		{
			if (Format)
				sJSONHelperCountDown(Level - 1)
				{
					sJSON.push_back('\t');
				}
			sJSON.push_back('}');
		}

		return sJSON;
	}
	static std::string WriteJSON(sJSONElementFinder &Finder, bool Format = true, bool Brackets = true, size_t Level = 1)
	{
		std::string sJSON;
		if (Brackets)
		{
			if (Format)
				sJSONHelperCountDown(Level)
				{
					sJSON.push_back('\t');
				}
			sJSON = "{";
			if (Format)
			{
				sJSON.push_back('\n');
			}
		}
		for (auto &Node : Finder)
		{
			if (Format)
				sJSONHelperCountDown(Level)
				{
					sJSON.push_back('\t');
				}

			sJSON.append(Node.first);
			sJSON.append(":");
			sJSON.append(WriteValue(Node.second->Value, Format, Brackets, Level));

			sJSON.push_back(',');
			if (Format)
			{
				sJSON.push_back('\n');
			}
		}

		sJSON.pop_back();
		if (Format)
		{
			sJSON.pop_back();
			sJSON.push_back('\n');
		}
		if (Brackets)
		{
			if (Format)
				sJSONHelperCountDown(Level - 1)
				{
					sJSON.push_back('\t');
				}
			sJSON.push_back('}');
		}

		return sJSON;
	}
	static std::string WriteJSONByObject(sJSONObject *Object, bool Format = true, bool Brackets = true,
										 size_t Level = 1)
	{
		std::string sJSON;
		if (Brackets)
		{
			sJSON = "{";
			if (Format)
			{
				sJSON.push_back('\n');
			}
		}
		for (auto &Node : Object->Children)
		{
			if (Format)
				sJSONHelperCountDown(Level)
				{
					sJSON.push_back('\t');
				}

			sJSON.append("\"" + Node.first + "\"");
			sJSON.append(":");
			sJSON.append(WriteValue(Node.second->Value, Format, Brackets, Level + 1));

			sJSON.push_back(',');
			if (Format)
			{
				sJSON.push_back('\n');
			}
		}

		sJSON.pop_back();
		if (Format)
		{
			sJSON.pop_back();
			sJSON.push_back('\n');
		}
		if (Brackets)
		{
			if (Format)
				sJSONHelperCountDown(Level - 1)
				{
					sJSON.push_back('\t');
				}
			sJSON.push_back('}');
		}

		return sJSON;
	}
	static std::string WriteJSONByArray(sJSONArray *Object, bool Format = true, bool Brackets = true, size_t Level = 1)
	{
		std::string sJSON;
		if (Brackets)
		{
			sJSON = "[";
			if (Format)
			{
				sJSON.push_back('\n');
			}
		}
		for (auto &Node : Object->ValueSet)
		{
			if (Format)
				sJSONHelperCountDown(Level)
				{
					sJSON.push_back('\t');
				}

			sJSON.append(WriteValue(Node, Format, Brackets, Level));

			sJSON.push_back(',');
			if (Format)
			{
				sJSON.push_back('\n');
			}
		}

		sJSON.pop_back();
		if (Format)
		{
			sJSON.pop_back();
			sJSON.push_back('\n');
		}
		if (Brackets)
		{
			if (Format)
				sJSONHelperCountDown(Level - 1)
				{
					sJSON.push_back('\t');
				}
			sJSON.push_back(']');
		}

		return sJSON;
	}
	static std::string WriteValue(sJSONValue *Node, bool Format = true, bool Brackets = true, size_t Level = 1)
	{
		if (Node->GetType() == sJSONValueType::Object)
		{
			return WriteJSONByObject(Node->To<sJSONObject *>(), Format, Brackets, Level + 1);
		}
		if (Node->GetType() == sJSONValueType::Array)
		{
			return WriteJSONByArray(Node->To<sJSONArray *>(), Format, Brackets, Level + 1);
		}
		if (Node->GetType() == sJSONValueType::Null)
		{
			return "null";
		}
		else
		{
			if (sJSONRealValue<std::string>::Equal(Node))
			{
				return "\"" + ((sJSONRealValue<std::string> *)Node)->Value + "\"";
			}
			if (sJSONRealValue<int>::Equal(Node))
			{
				return std::to_string(((sJSONRealValue<int> *)Node)->Value);
			}
			if (sJSONRealValue<double>::Equal(Node))
			{
				return std::to_string(((sJSONRealValue<double> *)Node)->Value);
			}
			if (sJSONRealValue<bool>::Equal(Node))
			{
				if (((sJSONRealValue<bool> *)Node)->Value)
				{
					return "true";
				}
				else
				{
					return "false";
				}
			}
		}

		return "?";
	}
};