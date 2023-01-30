#pragma once

#include <string>

namespace VASM
{

using byte = char;

byte GetRegRawByte(const std::wstring &RegName);

using qword = unsigned __int64;
using dword = unsigned long;
using word	= unsigned short;

using hword	 = short;
using hdword = int;
using hqword = long;

#define DAT		   (byte(0xA0))
#define DAT_STACK  (byte(0xA1))
#define DAT_STRING (byte(0xA2))
#define DAT_QWORD  (byte(0xA3))
#define DAT_DWORD  (byte(0xA4))
#define DAT_WORD   (byte(0xA5))
#define DAT_HQWORD (byte(0xA6))
#define DAT_HDWORD (byte(0xA7))
#define DAT_HWORD  (byte(0xA8))
#define DAT_INIT   (byte(0xA9))

#define CALL	   (byte(0xB0))
#define NEXT_PAR   (byte(0xB1))
#define CALL_END   (byte(0xB2))
#define PAR_NUM	   (byte(0xB3))
#define PAR_STRING (byte(0xB4))
#define PAR_VAR	   (byte(0xB5))
#define PAR_REG	   (byte(0xB6))

#define MOV	 (byte(0xC0)) // =
#define MOVA (byte(0xC1)) // +
#define MOVS (byte(0xC2)) // -
#define MOVM (byte(0xC3)) // *
#define MOVD (byte(0xC4)) // /

#define COMP   (byte(0xD0)) // ==
#define COMPB  (byte(0xD1)) // >
#define COMPL  (byte(0xD2)) // <
#define COMPLE (byte(0xD4)) // <=
#define COMPBE (byte(0xD5)) // >=
#define COMPN  (byte(0xD6)) // !=

#define READ  (byte(0xE0))
#define WRITE (byte(0xE1))

#define PUSH (byte(0xE2))
#define POP	 (byte(0xE3))

#define FNC	  (byte(0xF0))
#define LABEL (byte(0xF1))
#define GOTO  (byte(0xF2))

#define MEM	 (byte(0xF3))
#define DISK (byte(0xF4))

#define BGN_STRING (byte(0xF5))
#define END_STRING (byte(0xF6))
#define BGN_CALL_F (byte(0xF5))
#define END_CALL_F (byte(0xF6))
#define END		   (byte(0xF7))
#define EMPTY	   (byte(0xFFFF))

} // namespace VASM