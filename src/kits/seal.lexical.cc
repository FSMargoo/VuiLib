#pragma once

#pragma warning(disable : 26451)

/////////////////////////////////////////////////////////////////////////////
//         S E A L  P R O G R A M M I N G  L A N G U A G E (seal-lang.org)
//
//  seal.lexical.h :
//       @description :  This header file defines the token cutting in seal's
//       lexical analyzer
//       @birth       :  2021/2.15
//       @operator    :  Margoo
//

#include "../../include/kits/seal.lexical.h"

#include <string>

namespace VKits
{
_lexical_core::_lexical_core(std::wstring core_code, bool ignore_comment)
{
	core_info.lexical_code	 = core_code;
	core_info.ignore_comment = ignore_comment;
}

//////////////////////////////////////////////////////////////////////////
//    get_token  :
//           @description :   Cut a token from code_string
//           @birth       :   2021/2.15
//
_lexical_info::seal_lexical_token _lexical_core::get_token()
{
	bool number_point = false;

	if (lexical_update_line == true)
	{
		++core_info.lexical_line;

		lexical_update_line = false;
	}

	// Init core_info
	core_info.lexical_token.cache_token	 = NO_STATUS_TOKEN;
	core_info.lexical_token.token_string = L"";

	// If it's over
	if (!(core_info.lexical_index + 1 <= core_info.lexical_code.size()))
	{
		// Set eof status
		core_info.lexical_eof = true;

		return _lexical_info::_lexical_token_type{EOF_TOKEN, L""};
	}

	bool own_dot = false;

	// Traverse string
	for (; core_info.lexical_index < core_info.lexical_code.size(); ++core_info.lexical_index)
	{
		if (!(core_info.lexical_index < core_info.lexical_code.size()))
		{
			// Set eof status
			core_info.lexical_eof = true;

			return _lexical_info::_lexical_token_type{EOF_TOKEN, L""};
		}

		if (core_info.lexical_token.cache_token == COMMENT_TOKEN)
		{
			if (core_info.lexical_index + 1 < core_info.lexical_code.size() &&
				core_info.lexical_code[core_info.lexical_index] == L'*' &&
				core_info.lexical_code[core_info.lexical_index + 1] == L'/')
			{
				if (core_info.ignore_comment)
				{
					core_info.lexical_token.cache_token	 = NO_STATUS_TOKEN;
					core_info.lexical_token.token_string = L"";

					core_info.lexical_index += 2;

					if (!(core_info.lexical_index < core_info.lexical_code.size()))
					{
						// Set eof status
						core_info.lexical_eof = true;

						return _lexical_info::_lexical_token_type{EOF_TOKEN, L""};
					}

					continue;
				}
				else
				{
					core_info.lexical_index += 2;
					core_info.lexical_token.token_string += L"*/";

					return core_info.lexical_token;
				}
			}
			else
			{
				if (!core_info.ignore_comment)
				{
					core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_index];
				}
			}
		}
		else
		{
			// Skip null characters
			if (core_info.lexical_code[core_info.lexical_index] == '\t' ||
				core_info.lexical_code[core_info.lexical_index] == ' ' ||
				core_info.lexical_code[core_info.lexical_index] == '\n' ||
				core_info.lexical_code[core_info.lexical_index] == '\r')
			{
				if (core_info.lexical_token.cache_token != NO_STATUS_TOKEN)
				{
					break;
				}

				// If there is a newline
				if (core_info.lexical_index - 1 >= 0)
				{
					if (core_info.lexical_code[core_info.lexical_index - 1] == '\n' ||
						core_info.lexical_code[core_info.lexical_index - 1] == '\r')
					{
						if (core_info.lexical_token.cache_token != NO_STATUS_TOKEN)
						{
							lexical_update_line = true;
						}
						else
						{
							++core_info.lexical_line;

							if (core_info.lexical_index + 1 >= core_info.lexical_code.size())
							{
								core_info.lexical_eof = true;

								return _lexical_info::_lexical_token_type{EOF_TOKEN, L""};
							}

							continue;
						}
					}
				}

				if (core_info.lexical_index + 1 >= core_info.lexical_code.size())
				{
					core_info.lexical_eof = true;

					return _lexical_info::_lexical_token_type{EOF_TOKEN, L""};
				}

				continue;
			}

			// If is symbol
			if (_MACRO_IS_SYMBOL_(core_info.lexical_code[core_info.lexical_index]))
			{
				if (core_info.lexical_token.cache_token == CONST_NUMBER)
				{
					if (core_info.lexical_code[core_info.lexical_index] == '.')
					{
						if (number_point == false)
						{
							number_point = true;
						}
						else
						{
							core_info.lexical_token.token_string += L"0";

							break;
						}

						core_info.lexical_token.token_string += L".";

						continue;
					}
					else
					{
						break;
					}
				}
				// If it is not UNKNOW_TOKEN, then encountering a symbol means the end
				// of a token
				else if (core_info.lexical_token.cache_token != NO_STATUS_TOKEN)
				{
					break;
				}

				// Determine mandatory characters
				else if (core_info.lexical_code[core_info.lexical_index] == '=')
				{
					++core_info.lexical_index;

					core_info.lexical_token.token_string = L"=";

					// If there is a continuous
					if (core_info.lexical_index < core_info.lexical_code.size() &&
						(core_info.lexical_code[core_info.lexical_index] == '=' ||
						 core_info.lexical_code[core_info.lexical_index] == '>' ||
						 core_info.lexical_code[core_info.lexical_index] == '<'))
					{
						++core_info.lexical_index;
						// Set string variable
						core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_index];

						break;
					}
					else
					{
						break;
					}
				}
				else if (core_info.lexical_code[core_info.lexical_index] == '<' ||
						 core_info.lexical_code[core_info.lexical_index] == '>')
				{
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					// If there is a continuous
					if (core_info.lexical_index < core_info.lexical_code.size() &&
						(core_info.lexical_code[core_info.lexical_index] == '=' ||
						 (core_info.lexical_code[core_info.lexical_index] == '-' &&
						  core_info.lexical_token.token_string == L"<")))
					{
						++core_info.lexical_index;

						core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_index];

						break;
					}
					else
					{
						break;
					}
				}

				// Judge a single character
				else if ((core_info.lexical_code[core_info.lexical_index] == '{' ||
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
						 core_info.lexical_token.token_string == L"")
				{
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					break;
				}

				// Processing string
				else if (core_info.lexical_code[core_info.lexical_index] == '\"' ||
						 core_info.lexical_code[core_info.lexical_index] == '\'')
				{
					core_info.lexical_token.cache_token = CONST_STRING;

					char head_character = core_info.lexical_code[core_info.lexical_index];

					core_info.lexical_token.token_string = head_character;

					++core_info.lexical_index;

					for (; core_info.lexical_index < core_info.lexical_code.size(); ++core_info.lexical_index)
					{
						if (core_info.lexical_code[core_info.lexical_index] == '\\')
						{
							char insert_character = '\?';

							if (core_info.lexical_index + 1 < core_info.lexical_code.size())
							{
								if (core_info.lexical_code[core_info.lexical_index + 1] == 'n')
								{
									insert_character = '\n';
								}
								if (core_info.lexical_code[core_info.lexical_index + 1] == 'r')
								{
									insert_character = '\n';
								}
								if (core_info.lexical_code[core_info.lexical_index + 1] == 'b')
								{
									insert_character = '\b';
								}
								if (core_info.lexical_code[core_info.lexical_index + 1] == 'a')
								{
									insert_character = '\a';
								}
								if (core_info.lexical_code[core_info.lexical_index + 1] == '\"')
								{
									insert_character = '\"';
								}
							}

							++core_info.lexical_index;

							core_info.lexical_token.token_string += insert_character;
						}
						else
						{
							core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_index];

							if (core_info.lexical_code[core_info.lexical_index] == head_character)
							{
								++core_info.lexical_index;

								break;
							}
						}
					}

					break;
				}

				else if (core_info.lexical_code[core_info.lexical_index] == ';')
				{
					if (core_info.lexical_token.cache_token == NO_STATUS_TOKEN)
					{
						core_info.lexical_token.token_string = L";";

						++core_info.lexical_index;
					}

					break;
				}

				// Handling of binary and above operators
				else if (core_info.lexical_code[core_info.lexical_index] == '-' ||
						 core_info.lexical_code[core_info.lexical_index] == '+')
				{
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] ==
						core_info.lexical_code[core_info.lexical_index - 1])
					{
						core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_index];

