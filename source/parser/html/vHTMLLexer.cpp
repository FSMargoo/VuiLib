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
VHTMLUnexpectedToken::VHTMLUnexpectedToken(const OString &What, const int &Line, const int &Position) {
	_info = ostr::format("VHTMLLexer Syntax Error : Unexpected token for \"{}\" at {}:{}.", What, Line, Position);
	printf("%s", _info.c_str());
}

VHTMLLexer::VHTMLLexer(const OString &Code) : _code(Code) {
	InitLexer();
}
void VHTMLLexer::InitLexer() {
	_position	   = 0;
	_line		   = 0;
	_linePosition  = 0;
	_inSlash	   = false;
	_inContext	   = false;
	_inLabelLexer  = false;
	_ignoreCasting = false;
}
int VHTMLLexer::GetLine() const {
	return _linePosition;
}
int VHTMLLexer::GetLinePosition() const {
	return _linePosition;
}
int VHTMLLexer::GetCursor() const {
	return _position;
}
VHTMLLexerToken VHTMLLexer::ExceptToken(const VHTMLTokenType &Type) {
	auto token = NextToken();
	if (token.Type != Type) {
		throw VHTMLUnexpectedToken(token.String, _line, _linePosition);
	}

	return token;
}
VHTMLLexerToken VHTMLLexer::NextToken() {
	if (_position >= _code.size()) {
		return {.Type = VHTMLTokenType::End, .String = ""};
	}

	OString		   string;
	VHTMLTokenType type = VHTMLTokenType::Invalid;
	while (true) {
		auto character = char32_t(_code[_position]);
		switch (character) {
		case u8'|': {
			if (type != VHTMLTokenType::Invalid) {
				_localToken = {.Type = type, .String = string};
				return _localToken;
			}

			++_position;

			return {.Type = VHTMLTokenType::Split, .String = "|"};
		}
		case u8' ':
		case u8'\t': {
			++_position;
			break;
		}
		case u8'\"': {
			if (type != VHTMLTokenType::Invalid) {
				_localToken = {.Type = type, .String = string};
				return _localToken;
			}

			++_position;
			_localToken = FetchStringToken();

			++_position;

			return _localToken;
		}
		case u8'\n': {
			++_line;
			++_position;
			_linePosition = 0;

			break;
		}
		case u8'&': {
			OString escape = "&";
			while (true) {
				++_position;
				_missTokenIfOutRange("&", _line, _linePosition);

				escape += _code[_position];
				if (_code[_position] == u8';') {
					break;
				}
			}

			if (!_ignoreCasting) {
				string += VHTMLEscapeMap::EscapeConvert(escape);
			} else {
				string += escape;
			};

			++_position;
			++_linePosition;

			break;
		}
		case u8'<': {
			if (type != VHTMLTokenType::Invalid && !_isEmptyString(string)) {
				_localToken = {.Type = type, .String = string};
				return _localToken;
			}

			_inLabelLexer = true;
			_inContext	  = false;

			++_position;
			++_linePosition;

			_localToken = {.Type = VHTMLTokenType::LeftBracket, .String = "<"};
			return _localToken;
		}
		case u8'>': {
			if (type != VHTMLTokenType::Invalid && !_isEmptyString(string)) {
				_localToken = {.Type = type, .String = string};
				return _localToken;
			}

			_inLabelLexer = false;
			if (_inSlash) {
				_inSlash = false;
			} else {
				_inContext = true;
			}

			++_position;
			++_linePosition;

			_localToken = {.Type = VHTMLTokenType::RightBracket, .String = ">"};
			return _localToken;
		}
		case u8'=': {
			if (!_inLabelLexer) {
				if (_inContext) {
					type = VHTMLTokenType::Text;
				} else {
					type = VHTMLTokenType::Id;
				}
				string += character;

				++_position;

				break;
			}

			if (type != VHTMLTokenType::Invalid && !_isEmptyString(string)) {
				_localToken = {.Type = type, .String = string};
				return _localToken;
			}

			++_position;
			++_linePosition;

			_localToken = {.Type = VHTMLTokenType::Equal, .String = "="};
			return _localToken;
		}
		case u8'/': {
			if (!_inLabelLexer) {
				if (_inContext) {
					type = VHTMLTokenType::Text;
				} else {
					type = VHTMLTokenType::Id;
				}
				string += character;

				++_position;

				break;
			}

			if (type != VHTMLTokenType::Invalid && !_isEmptyString(string)) {
				_localToken = {.Type = type, .String = string};
				return _localToken;
			}

			_inSlash = true;
			++_position;
			++_linePosition;

			_localToken = {.Type = VHTMLTokenType::Slash, .String = "/"};
			return _localToken;
		}
		default: {
			if (_inContext) {
				type = VHTMLTokenType::Text;
			} else {
				type = VHTMLTokenType::Id;
			}
			string += character;

			++_position;
			++_linePosition;

			break;
		}
		}
	}
}
VHTMLLexerToken VHTMLLexer::SeekToken() const {
	return _localToken;
}
bool VHTMLLexer::End() const {
	return _position >= _code.size();
}
void VHTMLLexer::SetCastingIgnore(const bool &Status) {
	_ignoreCasting = Status;
}
VHTMLLexerToken VHTMLLexer::FetchStringToken() {
	int		line	 = _linePosition;
	int		position = _position;
	OString string	 = "\"";
	while (true) {
		_missTokenIfOutRange("\"", _line, _linePosition);

		if (_code[_position] == u8'\"') {
			string += _code[_position];
			break;
		}
		if (_code[_position] == u8'\\') {
			string += ProcessStringEscaping();
		} else {
			string += _code[_position];
		}

		++_position;
	}

	return {.Type = VHTMLTokenType::String, .String = string};
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
		throw new VHTMLLexerInvalidToken(ostr::codeunit_sequence(controlCharacter), _position, _linePosition);

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