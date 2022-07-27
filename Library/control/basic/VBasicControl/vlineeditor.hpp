/*
 * VLineEditor.hpp
 *	@description : The Line Editor
 *	@birth		 : 2022/7.14
*/

#pragma once

#include "vinteractivetextlabel.hpp"
#include "vboxshadowhelper.hpp"
#include "vanimation.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VLineEditor:
 *	@description  : The Line Text Editor
*/
class VLineEditor : public VUIObject {
private:
	VLineEditorTheme*  Theme;
	std::wstring       StringBuffer;

	int                MouseX = 0;
	int                CursorX = 0;

	bool               UserInteractiving = false;
	bool               ShowCursor        = false;
	bool               UserIMETyping     = false;

	VColorInterpolator BackgroundColor;
	VColorInterpolator LineColor;

	VTimer             CursorTimer;
	VTimer             AnimationTimer;

	VRect SurfaceRegion() override {
		if (Theme->EnableBoxShadow == false) {
			return Surface()->Rect;
		}

		auto ShadowRect = Surface()->Rect;

		ShadowRect.left = ShadowRect.left - Theme->BoxShadowPixel;
		ShadowRect.top = ShadowRect.top - Theme->BoxShadowPixel;
		ShadowRect.bottom = ShadowRect.bottom + Theme->BoxShadowPixel * 2;
		ShadowRect.right = ShadowRect.right + Theme->BoxShadowPixel * 2;

		return ShadowRect;
	}

public:
	VSignal<std::wstring> TextOnChange;

	std::function<bool(std::wstring)> CheckInput;

private:
	int GetTextWidth(std::wstring String) {
		VFont JudgeFont(Theme->FontFamily, Theme->FontSize);

		return JudgeFont.GetWidth(String, Theme->CacheFormat);
	}

private:
	int ConvertMouseXToCursorX(int X) {
		std::wstring TempString;
		VFont        JudgeFont(Theme->FontFamily, Theme->FontSize);

		X -= GetX();

		for (int Count = 0; Count < StringBuffer.size(); ++Count) {
			TempString.push_back(StringBuffer[Count]);

			if (JudgeFont.GetWidth(TempString, Theme->CacheFormat) > X) {
				return Count;
			}
		}

		return StringBuffer.size();
	}

	void EndType() {
		UserInteractiving = false;
		UserIMETyping     = false;

		UnlockGlobalFocusID();

		BackgroundColor.Start(Theme->CurrentBackgroundColor, Theme->BackgroundColor);
		LineColor.Start(Theme->CurrentLineColor, Theme->LineColor);

		ShowCursor = false;

		Update();
	}

	int  GetStartSpace(std::wstring String) {
		int Count = 0;

		for (auto& Element : String) {
			if (Element != L' ') {
				return Count;
			}

			++Count;
		}

		return Count;
	}
	int  GetEndSpace(std::wstring String) {
		int Count = 0;

		for (auto Element = String.rbegin(); Element != String.rend(); ++Element) {
			if (*Element != L' ') {
				return Count;
			}

			++Count;
		}

		return Count;
	}
	bool SpaceOnly(std::wstring String) {
		bool Flag = true;

		for (auto Element = String.rbegin(); Element != String.rend(); ++Element) {
			if (*Element != L' ') {
				Flag = false;

				return Flag;
			}
		}

		return Flag;
	}

public:
	VLineEditor(VUIObject* Parent) : VUIObject(Parent),
		BackgroundColor(0.1, VInterpolatorType::AccelerateInterpolator),
		LineColor(0.1, VInterpolatorType::AccelerateInterpolator) {
		Theme = new VLineEditorTheme(*(static_cast<VLineEditorTheme*>(SearchThemeFromParent(VLINEEDITOR_THEME))));

		CursorTimer.Start(800);
	}

