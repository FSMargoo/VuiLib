#pragma warning(disable : 26495)

#include "../../../include/core/control/veditorhighlighter.h"
#include "../../../include/kits/vcolorhelper.h"

VLIB_BEGIN_NAMESPACE

namespace Core {
	VHightlighterTheme::VHightlighterTheme(const VRenderHandle& StaticRenderHandle) {
		BackgroundColor = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#282C34"));
		KeyWorldColor	= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#D19A66"));
		StringColor		= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#98C379"));
		LabelColor		= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#E5C07B"));
		MetaCallColor	= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#E06C75"));
		TypeColor		= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#98C379"));
		CommentColor	= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#5C6370"));
		SymbolColor		= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#179FFF"));
		TagColor		= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#C477DB"));
		SubElementColor = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#E06C75"));
		IgnoreGrayColor = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#ABB2BF"));

		SelectBackgroundColor = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#3E4451"));
		SelectTextColor		  = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#000000"));
		SelectTextColor.SetA(0);

		SymbolBrush		= new Core::VSolidBrush(SymbolColor, StaticRenderHandle);
		StringBrush		= new Core::VSolidBrush(StringColor, StaticRenderHandle);
		KeyWorldBrush	= new Core::VSolidBrush(KeyWorldColor, StaticRenderHandle);
		MetaCallBrush	= new Core::VSolidBrush(MetaCallColor, StaticRenderHandle);
		LabelBrush		= new Core::VSolidBrush(LabelColor, StaticRenderHandle);
		TypeBrush		= new Core::VSolidBrush(TypeColor, StaticRenderHandle);
		CommentBrush	= new Core::VSolidBrush(CommentColor, StaticRenderHandle);
		TagBrush		= new Core::VSolidBrush(TagColor, StaticRenderHandle);
		SubElementBrush = new Core::VSolidBrush(SubElementColor, StaticRenderHandle);
		IgnoreGrayBrush = new Core::VSolidBrush(IgnoreGrayColor, StaticRenderHandle);

		FontFamily		= L"Consolas";
	}
	VHightlighterTheme::VHightlighterTheme(const VHightlighterTheme& Theme) {
		BackgroundColor = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#282C34"));
			KeyWorldColor	= Theme.KeyWorldColor;
			StringColor		= Theme.StringColor;
			LabelColor		= Theme.LabelColor;
			MetaCallColor	= Theme.MetaCallColor;
			TypeColor		= Theme.TypeColor;
			CommentColor	= Theme.CommentColor;
			SymbolColor		= Theme.SymbolColor;
			TagColor		= Theme.TagColor;
			SubElementColor = Theme.SubElementColor;
			IgnoreGrayColor = Theme.IgnoreGrayColor;

			SelectBackgroundColor = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#3E4451"));
			SelectTextColor		  = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#000000"));
			SelectTextColor.SetA(0);

			SymbolBrush		= Theme.SymbolBrush;
			StringBrush		= Theme.StringBrush;
			KeyWorldBrush	= Theme.KeyWorldBrush;
			MetaCallBrush	= Theme.MetaCallBrush;
			LabelBrush		= Theme.LabelBrush;
			TypeBrush		= Theme.TypeBrush;
			CommentBrush	= Theme.CommentBrush;
			TagBrush		= Theme.TagBrush;
			SubElementBrush = Theme.SubElementBrush;
			IgnoreGrayBrush = Theme.IgnoreGrayBrush;

			FontFamily		= Theme.FontFamily;
	}
	VHightlighterTheme::VHightlighterTheme(const VRenderHandle& StaticRenderHandle, const VBuiltInHightlighterTheme& Theme) {
		if (Theme == VBuiltInHightlighterTheme::OneDarkPro) {
			BackgroundColor = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#282C34"));
			KeyWorldColor	= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#D19A66"));
			StringColor		= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#98C379"));
			LabelColor		= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#E5C07B"));
			MetaCallColor	= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#E06C75"));
			TypeColor		= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#98C379"));
			CommentColor	= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#5C6370"));
			SymbolColor		= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#179FFF"));
			TagColor		= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#C477DB"));
			SubElementColor = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#E06C75"));
			IgnoreGrayColor = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#ABB2BF"));

			SelectBackgroundColor = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#3E4451"));
			SelectTextColor		  = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#000000"));
			SelectTextColor.SetA(0);

			SymbolBrush		= new Core::VSolidBrush(SymbolColor, StaticRenderHandle);
			StringBrush		= new Core::VSolidBrush(StringColor, StaticRenderHandle);
			KeyWorldBrush	= new Core::VSolidBrush(KeyWorldColor, StaticRenderHandle);
			MetaCallBrush	= new Core::VSolidBrush(MetaCallColor, StaticRenderHandle);
			LabelBrush		= new Core::VSolidBrush(LabelColor, StaticRenderHandle);
			TypeBrush		= new Core::VSolidBrush(TypeColor, StaticRenderHandle);
			CommentBrush	= new Core::VSolidBrush(CommentColor, StaticRenderHandle);
			TagBrush		= new Core::VSolidBrush(TagColor, StaticRenderHandle);
			SubElementBrush = new Core::VSolidBrush(SubElementColor, StaticRenderHandle);
			IgnoreGrayBrush = new Core::VSolidBrush(IgnoreGrayColor, StaticRenderHandle);

			FontFamily		= L"Consolas";
		}
		if (Theme == VBuiltInHightlighterTheme::FleetDark) {
			BackgroundColor = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#181818"));
			KeyWorldColor	= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#AF9CFF"));
			StringColor		= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#E394DC"));
			LabelColor		= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#87C3FF"));
			MetaCallColor	= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#CC5CC2"));
			TypeColor		= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#E394DC"));
			CommentColor	= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#898989"));
			SymbolColor		= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#898989"));
			TagColor		= Core::VColor(VKits::VSSColorHelper::HexToColor(L"#C477DB"));
			SubElementColor = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#E06C75"));
			IgnoreGrayColor = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#ABB2BF"));

			SelectBackgroundColor = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#3E4451"));
			SelectTextColor		  = Core::VColor(VKits::VSSColorHelper::HexToColor(L"#000000"));
			SelectTextColor.SetA(0);

			SymbolBrush		= new Core::VSolidBrush(SymbolColor, StaticRenderHandle);
			StringBrush		= new Core::VSolidBrush(StringColor, StaticRenderHandle);
			KeyWorldBrush	= new Core::VSolidBrush(KeyWorldColor, StaticRenderHandle);
			MetaCallBrush	= new Core::VSolidBrush(MetaCallColor, StaticRenderHandle);
			LabelBrush		= new Core::VSolidBrush(LabelColor, StaticRenderHandle);
			TypeBrush		= new Core::VSolidBrush(TypeColor, StaticRenderHandle);
			CommentBrush	= new Core::VSolidBrush(CommentColor, StaticRenderHandle);
			TagBrush		= new Core::VSolidBrush(TagColor, StaticRenderHandle);
			SubElementBrush = new Core::VSolidBrush(SubElementColor, StaticRenderHandle);
			IgnoreGrayBrush = new Core::VSolidBrush(IgnoreGrayColor, StaticRenderHandle);

			FontFamily		= L"Consolas";
		}
	}
	VHightlighterTheme::~VHightlighterTheme() {
		delete SymbolBrush;
		delete StringBrush;
		delete KeyWorldBrush;
		delete MetaCallBrush;
		delete LabelBrush;
		delete TypeBrush;
		delete CommentBrush;
	}

