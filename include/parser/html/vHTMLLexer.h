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

#include <OpenString-CMake/include/text.h>
#include <OpenString-CMake/include/format.h>

/**
 * The exception for invalid token in HTML lexer, it provides a format
 * function for exception info and will directly print the information
 * on the console whether in debug mode or not
 */
class VHTMLLexerInvalidToken : public std::exception {
public:
	VHTMLLexerInvalidToken(const ostr::text &What, const int &Line, const int &Position);

public:
	const char* what() const {
		return reinterpret_cast<const char*>(_info.c_str());
	}

private:
	ostr::text _info;
};
/**
 * The exception for missing token in HTML lexer, it provides a format
 * function for exception info and will directly print the information
 * on the console whether in debug mode or not
 */
class VHTMLLexerMissingToken {
public:
	VHTMLLexerMissingToken(const ostr::text &What, const int &Line, const int &Position);

public:
	const char* what() const {
		return reinterpret_cast<const char*>(_info.c_str());
	}

private:
	ostr::text _info;
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
	ostr::text_view	String;
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
	explicit VHTMLLexer(const ostr::text &Code);

public:
	/**
	 * Get the next token of lexer
	 * @return The next token of the lexer
	 */
	VHTMLLexerToken NextToken();

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
	ostr::text ProcessStringEscaping();

private:
	/**
	 * Check whether local position run out of the string range,
	 * if it is, a missing token exception will be thrown
	 */
	void MissTokenIfOutRange(const ostr::text &What, const int &Line, const int &Position);

private:
	ostr::text 			_code;
	int 				_position;
	int 				_line;
	int 				_linePosition;
	VHTMLLexerToken		_localToken;
};