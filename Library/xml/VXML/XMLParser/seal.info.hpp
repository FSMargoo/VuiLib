#pragma once

/////////////////////////////////////////////////////////////////////////////
//         S E A L  P R O G R A M M I N G  L A N G U A G E (seal-lang.org)
//  
//  seal.info.h : 
//       @description :  This header folder defines the token required by leixcal
//       @birth       :  2021/2.15
//       @operator    :  Margoo
//

///////////////////////////////////////////////////////////////
//   se_large_int, se_int[Selective definition] :
//          @description :   [se_large_int] (Large integer type in seal) 
//							 [se_int] (Common integer types in the seal source code)
//          @birth       :   2021/2.15
//          @operator    :   Margoo
#ifdef _WIN64
#	define se_large_int __int64
#	define se_int int
#else
#	define se_large_int __int32
#	define se_int size_t
#endif

// Disable warnings that must be blocked
#pragma warning(disable : 26812)

// Macro definitions provided to the lexical analyzer
#define _MACRO_IS_SYMBOL_(object) \
		(!(object >= 'a' && object <= 'z') && !(object >= 'A' && object <= 'Z') \
		&& !(object >= '0' && object <= '9') && object != '_')
#define _MACRO_IS_NUMBER_(object) \
		(object >= '0' && object <= '9')
