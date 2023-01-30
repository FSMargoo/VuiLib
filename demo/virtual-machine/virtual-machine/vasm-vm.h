#pragma once

#include "vasm-compiler.h"
#include <sstream>

namespace VASM
{
struct VirtualMachineConfig
{
	__int64 MemorySize;
	__int64 DiskSize;
};

class VASMGPU
{
private:
	std::wstring GP_OutputCache;

public:
	VASMGPU();

	virtual void PushCharacter(const wchar_t &Character);
	virtual void PushCharacter(const std::wstring &String);
	virtual void Flush();
	virtual void Clear();
};

class VASMBaseObject
{
public:
	virtual short GetType() = 0;
};
class VASMStringObject : public VASMBaseObject
{
public:
	std::wstring Data;

	short GetType()
	{
		return DAT_STRING;
	}
};
class VASMQWORDObject : public VASMBaseObject
{
public:
	qword Data;

	short GetType()
	{
		return DAT_QWORD;
	}
};
class VASMDWORDObject : public VASMBaseObject
{
public:
	dword Data;

	short GetType()
	{
		return DAT_DWORD;
	}
};
class VASMWORDObject : public VASMBaseObject
{
public:
	word Data;

	short GetType()
	{
		return DAT_WORD;
	}
};
class VASMHQWORDObject : public VASMBaseObject
{
public:
	hqword Data;

	short GetType()
	{
		return DAT_HQWORD;
	}
};
class VASMHDWORDObject : public VASMBaseObject
{
public:
	hdword Data;

	short GetType()
	{
		return DAT_HDWORD;
	}
};
class VASMHWORDObject : public VASMBaseObject
{
public:
	hword Data;

	short GetType()
	{
		return DAT_HWORD;
	}
};
class VASMStackObject : public VASMBaseObject
{
public:
	std::vector<qword> Data;

	short GetType()
	{
		return DAT_STACK;
	}
};
class VASMRegObject : public VASMBaseObject
{
public:
	hqword Data;

	VASMRegObject()
	{
		Data = 0;
	}

	operator qword()
	{
		return Data;
	}

	VASMRegObject &operator=(hqword Value)
	{
		Data = Value;

		return *this;
	}
	VASMRegObject &operator+=(hqword Value)
	{
		Data += Value;

		return *this;
	}
	VASMRegObject &operator*=(hqword Value)
	{
		Data *= Value;

		return *this;
	}
	VASMRegObject &operator/=(hqword Value)
	{
		Data /= Value;

		return *this;
	}

	short GetType()
	{
		return PAR_REG;
	}
};

typedef class VASMFunction
{
public:
	std::vector<byte> FncByte;
} VASMLabel;

class VASMInterpreter
{
public:
	VASMInterpreter(const std::wstring &VirtualDisk, const VirtualMachineConfig &Config, VASMGPU *GPU);
	~VASMInterpreter();

public:
	void RunVASMFile(const std::wstring &File);

private:
	template <class Type> Type Read(std::wstringstream &FileStream)
	{
		Type *Object = new Type;
		FileStream.read((wchar_t *)Object, sizeof(Type));

		return *Object;
	}
	std::wstring ReadString(std::wstringstream &FileStream);

private:
	std::map<std::wstring, VASMLabel>			   LabelList;
	std::map<std::wstring, VASMFunction>		   FunctionList;
	std::map<unsigned long long, VASMBaseObject *> ObjectList;

public:
	void Run(std::wstringstream &File);

public:
	void RunBoot();

private:
	std::fstream		 DiskStream;
	VirtualMachineConfig MachineConfig;
	byte				*Memory;
	byte				*Disk;
	VASMRegObject		 Reg[17577];
	VASMGPU				*GPU;
};
} // namespace VASM