#pragma once

#include "../core/stl-ex/vstring.h"
#include "../core/vbasiccore.h"
#include "../kits/seal.lexical.h"
#include "../kits/vparserhelper.h"
#include <map>
#include <vector>

VLIB_BEGIN_NAMESPACE

namespace VML
{
enum class VMLParserStatus
{
	Ok,
	Error,
	Failed
};

struct VMLParserError
{
	VString ErrorString;
	int		Line;
};

enum class VMLPropertyType
{
	IntValue,
	StringValue,
	DoubleValue,
	BooleanValue,
	NativeCall,
	ObjectCallParameter,
	VariableDefine,
	VariableCall
};

enum class VMLVariableType
{
	UndefineType,
	StringType,
	IntType,
	DoubleType,
	BooleanType
};

struct VMLPropertyValue
{
	VString PropertyName = L"";

	VMLPropertyType PropertyType = VMLPropertyType::IntValue;
	VMLVariableType VariableType = VMLVariableType::UndefineType;

	int		PropertyAsInt	 = 0;
	VString PropertyAsString = L"";
	double	PropertyAsDouble = 0.f;
	bool	PropertyAsBool	 = false;

	std::vector<VString> PropertyAsObjectCallParameter;

	VString						  NativeCallMethodName = L"";
	std::vector<VMLPropertyValue> NativeCallParameter;
	std::vector<VMLPropertyValue> VariableInitValue;
};

struct VMLNode
{
	VString								NodeTag;
	std::map<VString, VMLPropertyValue> NodeValue;

	std::map<VString, VMLNode> ChildrenNodes;
	int						   ChildrenSequence = 0;

	int BlockStart;
	int BlockEnd;

	VMLPropertyValue GetProperty(const VString &Type);
	bool			 PropertyExsit(const VString &Type);
};

struct VMLParserResult
{
	VString						FilePath;
	std::vector<VMLParserError> ErrorInfo;
	std::map<VString, VMLNode>	Nodes;
	VMLParserStatus				ParserStatus = VMLParserStatus::Ok;
};

enum class VMLParserParseMode
{
	FromFile,
	FromString
};

class VMLParser
{
private:
	VKits::seal_lexical *ParserLexical = nullptr;
	VString				 FilePath;

private:
	bool			 CheckParameter(const std::vector<VMLPropertyValue>	  &CheckedObject,
									std::initializer_list<VMLPropertyType> CheckedList);
	void			 ThrowError(VMLParserResult *Result, const VString &ErrorString);
	VMLPropertyValue ToPropertyValue(const VString &String);
	int				 BaseLine = 0;

private:
	bool FileExist = true;

public:
	VMLParser(const VString &VMLString, VMLParserParseMode VMLParserMode = VMLParserParseMode::FromString,
			  const int &Line = 0);
	~VMLParser();

	VMLParserResult ParseVML();
};
} // namespace VML

VLIB_END_NAMESPACE