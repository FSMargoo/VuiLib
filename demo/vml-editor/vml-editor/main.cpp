#include "../../../include/vml/vmlwidget.h"

class VMLEditor : public Core::VEditor
{
public:
	Core::VColor SymbolColor;
	Core::VColor BackgroundColor;
	Core::VColor KeyWorldColor;
	Core::VColor StringColor;
	Core::VColor LabelColor;
	Core::VColor TypeColor;
	Core::VColor MetaCallColor;
	Core::VColor CommentColor;

	Core::VSolidBrush SymbolBrush;
	Core::VSolidBrush KeyWorldBrush;
	Core::VSolidBrush StringBrush;
	Core::VSolidBrush TypeBrush;
	Core::VSolidBrush MetaCallBrush;
	Core::VSolidBrush LabelBrush;
	Core::VSolidBrush CommentBrush;

	std::vector<std::wstring>		LastString;
	std::vector<Core::VEditorCaret> LastCaret;

public:
	VMLEditor(Core::VUIObject *Parent)
		: VEditor(Parent), SymbolColor(VKits::VSSColorHelper::HexToColor(L"#179FFF")),
		  BackgroundColor(VKits::VSSColorHelper::HexToColor(L"#282C34")),
		  KeyWorldColor(VKits::VSSColorHelper::HexToColor(L"#D19A66")),
		  StringColor(VKits::VSSColorHelper::HexToColor(L"#98C379")),
		  LabelColor(VKits::VSSColorHelper::HexToColor(L"#E5C07B")),
		  MetaCallColor(VKits::VSSColorHelper::HexToColor(L"#E06C75")),
		  TypeColor(VKits::VSSColorHelper::HexToColor(L"#98C379")),
		  CommentColor(VKits::VSSColorHelper::HexToColor(L"#5C6370")),
		  SymbolBrush(SymbolColor, CallWidgetGetStaticRenderHandle()),
		  StringBrush(StringColor, CallWidgetGetStaticRenderHandle()),
		  KeyWorldBrush(KeyWorldColor, CallWidgetGetStaticRenderHandle()),
		  MetaCallBrush(MetaCallColor, CallWidgetGetStaticRenderHandle()),
		  LabelBrush(LabelColor, CallWidgetGetStaticRenderHandle()),
		  TypeBrush(TypeColor, CallWidgetGetStaticRenderHandle()),
		  CommentBrush(CommentColor, CallWidgetGetStaticRenderHandle())
	{
		auto Theme = GetTheme();

		SetAllowFontSizeDragStatus(true);

		Theme->LocalTheme.BackgroundColor = BackgroundColor;
		Theme->LocalTheme.BorderColor	  = BackgroundColor;
		Theme->LocalTheme.Radius		  = {0, 0};

		delete Theme->LabelFont;

		Theme->LabelFont =
			new Core::VFont(L"Consolas", Core::VFont::FontWeight::WEIGHT_NORMAL, Core::VFont::FontStyle::STYLE_NORMAL,
							Core::VFont::FontStretch::STRETCH_NORMAL, 18, L"ZH-CN");

		Theme->ActiveTheme	= Theme->LocalTheme;
		Theme->OnHoverTheme = Theme->LocalTheme;

		TextBeforeChange.Connect(this, &VMLEditor::BeforeChange);
		TextOnChange.Connect(this, &VMLEditor::ApplyColor);
		PushNewCharacter.Connect(this, &VMLEditor::NewCharacter);
	}
	VMLEditor(const int &Width, const int &Height, Core::VUIObject *Parent)
		: VEditor(Width, Height, Parent), SymbolColor(VKits::VSSColorHelper::HexToColor(L"#179FFF")),
		  BackgroundColor(VKits::VSSColorHelper::HexToColor(L"#282C34")),
		  KeyWorldColor(VKits::VSSColorHelper::HexToColor(L"#D19A66")),
		  StringColor(VKits::VSSColorHelper::HexToColor(L"#98C379")),
		  LabelColor(VKits::VSSColorHelper::HexToColor(L"#E5C07B")),
		  MetaCallColor(VKits::VSSColorHelper::HexToColor(L"#E06C75")),
		  TypeColor(VKits::VSSColorHelper::HexToColor(L"#98C379")),
		  CommentColor(VKits::VSSColorHelper::HexToColor(L"#5C6370")),
		  SymbolBrush(SymbolColor, CallWidgetGetStaticRenderHandle()),
		  StringBrush(StringColor, CallWidgetGetStaticRenderHandle()),
		  KeyWorldBrush(KeyWorldColor, CallWidgetGetStaticRenderHandle()),
		  MetaCallBrush(MetaCallColor, CallWidgetGetStaticRenderHandle()),
		  LabelBrush(LabelColor, CallWidgetGetStaticRenderHandle()),
		  TypeBrush(TypeColor, CallWidgetGetStaticRenderHandle()),
		  CommentBrush(CommentColor, CallWidgetGetStaticRenderHandle())
	{
		auto Theme = GetTheme();

		SetAllowFontSizeDragStatus(true);

		Theme->LocalTheme.BackgroundColor = BackgroundColor;
		Theme->LocalTheme.BorderColor	  = BackgroundColor;
		Theme->LocalTheme.Radius		  = {0, 0};

		delete Theme->LabelFont;

		Theme->LabelFont =
			new Core::VFont(L"Consolas", Core::VFont::FontWeight::WEIGHT_NORMAL, Core::VFont::FontStyle::STYLE_NORMAL,
							Core::VFont::FontStretch::STRETCH_NORMAL, 18, L"ZH-CN");

		Theme->ActiveTheme	= Theme->LocalTheme;
		Theme->OnHoverTheme = Theme->LocalTheme;

		TextBeforeChange.Connect(this, &VMLEditor::BeforeChange);
		TextOnChange.Connect(this, &VMLEditor::ApplyColor);
		PushNewCharacter.Connect(this, &VMLEditor::NewCharacter);
		CheckInput.Connect(this, &VMLEditor::CheckInputChar);
	}

