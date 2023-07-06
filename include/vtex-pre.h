#pragma once

#include <map>
#include <windows.h>

#include "core/stl-ex/vstring.h"
#include "vml/vmlparser.h"

namespace VTeX
{
class VTeXPreEnvironment
{
public:
	VTeXPreEnvironment();
	~VTeXPreEnvironment();

public:
	static const bool AlreadyInit();

private:
	static bool Init;
};

class VTeXSymbolMap
{
public:
	VTeXSymbolMap(const VString &MapFile);

private:
	bool ReadFile(const VString &Model, std::map<VString, VString> &Map, VML::VMLParserResult &AST);

private:
	std::map<VString, VString> OperatorMap;
	std::map<VString, VString> RelationMap;
	std::map<VString, VString> ArrowMap;
	std::map<VString, VString> MathbbMap;
	std::map<VString, VString> RestMap;
	std::map<VString, VString> GreekMap;
};

extern VTeXPreEnvironment Environment;
extern VTeXSymbolMap	 *SymbolMap;

} // namespace VTeX