	VVMLHighlighter::VVMLHighlighter(VEditor* Editor)
		: HighlightTheme(Editor->CallWidgetGetStaticRenderHandle()) {
		BindEditor(Editor);
	}
	int VVMLHighlighter::CountSpace(VML::VMLNode Node, const int& PreSpaceCount) {
		auto Caret = Editor->GetCaret();

		if (Node.BlockStart <= Caret.CaretStart && Node.BlockEnd > Caret.CaretStart) {
			int Result = PreSpaceCount;

			if (!Node.ChildrenNodes.empty()) {
				for (auto& SubNode : Node.ChildrenNodes) {
					auto ChildSpace = CountSpace(SubNode.second, PreSpaceCount + 1);
					if (ChildSpace != -1) {
						return ChildSpace;
					}
				}
			}
			else {
				return PreSpaceCount + 1;
			}
		}
		else {
			return -1;
		}
	}
	VVMLHighlighter::VVMLHighlighter(const VRenderHandle& StaticRenderHandle) 
		: HighlightTheme(StaticRenderHandle) {

	}
	VVMLHighlighter::VVMLHighlighter(const VRenderHandle& StaticRenderHandle, const VBuiltInHightlighterTheme& Theme)
		: HighlightTheme(StaticRenderHandle, Theme) {

	}
	VVMLHighlighter::VVMLHighlighter(const VHightlighterTheme& Theme) : HighlightTheme(Theme) {

	}
	void VVMLHighlighter::BindEditor(VEditor* TargetEditor) {
		if (Editor == TargetEditor) {
			return;
		}

		VBasicHightlighter::BindEditor(TargetEditor);

		auto Theme = TargetEditor->GetTheme();

		TargetEditor->SetAllowFontSizeDragStatus(true);

		Theme->OnSelectedBackgroundColor	= HighlightTheme.SelectBackgroundColor;
		Theme->OnSelectedColor				= HighlightTheme.SelectTextColor;

		Theme->LocalTheme.BackgroundColor	= HighlightTheme.BackgroundColor;
		Theme->LocalTheme.BorderColor		= HighlightTheme.BackgroundColor;
		Theme->LocalTheme.Radius			= { 0, 0 };

		Theme->LabelFont->SetFamilyName(HighlightTheme.FontFamily);

		Theme->ActiveTheme	= Theme->LocalTheme;
		Theme->StaticTheme	= Theme->LocalTheme;
		Theme->OnHoverTheme = Theme->LocalTheme;

		TargetEditor->TextOnChange.Connect(this, &VVMLHighlighter::RenderColor);
		TargetEditor->CheckInput.Connect(this, &VVMLHighlighter::CheckInputChar);
		TargetEditor->PushNewCharacter.Connect(this, &VVMLHighlighter::NewCharacter);
		TargetEditor->TextClicked.Connect(this, &VVMLHighlighter::MouseClicked);
	}
	void VVMLHighlighter::CheckInputChar(const wchar_t& NewChar, bool* Flag) {
		auto Caret = Editor->GetCaret();

		if (NewChar == L'\"') {
			auto Text  = Editor->GetPlaneText();

			if (Text.size() > Caret.CaretStart - 1 && Text[Caret.CaretStart - 1] != L'\"') {
				*Flag = true;
			}
			else if (Text.size() > Caret.CaretStart - 1) {
				*Flag = false;
			}
		}
		if (NewChar == L'>') {
			auto Text = Editor->GetPlaneText();

			if (Text.size() > Caret.CaretStart - 1 && Text[Caret.CaretStart - 1] != L'<') {
				*Flag = true;
			}
			else if (Text.size() > Caret.CaretStart - 1) {
				*Flag = false;
			}
		}
		if (NewChar == L'\b' && Editor->GetPlaneText().size() > Caret.CaretStart && Caret.CaretStart - 1 >= 0) {
			auto Text = Editor->GetPlaneText();

			if (Text[Caret.CaretStart - 1] == L'\"' && Text[Caret.CaretStart] == L'\"') {
				Text.erase(Text.begin() + Caret.CaretStart);
			}
			if (Text[Caret.CaretStart - 1] == L'<' && Text[Caret.CaretStart] == L'>') {
				Text.erase(Text.begin() + Caret.CaretStart);
			}

			Editor->SetPlaneText(Text);
			Editor->SetCaret(Caret);
		}
	}
	void VVMLHighlighter::NewCharacter(const wchar_t& NewChar) {
		auto Caret = Editor->GetCaret();

		if (NewChar == L'\"') {
			auto OldCaret	= Caret;
			auto Text		= Editor->GetPlaneText();

			if (Text.size() > Caret.CaretStart) {
				if (Text[Caret.CaretStart] != L'\"') {
					Text.insert(Text.begin() + Caret.CaretStart, L'\"');

					Editor->SetPlaneText(Text);
					Editor->SetCaret(Caret);
				}
			}
			else {
				Text.insert(Text.begin() + Caret.CaretStart, L'\"');

				Editor->SetPlaneText(Text);
				Editor->SetCaret(Caret);
			}
		}
		if (NewChar == L'<') {
			auto Text = Editor->GetPlaneText();

			Text.insert(Text.begin() + Caret.CaretStart, L'>');

			Editor->SetPlaneText(Text);
			Editor->SetCaret(Caret);
		}
	}
	void VVMLHighlighter::RenderColor(const std::wstring& PlaneText) {
		VKits::seal_lexical Lexical(PlaneText);
		auto Token = Lexical.get_token();
		auto LastToken = Token;
		auto InType = false;

		Editor->TextEffect.clear();
		Editor->TextStyle.clear();

		while (!Lexical.is_eof()) {
			DWRITE_TEXT_RANGE Range;
			Range.startPosition = Lexical.get_index() - Token.token_string.size();
			Range.length = Token.token_string.size() == 1 ? 1 : Token.token_string.size() + 1;

			if (Token.cache_token == LESS_THAN_TOKEN || Token.cache_token == MORE_THAN_TOKEN ||
				Token.cache_token == EQUAL_SIGN_TOKEN || Token.cache_token == SLASH_TOKEN) {
				if (Token.cache_token == LESS_THAN_TOKEN) {
					LastToken = Token;
					Token = Lexical.view_token();
				}

				if (Token.cache_token == EXCLAMATION_MARK && LastToken.cache_token == LESS_THAN_TOKEN) {
					while (!Lexical.is_eof()) {
						Token = Lexical.get_token();

						if (Token.cache_token == DOUBLE_MINUS_TOKEN) {
							Token = Lexical.get_token();

							if (Token.cache_token == MORE_THAN_TOKEN) {
								break;
							}
						}
					}

					Range.length = Lexical.get_index() - Range.startPosition;

					Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.CommentBrush->GetDxBrush(), Range));
					Editor->TextStyle.push_back(std::pair<DWRITE_FONT_STYLE, DWRITE_TEXT_RANGE>(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_ITALIC, Range));
				}
				else {
					Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.SymbolBrush->GetDxBrush(), Range));
				}

				Token = Lexical.get_token();

				continue;
			}
			if (Token.cache_token == UNKNOW_TOKEN && (LastToken.cache_token == LESS_THAN_TOKEN || LastToken.cache_token == SLASH_TOKEN)) {
				Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.LabelBrush->GetDxBrush(), Range));
			}
			else if (Token.cache_token == UNKNOW_TOKEN) {
				Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.KeyWorldBrush->GetDxBrush(), Range));

				if (Token.token_string == L"type") {
					InType = true;
				}
			}
			if (Token.cache_token == CONST_STRING) {
				Range.length += 1;

				if (Token.token_string.find(L"@") == std::wstring::npos) {
					Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.StringBrush->GetDxBrush(), Range));

					if (InType) {
						InType = false;

						Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.TypeBrush->GetDxBrush(), Range));
					}
				}
				else {
					Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.MetaCallBrush->GetDxBrush(), Range));
				}
			}

			LastToken = Token;
			Token = Lexical.get_token();
		}

		Editor->ResetTextLayout();
	}
	void VVMLHighlighter::MouseClicked(const int& TextPosition) {
		if (!DoubleClickTimer.End()) {
			VKits::seal_lexical Lexical(Editor->GetPlaneText());
			auto Token		= Lexical.get_token();
			auto InType		= false;

			while (!Lexical.is_eof()) {
				DWRITE_TEXT_RANGE Range;
				Range.startPosition = Lexical.get_index() - Token.token_string.size();
				Range.length		= Range.startPosition + (Token.token_string.size() == 1 ? 1 : Token.token_string.size());

				if (TextPosition >= Range.startPosition && TextPosition <= Range.length) {
					if (Token.cache_token == CONST_STRING) {
						VKits::seal_lexical SubLexical(Token.token_string.substr(1, Token.token_string.size() - 2));

						while (!SubLexical.is_eof()) {
							Token = SubLexical.get_token();

							DWRITE_TEXT_RANGE SubRange;

							SubRange.startPosition	= SubLexical.get_index() - Token.token_string.size() + 1 + Range.startPosition;
							SubRange.length			= SubRange.startPosition + (Token.token_string.size() == 1 ? 1 : Token.token_string.size());

							if (TextPosition >= SubRange.startPosition &&
								TextPosition <= SubRange.length) {
								auto Caret = Editor->GetCaret();

								Caret.CaretStart	= SubRange.startPosition;
								Caret.CaretEnd		= SubRange.length;
								Caret.SelectMode	= VEditorCaretSelectMode::Right;
								Caret.InSelecting	= true;

								Editor->SetCaret(Caret);

								break;
							}
						}
					}
					else {
						auto Caret = Editor->GetCaret();

						Caret.CaretStart	= Range.startPosition;
						Caret.CaretEnd		= Range.length;
						Caret.SelectMode	= VEditorCaretSelectMode::Right;
						Caret.InSelecting	= true;

						Editor->SetCaret(Caret);
					}
					Editor->Update();

					break;
				}

				Token = Lexical.get_token();
			}
		}
		else {
			DoubleClickTimer.Start(500);
		}
	}
	VVSSHighlighter::VVSSHighlighter(const VRenderHandle& StaticRenderHandle)
		: HighlightTheme(StaticRenderHandle) {

	}
	VVSSHighlighter::VVSSHighlighter(const VRenderHandle& StaticRenderHandle, const VBuiltInHightlighterTheme& Theme)
		: HighlightTheme(StaticRenderHandle, Theme) {

	}
	VVSSHighlighter::VVSSHighlighter(VEditor* Editor)
		: HighlightTheme(Editor->CallWidgetGetStaticRenderHandle()) {
		BindEditor(Editor);
	}
	VVSSHighlighter::VVSSHighlighter(const VHightlighterTheme& Theme) : HighlightTheme(Theme) {

	}
	void VVSSHighlighter::BindEditor(VEditor* TargetEditor) {
		if (Editor == TargetEditor) {
			return;
		}

		VBasicHightlighter::BindEditor(TargetEditor);

		auto Theme = TargetEditor->GetTheme();

		TargetEditor->SetAllowFontSizeDragStatus(true);

		Theme->LocalTheme.BackgroundColor = HighlightTheme.BackgroundColor;
		Theme->LocalTheme.BorderColor = HighlightTheme.BackgroundColor;
		Theme->LocalTheme.Radius = { 0, 0 };

		Theme->LabelFont->SetFamilyName(HighlightTheme.FontFamily);

		Theme->ActiveTheme  = Theme->LocalTheme;
		Theme->OnHoverTheme = Theme->LocalTheme;

		TargetEditor->TextOnChange.Connect(this, &VVSSHighlighter::RenderColor);
		TargetEditor->CheckInput.Connect(this, &VVSSHighlighter::CheckInputChar);
		TargetEditor->PushNewCharacter.Connect(this, &VVSSHighlighter::NewCharacter);
		TargetEditor->TextClicked.Connect(this, &VVSSHighlighter::MouseClicked);
	}
	void VVSSHighlighter::CheckInputChar(const wchar_t& NewChar, bool* Flag) {
		auto Caret = Editor->GetCaret();

		if (NewChar == L'}') {
			auto Text = Editor->GetPlaneText();

			if (Text.size() > Caret.CaretStart - 1 && Text[Caret.CaretStart - 1] != L'{') {
				*Flag = true;
			}
			else if (Text.size() > Caret.CaretStart - 1) {
				*Flag = false;
			}
		}
		if (NewChar == L'\b' && Editor->GetPlaneText().size() > Caret.CaretStart && Caret.CaretStart - 1 >= 0) {
			auto Text = Editor->GetPlaneText();

			if (Text[Caret.CaretStart - 1] == L'{' && Text[Caret.CaretStart] == L'}') {
				Text.erase(Text.begin() + Caret.CaretStart);
			}

			Editor->SetPlaneText(Text);
			Editor->SetCaret(Caret);
		}
	}
	void VVSSHighlighter::NewCharacter(const wchar_t& NewChar) {
		auto Caret = Editor->GetCaret();

		if (NewChar == L'{') {
			auto Text = Editor->GetPlaneText();

			Text.insert(Text.begin() + Caret.CaretStart, L'}');

			Editor->SetPlaneText(Text);
			Editor->SetCaret(Caret);
		}
		if (NewChar == L'*') {
			auto Text = Editor->GetPlaneText();

			if (Text.size() > Caret.CaretStart - 1 && Text[Caret.CaretStart - 1] != L'/') {
				Text.insert(Caret.CaretStart, L"*/");

				Editor->SetPlaneText(Text);
				Editor->SetCaret(Caret);
			}
		}
	}
	void VVSSHighlighter::RenderColor(const std::wstring& PlaneText) {
		VKits::seal_lexical Lexical(PlaneText, false);
		auto Token		= Lexical.get_token();
		auto LastToken	= Token;
		auto InBlock	= false;

		Editor->TextEffect.clear();
		Editor->TextStyle.clear();

		while (!Lexical.is_eof()) {
			DWRITE_TEXT_RANGE Range;
			Range.startPosition = Lexical.get_index() - Token.token_string.size();
			Range.length = Token.token_string.size() == 1 ? 1 : Token.token_string.size();

			if (Token.cache_token == BIG_LEFT_BRACKETS) {
				InBlock = true;
			}
			if (Token.cache_token == BIG_RIGHT_BRACKETS) {
				InBlock = false;
			}
			if (Token.cache_token == BIG_LEFT_BRACKETS || Token.cache_token == BIG_RIGHT_BRACKETS ||
				Token.cache_token == RIGHT_BRACKETS || Token.cache_token == LEFT_BRACKETS || 
				Token.cache_token == SEMICOLON_TOKEN) {
				Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.IgnoreGrayBrush->GetDxBrush(), Range));
			}
			if (Token.cache_token == UNKNOW_TOKEN && LastToken.cache_token == CONST_NUMBER) {
				Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.KeyWorldBrush->GetDxBrush(), Range));
			}
			else if (Token.cache_token == UNKNOW_TOKEN && LastToken.cache_token == DOT_TOKEN) {
				Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.TagBrush->GetDxBrush(), Range));
			}
			else if (Token.cache_token == UNKNOW_TOKEN && LastToken.token_string == L"#") {
				Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.LabelBrush->GetDxBrush(), Range));
			}
			else if (Token.cache_token == COMMENT_TOKEN) {
				Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.CommentBrush->GetDxBrush(), Range));
				Editor->TextStyle.push_back(std::pair<DWRITE_FONT_STYLE, DWRITE_TEXT_RANGE>(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_ITALIC, Range));
			}
			else if (Token.cache_token == UNKNOW_TOKEN && InBlock) {
				auto ViewToken = Lexical.view_token();

				if (ViewToken.cache_token != LEFT_BRACKETS) {
					Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.SubElementBrush->GetDxBrush(), Range));
				}
				else {
					Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.SymbolBrush->GetDxBrush(), Range));
				}
			}
			else if (Token.cache_token == UNKNOW_TOKEN && !InBlock) {
				Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.KeyWorldBrush->GetDxBrush(), Range));
			}
			else if (Token.cache_token == CONST_NUMBER) {
				Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.LabelBrush->GetDxBrush(), Range));
			}
			if (Token.cache_token == CONST_STRING) {
				Range.length += 1;

				Editor->TextEffect.push_back(std::pair<ID2D1Effect*, DWRITE_TEXT_RANGE>((ID2D1Effect*)HighlightTheme.TypeBrush->GetDxBrush(), Range));
			}

			LastToken = Token;
			Token = Lexical.get_token();
		}

		Editor->ResetTextLayout();
	}
	void VVSSHighlighter::MouseClicked(const int& TextPosition) {
		if (!DoubleClickTimer.End()) {
			VKits::seal_lexical Lexical(Editor->GetPlaneText());
			auto Token = Lexical.get_token();
			auto InType = false;

			while (!Lexical.is_eof()) {
				DWRITE_TEXT_RANGE Range;
				Range.startPosition = Lexical.get_index() - Token.token_string.size();
				Range.length = Range.startPosition + (Token.token_string.size() == 1 ? 1 : Token.token_string.size());

				if (TextPosition >= Range.startPosition && TextPosition <= Range.length) {
					auto Caret = Editor->GetCaret();

					Caret.CaretStart = Range.startPosition;
					Caret.CaretEnd = Range.length;
					Caret.SelectMode = VEditorCaretSelectMode::Right;
					Caret.InSelecting = true;

					Editor->SetCaret(Caret);

					Editor->Update();

					break;
				}

				Token = Lexical.get_token();
			}
		}
		else {
			DoubleClickTimer.Start(500);
		}
	}

	VBasicHightlighter::VBasicHightlighter() {
		Editor = nullptr;
	}
	void VBasicHightlighter::BindEditor(VEditor* TargetEditor) {
		Editor = TargetEditor;

		RenderColor(Editor->GetPlaneText());
	}
}

VLIB_END_NAMESPACE