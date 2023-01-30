#include "vasm-vm.h"

namespace VASM
{
VASMGPU::VASMGPU()
{
}
void VASMGPU::PushCharacter(const wchar_t &Character)
{
	GP_OutputCache.push_back(Character);
}
void VASMGPU::PushCharacter(const std::wstring &String)
{
	GP_OutputCache += String;
}
void VASMGPU::Flush()
{
	system("cls");
	printf("%ws", GP_OutputCache.c_str());
}
void VASMGPU::Clear()
{
	GP_OutputCache.clear();
}
std::wstring VASMInterpreter::ReadString(std::wstringstream &FileStream)
{
	auto		 OP_Code = Read<byte>(FileStream);
	std::wstring Result;

	if (OP_Code == BGN_STRING)
	{
		byte Character[3];
		byte Count = 0;

		Character[2] = L'\0';

		while (true)
		{
			auto Byte = Read<byte>(FileStream);

			if (Byte != END_STRING)
			{
				Character[Count] = Byte;
				++Count;

				if (Count == 2)
				{
					Count = 0;

					Result.push_back(*(wchar_t *)Character);
				}
			}
			else
			{
				break;
			}
		}
	}

	return Result;
}
VASMInterpreter::~VASMInterpreter()
{
	delete Memory;
	DiskStream.close();
}
VASMInterpreter::VASMInterpreter(const std::wstring &VirtualDisk, const VirtualMachineConfig &Config, VASMGPU *GPU)
{
	Memory		  = new byte[Config.MemorySize];
	Disk		  = new byte[Config.DiskSize];
	MachineConfig = Config;
	this->GPU	  = GPU;
}
void VASMInterpreter::RunVASMFile(const std::wstring &File)
{
	std::wifstream FileStream(File);

	std::wstring TempLine;
	std::wstring TotalFile;

	while (true)
	{
		std::getline(FileStream, TempLine);

		if (!FileStream.eof())
		{
			TotalFile += TempLine + L"\n";
		}
		else
		{
			TotalFile += TempLine;

			break;
		}
	}

	FileStream.close();

	auto Stream = std::wstringstream(TotalFile);

	Run(Stream);
}
void VASMInterpreter::Run(std::wstringstream &Stream)
{
	using ObjectPair = std::pair<long long, VASMBaseObject *>;
	while (!Stream.eof())
	{
		auto OP_Code = Read<byte>(Stream);

		switch (OP_Code)
		{
		case FNC: {
			VASMFunction Function;

			auto DeepthLevel = 0u;
			auto FncName	 = ReadString(Stream);

			while (!Stream.eof())
			{
				auto FncByte = Read<byte>(Stream);

				if (FncByte == LABEL)
				{
					++DeepthLevel;
				}
				if (FncByte != END)
				{
					Function.FncByte.push_back(FncByte);
				}
				else
				{
					if (DeepthLevel == 0u)
					{
						break;
					}
					else
					{
						Function.FncByte.push_back(FncByte);

						--DeepthLevel;
					}
				}
			}

			printf("\n");

			FunctionList.insert(std::pair<std::wstring, VASMFunction>(FncName, Function));

			break;
		}
		case LABEL: {
			VASMFunction Function;

			auto DeepthLevel = 0u;
			auto FncName	 = ReadString(Stream);

			while (!Stream.eof())
			{
				auto FncByte = Read<byte>(Stream);

				if (FncByte == LABEL)
				{
					++DeepthLevel;
				}
				if (FncByte != END)
				{
					Function.FncByte.push_back(FncByte);
				}
				else
				{
					if (DeepthLevel == 0u)
					{
						break;
					}
					else
					{
						Function.FncByte.push_back(FncByte);

						--DeepthLevel;
					}
				}
			}

			LabelList.insert(std::pair<std::wstring, VASMLabel>(FncName, Function));

			break;
		}
		case DAT: {
			OP_Code = Read<byte>(Stream);

			switch (OP_Code)
			{
			case DAT_STACK: {
				auto Stack = new VASMStackObject;

				Read<byte>(Stream);

				auto ObjId = Read<long long>(Stream);

				ObjectList.insert(ObjectPair(ObjId, Stack));

				break;
			}
			case DAT_STRING: {
				auto String = new VASMStringObject;

				OP_Code		= Read<byte>(Stream);
				qword ObjId = 0;

				if (OP_Code == DAT_INIT)
				{
					String->Data = ReadString(Stream);
					ObjId		 = Read<long long>(Stream);
				}
				else
				{
					ObjId = Read<long long>(Stream);
				}

				ObjectList.insert(ObjectPair(ObjId, String));

				break;
			}
			case DAT_QWORD: {
				auto Num = new VASMQWORDObject;

				OP_Code			= Read<byte>(Stream);
				long long ObjId = 0;

				if (OP_Code == DAT_INIT)
				{
					Num->Data = Read<qword>(Stream);
					ObjId	  = Read<long long>(Stream);
				}
				else
				{
					ObjId = Read<long long>(Stream);
				}

				ObjectList.insert(ObjectPair(ObjId, Num));

				break;
			}
			case DAT_DWORD: {
				auto Num = new VASMDWORDObject;

				OP_Code			= Read<byte>(Stream);
				long long ObjId = 0;

				if (OP_Code == DAT_INIT)
				{
					Num->Data = Read<dword>(Stream);
					ObjId	  = Read<long long>(Stream);
				}
				else
				{
					ObjId = Read<long long>(Stream);
				}

				ObjectList.insert(ObjectPair(ObjId, Num));

				break;
			}
			case DAT_WORD: {
				auto Num = new VASMQWORDObject;

				OP_Code			= Read<byte>(Stream);
				long long ObjId = 0;

				if (OP_Code == DAT_INIT)
				{
					Num->Data = Read<word>(Stream);
					ObjId	  = Read<long long>(Stream);
				}
				else
				{
					ObjId = Read<long long>(Stream);
				}

				ObjectList.insert(ObjectPair(ObjId, Num));

				break;
			}
			case DAT_HQWORD: {
				auto Num = new VASMHQWORDObject;

				OP_Code			= Read<byte>(Stream);
				long long ObjId = 0;

				if (OP_Code == DAT_INIT)
				{
					Num->Data = Read<hqword>(Stream);
					ObjId	  = Read<long long>(Stream);
				}
				else
				{
					ObjId = Read<long long>(Stream);
				}

				ObjectList.insert(ObjectPair(ObjId, Num));

				break;
			}
			case DAT_HDWORD: {
				auto Num = new VASMHDWORDObject;

				OP_Code			= Read<byte>(Stream);
				long long ObjId = 0;

				if (OP_Code == DAT_INIT)
				{
					Num->Data = Read<hdword>(Stream);
					ObjId	  = Read<long long>(Stream);
				}
				else
				{
					ObjId = Read<long long>(Stream);
				}

				ObjectList.insert(ObjectPair(ObjId, Num));

				break;
			}
			case DAT_HWORD: {
				auto Num = new VASMHWORDObject;

				OP_Code			= Read<byte>(Stream);
				long long ObjId = 0;

				if (OP_Code == DAT_INIT)
				{
					Num->Data = Read<hword>(Stream);
					ObjId	  = Read<long long>(Stream);
				}
				else
				{
					ObjId = Read<long long>(Stream);
				}

				ObjectList.insert(ObjectPair(ObjId, Num));

				break;
			}
			}

			break;
		}
		case MOV: {
			OP_Code = Read<byte>(Stream);

			if (OP_Code == PAR_REG)
			{
				OP_Code = Read<byte>(Stream);

				auto RegTarget = &Reg[OP_Code];

				OP_Code = Read<byte>(Stream);
				if (OP_Code == PAR_REG)
				{
					OP_Code	   = Read<byte>(Stream);
					*RegTarget = Reg[OP_Code];
				}
				if (OP_Code == PAR_VAR)
				{
					auto VarPosition = Read<long long>(Stream);
					auto Object		 = ObjectList[VarPosition];

					if (Object->GetType() == DAT_QWORD)
					{
						*RegTarget = ((VASMQWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_DWORD)
					{
						*RegTarget = ((VASMDWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_WORD)
					{
						*RegTarget = ((VASMWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_HQWORD)
					{
						*RegTarget = ((VASMHQWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_HDWORD)
					{
						*RegTarget = ((VASMHDWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_HWORD)
					{
						*RegTarget = ((VASMHWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_STACK)
					{
						*RegTarget = ((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
					}
				}
				if (OP_Code == PAR_NUM)
				{
					*RegTarget = Read<hqword>(Stream);
				}
			}
			if (OP_Code == PAR_VAR)
			{
				auto VarPosition = Read<long long>(Stream);
				auto Object		 = ObjectList[VarPosition];

				if (Object->GetType() == DAT_QWORD)
				{
					auto Left = ((VASMQWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code	   = Read<byte>(Stream);
						Left->Data = Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data = ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data = ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data = ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data = ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data = ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data = ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data =
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data = Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_DWORD)
				{
					auto Left = ((VASMDWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code	   = Read<byte>(Stream);
						Left->Data = Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data = ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data = ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data = ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data = ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data = ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data = ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data =
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data = Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_STRING)
				{
					auto Left = ((VASMStringObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_STRING)
					{
						Left->Data = ReadString(Stream);
					}
				}
				if (Object->GetType() == DAT_WORD)
				{
					auto Left = ((VASMWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code	   = Read<byte>(Stream);
						Left->Data = Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data = ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data = ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data = ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data = ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data = ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data = ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data =
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data = Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_HQWORD)
				{
					auto Left = ((VASMHQWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code	   = Read<byte>(Stream);
						Left->Data = Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data = ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data = ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data = ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data = ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data = ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data = ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data =
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data = Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_HDWORD)
				{
					auto Left = ((VASMHDWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code	   = Read<byte>(Stream);
						Left->Data = Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data = ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data = ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data = ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data = ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data = ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data = ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data =
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data = Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_HWORD)
				{
					auto Left = ((VASMHWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code	   = Read<byte>(Stream);
						Left->Data = Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data = ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data = ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data = ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data = ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data = ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data = ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data =
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data = Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_STACK)
				{
					auto Left = ((VASMStackObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code						  = Read<byte>(Stream);
						Left->Data[Left->Data.size()] = Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						auto Object = ObjectList[OP_Code];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data[Left->Data.size()] = ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data[Left->Data.size()] = ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data[Left->Data.size()] = ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data[Left->Data.size()] = ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data[Left->Data.size()] = ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data[Left->Data.size()] = ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data[Left->Data.size()] =
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data[Left->Data.size() - 1] = Read<hqword>(Stream);
					}
				}
			}

			break;
		}
		case MOVA: {
			OP_Code = Read<byte>(Stream);

			if (OP_Code == PAR_REG)
			{
				OP_Code = Read<byte>(Stream);

				auto RegTarget = &Reg[OP_Code];

				OP_Code = Read<byte>(Stream);
				if (OP_Code == PAR_REG)
				{
					OP_Code = Read<byte>(Stream);
					*RegTarget += Reg[OP_Code];
				}
				if (OP_Code == PAR_VAR)
				{
					auto VarPosition = Read<long long>(Stream);
					auto Object		 = ObjectList[VarPosition];

					if (Object->GetType() == DAT_QWORD)
					{
						*RegTarget += ((VASMQWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_DWORD)
					{
						*RegTarget += ((VASMDWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_WORD)
					{
						*RegTarget += ((VASMWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_HQWORD)
					{
						*RegTarget += ((VASMHQWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_HDWORD)
					{
						*RegTarget += ((VASMHDWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_HWORD)
					{
						*RegTarget += ((VASMHWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_STACK)
					{
						*RegTarget += ((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
					}
				}
				if (OP_Code == PAR_NUM)
				{
					*RegTarget += Read<hqword>(Stream);
				}
			}
			if (OP_Code == PAR_VAR)
			{
				auto VarPosition = Read<long long>(Stream);
				auto Object		 = ObjectList[VarPosition];

				if (Object->GetType() == DAT_QWORD)
				{
					auto Left = ((VASMQWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data += Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data += ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data += ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data += ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data += ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data += ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data += ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data +=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data += Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_DWORD)
				{
					auto Left = ((VASMDWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data += Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data += ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data += ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data += ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data += ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data += ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data += ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data +=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data += Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_STRING)
				{
					auto Left = ((VASMStringObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_STRING)
					{
						Left->Data += ReadString(Stream);
					}
				}
				if (Object->GetType() == DAT_WORD)
				{
					auto Left = ((VASMWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data += Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data += ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data += ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data += ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data += ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data += ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data += ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data +=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data += Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_HQWORD)
				{
					auto Left = ((VASMHQWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data += Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data += ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data += ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data += ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data += ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data += ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data += ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data +=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data += Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_HDWORD)
				{
					auto Left = ((VASMHDWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data += Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data += ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data += ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data += ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data += ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data += ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data += ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data +=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data += Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_HWORD)
				{
					auto Left = ((VASMHWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data += Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data += ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data += ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data += ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data += ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data += ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data += ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data +=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data += Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_STACK)
				{
					auto Left = ((VASMStackObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data[Left->Data.size()] += Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						auto Object = ObjectList[OP_Code];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data[Left->Data.size()] += ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data[Left->Data.size()] += ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data[Left->Data.size()] += ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data[Left->Data.size()] += ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data[Left->Data.size()] += ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data[Left->Data.size()] += ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data[Left->Data.size()] +=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data[Left->Data.size() - 1] += Read<hqword>(Stream);
					}
				}
			}

			break;
		}
		case MOVM: {
			OP_Code = Read<byte>(Stream);

			if (OP_Code == PAR_REG)
			{
				OP_Code = Read<byte>(Stream);

				auto RegTarget = &Reg[OP_Code];

				OP_Code = Read<byte>(Stream);
				if (OP_Code == PAR_REG)
				{
					OP_Code = Read<byte>(Stream);
					*RegTarget *= Reg[OP_Code];
				}
				if (OP_Code == PAR_VAR)
				{
					auto VarPosition = Read<long long>(Stream);
					auto Object		 = ObjectList[VarPosition];

					if (Object->GetType() == DAT_QWORD)
					{
						*RegTarget *= ((VASMQWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_DWORD)
					{
						*RegTarget *= ((VASMDWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_WORD)
					{
						*RegTarget *= ((VASMWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_HQWORD)
					{
						*RegTarget *= ((VASMHQWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_HDWORD)
					{
						*RegTarget *= ((VASMHDWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_HWORD)
					{
						*RegTarget *= ((VASMHWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_STACK)
					{
						*RegTarget *= ((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
					}
				}
				if (OP_Code == PAR_NUM)
				{
					*RegTarget *= Read<hqword>(Stream);
				}
			}
			if (OP_Code == PAR_VAR)
			{
				auto VarPosition = Read<long long>(Stream);
				auto Object		 = ObjectList[VarPosition];

				if (Object->GetType() == DAT_QWORD)
				{
					auto Left = ((VASMQWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data *= Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data *= ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data *= ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data *= ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data *= ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data *= ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data *= ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data *=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data *= Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_DWORD)
				{
					auto Left = ((VASMDWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data *= Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data *= ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data *= ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data *= ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data *= ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data *= ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data *= ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data *=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data *= Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_WORD)
				{
					auto Left = ((VASMWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data *= Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data *= ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data *= ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data *= ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data *= ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data *= ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data *= ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data *=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data *= Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_HQWORD)
				{
					auto Left = ((VASMHQWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data *= Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data *= ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data *= ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data *= ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data *= ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data *= ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data *= ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data *=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data *= Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_HDWORD)
				{
					auto Left = ((VASMHDWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data *= Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data *= ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data *= ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data *= ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data *= ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data *= ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data *= ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data *=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data *= Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_HWORD)
				{
					auto Left = ((VASMHWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data *= Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data *= ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data *= ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data *= ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data *= ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data *= ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data *= ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data *=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data *= Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_STACK)
				{
					auto Left = ((VASMStackObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data[Left->Data.size()] *= Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						auto Object = ObjectList[OP_Code];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data[Left->Data.size()] *= ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data[Left->Data.size()] *= ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data[Left->Data.size()] *= ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data[Left->Data.size()] *= ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data[Left->Data.size()] *= ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data[Left->Data.size()] *= ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data[Left->Data.size()] *=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data[Left->Data.size() - 1] *= Read<hqword>(Stream);
					}
				}
			}

			break;
		}
		case MOVD: {
			OP_Code = Read<byte>(Stream);

			if (OP_Code == PAR_REG)
			{
				OP_Code = Read<byte>(Stream);

				auto RegTarget = &Reg[OP_Code];

				OP_Code = Read<byte>(Stream);
				if (OP_Code == PAR_REG)
				{
					OP_Code = Read<byte>(Stream);
					*RegTarget /= Reg[OP_Code];
				}
				if (OP_Code == PAR_VAR)
				{
					auto VarPosition = Read<long long>(Stream);
					auto Object		 = ObjectList[VarPosition];

					if (Object->GetType() == DAT_QWORD)
					{
						*RegTarget /= ((VASMQWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_DWORD)
					{
						*RegTarget /= ((VASMDWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_WORD)
					{
						*RegTarget /= ((VASMWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_HQWORD)
					{
						*RegTarget /= ((VASMHQWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_HDWORD)
					{
						*RegTarget /= ((VASMHDWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_HWORD)
					{
						*RegTarget /= ((VASMHWORDObject *)Object)->Data;
					}
					if (Object->GetType() == DAT_STACK)
					{
						*RegTarget /= ((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
					}
				}
				if (OP_Code == PAR_NUM)
				{
					*RegTarget /= Read<hqword>(Stream);
				}
			}
			if (OP_Code == PAR_VAR)
			{
				auto VarPosition = Read<long long>(Stream);
				auto Object		 = ObjectList[VarPosition];

				if (Object->GetType() == DAT_QWORD)
				{
					auto Left = ((VASMQWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data /= Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data /= ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data /= ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data /= ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data /= ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data /= ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data /= ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data /=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data /= Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_DWORD)
				{
					auto Left = ((VASMDWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data /= Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data /= ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data /= ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data /= ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data /= ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data /= ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data /= ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data /=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data /= Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_WORD)
				{
					auto Left = ((VASMWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data /= Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data /= ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data /= ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data /= ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data /= ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data /= ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data /= ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data /=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data /= Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_HQWORD)
				{
					auto Left = ((VASMHQWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data /= Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data /= ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data /= ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data /= ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data /= ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data /= ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data /= ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data /=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data /= Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_HDWORD)
				{
					auto Left = ((VASMHDWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data /= Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data /= ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data /= ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data /= ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data /= ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data /= ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data /= ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data /=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data /= Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_HWORD)
				{
					auto Left = ((VASMHWORDObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data /= Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						VarPosition = Read<long long>(Stream);
						auto Object = ObjectList[VarPosition];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data /= ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data /= ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data /= ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data /= ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data /= ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data /= ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data /=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data /= Read<hqword>(Stream);
					}
				}
				if (Object->GetType() == DAT_STACK)
				{
					auto Left = ((VASMStackObject *)Object);

					OP_Code = Read<byte>(Stream);
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						Left->Data[Left->Data.size()] /= Reg[OP_Code];
					}
					if (OP_Code == PAR_VAR)
					{
						auto Object = ObjectList[OP_Code];

						if (Object->GetType() == DAT_QWORD)
						{
							Left->Data[Left->Data.size()] /= ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							Left->Data[Left->Data.size()] /= ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							Left->Data[Left->Data.size()] /= ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							Left->Data[Left->Data.size()] /= ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							Left->Data[Left->Data.size()] /= ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							Left->Data[Left->Data.size()] /= ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							Left->Data[Left->Data.size()] /=
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_NUM)
					{
						Left->Data[Left->Data.size() - 1] /= Read<hqword>(Stream);
					}
				}
			}

			break;
		}
		case CALL: {
			auto CalleFunction = ReadString(Stream);

			if (CalleFunction == L"GTds_size")
			{
				while (true)
				{
					OP_Code = Read<byte>(Stream);

					if (OP_Code == CALL_END)
					{
						break;
					}
					if (OP_Code == PAR_VAR)
					{
						auto ObjectPosititon = Read<long long>(Stream);
						auto Object			 = ObjectList[ObjectPosititon];
						if (Object->GetType() == DAT_QWORD)
						{
							((VASMQWORDObject *)Object)->Data = MachineConfig.DiskSize;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							((VASMDWORDObject *)Object)->Data = MachineConfig.DiskSize;
						}
						if (Object->GetType() == DAT_WORD)
						{
							((VASMWORDObject *)Object)->Data = MachineConfig.DiskSize;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							((VASMHQWORDObject *)Object)->Data = MachineConfig.DiskSize;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							((VASMHDWORDObject *)Object)->Data = MachineConfig.DiskSize;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							((VASMHWORDObject *)Object)->Data = MachineConfig.DiskSize;
						}
						if (Object->GetType() == DAT_STACK)
						{
							((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1] =
								MachineConfig.DiskSize;
						}
						if (Object->GetType() == DAT_STRING)
						{
							((VASMStringObject *)Object)->Data = std::to_wstring(MachineConfig.DiskSize);
						}
					}
				}
			}
			else if (CalleFunction == L"GTme_size")
			{
				while (true)
				{
					OP_Code = Read<byte>(Stream);

					if (OP_Code == CALL_END)
					{
						break;
					}
					if (OP_Code == PAR_VAR)
					{
						auto ObjectPosititon = Read<long long>(Stream);
						auto Object			 = ObjectList[ObjectPosititon];
						if (Object->GetType() == DAT_QWORD)
						{
							((VASMQWORDObject *)Object)->Data = MachineConfig.MemorySize;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							((VASMDWORDObject *)Object)->Data = MachineConfig.MemorySize;
						}
						if (Object->GetType() == DAT_WORD)
						{
							((VASMWORDObject *)Object)->Data = MachineConfig.MemorySize;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							((VASMHQWORDObject *)Object)->Data = MachineConfig.MemorySize;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							((VASMHDWORDObject *)Object)->Data = MachineConfig.MemorySize;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							((VASMHWORDObject *)Object)->Data = MachineConfig.MemorySize;
						}
						if (Object->GetType() == DAT_STACK)
						{
							((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1] =
								MachineConfig.MemorySize;
						}
						if (Object->GetType() == DAT_STRING)
						{
							((VASMStringObject *)Object)->Data = std::to_wstring(MachineConfig.MemorySize);
						}
					}
				}
			}
			else if (CalleFunction == L"GPCS_push")
			{
				int Count = 0;
				while (true)
				{
					OP_Code = Read<byte>(Stream);

					if (OP_Code == CALL_END)
					{
						if (Count == 0)
						{
							GPU->PushCharacter(L"\n");
						}
						break;
					}
					if (OP_Code == PAR_VAR)
					{
						auto ObjectPosititon = Read<long long>(Stream);
						auto Object			 = ObjectList[ObjectPosititon];
						if (Object->GetType() == DAT_QWORD)
						{
							GPU->PushCharacter(std::to_wstring(((VASMQWORDObject *)Object)->Data));
						}
						if (Object->GetType() == DAT_DWORD)
						{
							GPU->PushCharacter(std::to_wstring(((VASMDWORDObject *)Object)->Data));
						}
						if (Object->GetType() == DAT_WORD)
						{
							GPU->PushCharacter(std::to_wstring(((VASMWORDObject *)Object)->Data));
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							GPU->PushCharacter(std::to_wstring(((VASMHQWORDObject *)Object)->Data));
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							GPU->PushCharacter(std::to_wstring(((VASMHDWORDObject *)Object)->Data));
						}
						if (Object->GetType() == DAT_HWORD)
						{
							GPU->PushCharacter(std::to_wstring(((VASMWORDObject *)Object)->Data));
						}
						if (Object->GetType() == DAT_STACK)
						{
							GPU->PushCharacter(std::to_wstring(
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1]));
						}
						if (Object->GetType() == DAT_STRING)
						{
							GPU->PushCharacter(((VASMStringObject *)Object)->Data);
						}
					}
					if (OP_Code == PAR_REG)
					{
						OP_Code = Read<byte>(Stream);
						GPU->PushCharacter(Reg[OP_Code]);
					}
					if (OP_Code == PAR_STRING)
					{
						auto String = ReadString(Stream);
						GPU->PushCharacter(String);
					}

					++Count;
				}
			}
			else if (CalleFunction == L"GPCT_flush")
			{
				while (true)
				{
					OP_Code = Read<byte>(Stream);

					if (OP_Code == CALL_END)
					{
						break;
					}
				}

				GPU->Flush();
			}
			else
			{
				std::wstring ByteString;
				for (auto &Byte : FunctionList[CalleFunction].FncByte)
				{
					ByteString.push_back(Byte);
				}

				std::wstringstream StringStream(ByteString);

				wchar_t RegNum = L'a';

				while (true)
				{
					OP_Code = Read<byte>(Stream);

					if (OP_Code == CALL_END)
					{
						break;
					}

					if (OP_Code == PAR_VAR)
					{
						auto ObjectPosititon = Read<long long>(Stream);
						auto RegTarget		 = &Reg[GetRegRawByte(L"fq" + RegNum)];

						auto Object = ObjectList[ObjectPosititon];
						if (Object->GetType() == DAT_QWORD)
						{
							*RegTarget = ((VASMQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_DWORD)
						{
							*RegTarget = ((VASMDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_WORD)
						{
							*RegTarget = ((VASMWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HQWORD)
						{
							*RegTarget = ((VASMHQWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HDWORD)
						{
							*RegTarget = ((VASMHDWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_HWORD)
						{
							*RegTarget = ((VASMHWORDObject *)Object)->Data;
						}
						if (Object->GetType() == DAT_STACK)
						{
							*RegTarget =
								((VASMStackObject *)Object)->Data[((VASMStackObject *)Object)->Data.size() - 1];
						}
					}
					if (OP_Code == PAR_REG)
					{
						auto RegPosition				   = Read<byte>(Stream);
						Reg[GetRegRawByte(L"fq" + RegNum)] = Reg[RegPosition];
					}

					++RegNum;
				}

				Run(StringStream);
			}
		}
		}
	}
}
} // namespace VASM