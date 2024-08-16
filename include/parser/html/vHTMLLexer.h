/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file vHTMLLexer.h
 * \brief The lexer of HTML in VUILib HTML parser
 */

#pragma once

#include <string>
#include <format>

#include <include/base/vBase.h>
#include <include/parser/html/vHTMLEscapeCharacter.h>

/**
 * The exception for invalid token in HTML lexer, it provides a format
 * function for exception info and will directly print the information
 * on the console whether in debug mode or not
 */
class VHTMLLexerInvalidToken : public std::exception {
public:
	VHTMLLexerInvalidToken(const OString &What, const int &Line, const int &Position);

public:
	const char* what() const {
		return reinterpret_cast<const char*>(_info.c_str());
	}

private:
	OString _info;
};
/**
 * The exception for missing token in HTML lexer, it provides a format
 * function for exception info and will directly print the information
 * on the console whether in debug mode or not
 */
class VHTMLLexerMissingToken {
public:
	VHTMLLexerMissingToken(const OString &What, const int &Line, const int &Position);

public:
	const char* what() const {
		return reinterpret_cast<const char*>(_info.c_str());
	}

private:
	OString _info;
};
/**
 * The exception for missing token in HTML lexer, it provides a format
 * function for exception info and will directly print the information
 * on the console whether in debug mode or not
 */
class VHTMLUnexpectedToken {
public:
	VHTMLUnexpectedToken(const OString &What, const int &Line, const int &Position);

public:
	const char* what() const {
		return reinterpret_cast<const char*>(_info.c_str());
	}

private:
	OString _info;
};

/**
 * The token type of HTML language
 */
enum class VHTMLTokenType {
	Text, LeftBracket, RightBracket, Equal, String, Slash, End, Id, Invalid
};

/**
 * One single token type in HTML lexer
 */
struct VHTMLLexerToken {
	VHTMLTokenType  Type;
	OString			String;
};

/**
 * The lexer providing for HTML language
 */
class VHTMLLexer {
public:
	/**
	 * Construct the lexer with specified HTML code
	 * @param Code The HTML code in string format
	 */
	explicit VHTMLLexer(const OString &Code);

public:
	/**
	 * Get the current line of the lexer
	 * @return The line count of the lexer
	 */
	[[nodiscard]] int GetLine() const;
	/**
	 * Get the current line position of the lexer
	 * @return The line position of the lexer
	 */
	[[nodiscard]] int GetLinePosition() const;
	/**
	 * Get the current cursor of the lexer in code string
	 * @return The cursor of the lexer in code string
	 */
	[[nodiscard]] int GetCursor() const;
	/**
	 * Get the token with except type, if the NextToken function return a
	 * token that type was not excepted, this function will throw a VHTMLLexerUnexpectedToken
	 * error
	 * @param Type The type of token
	 * @return The token in except type
	 */
	VHTMLLexerToken ExceptToken(const VHTMLTokenType &Type);
	/**
	 * Get the next token of lexer
	 * @return The next token of the lexer
	 */
	VHTMLLexerToken NextToken();
	/**
	 * Get the current token of the lexer
	 * @return The current token of the lexer, this function won't step
	 */
	[[nodiscard]] VHTMLLexerToken SeekToken() const;
	/**
	 * Judge whether the lexer met the end of the code
	 * @return If true, the lexer meeting the end of the code, nor false
	 */
	[[nodiscard]] bool End() const;

private:
	/**
	 * Init the lexer status
	 */
	void InitLexer();

private:
	/**
	 * Fetch string in HTML
	 * @return The string token instance in VHTMLLexerToken
	 */
	VHTMLLexerToken FetchStringToken();
	/**
	 * Convert the escaping in string into real string
	 * @return The converted escaping string
	 */
	OString ProcessStringEscaping();

private:
	/**
	 * Check whether local position run out of the string range,
	 * if it is, a missing token exception will be thrown
	 */
	void _missTokenIfOutRange(const OString &What, const int &Line, const int &Position);
	/**
	 * Convert the string from specified radix to the decimal number
	 * @param string The origin string
	 * @param radix The source radix
	 * @return The number in decimal number converted from the radix
	 */
	static int _toDecimal(const OString &string, int radix) {
		int result = 0;
		for (int count = 0; count < string.size(); count++) {
			auto ch = char32_t(string[count]);

			if(ch >= '0' && ch <= '9') {
				result = result * radix + (ch - '0');
			}
			else {
				result = result * radix + (ch - 'a' + 10);
			}
		}

		return result;
	}
	/**
	 * Judge whether the specified is a empty string (Only contains blank character)
	 * @param String The specified string tobe judged
	 * @return If the string is empty, return true, nor false
	 */
	static bool _isEmptyString(OString &String) {
		for (auto Character : String) {
			auto codepoint = (char32_t )(Character.get_codepoint());
			if (codepoint == u'\0') {
				break;
			}
			if ((codepoint != u' ' && codepoint != u'\t') || (codepoint != u'\t' && codepoint != u' ')) {
				return false;
			}
		}

		String = "";
		return true;
	}

private:
	bool				_inSlash;
	bool				_inContext;
	OString 			_code;
	int 				_position;
	int 				_line;
	int 				_linePosition;
	VHTMLLexerToken		_localToken;
};