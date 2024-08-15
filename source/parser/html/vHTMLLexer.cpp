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
 * \file vHTMLLexer.cpp
 * \brief The lexer of HTML in VUILib HTML parser
 */

#include <include/parser/html/vHTMLLexer.h>

VHTMLLexerInvalidToken::VHTMLLexerInvalidToken(const OString &What, const int &Line, const int &Position) {
	_info = ostr::format("VHTMLLexer Syntax Error : Invalid token \"{}\" at {}:{}.", What, Line, Position);
	printf("%s", _info.c_str());
}
VHTMLLexerMissingToken::VHTMLLexerMissingToken(const OString &What, const int &Line, const int &Position) {
	_info = ostr::format("VHTMLLexer Syntax Error : Missing pairing token for \"{}\" at {}:{}.", What, Line, Position);
	printf("%s", _info.c_str());
}

VHTMLLexer::VHTMLLexer(const OString &Code) : _code(Code) {
	InitLexer();
}
void VHTMLLexer::InitLexer() {
	_position     = 0;
	_line         = 0;
	_linePosition = 0;
	_inSlash	  = false;
	_inContext	  = false;
}
VHTMLLexerToken VHTMLLexer::NextToken() {
	if (_position >= _code.size()) {
		return { .Type = VHTMLTokenType::End, .String = "" };
	}

	OString	 		string;
	VHTMLTokenType	type = VHTMLTokenType::Invalid;
	while (true) {
		auto character = char32_t(_code[_position]);
		switch (character) {
			case u8' ':
			case u8'\t': {
				if (_inContext) {
					type 	= VHTMLTokenType::Id;
					string += _code[_position];
				}
				else {
					if (type != VHTMLTokenType::Invalid) {
						return { .Type = type, .String = string };
					}
				}
				++_position;
				++_linePosition;

				break;
			}
			case u8'\"': {
				if (type != VHTMLTokenType::Invalid) {
					return { .Type = type, .String = string };
				}

				auto token = FetchStringToken();
				++_position;

				return token;
			}
			case u8'\n': {
				++_line;
				++_position;
				_linePosition = 0;

				if (!string.is_empty()) {
					return { .Type = type, .String = string };
				}

				break;
			}
			case u8'&': {
				OString escape = "&";
				while (true) {
					++_position;
					_missTokenIfOutRange("&", _line, _position);

					escape += _code[_position];
					if (_code[_position] == u8';') {
						break;
					}
				}

				string += VHTMLEscapeMap::EscapeConvert(escape);

				++_position;

				break;
			}
			case u8'<': {
				if (type != VHTMLTokenType::Invalid && !_isEmptyString(string)) {
					return { .Type = type, .String = string };
				}

				_inContext = false;

				++_position;

				return { .Type = VHTMLTokenType::LeftBracket, .String = "<" };
			}
			case u8'>': {
				if (type != VHTMLTokenType::Invalid && !_isEmptyString(string)) {
					return { .Type = type, .String = string };
				}

				if (_inSlash) {
					_inSlash = false;
				}
				else {
					_inContext = true;
				}

				++_position;

				return { .Type = VHTMLTokenType::RightBracket, .String = ">" };
			}
			case u8'/': {
				if (type != VHTMLTokenType::Invalid && !_isEmptyString(string)) {
					return { .Type = type, .String = string };
				}

				_inSlash = true;
				++_position;
				return { .Type = VHTMLTokenType::Slash, .String = "/" };
			}
			default: {
				type 	= VHTMLTokenType::Id;
				string += character;

				++_position;

				break;
			}
		}
	}
}
bool VHTMLLexer::End() {
	return _position >= _code.size();
}
VHTMLLexerToken VHTMLLexer::FetchStringToken() {
	int 		line     = _linePosition;
	int 		position = _position;
	OString  string;
	while (true) {
		_missTokenIfOutRange("\"", _line, _position);

		if (_code[_position] == u8'\"') {
			break;
		}
		if (_code[_position] == u8'\\') {
			string += ProcessStringEscaping();
		}
		else {
			string += _code[_position];
		}

		++_position;
	}

	return { .Type = VHTMLTokenType::String, .String = string };
}
OString VHTMLLexer::ProcessStringEscaping() {
	++_position;
	_missTokenIfOutRange("\\", _line, _position - 1);
	auto controlCharacter = char32_t(_code[_position]);

	switch (controlCharacter) {
		case 'a': {
			return "\a";
		}
		case 'n': {
			return "\n";
		}
		case 'b': {
			return "\b";
		}
		case 't': {
			return "\t";
		}
		case 'f': {
			return "\f";
		}
		case 'r': {
			return "\rf";
		}
		case 'v': {
			return "\v";
		}
		case 'u': {
			OString hexString;
			for (auto count = 0; count < 4; ++count) {
				++_position;
				_missTokenIfOutRange("Number of hex string", _line, _position - 1);

				hexString += _code[_position];
			}

			char32_t character = _toDecimal(hexString, 16);

			return ostr::codeunit_sequence(character);
		}
		case '\'': {
			return "'";
		}
		case '"': {
			return "\"";
		}
		case '\\': {
			return "\\";
		}
		default: {
			throw new VHTMLLexerInvalidToken(ostr::codeunit_sequence(controlCharacter), _position, _line);

			break;
		}
	}

	return "";
}
void VHTMLLexer::_missTokenIfOutRange(const OString &What, const int &Line, const int &Position) {
	if (_position >= _code.size()) {
		throw new VHTMLLexerMissingToken(What, Line, Position);
	}
}