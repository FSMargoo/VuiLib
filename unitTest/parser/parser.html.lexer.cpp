﻿/*
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
 * \file parser.html.lexer.cpp
 * \brief The HTML parser lexer test
 */

#include <unitTest/parser/parser.html.lexer.h>

#include <fstream>

bool VUnitTest(HTMLParser, Lexer) {
	VHTMLLexer lexer(R"(<html>
<head>
    <title>My First Web Page</title>
</head>
<body>
    <h1>Hello, World!</h1>
</body>
</html>)");
	OString result;
	while (!lexer.End()) {
		auto token = lexer.NextToken();
		token.String = ostr::format("\"{}\",\n", token.String);
		result += token.String;
	}

	return result == R"("<",
"html",
">",
"<",
"head",
">",
"<",
"title",
">",
"My First Web Page",
"<",
"/",
"title",
">",
"<",
"/",
"head",
">",
"<",
"body",
">",
"<",
"h1",
">",
"Hello, World!",
"<",
"/",
"h1",
">",
"<",
"/",
"body",
">",
"<",
"/",
"html",
">",
)";
}
