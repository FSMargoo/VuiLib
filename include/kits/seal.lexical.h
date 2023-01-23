#pragma once

#pragma warning(disable : 26451)

/////////////////////////////////////////////////////////////////////////////
//           S E A L  P R O G R A M M I N G  L A N G U A G E
//
//  seal.lexical.h :
//       @description :  This header file defines the token cutting in seal's lexical analyzer
//       @birth       :  2021/2.15
//       @operator    :  Margoo
//

#include <string>

#include "seal.info.h"
#include "seal.lexical.token.h"

namespace VKits
{
///////////////////////////////////////////////////////////////
//   _lexical_info[typedef with seal_lexical_type_info] :
//          @description :   Token splitter information
//          @birth       :   2021/2.15
//          @operator    :   Margoo
//
//   member :
//        lexical_index [var]
//		  lexical_line  [var]
//        lexical_code  [var]
//        lexical_token [var]
//
//        _lexical_token [struct]
typedef struct _lexical_info
{
	// lexical traverses the subscripts of the source string
	se_int lexical_index = 0;
	// read the line relative to the code
	se_int lexical_line = 0;
	// ignore the comment token when lexical is parsing
	bool ignore_comment = true;

	// lexical temporarily stored script code to be interpreted
	std::wstring lexical_code;

	///////////////////////////////////////////////////////////////
	//   _lexical_token[typedef with seal_lexical_token] :
	//          @description :   Token type
	//          @birth       :   2021/2.15
	//          @operator    :   Margoo
	//
	typedef struct _lexical_token_type
	{
		// Temporary token
		_lexical_token cache_token = NO_STATUS_TOKEN;
		// Token's string
		std::wstring token_string = L"";
	} seal_lexical_token;

	_lexical_token_type lexical_token;

	// Is lexical eof
	bool lexical_eof = false;
} seal_lexical_type_info;

///////////////////////////////////////////////////////////////
//   _lexical_core[typedef with seal_lexical] :
//          @description :   Token split core
//          @birth       :   2021/2.15
//          @operator    :   Margoo
typedef class _lexical_core
{
private:
	// Define info class
	seal_lexical_type_info core_info;

	// Is or not update line
	bool lexical_update_line = false;

public:
	//////////////////////////////////////////////////////////////////////////
	//    _lexical_core[constructor]  :
	//           @description :   The default constructor of lexical core
	//           @birth       :   2021/2.15
	//			 @last change :	  2023/1.7
	//
	//
	_lexical_core(std::wstring core_code = L"", bool ignore_comment = true);

	//////////////////////////////////////////////////////////////////////////
	//    get_token  :
	//           @description :   Cut a token from code_string
	//           @birth       :   2021/2.15
	//
	_lexical_info::seal_lexical_token get_token();
	//////////////////////////////////////////////////////////////////////////
	//    view_token  :
	//           @description :   Cut a token from code_string ( in view mode )
	//           @birth       :   2022/8.26
	//
	_lexical_info::seal_lexical_token view_token();
	//////////////////////////////////////////////////////////////////////////
	//    view_token  :
	//           @description :   Cut a token from code_string ( in view mode )
	//           @birth       :   2022/8.26
	//
	_lexical_info::seal_lexical_token view_token(int count);

	//////////////////////////////////////////////////////////////////////////
	//    is_eof  :
	//           @description :   Whether it ends, if end, return true
	//           @birth       :   2021/2.16
	//
	bool is_eof();

	//////////////////////////////////////////////////////////////////////////
	//    get_index  :
	//           @description :   Get index position
	//           @birth       :   2021/3.5
	//
	se_int get_index();

	//////////////////////////////////////////////////////////////////////////
	//    get_line  :
	//           @description :   Get line
	//           @birth       :   2022/7.19
	//
	se_int get_line();

	//////////////////////////////////////////////////////////////////////////
	//    get_buffer  :
	//           @description :   Get raw code
	//           @birth       :   2022/12.21
	//
	std::wstring get_buffer() const;
} seal_lexical;
} // namespace VKits