	void CheckInputChar(const wchar_t &NewChar, bool *Flag)
	{
		if (NewChar == L'\"')
		{
			auto Text = GetPlainText();

			if (Text.size() > Caret.CaretStart - 1 && Text[Caret.CaretStart - 1] != L'\"')
			{
				*Flag = true;
			}
			else if (Text.size() > Caret.CaretStart - 1)
			{
				*Flag = false;
			}
		}
		if (NewChar == L'>')
		{
			auto Text = GetPlainText();

			if (Text.size() > Caret.CaretStart - 1 && Text[Caret.CaretStart - 1] != L'<')
			{
				*Flag = true;
			}
			else if (Text.size() > Caret.CaretStart - 1)
			{
				*Flag = false;
			}
		}
		if (NewChar == L'\b' && GetPlainText().size() > Caret.CaretStart && Caret.CaretStart - 1 >= 0)
		{
			auto Text = GetPlainText();

			if (Text[Caret.CaretStart - 1] == L'\"' && Text[Caret.CaretStart] == L'\"')
			{
				Text.erase(Text.begin() + Caret.CaretStart);
			}
			if (Text[Caret.CaretStart - 1] == L'<' && Text[Caret.CaretStart] == L'>')
			{
				Text.erase(Text.begin() + Caret.CaretStart);
			}

			auto OldCaret = Caret;
			SetPlainText(Text);
			Caret = OldCaret;
		}
	}
	void NewCharacter(const wchar_t &NewChar)
	{
		if (NewChar == L'\"')
		{
			auto OldCaret = Caret;

			auto Text = GetPlainText();

			if (Text.size() > Caret.CaretStart)
			{
				if (Text[Caret.CaretStart] != L'\"')
				{
					Text.insert(Text.begin() + Caret.CaretStart, L'\"');

					SetPlainText(Text);

					Caret = OldCaret;
				}
			}
			else
			{
				Text.insert(Text.begin() + Caret.CaretStart, L'\"');

				SetPlainText(Text);

				Caret = OldCaret;
			}
		}
		if (NewChar == L'<')
		{
			auto OldCaret = Caret;

			auto Text = GetPlainText();

			Text.insert(Text.begin() + Caret.CaretStart, L'>');

			SetPlainText(Text);

			Caret = OldCaret;
		}
	}
	void BeforeChange(const std::wstring &PlainText)
	{
		LastString.push_back(PlainText);
		LastCaret.push_back(Caret);
	}
	void ApplyColor(const std::wstring &PlainText)
	{
		VKits::seal_lexical Lexical(PlainText);
		auto				Token	  = Lexical.get_token();
		auto				LastToken = Token;
		auto				InType	  = false;

		TextEffect.clear();
		TextStyle.clear();

		while (!Lexical.is_eof())
		{
			DWRITE_TEXT_RANGE Range;
			Range.startPosition = Lexical.get_index() - Token.token_string.size();
			Range.length		= Token.token_string.size() == 1 ? 1 : Token.token_string.size() + 1;

			if (Token.cache_token == LESS_THAN_TOKEN || Token.cache_token == MORE_THAN_TOKEN ||
				Token.cache_token == EQUAL_SIGN_TOKEN || Token.cache_token == SLASH_TOKEN)
			{
				if (Token.cache_token == LESS_THAN_TOKEN)
				{
					LastToken = Token;
					Token	  = Lexical.view_token();
				}

				if (Token.cache_token == EXCLAMATION_MARK && LastToken.cache_token == LESS_THAN_TOKEN)
				{
					while (!Lexical.is_eof())
					{
						Token = Lexical.get_token();

						if (Token.cache_token == DOUBLE_MINUS_TOKEN)
						{
							Token = Lexical.get_token();

							if (Token.cache_token == MORE_THAN_TOKEN)
							{
								break;
							}
						}
					}

					Range.length = Lexical.get_index() - Range.startPosition;

					TextEffect.push_back(
						std::pair<ID2D1Effect *, DWRITE_TEXT_RANGE>((ID2D1Effect *)CommentBrush.GetDxBrush(), Range));
					TextStyle.push_back(std::pair<DWRITE_FONT_STYLE, DWRITE_TEXT_RANGE>(
						DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_ITALIC, Range));
				}
				else
				{
					TextEffect.push_back(
						std::pair<ID2D1Effect *, DWRITE_TEXT_RANGE>((ID2D1Effect *)SymbolBrush.GetDxBrush(), Range));
				}

				Token = Lexical.get_token();

				continue;
			}
			if (Token.cache_token == UNKNOW_TOKEN &&
				(LastToken.cache_token == LESS_THAN_TOKEN || LastToken.cache_token == SLASH_TOKEN))
			{
				TextEffect.push_back(
					std::pair<ID2D1Effect *, DWRITE_TEXT_RANGE>((ID2D1Effect *)LabelBrush.GetDxBrush(), Range));
			}
			else if (Token.cache_token == UNKNOW_TOKEN)
			{
				TextEffect.push_back(
					std::pair<ID2D1Effect *, DWRITE_TEXT_RANGE>((ID2D1Effect *)KeyWorldBrush.GetDxBrush(), Range));

				if (Token.token_string == L"type")
				{
					InType = true;
				}
			}
			if (Token.cache_token == CONST_STRING)
			{
				Range.length += 1;

				if (Token.token_string.find(L"@") == std::wstring::npos)
				{
					TextEffect.push_back(
						std::pair<ID2D1Effect *, DWRITE_TEXT_RANGE>((ID2D1Effect *)StringBrush.GetDxBrush(), Range));

					if (InType)
					{
						InType = false;

						TextEffect.push_back(
							std::pair<ID2D1Effect *, DWRITE_TEXT_RANGE>((ID2D1Effect *)TypeBrush.GetDxBrush(), Range));
					}
				}
				else
				{
					TextEffect.push_back(
						std::pair<ID2D1Effect *, DWRITE_TEXT_RANGE>((ID2D1Effect *)MetaCallBrush.GetDxBrush(), Range));
				}
			}

			LastToken = Token;
			Token	  = Lexical.get_token();
		}

		ResetTextLayout();
	}
};

int main()
{
	Core::VApplication App;
	Core::VMainWindow  MainWindow(640, 480, &App);
	VMLEditor		   Editor(640, 380, &MainWindow);
	Core::VScaleLayout EditorLayout(&Editor, &MainWindow);

	EditorLayout.SetWidthScalePercent(1.f);
	EditorLayout.SetHeightScalePercent(1.f);

	MainWindow.Show();

	return App.Exec();
}