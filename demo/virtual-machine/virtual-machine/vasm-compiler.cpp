#include "vasm-compiler.h"

#include <codecvt>
#include <sstream>

#define THROW_EXECPTION(Exp, ErrorString)                                                                              \
	if (!(Exp))                                                                                                        \
	{                                                                                                                  \
		Status.IsOkay = false;                                                                                         \
		FileStream->close();                                                                                           \
		abort();                                                                                                       \
		return Status;                                                                                                 \
	}

namespace VASM
{
Compiler::Compiler(const std::wstring &StringParameter, const CompilerMode &Mode)
{
	MapRef				 = 0u;
	std::wstring RawCode = StringParameter;

	Status.IsOkay = true;

	if (Mode == CompilerMode::FromFile)
	{
		if (_waccess(RawCode.c_str(), 00) == ENOENT)
		{
			Status.IsOkay = false;
		}

		std::ifstream Stream(RawCode);

		std::string TempLine;
		std::string TotalFile;

		while (true)
		{
			std::getline(Stream, TempLine);

			if (!Stream.eof())
			{
				TotalFile += TempLine + "\n";
			}
			else
			{
				TotalFile += TempLine;

				break;
			}
		}

		Stream.close();

		std::wstring_convert<std::codecvt_utf8<wchar_t>> Convertor;

		RawCode = Convertor.from_bytes(TotalFile);
	}

	Lexical = new seal_lexical(RawCode);
}
Compiler::~Compiler()
{
	FileStream->close();

	delete FileStream;
	delete Lexical;
}

void Compiler::PushString(const std::wstring &String)
{
	Write(BGN_STRING);
	for (auto &Character : String)
	{
		Write<wchar_t>(Character);
	}
	Write(END_STRING);
}
void Compiler::ReleaseFile()
{
}

CompileStatus Compiler::WriteParameter(std::map<std::wstring, unsigned long long> &Mapping)
{
	CompileStatus Status;
	Status.IsOkay = true;

	auto Token = Lexical->get_token();

	if (Token.token_string == L"$")
	{
		Write(PAR_VAR);

		Token = Lexical->get_token();
		THROW_EXECPTION(Token.cache_token == UNKNOW_TOKEN, L"");
		THROW_EXECPTION(Mapping.find(Token.token_string) != Mapping.end(), L"");

		Write(Mapping[Token.token_string]);
	}
	else if (Token.cache_token == UNKNOW_TOKEN)
	{
		Write(PAR_REG);

		THROW_EXECPTION(Token.cache_token == UNKNOW_TOKEN, L"");

		Write(GetRegRawByte(Token.token_string));
	}
	else if (Token.cache_token == CONST_NUMBER)
	{
		Write(PAR_NUM);

		std::wistringstream StringStream(Token.token_string);
		hqword				Instance;

		StringStream >> Instance;

		Write(Instance);
	}
	else if (Token.cache_token == CONST_STRING)
	{
		Write(PAR_STRING);

		PushString(Token.token_string);
	}
}

CompileStatus Compiler::Compile(const std::wstring &OutputFile)
{
	return Compile(new std::ofstream(OutputFile));
}
CompileStatus Compiler::Compile(std::ofstream *Stream)
{
	if (!Status.IsOkay)
	{
		return Status;
	}

	FileStream = Stream;

	using VariablePair = std::pair<std::wstring, unsigned long long>;

	auto Token = Lexical->get_token();
	while (!Lexical->is_eof())
	{
		if (Token.cache_token == UNKNOW_TOKEN || Token.token_string == L".")
		{
			if (Token.token_string == L"dat")
			{
				Write(DAT);

				bool Flag = false;

				Token = Lexical->get_token();

				if (Token.token_string == L"stack")
				{
					Write(DAT_STACK);
				}
				if (Token.token_string == L"string")
				{
					Write(DAT_STRING);

					auto ViewToken = Lexical->view_token(3);
					if (ViewToken.cache_token == CONST_STRING)
					{
						Write(DAT_INIT);

						PushString(ViewToken.token_string);

						Flag = true;
					}
				}
				if (Token.token_string == L"qword")
				{
					Write(DAT_QWORD);

					auto ViewToken = Lexical->view_token(3);
					if (ViewToken.cache_token == CONST_NUMBER)
					{
						Write(DAT_INIT);

						std::wistringstream StringStream(ViewToken.token_string);
						qword				Instance;

						StringStream >> Instance;

						Write(Instance);

						Flag = true;
					}
				}
				if (Token.token_string == L"dword")
				{
					Write(DAT_DWORD);

					auto ViewToken = Lexical->view_token(3);
					if (ViewToken.cache_token == CONST_NUMBER)
					{
						Write(DAT_INIT);

						std::wistringstream StringStream(ViewToken.token_string);
						dword				Instance;

						StringStream >> Instance;

						Write(Instance);

						Flag = true;
					}
				}
				if (Token.token_string == L"word")
				{
					Write(DAT_WORD);

					auto ViewToken = Lexical->view_token(3);
					if (ViewToken.cache_token == CONST_NUMBER)
					{
						Write(DAT_INIT);

						std::wistringstream StringStream(ViewToken.token_string);
						word				Instance;

						StringStream >> Instance;

						Write(Instance);

						Flag = true;
					}
				}
				if (Token.token_string == L"hqword")
				{
					Write(DAT_QWORD);

					auto ViewToken = Lexical->view_token(3);
					if (ViewToken.cache_token == CONST_NUMBER)
					{
						Write(DAT_INIT);

						std::wistringstream StringStream(ViewToken.token_string);
						hqword				Instance;

						StringStream >> Instance;

						Write(Instance);

						Flag = true;
					}
				}
				if (Token.token_string == L"hdword")
				{
					Write(DAT_DWORD);

					auto ViewToken = Lexical->view_token(3);
					if (ViewToken.cache_token == CONST_NUMBER)
					{
						Write(DAT_INIT);

						std::wistringstream StringStream(ViewToken.token_string);
						hdword				Instance;

						StringStream >> Instance;

						Write(Instance);

						Flag = true;
					}
				}
				if (Token.token_string == L"hword")
				{
					Write(DAT_WORD);

					auto ViewToken = Lexical->view_token(3);
					if (ViewToken.cache_token == CONST_NUMBER)
					{
						Write(DAT_INIT);

						std::wistringstream StringStream(ViewToken.token_string);
						hword				Instance;

						StringStream >> Instance;

						Write(Instance);

						Flag = true;
					}
				}

				if (!Flag)
				{
					Write(EMPTY);
				}

				Token = Lexical->get_token();
				THROW_EXECPTION(Token.token_string == L"$", L"\'$\' symbol lost");
				Token = Lexical->get_token();
				THROW_EXECPTION(Token.cache_token == UNKNOW_TOKEN, L"");

				Write(MapRef);
				Mapping.insert(VariablePair(Token.token_string, MapRef));

				if (Flag)
				{
					Token = Lexical->get_token();
				}

				++MapRef;
			}
			else if (Token.token_string == L"call")
			{
				Write(CALL);

				auto CallStatmentLine = Lexical->get_line();

				Token = Lexical->get_token();
				THROW_EXECPTION(Token.cache_token == UNKNOW_TOKEN, L"");

				PushString(Token.token_string);

				while (CallStatmentLine == Lexical->get_line() && !Lexical->is_eof())
				{
					WriteParameter(Mapping);
				}

				Write(CALL_END);
			}
			else if (Token.token_string == L"goto")
			{
				Write(GOTO);

				Token = Lexical->get_token();
				THROW_EXECPTION(Token.cache_token == UNKNOW_TOKEN, L"");
				THROW_EXECPTION(Mapping.find(Token.token_string) != Mapping.end(), L"");

				Write(Mapping[Token.token_string]);
			}
			else if (Token.token_string == L"mov")
			{
				Write(MOV);

				WriteParameter(Mapping);
				WriteParameter(Mapping);
			}
			else if (Token.token_string == L"mova")
			{
				Write(MOVA);

				WriteParameter(Mapping);
				WriteParameter(Mapping);
			}
			else if (Token.token_string == L"movs")
			{
				Write(MOVS);

				WriteParameter(Mapping);
				WriteParameter(Mapping);
			}
			else if (Token.token_string == L"movm")
			{
				Write(MOVM);

				WriteParameter(Mapping);
				WriteParameter(Mapping);
			}
			else if (Token.token_string == L"movd")
			{
				Write(MOVD);

				WriteParameter(Mapping);
				WriteParameter(Mapping);
			}
			else if (Token.token_string == L"comp")
			{
				Write(COMP);

				WriteParameter(Mapping);
				WriteParameter(Mapping);
			}
			else if (Token.token_string == L"compb")
			{
				Write(COMPB);

				WriteParameter(Mapping);
				WriteParameter(Mapping);
			}
			else if (Token.token_string == L"compl")
			{
				Write(COMPL);

				WriteParameter(Mapping);
				WriteParameter(Mapping);
			}
			else if (Token.token_string == L"comple")
			{
				Write(COMPLE);

				WriteParameter(Mapping);
				WriteParameter(Mapping);
			}
			else if (Token.token_string == L"compbe")
			{
				Write(COMPBE);

				WriteParameter(Mapping);
				WriteParameter(Mapping);
			}
			else if (Token.token_string == L"compn")
			{
				Write(COMPN);

				WriteParameter(Mapping);
				WriteParameter(Mapping);
			}
			else if (Token.token_string == L"read")
			{
				Write(READ);

				Token = Lexical->get_token();

				if (Token.token_string == L"mem")
				{
					Write(MEM);
				}
				else
				{
					Write(DISK);
				}

				WriteParameter(Mapping);
			}
			else if (Token.token_string == L"write")
			{
				Write(WRITE);

				Token = Lexical->get_token();

				if (Token.token_string == L"mem")
				{
					Write(MEM);
				}
				else
				{
					Write(DISK);
				}

				WriteParameter(Mapping);
			}
			else if (Token.token_string == L"push")
			{
				Write(PUSH);

				WriteParameter(Mapping);
				WriteParameter(Mapping);
			}
			else if (Token.token_string == L"pop")
			{
				Write(POP);

				WriteParameter(Mapping);
				WriteParameter(Mapping);
			}
			else if (Token.token_string == L".")
			{
				std::wstring LabelContent;

				Token = Lexical->get_token();
				THROW_EXECPTION(Token.cache_token == UNKNOW_TOKEN, L"");

				Mapping.insert(VariablePair(Token.token_string, MapRef));

				Write(LABEL);
				Write(MapRef);

				Token = Lexical->get_token();
				THROW_EXECPTION(Token.token_string == L":", L"");

				++MapRef;

				auto EndLevel  = 0;
				auto CacheLine = Lexical->get_line();
				while (!Lexical->is_eof())
				{
					Token = Lexical->get_token();

					if (Token.token_string == L".")
					{
						++EndLevel;
					}
					if (Token.token_string != L"end")
					{
						LabelContent += Token.token_string + L" ";
					}
					else
					{
						if (EndLevel == 0)
						{
							break;
						}
						else
						{
							--EndLevel;
						}
					}

					if (CacheLine != Lexical->get_line())
					{
						CacheLine = Lexical->get_line();
						LabelContent += L"\n";
					}
				}

				Compiler *SubCompiler = new Compiler(LabelContent, VASM::CompilerMode::FromString);

				SubCompiler->Mapping = Mapping;
				SubCompiler->MapRef	 = MapRef;

				Status = SubCompiler->Compile(FileStream);

				Mapping = SubCompiler->Mapping;
				MapRef	= SubCompiler->MapRef;

				if (!Status.IsOkay)
				{
					return Status;
				}

				Write(END);
			}
			else if (Token.cache_token == UNKNOW_TOKEN)
			{
				Write(FNC);

				PushString(Token.token_string);

				std::wstring LabelContent;

				Token = Lexical->get_token();
				THROW_EXECPTION(Token.token_string == L":", L"");

				auto EndLevel  = 0;
				auto CacheLine = Lexical->get_line();
				while (!Lexical->is_eof())
				{
					Token = Lexical->get_token();

					if (Token.token_string == L":")
					{
						++EndLevel;
					}
					if (Token.token_string != L"end")
					{
						LabelContent += Token.token_string + L" ";
					}
					else
					{
						if (EndLevel == 0)
						{
							break;
						}
						else
						{
							--EndLevel;
							LabelContent += Token.token_string + L" ";
						}
					}

					if (CacheLine != Lexical->get_line())
					{
						CacheLine = Lexical->get_line();
						LabelContent += L"\n";
					}
				}

				Compiler *SubCompiler = new Compiler(LabelContent, VASM::CompilerMode::FromString);

				SubCompiler->Mapping = Mapping;
				SubCompiler->MapRef	 = MapRef;

				Status = SubCompiler->Compile(FileStream);

				Mapping = SubCompiler->Mapping;
				MapRef	= SubCompiler->MapRef;

				if (!Status.IsOkay)
				{
					return Status;
				}

				Write(END);
			}
		}

		Token = Lexical->get_token();
	}

	return Status;
}
} // namespace VASM