	void MouseLeftClicked(VMouseClickedFlag Flag) override {
		if (Flag == VMouseClickedFlag::Up) {
			UserInteractiving = true;

			LockGlobalFocusID(Kernel()->GlobalID);

			Update();

			CursorX = ConvertMouseXToCursorX(MouseX);

			BackgroundColor.Start(Theme->CurrentBackgroundColor, Theme->OnClickedBackgroundColor);
			LineColor.Start(Theme->CurrentLineColor, Theme->OnClickedLineColor);

			AnimationTimer.Start(16);
		}
	}
	void GotMouseFocus() override {
		HCURSOR ArrowCursor = LoadCursor(NULL, IDC_IBEAM);
		SetClassLongPtr(GetParentWindowHandle(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(ArrowCursor));
	}
	void LosedMouseFocus() override {
		HCURSOR ArrowCursor = LoadCursor(NULL, IDC_ARROW);
		SetClassLongPtr(GetParentWindowHandle(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(ArrowCursor));
	}

	void OnPaint(VCanvas* Canvas) override {
		if (Theme->EnableBoxShadow == false) {
			VPen           Border(Theme->CurrentLineColor);
			VSolidBrush	   CursorBrush(Theme->CursorColor);
			VSolidBrush    Brush(Theme->CurrentBackgroundColor);
			VSolidBrush    FontBrush(Theme->CurrentTextColor);

			VPainterDevice Device(Canvas);
			VFontFormat    Format;
			VFont          Font(Theme->FontFamily, Theme->FontSize);

			Format.SetLineAlignment(VStringAlignment::AlignmentCenter);
			Device.FillRoundedRectangle(&Border, &Brush, { 0, 0, GetWidth(), GetHeight() }, Theme->Radius);
			Device.DrawString(StringBuffer, &Font, &FontBrush, &Format, { 2, 0, GetWidth(), GetHeight() });

			auto JudgeString = StringBuffer.substr(0, CursorX);

			int TextHeight = (GetHeight() / 2 - Theme->FontSize / 2);
			int TextWidth = Font.GetWidth(JudgeString, &Format);

			if (SpaceOnly(JudgeString) == false) {
				TextWidth += GetStartSpace(JudgeString) * 4;
				TextWidth += GetEndSpace(JudgeString) * 4;
			}
			else {
				TextWidth += GetStartSpace(JudgeString) * 2;
				TextWidth += GetEndSpace(JudgeString) * 2;
			}

			int CursorY = GetHeight() / 2 - TextHeight / 2;

			if (UserIMETyping == true) {
				SetGlobalIMEPosition(GetX() + TextWidth, GetY() + TextHeight);
			}

			if (ShowCursor == true) {
				Device.SolidRectangle(&CursorBrush,
					{ TextWidth + 3, TextHeight, TextWidth + 4, TextHeight + Theme->FontSize });
			}
		}
		else {
			VPen           Border(Theme->CurrentLineColor);
			VSolidBrush	   CursorBrush(Theme->CursorColor);
			VSolidBrush    Brush(Theme->CurrentBackgroundColor);
			VSolidBrush    FontBrush(Theme->CurrentTextColor);

			VPainterDevice Device(Canvas);
			VFontFormat    Format;
			VFont          Font(Theme->FontFamily, Theme->FontSize);

			Border.SetStyle(Theme->BorderStyle);

			Format.SetLineAlignment(VStringAlignment::AlignmentCenter);
			Device.DrawBoxShadow(Theme->BoxShadowColor,
				VBoxShadowHelper::GetShadowRect(GetWidth(), GetHeight(), Theme->BoxShadowPixel),
				Theme->Radius, Theme->BoxShadowPixel);
			Device.FillRoundedRectangle(&Border, &Brush,
				VBoxShadowHelper::GetShadowElementRect(GetWidth(), GetHeight(), Theme->BoxShadowPixel)
				, Theme->Radius);
			Device.DrawString(StringBuffer, &Font, &FontBrush, &Format, 
				{ Theme->BoxShadowPixel + 2, Theme->BoxShadowPixel + 0, GetWidth(), GetHeight() });

			auto JudgeString = StringBuffer.substr(0, CursorX);

			int TextHeight = (GetHeight() / 2 - Theme->FontSize / 2);
			int TextWidth = Font.GetWidth(JudgeString, &Format);

			if (SpaceOnly(JudgeString) == false) {
				TextWidth += GetStartSpace(JudgeString) * 4;
				TextWidth += GetEndSpace(JudgeString) * 4;
			}
			else {
				TextWidth += GetStartSpace(JudgeString) * 2;
				TextWidth += GetEndSpace(JudgeString) * 2;
			}

			int CursorY = GetHeight() / 2 - TextHeight / 2;

			if (UserIMETyping == true) {
				SetGlobalIMEPosition(GetX() + TextWidth, GetY() + TextHeight);
			}

			if (ShowCursor == true) {
				Device.SolidRectangle(&CursorBrush,
					{ TextWidth + 3, TextHeight, TextWidth + 4, TextHeight + Theme->FontSize });
			}
		}
	}
	void DealyMessage(VMessage* Message) {
		switch (Message->GetType()) {
		case VMessageType::MouseClickedMessage: {
			VMouseClickedMessage* MouseClickedMessage = static_cast<VMouseClickedMessage*>(Message);

			if (UserInteractiving == true) {
				if (MouseClickedMessage->ClickedMethod == VMouseClickedFlag::Down &&
					MouseClickedMessage->ClickedKey == VMouseKeyFlag::Left &&
					MouseClickedMessage->MousePosition.InsideRect(Surface()->Rect) == false) {

					SetGlobalIMEPosition(-1, -1);

					EndType();
				}
			}

			MouseX = MouseClickedMessage->MousePosition.x;

			break;
		}
		case VMessageType::KillFocusMessage: {
			SetGlobalIMEPosition(-1, -1);

			EndType();

			break;
		}
		case VMessageType::MouseMoveMessage: {
			VMouseMoveMessage* MouseMoveMessage = static_cast<VMouseMoveMessage*>(Message);

			MouseX = MouseMoveMessage->MousePosition.x;

			if (UserInteractiving == true) {
				if (MouseMoveMessage->MousePosition.InsideRect(Surface()->Rect) == false) {
					HCURSOR ArrowCursor = LoadCursor(NULL, IDC_ARROW);
					SetClassLongPtr(GetParentWindowHandle(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(ArrowCursor));
				}
			}

			break;
		}
		case VMessageType::KeyClickedMessage: {
			if (UserInteractiving == true) {
				VKeyClickedMessage* ClickedMessage = static_cast<VKeyClickedMessage*>(Message);

				if (ClickedMessage->KeyPrevDown == true) {
					if (ClickedMessage->KeyVKCode == VK_HOME) {
						CursorX = 0;
					}
					else if (ClickedMessage->KeyVKCode == VK_END) {
						CursorX = StringBuffer.size();
					}
					else if (ClickedMessage->KeyVKCode == VK_LEFT) {
						if (CursorX - 1 >= 0) {
							--CursorX;
						}
					}
					else if (ClickedMessage->KeyVKCode == VK_RIGHT) {
						if (CursorX + 1 <= StringBuffer.size()) {
							++CursorX;
						}
					}
					else if (ClickedMessage->KeyVKCode == VK_DELETE) {
						if (CursorX + 1 <= StringBuffer.size()) {
							StringBuffer.erase(StringBuffer.begin() + CursorX);

							TextOnChange.Emit(StringBuffer);
						}
					}
				}

				CursorTimer.Start(700);
				ShowCursor = true;

				Update();
			}

			break;
		}
		case VMessageType::IMECharMessage: {
			if (UserInteractiving == true) {
				UserIMETyping = true;

				VIMECharMessage* IMEMessage = static_cast<VIMECharMessage*>(Message);

				if (IMEMessage->IMEChar == L'\r') {
					EndType();

					return;
				}
				else if (IMEMessage->IMEChar == L'\b') {
					if (CursorX - 1 >= 0) {
						--CursorX;

						StringBuffer.erase(StringBuffer.begin() + CursorX);
					}
				}
				else if (IMEMessage->IMEChar == L'\n') {
					if (CursorX + 1 <= StringBuffer.size()) {
						StringBuffer.erase(StringBuffer.begin() + CursorX);
					}
				}
				else {
					std::wstring TempString = StringBuffer + IMEMessage->IMEChar;

					if (GetTextWidth(TempString) + 5 < GetWidth()) {
						if (CheckInput != NULL) {
							TempString = StringBuffer;
							TempString.insert(TempString.begin() + CursorX, IMEMessage->IMEChar);

							if (CheckInput(TempString) == true) {
								StringBuffer.insert(StringBuffer.begin() + CursorX, IMEMessage->IMEChar);
							}
							else {
								return;
							}
						}
						else {
							StringBuffer.insert(StringBuffer.begin() + CursorX, IMEMessage->IMEChar);
						}

						++CursorX;
					}
				}

				TextOnChange.Emit(StringBuffer);

				CursorTimer.Start(700);
				ShowCursor = true;

				Update();
			}

			break;
		}
		}
	}
	void CheckFrame() override {
		if (AnimationTimer.End() == true &&
			(BackgroundColor.IsAnimationEnd() == false ||
				LineColor.IsAnimationEnd() == false)) {
			Theme->CurrentBackgroundColor = BackgroundColor.GetOneFrame();
			Theme->CurrentLineColor = LineColor.GetOneFrame();

			AnimationTimer.Start(16);

			Update();
		}

		if (CursorTimer.End() == true &&
			UserInteractiving == true) {
			CursorTimer.Start(800);  
									 
			if (ShowCursor == true) {
				ShowCursor = false;  
			}                        
			else {                   
				ShowCursor = true;   
			}                        

			Update();
		}
	}

	std::wstring GetPlaneString() {
		return StringBuffer;
	}

public:
	void SetTheme(VLineEditorTheme* NewTheme) {
		delete Theme;

		Theme = new VLineEditorTheme(*NewTheme);
	}
	VLineEditorTheme* GetTheme() {
		return Theme;
	}

public:
	void SetBoxShadowEnable(bool Enable) {
		Theme->EnableBoxShadow = Enable;

		Update();
	}
	void SetBoxShadowColor(VColor Color) {
		Theme->BoxShadowColor = Color;

		Update();
	}
	void SetBoxShadowPixel(int Pixel) {
		Theme->BoxShadowPixel = Pixel;

		Update();
	}
};

VLIB_END_NAMESPACE