#pragma once

#include "lexer/vasm.lexical.h"
#include "vasm-helper.h"
#include <fstream>
#include <map>
#include <vector>

namespace VASM
{
enum class CompilerMode
{
	FromFile,
	FromString
};

struct CompileStatus
{
	bool IsOkay;
};

class Compiler
{
public:
	Compiler(const std::wstring &StringParameter, const CompilerMode &Mode);
	~Compiler();

public:
	CompileStatus Compile(const std::wstring &OutputFile);
	CompileStatus Compile(std::ofstream *Stream);
	CompileStatus WriteParameter(std::map<std::wstring, unsigned long long> &Mapping);
	void		  PushString(const std::wstring &String);
	void		  ReleaseFile();

	template <class DataType> void Write(const DataType &Object)
	{
		FileStream->write((const char *)&Object, sizeof(DataType));
	}

private:
	seal_lexical							  *Lexical;
	CompileStatus							   Status;
	std::ofstream							  *FileStream;
	std::map<std::wstring, unsigned long long> Mapping;
	unsigned long long						   MapRef;
};
} // namespace VASM