						++core_info.lexical_index;
					}
					if (core_info.lexical_code[core_info.lexical_index] == '=')
					{
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}
				else if (core_info.lexical_code[core_info.lexical_index] == '*')
				{
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == '=')
					{
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}
				else if (core_info.lexical_code[core_info.lexical_index] == '^')
				{
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == '=')
					{
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}

				// Processing logical expression operators
				else if (core_info.lexical_code[core_info.lexical_index] == '!')
				{
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == '=')
					{
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}
				else if (core_info.lexical_code[core_info.lexical_index] == '|')
				{
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == '|')
					{
						core_info.lexical_token.token_string += '&';

						++core_info.lexical_index;
					}
					if (core_info.lexical_code[core_info.lexical_index] == '=')
					{
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}
				else if (core_info.lexical_code[core_info.lexical_index] == '&')
				{
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == '&')
					{
						core_info.lexical_token.token_string += '&';

						++core_info.lexical_index;
					}
					if (core_info.lexical_code[core_info.lexical_index] == '=')
					{
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}

				// Processing comments
				else if (core_info.lexical_code[core_info.lexical_index] == '/')
				{
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;
					// Is it a comment or an expression

					if (core_info.lexical_code[core_info.lexical_index] == '=')
					{
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;

						break;
					}
					if (core_info.lexical_code[core_info.lexical_index] == '/')
					{
						core_info.lexical_token.token_string += '/';

						++core_info.lexical_index;

						break;
					}
					if (core_info.lexical_code[core_info.lexical_index] == '*')
					{
						core_info.lexical_token.token_string += '/';
						core_info.lexical_token.token_string += '*';
						core_info.lexical_token.cache_token = COMMENT_TOKEN;

						++core_info.lexical_index;

						continue;
					}

					break;
				}
				else
				{
					core_info.lexical_token.cache_token	 = UNKNOW_SMYBOL;
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					break;
				}
			}
			else
			{
				// If it is not a token
				if (core_info.lexical_token.cache_token > COMMENT_TOKEN &&
					core_info.lexical_token.cache_token <= BITWISE_OR_TOKEN)
				{
					break;
				}

				if (core_info.lexical_code[core_info.lexical_index] == L'.' && own_dot == false)
				{
					own_dot = true;

					++core_info.lexical_index;
				}

				if (_MACRO_IS_NUMBER_(core_info.lexical_code[core_info.lexical_index]) == false &&
						core_info.lexical_token.cache_token == CONST_NUMBER ||
					(core_info.lexical_code[core_info.lexical_index] == L'.' && own_dot == true))
				{
					break;
				}

				if (_MACRO_IS_NUMBER_(core_info.lexical_code[core_info.lexical_index]) == true &&
					core_info.lexical_token.cache_token != UNKNOW_TOKEN)
				{
					core_info.lexical_token.cache_token = CONST_NUMBER;
				}
				else
				{
					core_info.lexical_token.cache_token = UNKNOW_TOKEN;
				}
			}

			core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_index];
		}
	}
	// Processing token
	if (core_info.lexical_token.token_string == L"(")
	{
		core_info.lexical_token.cache_token = LEFT_BRACKETS;
	}
	else if (core_info.lexical_token.token_string == L")")
	{
		core_info.lexical_token.cache_token = RIGHT_BRACKETS;
	}
	else if (core_info.lexical_token.token_string == L"[")
	{
		core_info.lexical_token.cache_token = MIDDLE_LEFT_BRACKETS;
	}
	else if (core_info.lexical_token.token_string == L"]")
	{
		core_info.lexical_token.cache_token = MIDDLE_RIGHT_BRACKETS;
	}
	else if (core_info.lexical_token.token_string == L"{")
	{
		core_info.lexical_token.cache_token = BIG_LEFT_BRACKETS;
	}
	else if (core_info.lexical_token.token_string == L"}")
	{
		core_info.lexical_token.cache_token = BIG_RIGHT_BRACKETS;
	}
	else if (core_info.lexical_token.token_string == L";")
	{
		core_info.lexical_token.cache_token = SEMICOLON_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L".")
	{
		core_info.lexical_token.cache_token = DOT_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L",")
	{
		core_info.lexical_token.cache_token = COMMA_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L":")
	{
		core_info.lexical_token.cache_token = COLON_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"+")
	{
		core_info.lexical_token.cache_token = PLUS_SIGN_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"-")
	{
		core_info.lexical_token.cache_token = MINUS_SIGN_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"*")
	{
		core_info.lexical_token.cache_token = MULTIPLICATION_SIGN_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"/")
	{
		core_info.lexical_token.cache_token = SLASH_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"%")
	{
		core_info.lexical_token.cache_token = MODULO_SIGN_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"++")
	{
		core_info.lexical_token.cache_token = DOUBLE_PLUS_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"--")
	{
		core_info.lexical_token.cache_token = DOUBLE_MINUS_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"_")
	{
		core_info.lexical_token.cache_token = UNDERSCORE_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"=")
	{
		core_info.lexical_token.cache_token = EQUAL_SIGN_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"==")
	{
		core_info.lexical_token.cache_token = DOUBLE_EQUAL_SIGN_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"!")
	{
		core_info.lexical_token.cache_token = EXCLAMATION_MARK;
	}
	else if (core_info.lexical_token.token_string == L"!=")
	{
		core_info.lexical_token.cache_token = NOT_EQUAL_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L">")
	{
		core_info.lexical_token.cache_token = MORE_THAN_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"<")
	{
		core_info.lexical_token.cache_token = LESS_THAN_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L">=")
	{
		core_info.lexical_token.cache_token = MORE_THAN_EQUAL_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"<=")
	{
		core_info.lexical_token.cache_token = LESS_THAN_EQUAL_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"&")
	{
		core_info.lexical_token.cache_token = VERSUS_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"&&")
	{
		core_info.lexical_token.cache_token = LOGICALAND_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"|")
	{
		core_info.lexical_token.cache_token = VERTICAL_BAR_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"||")
	{
		core_info.lexical_token.cache_token = LOGICAL_OR_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"^")
	{
		core_info.lexical_token.cache_token = CARET_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"~")
	{
		core_info.lexical_token.cache_token = TILDE_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"<<")
	{
		core_info.lexical_token.cache_token = BR_LEFT_SHIFT_OPERATOR_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L">>")
	{
		core_info.lexical_token.cache_token = BR_RIGHT_SHIFT_OPERATOR_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"+=")
	{
		core_info.lexical_token.cache_token = ADDITIONAND_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"-=")
	{
		core_info.lexical_token.cache_token = MINUSITIONAND_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"*=")
	{
		core_info.lexical_token.cache_token = MULTIPLYAND_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"/=")
	{
		core_info.lexical_token.cache_token = EXCEPTAND_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"&=")
	{
		core_info.lexical_token.cache_token = BITWISEAND_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"<-")
	{
		core_info.lexical_token.cache_token = INHERITANCE_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"^=")
	{
		core_info.lexical_token.cache_token = BITWISEANDDIFFERENT_TOKEN;
	}
	else if (core_info.lexical_token.token_string == L"|=")
	{
		core_info.lexical_token.cache_token = BITWISE_OR_TOKEN;
	}

	return core_info.lexical_token;
}

