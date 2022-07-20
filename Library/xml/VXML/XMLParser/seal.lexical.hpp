#pragma once

#pragma warning(disable : 26451)

/////////////////////////////////////////////////////////////////////////////
//         S E A L  P R O G R A M M I N G  L A N G U A G E (seal-lang.org)
//  
//  seal.lexical.h : 
//       @description :  This header file defines the token cutting in seal's lexical analyzer
//       @birth       :  2021/2.15
//       @operator    :  Margoo
//

#include "seal.lexical.token.hpp"
#include "seal.info.hpp"

#include <string>

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
typedef struct _lexical_info {
	// lexical traverses the subscripts of the source string
	se_int lexical_index = 0;
	// read the line relative to the code
	se_int lexical_line = 0;

	// lexical temporarily stored script code to be interpreted
	std::wstring lexical_code;

	///////////////////////////////////////////////////////////////
	//   _lexical_token[typedef with seal_lexical_token] :
	//          @description :   Token type
	//          @birth       :   2021/2.15
	//          @operator    :   Margoo
	//
	typedef struct _lexical_token_type {
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
typedef class _lexical_core {
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
	//           
	//           
	_lexical_core(std::wstring core_code = L"") {
		core_info.lexical_code = core_code;
	}

	//////////////////////////////////////////////////////////////////////////
	//    get_token  :
	//           @description :   Cut a token from code_string
	//           @birth       :   2021/2.15
	//           
	_lexical_info::seal_lexical_token get_token() {
		bool number_point = false;

		if (lexical_update_line == true) {
			++core_info.lexical_line;
		}

		// Init core_info
		core_info.lexical_token.cache_token = NO_STATUS_TOKEN;
		core_info.lexical_token.token_string = L"";

		// If it's over
		if (!(core_info.lexical_index < core_info.lexical_code.size())) {
			// Set eof status
			core_info.lexical_eof = true;

			return _lexical_info::_lexical_token_type{ EOF_TOKEN, L"" };
		}

		// Traverse string
		for (; core_info.lexical_index < core_info.lexical_code.size(); ++core_info.lexical_index) {
			// Skip null characters
			if (core_info.lexical_code[core_info.lexical_index] == '\t' ||
				core_info.lexical_code[core_info.lexical_index] == ' ' ||
				core_info.lexical_code[core_info.lexical_index] == '\n') {
				if (core_info.lexical_token.cache_token != NO_STATUS_TOKEN) {
					break;
				}

				// If there is a newline
				if (core_info.lexical_code[core_info.lexical_index - 1] == '\n') {
					if (core_info.lexical_token.cache_token != NO_STATUS_TOKEN) {
						lexical_update_line = true;
					}
					else {
						++lexical_update_line;

						continue;
					}
				}

				if (core_info.lexical_index + 1 >= core_info.lexical_code.size()) {
					core_info.lexical_eof = true;

					return _lexical_info::_lexical_token_type{ EOF_TOKEN, L"" };
				}

				continue;
			}

			// If is symbol
			if (_MACRO_IS_SYMBOL_(core_info.lexical_code[core_info.lexical_index])) {
				if (core_info.lexical_token.cache_token == CONST_NUMBER) {
					if (core_info.lexical_code[core_info.lexical_index] == '.') {
						if (number_point == false) {
							number_point = true;
						}
						else {
							core_info.lexical_token.token_string += L"0";

							break;
						}

						core_info.lexical_token.token_string += L".";

						continue;
					}
					else {
						break;
					}
				}
				// If it is not UNKNOW_TOKEN, then encountering a symbol means the end of a token
				if (core_info.lexical_token.cache_token != NO_STATUS_TOKEN) {
					break;
				}

				// Determine mandatory characters
				if (core_info.lexical_code[core_info.lexical_index] == '=') {
					++core_info.lexical_index;

					core_info.lexical_token.token_string = L"=";

					// If there is a continuous
					if (core_info.lexical_index < core_info.lexical_code.size() &&
						(core_info.lexical_code[core_info.lexical_index] == '=' ||
							core_info.lexical_code[core_info.lexical_index] == '>' ||
							core_info.lexical_code[core_info.lexical_index] == '<'
							)
						) {
						++core_info.lexical_index;
						// Set string variable
						core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_index];

						break;
					}
					else {
						break;
					}
				}
				if (core_info.lexical_code[core_info.lexical_index] == '<' ||
					core_info.lexical_code[core_info.lexical_index] == '>') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					// If there is a continuous
					if (core_info.lexical_index < core_info.lexical_code.size() &&
						(
							core_info.lexical_code[core_info.lexical_index] == '=' ||
							(
								core_info.lexical_code[core_info.lexical_index] == '-' &&
								core_info.lexical_token.token_string == L"<"
								)
							)) {
						++core_info.lexical_index;

						core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_index];

						break;
					}
					else {
						break;
					}
				}

				// Judge a single character
				if ((core_info.lexical_code[core_info.lexical_index] == '{' ||
					core_info.lexical_code[core_info.lexical_index] == '[' ||
					core_info.lexical_code[core_info.lexical_index] == '(' ||
					core_info.lexical_code[core_info.lexical_index] == '}' ||
					core_info.lexical_code[core_info.lexical_index] == ']' ||
					core_info.lexical_code[core_info.lexical_index] == ')' ||
					core_info.lexical_code[core_info.lexical_index] == ',' ||
					core_info.lexical_code[core_info.lexical_index] == '%' ||
					core_info.lexical_code[core_info.lexical_index] == '~' ||
					core_info.lexical_code[core_info.lexical_index] == '&' ||
					core_info.lexical_code[core_info.lexical_index] == ':') &&
					core_info.lexical_token.token_string == L"") {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					break;
				}

				// Processing string
				if (core_info.lexical_code[core_info.lexical_index] == '\"' ||
					core_info.lexical_code[core_info.lexical_index] == '\'') {
					core_info.lexical_token.cache_token = CONST_STRING;

					char head_character = core_info.lexical_code[core_info.lexical_index];

					core_info.lexical_token.token_string = head_character;

					++core_info.lexical_index;

					for (; core_info.lexical_index < core_info.lexical_code.size(); ++core_info.lexical_index) {
						if (core_info.lexical_code[core_info.lexical_index] == '\\') {
							char insert_character = '\?';

							if (core_info.lexical_index + 1 < core_info.lexical_code.size()) {
								if (core_info.lexical_code[core_info.lexical_index + 1] == 'n') {
									insert_character = '\n';
								}
								if (core_info.lexical_code[core_info.lexical_index + 1] == 'r') {
									insert_character = '\n';
								}
								if (core_info.lexical_code[core_info.lexical_index + 1] == 'b') {
									insert_character = '\b';
								}
								if (core_info.lexical_code[core_info.lexical_index + 1] == 'a') {
									insert_character = '\a';
								}
								if (core_info.lexical_code[core_info.lexical_index + 1] == '\"') {
									insert_character = '\"';
								}
							}

							++core_info.lexical_index;

							core_info.lexical_token.token_string += insert_character;
						}
						else {
							core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_index];

							if (core_info.lexical_code[core_info.lexical_index] == head_character) {
								++core_info.lexical_index;

								break;
							}
						}
					}

					break;
				}

				if (core_info.lexical_code[core_info.lexical_index] == ';') {
					if (core_info.lexical_token.cache_token != NO_STATUS_TOKEN) {
						core_info.lexical_token.token_string = L";";

						++core_info.lexical_index;
					}

					break;
				}

				// Handling of binary and above operators
				if (core_info.lexical_code[core_info.lexical_index] == '-' ||
					core_info.lexical_code[core_info.lexical_index] == '+') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == core_info.lexical_code[core_info.lexical_index - 1]) {
						core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_index];

						++core_info.lexical_index;
					}
					if (core_info.lexical_code[core_info.lexical_index] == '=') {
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}
				if (core_info.lexical_code[core_info.lexical_index] == '*') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == '=') {
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}
				if (core_info.lexical_code[core_info.lexical_index] == '^') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == '=') {
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}

				// Processing logical expression operators
				if (core_info.lexical_code[core_info.lexical_index] == '!') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == '=') {
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}
				if (core_info.lexical_code[core_info.lexical_index] == '|') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == '|') {
						core_info.lexical_token.token_string += '&';

						++core_info.lexical_index;
					}
					if (core_info.lexical_code[core_info.lexical_index] == '=') {
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}
				if (core_info.lexical_code[core_info.lexical_index] == '&') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == '&') {
						core_info.lexical_token.token_string += '&';

						++core_info.lexical_index;
					}
					if (core_info.lexical_code[core_info.lexical_index] == '=') {
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}

				// Processing comments
				if (core_info.lexical_code[core_info.lexical_index] == '/') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;
					// Is it a comment or an expression

					if (core_info.lexical_code[core_info.lexical_index] == '=') {
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}
					if (core_info.lexical_code[core_info.lexical_index] == '/') {
						core_info.lexical_token.token_string += '/';

						++core_info.lexical_index;
					}
					if (core_info.lexical_code[core_info.lexical_index] == '*') {
						core_info.lexical_token.token_string += '*';

						++core_info.lexical_index;
					}

					break;
				}

				core_info.lexical_token.cache_token = UNKNOW_SMYBOL;
			}
			else {
				// If it is not a token
				if (core_info.lexical_token.cache_token > COMMENT_TOKEN && core_info.lexical_token.cache_token <= BITWISE_OR_TOKEN) {
					break;
				}

				if (_MACRO_IS_NUMBER_(core_info.lexical_code[core_info.lexical_index]) == true &&
					core_info.lexical_token.cache_token != UNKNOW_TOKEN) {
					core_info.lexical_token.cache_token = CONST_NUMBER;
				}
				else {
					core_info.lexical_token.cache_token = UNKNOW_TOKEN;
				}
			}

			core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_index];
		}

		// Processing keyword token
		if (core_info.lexical_token.token_string == L"int") {
			core_info.lexical_token.cache_token = INT_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"bool") {
			core_info.lexical_token.cache_token = BOOL_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"string") {
			core_info.lexical_token.cache_token = STRING_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"char") {
			core_info.lexical_token.cache_token = CHAR_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"float") {
			core_info.lexical_token.cache_token = FLOAT_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"def") {
			core_info.lexical_token.cache_token = DEF_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"if") {
			core_info.lexical_token.cache_token = IF_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"else") {
			core_info.lexical_token.cache_token = ELSE_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"elif") {
			core_info.lexical_token.cache_token = ELIF_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"switch") {
			core_info.lexical_token.cache_token = SWITCH_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"case") {
			core_info.lexical_token.cache_token = CASE_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"default") {
			core_info.lexical_token.cache_token = DEFAULT_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"while") {
			core_info.lexical_token.cache_token = WHILE_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"continue") {
			core_info.lexical_token.cache_token = CONTINUE_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"break") {
			core_info.lexical_token.cache_token = BREAK_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"operator") {
			core_info.lexical_token.cache_token = OPERATOR_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"class") {
			core_info.lexical_token.cache_token = CLASS_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"public") {
			core_info.lexical_token.cache_token = PUBLIC_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"private") {
			core_info.lexical_token.cache_token = PRIVATE_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"return") {
			core_info.lexical_token.cache_token = RETURN_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"enum") {
			core_info.lexical_token.cache_token = ENUM_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"using") {
			core_info.lexical_token.cache_token = USING_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"from") {
			core_info.lexical_token.cache_token = FROM_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"NULL") {
			core_info.lexical_token.cache_token = NULL_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"SAFENULL") {
			core_info.lexical_token.cache_token = SAFENULL_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"delete") {
			core_info.lexical_token.cache_token = DELETE_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"do") {
			core_info.lexical_token.cache_token = DO_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"for") {
			core_info.lexical_token.cache_token = FOR_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"parad") {
			core_info.lexical_token.cache_token = PARAD_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"static") {
			core_info.lexical_token.cache_token = STATIC_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"new_static") {
			core_info.lexical_token.cache_token = NEW_STATIC_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"un_gc") {
			core_info.lexical_token.cache_token = UN_GC_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"force_delete") {
			core_info.lexical_token.cache_token = FORCE_DELETE_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"no_refer_count") {
			core_info.lexical_token.cache_token = NO_REFER_COUNT_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"no_old") {
			core_info.lexical_token.cache_token = NO_OLD_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"const") {
			core_info.lexical_token.cache_token = CONST_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"seal_library_type") {
			core_info.lexical_token.cache_token = SEAL_LIBRARY_TYPE_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"seal_library_function") {
			core_info.lexical_token.cache_token = SEAL_LIBRARY_FUNCTION_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"seal_library_run_function") {
			core_info.lexical_token.cache_token = SEAL_LIBRARY_RUN_FUNCTION_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"type_alias") {
			core_info.lexical_token.cache_token = TYPE_ALIAS_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"tpye_define") {
			core_info.lexical_token.cache_token = TYPE_DEFINE_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"function") {
			core_info.lexical_token.cache_token = FUNCTION_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"retr_for") {
			core_info.lexical_token.cache_token = RETR_FOR_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"namespace") {
			core_info.lexical_token.cache_token = NAMESPACE_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"true" ||
			core_info.lexical_token.token_string == L"false") {
			core_info.lexical_token.cache_token = CONST_BOOLEAN;
		}

		// Processing token
		if (core_info.lexical_token.token_string == L"(") {
			core_info.lexical_token.cache_token = LEFT_BRACKETS;
		}
		else if (core_info.lexical_token.token_string == L")") {
			core_info.lexical_token.cache_token = RIGHT_BRACKETS;
		}
		else if (core_info.lexical_token.token_string == L"[") {
			core_info.lexical_token.cache_token = MIDDLE_LEFT_BRACKETS;
		}
		else if (core_info.lexical_token.token_string == L"]") {
			core_info.lexical_token.cache_token = MIDDLE_RIGHT_BRACKETS;
		}
		else if (core_info.lexical_token.token_string == L"{") {
			core_info.lexical_token.cache_token = BIG_LEFT_BRACKETS;
		}
		else if (core_info.lexical_token.token_string == L"}") {
			core_info.lexical_token.cache_token = BIG_RIGHT_BRACKETS;
		}
		else if (core_info.lexical_token.token_string == L";") {
			core_info.lexical_token.cache_token = SEMICOLON_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L".") {
			core_info.lexical_token.cache_token = DOT_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L",") {
			core_info.lexical_token.cache_token = COMMA_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L":") {
			core_info.lexical_token.cache_token = COLON_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"+") {
			core_info.lexical_token.cache_token = PLUS_SIGN_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"-") {
			core_info.lexical_token.cache_token = MINUS_SIGN_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"*") {
			core_info.lexical_token.cache_token = MULTIPLICATION_SIGN_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"/") {
			core_info.lexical_token.cache_token = SLASH_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"%") {
			core_info.lexical_token.cache_token = MODULO_SIGN_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"++") {
			core_info.lexical_token.cache_token = DOUBLE_PLUS_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"--") {
			core_info.lexical_token.cache_token = DOUBLE_MINUS_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"_") {
			core_info.lexical_token.cache_token = UNDERSCORE_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"=") {
			core_info.lexical_token.cache_token = EQUAL_SIGN_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"==") {
			core_info.lexical_token.cache_token = DOUBLE_EQUAL_SIGN_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"!") {
			core_info.lexical_token.cache_token = EXCLAMATION_MARK;
		}
		else if (core_info.lexical_token.token_string == L"!=") {
			core_info.lexical_token.cache_token = NOT_EQUAL_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L">") {
			core_info.lexical_token.cache_token = MORE_THAN_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"<") {
			core_info.lexical_token.cache_token = LESS_THAN_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L">=") {
			core_info.lexical_token.cache_token = MORE_THAN_EQUAL_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"<=") {
			core_info.lexical_token.cache_token = LESS_THAN_EQUAL_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"&") {
			core_info.lexical_token.cache_token = VERSUS_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"&&") {
			core_info.lexical_token.cache_token = LOGICALAND_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"|") {
			core_info.lexical_token.cache_token = VERTICAL_BAR_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"||") {
			core_info.lexical_token.cache_token = LOGICAL_OR_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"^") {
			core_info.lexical_token.cache_token = CARET_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"~") {
			core_info.lexical_token.cache_token = TILDE_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"<<") {
			core_info.lexical_token.cache_token = BR_LEFT_SHIFT_OPERATOR_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L">>") {
			core_info.lexical_token.cache_token = BR_RIGHT_SHIFT_OPERATOR_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"+=") {
			core_info.lexical_token.cache_token = ADDITIONAND_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"-=") {
			core_info.lexical_token.cache_token = MINUSITIONAND_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"*=") {
			core_info.lexical_token.cache_token = MULTIPLYAND_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"/=") {
			core_info.lexical_token.cache_token = EXCEPTAND_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"&=") {
			core_info.lexical_token.cache_token = BITWISEAND_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"<-") {
			core_info.lexical_token.cache_token = INHERITANCE_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"^=") {
			core_info.lexical_token.cache_token = BITWISEANDDIFFERENT_TOKEN;
		}
		else if (core_info.lexical_token.token_string == L"|=") {
			core_info.lexical_token.cache_token = BITWISE_OR_TOKEN;
		}

		return core_info.lexical_token;
	}

	//////////////////////////////////////////////////////////////////////////
	//    is_eof  :
	//           @description :   Whether it ends, if end, return true
	//           @birth       :   2021/2.16
	//           
	bool is_eof() {
		return core_info.lexical_eof;
	}

	//////////////////////////////////////////////////////////////////////////
	//    get_index  :
	//           @description :   Get index position
	//           @birth       :   2021/3.5
	//  
	se_int get_index() {
		return core_info.lexical_index;
	}

	//////////////////////////////////////////////////////////////////////////
	//    get_line  :
	//           @description :   Get line
	//           @birth       :   2022/7.19
	//  
	se_int get_line() {
		return core_info.lexical_line;
	}
} seal_lexical;
