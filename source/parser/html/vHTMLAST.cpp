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
 * \file vHTMLAST.cpp
 * \brief The HTML AST generator in VUILib
 */

#include <include/parser/HTML/vHTMLAST.h>

VHTMLASTMissingComparison::VHTMLASTMissingComparison(const OString &What, const int &Line, const int &Position) {
	_info = ostr::format("VHTMLAST Error : The label \"{}\" missing close label to be compared at {}:{}.", What, Line, Position);
	printf("%s", _info.c_str());
}
VHTMLASTNode::~VHTMLASTNode() {
	for (auto &object: _childNode) {
		delete object;
	}
}
bool VHTMLASTNode::ExistChild(const OString &Id) const {
	for (auto &object : _childNode) {
		if (object->_id == Id) {
			return true;
		}
	}

	return false;
}
const VHTMLASTNode* VHTMLASTNode::GetChild(const OString &Id) const {
	for (auto &object: _childNode) {
		if (object->_id == Id) {
			return object;
		}
	}

	return nullptr;
}
VHTMLAST::VHTMLAST(const OString &Code) : _lexer(Code) {
	GenerateAST();
}
VHTMLAST::~VHTMLAST() {
	delete _root;
}
VHTMLASTNode *VHTMLAST::GetRoot() const {
	return _root;
}
VHTMLASTNode *VHTMLAST::GenerateAST() {
	_root = new VHTMLASTNode;
	while (!_lexer.End()) {
		auto node = new VHTMLASTNode;

		_lexer.ExceptToken(VHTMLTokenType::LeftBracket);
		auto id = _lexer.ExceptToken(VHTMLTokenType::Id);
		node->_id = id.String;

		auto idLine         = _lexer.GetLine();
		auto idLinePosition = _lexer.GetLinePosition();

		std::unordered_map<OString, OString> property;
		bool slashEnd = false;
		while (!_lexer.End()) {
			auto token = _lexer.NextToken();
			if (token.Type == VHTMLTokenType::Slash) {
				slashEnd = true;
				break;
			}
			if (token.Type == VHTMLTokenType::RightBracket) {
				break;
			}
			if (token.Type != VHTMLTokenType::Id) {
				throw new VHTMLUnexpectedToken(token.String, _lexer.GetLine(), _lexer.GetLinePosition());
			}
			_lexer.ExceptToken(VHTMLTokenType::Equal);
			auto value = _lexer.ExceptToken(VHTMLTokenType::String);

			property.insert({ token.String, value.String });
		}

		node->Property = property;

		if (!slashEnd) {
			auto token = _lexer.NextToken();
			if (token.Type == VHTMLTokenType::Text) {
				node->Context = token.String;
				token         = _lexer.ExceptToken(VHTMLTokenType::LeftBracket);
			}

			OString context = token.String;
			int 	level   = 1;
			bool	inEnd	= false;
			std::vector<VHTMLLexerToken> tokenList;
			while (true) {
				token = _lexer.NextToken();
				if (token.Type == VHTMLTokenType::End) {
					throw new VHTMLASTMissingComparison(node->_id, idLine, idLinePosition);
				}
				if (token.Type == VHTMLTokenType::Slash) {
					--level;
					inEnd = true;

					if (level == 0) {
						if (!tokenList.empty()) {
							tokenList.erase(tokenList.end() - 1);
						}
						break;
					}
				}
				if (token.Type == VHTMLTokenType::RightBracket) {
					if (inEnd) {
						inEnd = false;
					}
					else {
						++level;
					}
				}

				tokenList.emplace_back(token);
			}

			for (auto &slice : tokenList) {
				context.append(slice.String);
			}

			_lexer.ExceptToken(VHTMLTokenType::Id);
			_lexer.ExceptToken(VHTMLTokenType::RightBracket);

			if (context != "<") {
				std::unique_ptr<VHTMLAST> AST = std::make_unique<VHTMLAST>(context);
				node->_childNode              = std::move(AST->_root->_childNode);
			}
		}

		_root->_childNode.emplace_back(node);
	}

	return _root;
}