//////////////////////////////////////////////////////////////////////////
//    view_token  :
//           @description :   Cut a token from code_string ( in view mode )
//           @birth       :   2022/8.26
//
_lexical_info::seal_lexical_token _lexical_core::view_token()
{
	bool				   update_status_cache = lexical_update_line;
	seal_lexical_type_info kernel_cache		   = core_info;

	kernel_cache.lexical_index = core_info.lexical_index;
	kernel_cache.lexical_eof   = core_info.lexical_eof;
	kernel_cache.lexical_line  = core_info.lexical_line;

	kernel_cache.lexical_token.cache_token	= core_info.lexical_token.cache_token;
	kernel_cache.lexical_token.token_string = core_info.lexical_token.token_string;

	auto token_cache = get_token();

	core_info.lexical_index = kernel_cache.lexical_index;
	core_info.lexical_eof	= kernel_cache.lexical_eof;
	core_info.lexical_line	= kernel_cache.lexical_line;
	lexical_update_line		= update_status_cache;

	core_info.lexical_token.cache_token	 = kernel_cache.lexical_token.cache_token;
	core_info.lexical_token.token_string = kernel_cache.lexical_token.token_string;

	return token_cache;
}

//////////////////////////////////////////////////////////////////////////
//    view_token  :
//           @description :   Cut a token from code_string ( in view mode )
//           @birth       :   2022/8.26
//
_lexical_info::seal_lexical_token _lexical_core::view_token(int count)
{
	bool									   update_status_cache = lexical_update_line;
	seal_lexical_type_info					   kernel_cache		   = core_info;
	seal_lexical_type_info::seal_lexical_token token_cache;

	while (count != 0)
	{
		token_cache = get_token();

		--count;
	}

	core_info			= kernel_cache;
	lexical_update_line = update_status_cache;

	return token_cache;
}

//////////////////////////////////////////////////////////////////////////
//    is_eof  :
//           @description :   Whether it ends, if end, return true
//           @birth       :   2021/2.16
//
bool _lexical_core::is_eof()
{
	return core_info.lexical_eof;
}

//////////////////////////////////////////////////////////////////////////
//    get_index  :
//           @description :   Get index position
//           @birth       :   2021/3.5
//
se_int _lexical_core::get_index()
{
	return core_info.lexical_index;
}

//////////////////////////////////////////////////////////////////////////
//    get_line  :
//           @description :   Get line
//           @birth       :   2022/7.19
//
se_int _lexical_core::get_line()
{
	return core_info.lexical_line;
}

//////////////////////////////////////////////////////////////////////////
//    get_buffer  :
//           @description :   Get raw code
//           @birth       :   2022/12.21
//
std::wstring _lexical_core::get_buffer() const
{
	return core_info.lexical_code;
}

} // namespace VKits