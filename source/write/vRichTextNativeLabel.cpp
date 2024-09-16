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
 * \file vRichTextNativeLabel.cpp
 * \brief The built-in native label for rich text renderer
 */

#include <include/write/vRichTextNativeLabel.h>

constexpr SkScalar VRichTextVarH1Size = 40;
constexpr SkScalar VRichTextVarH2Size = 32;
constexpr SkScalar VRichTextVarH3Size = 26;
constexpr SkScalar VRichTextVarH4Size = 18;

std::optional<VRichTextRendererContext> VRichTextGeneralRenderer::WidthMeasure(
	VHTMLASTNode *Node, const VRichTextRendererContext &Context, VRichTextLabelInterfaceManager &Manager) {
	auto			  context		= Context;
	sk_sp<SkTypeface> enTypeFace	= Context.EnglishFont.refTypeface();
	sk_sp<SkTypeface> otherTypeFace = Context.OtherFont.refTypeface();

	context.EnglishFont.setSize(context.Size);
	context.OtherFont.setSize(context.Size);

	context.MaxHeight = std::max(context.Size, context.MaxHeight);

	for (auto &node : *Node) {
		if (node->GetId() == "$context") {
			for (auto character : node->Context) {
				if (context.Y >= context.Bound.GetBottom()) {
					context.WidthCollections.push_back(context.X - context.Bound.GetLeft());
					context.HeightCollections.push_back(context.MaxHeight);

					return {};
				}

				auto codePoint = (char32_t)character.get_codepoint();
				if (codePoint == U'\0') {
					break;
				}
				if (codePoint == U'\n' || codePoint == U'\t') {
					context.Y += context.Size;
					context.WidthCollections.push_back(context.X - context.Bound.GetLeft());
					context.HeightCollections.push_back(context.MaxHeight);
					context.X = context.Bound.GetLeft();

					continue;
				}

				SkFont font;
				if ((codePoint >= U'a' && codePoint <= U'z') || (codePoint >= U'A' && codePoint <= U'Z')) {
					font = context.EnglishFont;
				} else {
					font = context.OtherFont;
				}

				auto marchX = context.X + font.measureText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32);
				if (marchX >= context.Bound.GetRight()) {
					context.WidthCollections.push_back(context.X - context.Bound.GetLeft());
					context.HeightCollections.push_back(context.MaxHeight);
					context.MaxHeight = context.Size;

					context.X = context.Bound.GetLeft();
					context.Y += context.Size + context.LineSpace;
				}

				context.X +=
					font.measureText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32) + context.WordSpace;
			}

			continue;
		}

		std::optional<VRichTextRendererContext> value;
		if (Manager.contains(node->GetId())) {
			auto nextLevelContext = Manager[node->GetId()]->ContextMeasure(node, context);
			value				  = Manager[node->GetId()]->WidthMeasure(node, nextLevelContext, Manager);
		} else {
			auto nextLevelContext = Manager["$Default"]->ContextMeasure(node, context);
			value				  = Manager["$Default"]->WidthMeasure(node, nextLevelContext, Manager);
		}
		if (!value.has_value()) {
			return {};
		}

		auto height		  = value.value().MaxHeight;
		context.X		  = value.value().X;
		context.Y		  = value.value().Y;
		context.LineCount = value.value().LineCount;

		context.MaxHeight = std::max(height, context.MaxHeight);
	}

	return context;
}
VRichTextRendererContext VRichTextGeneralRenderer::Render(VHTMLASTNode *Node, SkCanvas *Canvas,
														  VRichTextRendererContext		 &Context,
														  VRichTextLabelInterfaceManager &Manager) {
	auto			  context		= Context;
	sk_sp<SkTypeface> enTypeFace	= Context.EnglishFont.refTypeface();
	sk_sp<SkTypeface> otherTypeFace = Context.OtherFont.refTypeface();

	context.BaseLineHeight = context.HeightCollections[context.LineCount];

	context.EnglishFont.setSize(context.Size);
	context.OtherFont.setSize(context.Size);

	SkPaint textColor;
	textColor.setColor(context.Color);

	for (auto &node : *Node) {
		if (node->GetId() == "$context") {
			for (auto character : node->Context) {
				if (context.Y >= context.Bound.GetBottom()) {
					break;
				}

				auto codePoint = (char32_t)character.get_codepoint();
				if (codePoint == U'\0') {
					break;
				}
				if (codePoint == U'\n' || codePoint == U'\t') {
					context.Y += context.BaseLineHeight + context.LineSpace;

					context.X		  = 0;
					context.TempWidth = context.Bound.GetLeft();

					++context.LineCount;
					continue;
				}

				SkFont font;
				if ((codePoint >= U'a' && codePoint <= U'z') || (codePoint >= U'A' && codePoint <= U'Z')) {
					font = context.EnglishFont;
				} else {
					font = context.OtherFont;
				}

				auto testX = context.X + font.measureText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32) +
							 context.WordSpace;
				if (testX >= context.Bound.GetRight()) {
					context.X		  = context.Bound.GetLeft();
					context.TempWidth = context.Bound.GetLeft();
					context.Y += context.BaseLineHeight + context.LineSpace;
					++context.LineCount;
				}

				Canvas->drawSimpleText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32, context.X,
									   context.Y + context.BaseLineHeight, font, textColor);

				auto fontMeasure =
					font.measureText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32) + context.WordSpace;
				context.TempWidth += fontMeasure;
				context.X += fontMeasure;
			}

			continue;
		}

		if (Manager.contains(node->GetId())) {
			auto nextLevelContext = Manager[node->GetId()]->ContextMeasure(node, context);
			nextLevelContext	  = Manager[node->GetId()]->Render(node, Canvas, nextLevelContext, Manager);

			context.X		  = nextLevelContext.X;
			context.Y		  = nextLevelContext.Y;
			context.LineCount = nextLevelContext.LineCount;
			context.MaxHeight = nextLevelContext.MaxHeight;
			context.TempWidth = nextLevelContext.TempWidth;
		} else {
			auto nextLevelContext = Manager["$Default"]->ContextMeasure(node, context);
			nextLevelContext	  = Manager["$Default"]->Render(node, Canvas, nextLevelContext, Manager);

			context.X		  = nextLevelContext.X;
			context.Y		  = nextLevelContext.Y;
			context.LineCount = nextLevelContext.LineCount;
			context.MaxHeight = nextLevelContext.MaxHeight;
			context.TempWidth = nextLevelContext.TempWidth;
		}
	}

	return context;
}
VRichTextRendererContext VRichTextH1Label::ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) {
	VRichTextRendererContext context = Context;
	context.Size					 = VRichTextVarH1Size;

	return context;
}
VRichTextRendererContext VRichTextH2Label::ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) {
	VRichTextRendererContext context = Context;
	context.Size					 = VRichTextVarH2Size;

	return context;
}
VRichTextRendererContext VRichTextH3Label::ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) {
	VRichTextRendererContext context = Context;
	context.Size					 = VRichTextVarH3Size;

	return context;
}
VRichTextRendererContext VRichTextH4Label::ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) {
	VRichTextRendererContext context = Context;
	context.Size					 = VRichTextVarH4Size;

	return context;
}
VRichTextRendererContext VRichTextULabel::Render(VHTMLASTNode *Node, SkCanvas *Canvas, VRichTextRendererContext &Context,
								VRichTextLabelInterfaceManager &Manager) {
	auto			  context		= Context;
	sk_sp<SkTypeface> enTypeFace	= Context.EnglishFont.refTypeface();
	sk_sp<SkTypeface> otherTypeFace = Context.OtherFont.refTypeface();

	context.BaseLineHeight = context.HeightCollections[context.LineCount];

	context.EnglishFont.setSize(context.Size);
	context.OtherFont.setSize(context.Size);

	SkColor underline = context.Color;
	SkScalar underlineBorder = 1;
	if (Node->Property.contains("color")) {
		underline = VColorFactory::MakeFromHexString(Node->Property["color"]);
	}
	if (Node->Property.contains("border")) {
		underlineBorder = atoi(Node->Property["border"].c_str());
	}

	SkPaint underlineColor;
	SkPaint textColor;
	underlineColor.setColor(underline);
	underlineColor.setStrokeWidth(underlineBorder);
	textColor.setColor(context.Color);

	for (auto &node : *Node) {
		if (node->GetId() == "$context") {
			auto startX = context.X;
			for (auto character : node->Context) {
				if (context.Y >= context.Bound.GetBottom()) {
					break;
				}

				auto codePoint = (char32_t)character.get_codepoint();
				if (codePoint == U'\0') {
					break;
				}
				if (codePoint == U'\n' || codePoint == U'\t') {
					Canvas->drawLine(startX, context.Y + context.BaseLineHeight, context.X, context.Y + context.BaseLineHeight, underlineColor);

					context.Y += context.BaseLineHeight + context.LineSpace;

					context.X		  = 0;
					context.TempWidth = context.Bound.GetLeft();

					++context.LineCount;

					startX = context.X;

					continue;
				}

				SkFont font;
				if ((codePoint >= U'a' && codePoint <= U'z') || (codePoint >= U'A' && codePoint <= U'Z')) {
					font = context.EnglishFont;
				} else {
					font = context.OtherFont;
				}

				auto testX = context.X + font.measureText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32) +
							 context.WordSpace;
				if (testX >= context.Bound.GetRight()) {
					context.X		  = context.Bound.GetLeft();
					context.TempWidth = context.Bound.GetLeft();
					context.Y += context.BaseLineHeight + context.LineSpace;
					++context.LineCount;
				}

				Canvas->drawSimpleText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32, context.X,
									   context.Y + context.BaseLineHeight, font, textColor);

				auto fontMeasure =
					font.measureText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32) + context.WordSpace;
				context.TempWidth += fontMeasure;
				context.X += fontMeasure;
			}

			Canvas->drawLine(startX, context.Y + context.BaseLineHeight, context.X, context.Y + context.BaseLineHeight, underlineColor);

			continue;
		}

		if (Manager.contains(node->GetId())) {
			auto nextLevelContext = Manager[node->GetId()]->ContextMeasure(node, context);
			nextLevelContext	  = Manager[node->GetId()]->Render(node, Canvas, nextLevelContext, Manager);

			context.X		  = nextLevelContext.X;
			context.Y		  = nextLevelContext.Y;
			context.LineCount = nextLevelContext.LineCount;
			context.MaxHeight = nextLevelContext.MaxHeight;
			context.TempWidth = nextLevelContext.TempWidth;
		} else {
			auto nextLevelContext = Manager["$Default"]->ContextMeasure(node, context);
			nextLevelContext	  = Manager["$Default"]->Render(node, Canvas, nextLevelContext, Manager);

			context.X		  = nextLevelContext.X;
			context.Y		  = nextLevelContext.Y;
			context.LineCount = nextLevelContext.LineCount;
			context.MaxHeight = nextLevelContext.MaxHeight;
			context.TempWidth = nextLevelContext.TempWidth;
		}
	}

	return context;
}
VRichTextRendererContext VRichTextStrikeLabel::Render(VHTMLASTNode *Node, SkCanvas *Canvas, VRichTextRendererContext &Context,
												 VRichTextLabelInterfaceManager &Manager) {
	auto			  context		= Context;
	sk_sp<SkTypeface> enTypeFace	= Context.EnglishFont.refTypeface();
	sk_sp<SkTypeface> otherTypeFace = Context.OtherFont.refTypeface();

	context.BaseLineHeight = context.HeightCollections[context.LineCount];

	context.EnglishFont.setSize(context.Size);
	context.OtherFont.setSize(context.Size);

	SkColor underline = context.Color;
	SkScalar underlineBorder = 1;
	if (Node->Property.contains("color")) {
		underline = VColorFactory::MakeFromHexString(Node->Property["color"]);
	}
	if (Node->Property.contains("border")) {
		underlineBorder = atoi(Node->Property["border"].c_str());
	}

	SkPaint underlineColor;
	SkPaint textColor;
	underlineColor.setColor(underline);
	underlineColor.setStrokeWidth(underlineBorder);
	textColor.setColor(context.Color);

	for (auto &node : *Node) {
		if (node->GetId() == "$context") {
			for (auto character : node->Context) {
				auto startX = context.X;
				auto y = context.Y + context.BaseLineHeight - context.Size / 3;
				if (context.Y >= context.Bound.GetBottom()) {
					break;
				}

				auto codePoint = (char32_t)character.get_codepoint();
				if (codePoint == U'\0') {
					break;
				}
				if (codePoint == U'\n' || codePoint == U'\t') {
					Canvas->drawLine(startX, context.Y + context.BaseLineHeight / 2, context.X, context.Y + context.BaseLineHeight / 2, underlineColor);

					context.Y += context.BaseLineHeight + context.LineSpace;

					context.X		  = 0;
					context.TempWidth = context.Bound.GetLeft();

					++context.LineCount;

					continue;
				}

				SkFont font;
				if ((codePoint >= U'a' && codePoint <= U'z') || (codePoint >= U'A' && codePoint <= U'Z')) {
					font = context.EnglishFont;
				} else {
					font = context.OtherFont;
				}

				auto testX = context.X + font.measureText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32) +
							 context.WordSpace;
				if (testX >= context.Bound.GetRight()) {
					context.X		  = context.Bound.GetLeft();
					context.TempWidth = context.Bound.GetLeft();
					context.Y += context.BaseLineHeight + context.LineSpace;
					++context.LineCount;
				}

				Canvas->drawSimpleText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32, context.X,
									   context.Y + context.BaseLineHeight, font, textColor);

				auto fontMeasure =
					font.measureText(&codePoint, sizeof(codePoint), SkTextEncoding::kUTF32) + context.WordSpace;
				context.TempWidth += fontMeasure;
				context.X += fontMeasure;

				Canvas->drawLine(startX, y, context.X, y, underlineColor);
			}

			continue;
		}

		if (Manager.contains(node->GetId())) {
			auto nextLevelContext = Manager[node->GetId()]->ContextMeasure(node, context);
			nextLevelContext	  = Manager[node->GetId()]->Render(node, Canvas, nextLevelContext, Manager);

			context.X		  = nextLevelContext.X;
			context.Y		  = nextLevelContext.Y;
			context.LineCount = nextLevelContext.LineCount;
			context.MaxHeight = nextLevelContext.MaxHeight;
			context.TempWidth = nextLevelContext.TempWidth;
		} else {
			auto nextLevelContext = Manager["$Default"]->ContextMeasure(node, context);
			nextLevelContext	  = Manager["$Default"]->Render(node, Canvas, nextLevelContext, Manager);

			context.X		  = nextLevelContext.X;
			context.Y		  = nextLevelContext.Y;
			context.LineCount = nextLevelContext.LineCount;
			context.MaxHeight = nextLevelContext.MaxHeight;
			context.TempWidth = nextLevelContext.TempWidth;
		}
	}

	return context;
}
VRichTextRendererContext VRichTextBRLabel::ContextMeasure(VHTMLASTNode *Node, const VRichTextRendererContext &Context) {
	VRichTextRendererContext context = Context;

	context.Y += context.BaseLineHeight + context.LineSpace;
	context.X		  = context.Bound.GetLeft();
	context.TempWidth = 0;
	++context.LineCount;

	return context;
}
VRichTextRendererContext VRichTextBoldLabel::ContextMeasure(VHTMLASTNode				   *Node,
															const VRichTextRendererContext &Context) {
	VRichTextRendererContext context = Context;

	SkString familyName;
	SkString otherFamilyName;
	context.EnglishFont.getTypeface()->getFamilyName(&familyName);
	context.OtherFont.getTypeface()->getFamilyName(&otherFamilyName);
	SkFontStyle fontStyle =
		SkFontStyle(SkFontStyle::kBold_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kUpright_Slant);

	auto enTypeFace	   = SkTypeface::MakeFromName(familyName.c_str(), fontStyle);
	auto otherTypeFace = SkTypeface::MakeFromName(otherFamilyName.c_str(), fontStyle);

	context.EnglishFont.setTypeface(std::move(enTypeFace));
	context.OtherFont.setTypeface(std::move(otherTypeFace));

	return context;
}
VRichTextRendererContext VRichTextItalicLabel::ContextMeasure(VHTMLASTNode					 *Node,
															  const VRichTextRendererContext &Context) {
	VRichTextRendererContext context = Context;

	SkString familyName;
	SkString otherFamilyName;
	context.EnglishFont.getTypeface()->getFamilyName(&familyName);
	context.OtherFont.getTypeface()->getFamilyName(&otherFamilyName);
	SkFontStyle fontStyle =
		SkFontStyle(SkFontStyle::kNormal_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kItalic_Slant);

	auto enTypeFace	   = SkTypeface::MakeFromName(familyName.c_str(), fontStyle);
	auto otherTypeFace = SkTypeface::MakeFromName(otherFamilyName.c_str(), fontStyle);

	context.EnglishFont.setTypeface(std::move(enTypeFace));
	context.OtherFont.setTypeface(std::move(otherTypeFace));

	return context;
}
VRichTextRendererContext VRichTextBoldItalicLabel::ContextMeasure(VHTMLASTNode					 *Node,
																  const VRichTextRendererContext &Context) {
	VRichTextRendererContext context = Context;

	SkString familyName;
	SkString otherFamilyName;
	context.EnglishFont.getTypeface()->getFamilyName(&familyName);
	context.OtherFont.getTypeface()->getFamilyName(&otherFamilyName);
	SkFontStyle fontStyle =
		SkFontStyle(SkFontStyle::kBold_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kItalic_Slant);

	auto enTypeFace	   = SkTypeface::MakeFromName(familyName.c_str(), fontStyle);
	auto otherTypeFace = SkTypeface::MakeFromName(otherFamilyName.c_str(), fontStyle);

	context.EnglishFont.setTypeface(std::move(enTypeFace));
	context.OtherFont.setTypeface(std::move(otherTypeFace));

	return context;
}
VRichTextRendererContext VRichTextFontLabel::ContextMeasure(VHTMLASTNode				   *Node,
															const VRichTextRendererContext &Context) {
	VRichTextRendererContext context = Context;

	auto newSize		= Node->Property.find("size");
	auto newColor		= Node->Property.find("color");
	auto newFace		= Node->Property.find("face");
	auto newEnglishFace = Node->Property.find("en-face");
	auto newOtherFace	= Node->Property.find("otr-face");

	auto enFontStyle	= context.EnglishFont.getTypeface()->fontStyle();
	auto otherFontStyle = context.OtherFont.getTypeface()->fontStyle();
	if (newSize != Node->Property.end()) {
		context.Size = atoi(newSize->second.c_str());
	}
	if (newColor != Node->Property.end()) {
		context.Color = VColorFactory::MakeFromHexString(newColor->second);
	}
	if (newFace != Node->Property.end()) {
		auto enTypeFace	   = SkTypeface::MakeFromName(newFace->second.c_str(), enFontStyle);
		auto otherTypeFace = SkTypeface::MakeFromName(newFace->second.c_str(), otherFontStyle);

		context.EnglishFont.setTypeface(std::move(enTypeFace));
		context.OtherFont.setTypeface(std::move(otherTypeFace));
	}
	if (newEnglishFace != Node->Property.end()) {
		auto enTypeFace = SkTypeface::MakeFromName(newEnglishFace->second.c_str(), enFontStyle);
		context.EnglishFont.setTypeface(std::move(enTypeFace));
	}
	if (newOtherFace != Node->Property.end()) {
		auto otherTypeFace = SkTypeface::MakeFromName(newFace->second.c_str(), otherFontStyle);
		context.OtherFont.setTypeface(std::move(otherTypeFace));
	}

	return context;
}
VRichTextRendererContext VRichTextCenterLabel::ContextMeasure(VHTMLASTNode					 *Node,
															  const VRichTextRendererContext &Context) {
	VRichTextRendererContext context = Context;

	if (context.LineCount < context.WidthCollections.size() && !context.WidthCollections.empty()) {
		context.X = context.Bound.GetWidth() / 2 - (context.WidthCollections[context.LineCount] - context.TempWidth) / 2;
	}

	return context;
}
VRichTextRendererContext VRichTextLeftLabel::ContextMeasure(VHTMLASTNode				   *Node,
															const VRichTextRendererContext &Context) {
	VRichTextRendererContext context = Context;

	context.X = context.Bound.GetLeft();

	return context;
}
VRichTextRendererContext VRichTextRightLabel::ContextMeasure(VHTMLASTNode					*Node,
															 const VRichTextRendererContext &Context) {
	VRichTextRendererContext context = Context;

	if (context.LineCount < context.WidthCollections.size() && !context.WidthCollections.empty()) {
		context.X = context.Bound.GetWidth() - (context.WidthCollections[context.LineCount] - context.TempWidth);
	}

	return context;
}
VRichTextRendererContext VRichTextVCenterLabel::ContextMeasure(VHTMLASTNode					  *Node,
															   const VRichTextRendererContext &Context) {
	VRichTextRendererContext context = Context;

	int heightSum = 0;
	for (auto count = context.LineCount; count < context.HeightCollections.size(); ++count) {
		heightSum += context.HeightCollections[count] + context.LineSpace;
	}

	context.Y = context.Y + (context.Bound.GetBottom() - context.Y) / 2 - heightSum / 2;

	return context;
}