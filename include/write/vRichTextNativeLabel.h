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
 * \file vRichTextNativeLabel.h
 * \brief The built-in native label for rich text renderer
 */

#pragma once

#include <include/base/geometry/vRect.h>
#include <include/parser/html/vHTMLAST.h>
#include <include/renderer/vColorFactory.h>
#include <include/renderer/vRendererBase.h>

/**
 * The context structure for rich text renderer
 */
struct VRichTextRendererContext {
	VRichTextRendererContext(const VRichTextRendererContext &Object) = default;
	VRichTextRendererContext(std::vector<SkScalar> &Width, std::vector<SkScalar> &Height)
		: WidthCollections(Width), HeightCollections(Height) {
	}
	VRichTextRendererContext &operator=(const VRichTextRendererContext &Object) {
		X				  = Object.X;
		Y				  = Object.Y;
		Size			  = Object.Size;
		WordSpace		  = Object.WordSpace;
		LineSpace		  = Object.LineSpace;
		Bound			  = Object.Bound;
		EnglishFont		  = Object.EnglishFont;
		OtherFont		  = Object.OtherFont;
		MaxHeight		  = Object.MaxHeight;
		Color			  = Object.Color;
		WidthCollections  = Object.WidthCollections;
		HeightCollections = Object.HeightCollections;
		TempWidth		  = Object.TempWidth;

		return *this;
	}
	~VRichTextRendererContext() = default;

	SkScalar X				= 0;
	SkScalar Y				= 0;
	SkScalar Size			= 0;
	SkScalar WordSpace		= 0;
	SkScalar TempWidth		= 0;
	SkScalar LineSpace		= 0;
	SkScalar BaseLineHeight = 0;
	VRect	 Bound{};
	SkFont	 EnglishFont{};
	SkFont	 OtherFont{};
	SkColor	 Color{};
	SkScalar MaxHeight = 0;
	int		 LineCount = 0;

	std::vector<SkScalar> &WidthCollections;
	std::vector<SkScalar> &HeightCollections;
};

class VRichTextLabelRendererInterface;

using VRichTextLabelInterfaceManager = std::unordered_map<OString, VRichTextLabelRendererInterface *>;

/**
 * The interface base class for rich text label rendering
 */
class VRichTextLabelRendererInterface {
public:
	/**
	 * The interface for measure the style function of the label
	 * @param Node The node to be measured
	 * @param Context The old context
	 * @return The new context
	 */
	virtual VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) = 0;
	/**
	 * Measure the context by pre-rendering
	 * @param Node The node to be measured
	 * @param Context The old context
	 * @param Manager THe manager of the label interface
	 * @return The new context
	 */
	virtual std::optional<VRichTextRendererContext> WidthMeasure(VHTMLASTNode					*Node,
																 const VRichTextRendererContext &Context,
																 VRichTextLabelInterfaceManager &Manager) = 0;
	/**
	 * Render on the specified canvas
	 * @param Node The node to be rendered
	 * @param Canvas The specified canvas to be rendered on
	 * @param Context The context of the renderer
	 * @param Manager THe manager of the label interface
	 * @return The new context
	 */
	virtual VRichTextRendererContext Render(VHTMLASTNode *Node, SkCanvas *Canvas, VRichTextRendererContext &Context,
											VRichTextLabelInterfaceManager &Manager) = 0;
	/**
	 * Get the id of the label
	 * @return The id of the label
	 */
	virtual OString GetID() = 0;
};

/**
 * The general renderer of the rich text, which overriding the render method
 * as the default render method
 */
class VRichTextGeneralRenderer : public VRichTextLabelRendererInterface {
public:
	VRichTextRendererContext Render(VHTMLASTNode *Node, SkCanvas *Canvas, VRichTextRendererContext &Context,
									VRichTextLabelInterfaceManager &Manager) override;
	std::optional<VRichTextRendererContext> WidthMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context,
														 VRichTextLabelInterfaceManager &Manager) override;
};
class VRichDefaultLabel : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override {
		return Context;
	}

public:
	OString GetID() override {
		return "$Default";
	}
};
class VRichTextH1Label : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override;

public:
	OString GetID() override {
		return "h1";
	}
};
class VRichTextH2Label : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override;

public:
	OString GetID() override {
		return "h2";
	}
};
class VRichTextH3Label : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override;

public:
	OString GetID() override {
		return "h3";
	}
};
class VRichTextH4Label : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override;

public:
	OString GetID() override {
		return "h4";
	}
};
class VRichTextULabel : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext Render(VHTMLASTNode *Node, SkCanvas *Canvas, VRichTextRendererContext &Context,
									VRichTextLabelInterfaceManager &Manager) override;
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override {
		return Context;
	}

public:
	OString GetID() override {
		return "u";
	}
};
class VRichTextStrikeLabel : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext Render(VHTMLASTNode *Node, SkCanvas *Canvas, VRichTextRendererContext &Context,
									VRichTextLabelInterfaceManager &Manager) override;
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override {
		return Context;
	}

public:
	OString GetID() override {
		return "strike";
	}
};
class VRichTextBRLabel : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override;

public:
	OString GetID() override {
		return "br";
	}
};
class VRichTextBoldLabel : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override;

public:
	OString GetID() override {
		return "bold";
	}
};
class VRichTextItalicLabel : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override;

public:
	OString GetID() override {
		return "italic";
	}
};
class VRichTextBoldItalicLabel : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override;

public:
	OString GetID() override {
		return "bold-italic";
	}
};
class VRichTextFontLabel : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override;

public:
	OString GetID() override {
		return "font";
	}
};
class VRichTextCenterLabel : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override;

public:
	OString GetID() override {
		return "center";
	}
};
class VRichTextLeftLabel : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override;

public:
	OString GetID() override {
		return "left";
	}
};
class VRichTextRightLabel : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override;

public:
	OString GetID() override {
		return "right";
	}
};
class VRichTextVCenterLabel : public VRichTextGeneralRenderer {
public:
	VRichTextRendererContext ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) override;

public:
	OString GetID() override {
		return "vcenter";
	}
};