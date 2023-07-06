#include "../include/vtex-pre.h"

namespace VTeX
{
VTeXPreEnvironment Environment;
VTeXSymbolMap	  *SymbolMap;
VTeXPreEnvironment::VTeXPreEnvironment()
{
	Init = true;

	AddFontResource(TEXT("./font/KaTeX_Main-Regular.ttf"));
	AddFontResource(TEXT("./font/KaTeX_Main-Italic.ttf"));

	SymbolMap = new VTeXSymbolMap(VStr("./mapping/symbol-mapping.xml"));
}
VTeXPreEnvironment::~VTeXPreEnvironment()
{
	RemoveFontResource(TEXT("./font/KaTeX_Main-Regular.ttf"));
	RemoveFontResource(TEXT("./font/KaTeX_Main-Italic.ttf"));
}
const bool VTeXPreEnvironment::AlreadyInit()
{
	return Init;
}
VTeXSymbolMap::VTeXSymbolMap(const VString &MapFile)
{
	VML::VMLParser Parser(MapFile, VML::VMLParserParseMode::FromFile);
	auto		   Result = Parser.ParseVML();

	if (Result.ParserStatus != VML::VMLParserStatus::Ok)
	{
		abort();

		throw new std::logic_error("Failed to parse XML symbol map file");

		return;
	}

	assert(ReadFile(VStr("operator"), OperatorMap, Result));
	assert(ReadFile(VStr("greek"), GreekMap, Result));
	assert(ReadFile(VStr("relation"), RelationMap, Result));
	assert(ReadFile(VStr("arrow"), ArrowMap, Result));
	assert(ReadFile(VStr("mathbb"), MathbbMap, Result));
	assert(ReadFile(VStr("rest"), RestMap, Result));
}
bool VTeXSymbolMap::ReadFile(const VString &Model, std::map<VString, VString> &Map, VML::VMLParserResult &AST)
{
	if (AST.Nodes.find(Model) != AST.Nodes.end())
	{
		auto Node = AST.Nodes.find(Model);
		for (auto Children = Node->second.ChildrenNodes.begin(); Children != Node->second.ChildrenNodes.end();
			 ++Children)
		{
			if (VKits::VIf(Children->second.PropertyExsit(VStr("tk")))
					.Judge(Children->second.GetProperty(VStr("tk")).PropertyType == VML::VMLPropertyType::StringValue))
			{
				Map.insert(std::pair<VString, VString>(Children->first,
													   Children->second.GetProperty(VStr("tk")).PropertyAsString));
			}
			else
			{
				throw new std::logic_error("Mapping file is corrupted!");

				return false;
			}

			if (VKits::VIf(Children->second.PropertyExsit(VStr("alias")))
					.Judge(Children->second.GetProperty(VStr("alias")).PropertyType ==
						   VML::VMLPropertyType::StringValue))
			{
				Map.insert(std::pair<VString, VString>(Children->second.GetProperty(VStr("alias")).PropertyAsString,
													   Children->second.GetProperty(VStr("tk")).PropertyAsString));
			}
		}
	}

	return true;
}

} // namespace VTeX
