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
 * \file vHTMLAST.h
 * \brief The HTML AST generator in VUILib
 */

#pragma once

#include <include/parser/HTML/vHTMLLexer.h>

/**
 * The exception for missing comparison in HTML AST, it provides a format
 * function for exception info and will directly print the information
 * on the console whether in debug mode or not
 */
class VHTMLASTMissingComparison : public std::exception {
public:
	VHTMLASTMissingComparison(const OString &What, const int &Line, const int &Position);

public:
	const char *what() const {
		return reinterpret_cast<const char *>(_info.c_str());
	}

private:
	OString _info;
};

/**
 * The tree node of HTML AST
 */
class VHTMLASTNode {
public:
	/**
	 * The default construction of AST node
	 */
	VHTMLASTNode() = default;
	~VHTMLASTNode();

public:
	/**
	 * Judge whether this node exist a child node with specified ID
	 * @param Id The specified ID of the child node
	 * @return If the return value is true, it's existing, nor not exist
	 */
	[[nodiscard]] bool ExistChild(const OString &Id) const;
	/**
	 * Get the child with specified ID
	 * @param Id The specified ID of the child node
	 * @return If the child dose not existing, it will return nullptr, nor
	 * returning the readonly pointer to children
	 */
	[[nodiscard]] std::optional<const VHTMLASTNode*> GetChild(const OString &Id) const;
	/**
	 * Get the beginning iterator of the children list
	 * @return The beginning iterator
	 */
	[[nodiscard]] auto begin() const {
		return _childNode.begin();
	}
	/**
	 * Get the end iterator of the children list
	 * @return The end iterator
	 */
	[[nodiscard]] auto end() const {
		return _childNode.end();
	}
	/**
	 * Get the id of the HTML AST node
	 * @return The id of the HTML AST node
	 */
	[[nodiscard]] const OString GetId() const {
		return _id;
	}

public:
	/**
	 * The property of the node
	 */
	std::unordered_map<OString, OString> Property;
	/**
	 * The context of the node
	 */
	OString Context;

private:
	friend class VHTMLAST;

private:
	std::vector<VHTMLASTNode *> _childNode;
	OString						_id;
};

/**
 * The AST generator of HTML in VUILib
 */
class VHTMLAST {
public:
	/**
	 * Construct the AST generator by the HTML code
	 * @param Code The code of HTML
	 */
	explicit VHTMLAST(const OString &Code);
	~VHTMLAST();

public:
	/**
	 * Get the root node of the AST tree
	 * @return The pointer referred to root node
	 */
	[[nodiscard]] VHTMLASTNode *GetRoot() const;

private:
	/**
	 * Generate the AST
	 * @param Parent The parent node of the AST
	 * @return The root ast node pointer
	 */
	VHTMLASTNode *GenerateAST();

private:
	VHTMLASTNode *_root;
	VHTMLLexer	  _lexer;
};