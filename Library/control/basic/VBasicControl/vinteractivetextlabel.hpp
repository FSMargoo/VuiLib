/*
 * VInteractiveTextLabel.hpp
 *	@description : An User Interactivble Text Label
 *	@birth		 : 2022/7.14
*/

#pragma once

#pragma warning(disable : 4267)
#pragma warning(disable : 26451)

#include "vuiobject.hpp"
#include "../../../basic/vbasic/vtimer.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VTextStorage class:
 *	@description  : A Text Storage Class 
 *                  ( Use Chain of Lines as Storage)
*/
class VTextStorage {
private:
	std::vector<std::wstring> StringChain;
	
	bool                      InEndOfLine = false;

public:
	VTextStorage() {
		
	}

	void         AppendString(std::wstring String) {
		if (StringChain.empty() == false) {
			if (InEndOfLine == false) {
				StringChain[StringChain.size() - 1].append(String);

				if (String[String.size() - 1] == L'\n') {
					InEndOfLine = true;
				}
			}
			else {
				StringChain.push_back(String);

				InEndOfLine = false;
			}
		}
		else {
			StringChain.push_back(String);
		}
	}
	void         SetStringByParser(std::wstring String) {
		auto Element = String.begin();

		if (StringChain.empty() == true) {
			std::wstring TempString;
			TempString.push_back(*Element);

			StringChain.push_back(TempString);

			++Element;
		}

		int Index = 0;

		for (; Element != String.end(); ++Element) {
			StringChain[Index].push_back(*Element);

			if (*Element == L'\n') {
				if (Element + 1 != String.end()) {
					++Element;

					std::wstring TempString;
					TempString.push_back(*Element);

					StringChain.push_back(TempString);
				}
				else {
					InEndOfLine = true;

					break;
				}

				++Index;
			}
		}
	}
	void         DeleteLineString(int TargetLine, int StartPosition, int EndPosition) {
		StringChain[TargetLine - 1].erase(StartPosition, EndPosition);
	}
	void         DeleteLinesString(int StartLine, int StartLinePosition, int EndLine, int EndLinePosition) {
		StringChain[StartLine - 1].erase(StartLinePosition, 
			StringChain[StartLine - 1].size() - StartLinePosition - 1);

		if (StringChain[StartLine - 1].empty() == true) {
			StringChain.erase(StringChain.begin() + (StartLine - 1));

			--StartLine;
		}

		if (StartLine + 1 < StringChain.size()) {
			StringChain.erase(StringChain.begin() + (StartLine),
				StringChain.begin() + (EndLine - StartLine));
		}

		StringChain[StartLine].erase(0, EndLinePosition);

		if (StringChain[StartLine].empty() == true) {
			StringChain.erase(StringChain.begin() + (StartLine - 1));
		}
	}

	std::wstring GetTargetLine(int Line) {
		return StringChain[Line - 1];
	}
	std::wstring GetTarget(int Line) {
		return StringChain[Line];
	}

	auto         Begin() -> decltype(StringChain.begin()) {
		return StringChain.begin();
	}
	auto         End() -> decltype(StringChain.end()) {
		return StringChain.end();
	}

	int          Size() {
		return StringChain.size();
	}
};

/*
 * VInteractiveTextLabel class :
 *	@description  : An User Interactivble Text Label
*/
class VInteractiveTextLabel : public VUIObject {
protected:
	VTextStorage                StringStorage;
	VInteractiveTextLabelTheme* Theme;

	int                         CursorX    = 0;
	int                         CursorY    = 0;

	int                         MouseX     = 0;
	int                         MouseY     = 0;
	
	bool                        ShowCursor = false;
	VTimer                      CursorTimer;

	bool                        UserInteractiving = false;

protected:
	VPoint ConvertMousePositionToPlanePosition(VPoint MousePoint) {
		MousePoint.Offset(-GetX(), -GetY());
		MousePoint.Offset(-5, -7);

		VFont JudgeFont(Theme->FontFamily, Theme->FontSize);

		int          TargetLine = 0;
		std::wstring HeightString;

		while (TargetLine < StringStorage.Size()) {
			if (JudgeFont.GetHeight(HeightString, Theme->FontFormat) > MouseY) {
				break;
			}

			HeightString.append(StringStorage.GetTarget(TargetLine));

			++TargetLine;
		}

		--TargetLine;

		if (TargetLine >= StringStorage.Size()) {
			--TargetLine;
		}

		std::wstring WidthString = StringStorage.GetTarget(TargetLine);
		std::wstring TempWidthString;

		int TargetPosition = 0;

		while (TempWidthString.size() <= WidthString.size()) {
			if (JudgeFont.GetWidth(TempWidthString, Theme->FontFormat) > MouseX) {
				break;
			}

			TempWidthString.push_back(WidthString[TempWidthString.size()]);

			++TargetPosition;
		}

		--TargetPosition;

		return { TargetPosition, TargetLine };
	}

public:
	VInteractiveTextLabel(VUIObject* Parent) : VUIObject(Parent) {
		Theme = new VInteractiveTextLabelTheme(*(static_cast<VInteractiveTextLabelTheme*>(SearchThemeFromParent(VINTERACTIVETEXTLABEL_THEME))));

		CursorTimer.Start(700);
	}

	void SetPlaneString(std::wstring String) {
		StringStorage.SetStringByParser(String);
	}
	void OnPaint(VCanvas* Canvas) override {
		VPainterDevice Device(Canvas);
		VFont          Font(Theme->FontFamily, Theme->FontSize);
		VSolidBrush    TextBrush(Theme->TextColor);
		VSolidBrush    BackgroundBrush(Theme->BackgroundColor);
		VSolidBrush    CursorBrush(Theme->CursorColor);
		VPen           BorderPen(Theme->LineColor);

		std::wstring String;
		unsigned int StringIndex = 1;

		for (auto StringElement = StringStorage.Begin(); StringElement != StringStorage.End(); ++StringElement) {
			if (static_cast<signed int>(StringIndex * Theme->FontSize) > Surface()->Rect.GetHeight()) {
				break;
			}

			String.append(*StringElement);
		}

		Device.FillRectangle(&BorderPen, &BackgroundBrush, { 0, 0, GetWidth(), GetHeight() });
		Device.DrawString(String, &Font, &TextBrush, Theme->FontFormat, { 0, 0, GetWidth(), GetHeight() });

		if (ShowCursor == true) {
			std::wstring TargetLineString = StringStorage.GetTarget(CursorY);
			std::wstring AppendLine;

			TargetLineString = TargetLineString.substr(0, CursorX);

			for (int Count = 0; Count < CursorY; ++Count) {
				AppendLine.append(StringStorage.GetTarget(Count));
			}

			for (int Count = 0; Count < AppendLine.size(); ++Count) {
				if (AppendLine[Count] >= L'A' && AppendLine[Count] <= L'Z') {
					AppendLine[Count] += 32;
				}
				if (AppendLine[Count] == L'y' || AppendLine[Count] == L'g' ||
					AppendLine[Count] == L'j' || AppendLine[Count] == L'q' ||
					AppendLine[Count] == L'p') {
					AppendLine[Count] = L'a';
				}
			}

			int StringWidth  = Font.GetWidth(TargetLineString, Theme->FontFormat);
			int StringHeight = Font.GetHeight(AppendLine, Theme->FontFormat);

			if (StringHeight != 0) {
				Device.SolidRectangle(&CursorBrush, { StringWidth + 4, StringHeight 
					 + 13
					, StringWidth + 5, StringHeight + 13 + Theme->FontSize });
			}
			else {
				Device.SolidRectangle(&CursorBrush, { StringWidth + 4, StringHeight + 2, StringWidth + 5, StringHeight + Theme->FontSize + 3 });
			}
		}
	}

	void MouseLeftClicked(VMouseClickedFlag Flag) override {
		if (Flag == VMouseClickedFlag::Up) {
			UserInteractiving = true;

			LockGlobalFocusID(Kernel()->GlobalID);

			auto Position = ConvertMousePositionToPlanePosition({MouseX, MouseY});

			CursorX = Position.x;
			CursorY = Position.y;

			CursorTimer.Start(700);
			ShowCursor = true;

			Update();
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
					UserInteractiving = false;

					UnlockGlobalFocusID();

					SendMessageToGlobalWidget(Message);
				}
			}

			MouseX = MouseClickedMessage->MousePosition.x;
			MouseY = MouseClickedMessage->MousePosition.y;

			break;
		}
		case VMessageType::MouseMoveMessage: {
			VMouseMoveMessage* MouseMoveMessage = static_cast<VMouseMoveMessage*>(Message);

			MouseX = MouseMoveMessage->MousePosition.x;
			MouseY = MouseMoveMessage->MousePosition.y;

			break;
		}
		case VMessageType::KeyClickedMessage: {
			if (UserInteractiving == true) {
				VKeyClickedMessage* ClickedMessage = static_cast<VKeyClickedMessage*>(Message);

				if (ClickedMessage->KeyPrevDown == false) {
					if (ClickedMessage->KeyVKCode == VK_LEFT) {
						if (CursorX - 1 >= 0) {
							--CursorX;
						}
					}
					if (ClickedMessage->KeyVKCode == VK_RIGHT) {
						std::wstring TargetLine = StringStorage.GetTarget(CursorY);

						if (CursorX + 1 <= TargetLine.size()) {
							++CursorX;
						}
					}
					if (ClickedMessage->KeyVKCode == VK_UP) {
						if (CursorY - 1 >= 0) {
							--CursorY;

							if (CursorX > StringStorage.GetTarget(CursorY).size()) {
								CursorX = StringStorage.GetTarget(CursorY).size() - 1;
							}
						}
					}
					if (ClickedMessage->KeyVKCode == VK_DOWN) {
						if (CursorY + 1 < StringStorage.Size()) {
							++CursorY;

							if (CursorX > StringStorage.GetTarget(CursorY).size()) {
								CursorX = StringStorage.GetTarget(CursorY).size() - 1;
							}
						}
					}
					if (ClickedMessage->KeyVKCode == VK_END) {
						CursorX = StringStorage.GetTarget(CursorY).size();
					}
					if (ClickedMessage->KeyVKCode == VK_HOME) {
						CursorX = 0;
					}
				}

				CursorTimer.Start(700);
				ShowCursor = true;

				Update();
			}

			break;
		}
		}
	}

	void CheckFrame() override {
		if (UserInteractiving == true) {
			if (CursorTimer.End() == true) {
				CursorTimer.Start(700);

				if (ShowCursor == true) {
					ShowCursor = false;
				}
				else {
					ShowCursor = true;
				}

				Update();
			}
		}
		else {
			if (ShowCursor == true) {
				ShowCursor = false;

				Update();
			}
		}
	}
};

VLIB_END_NAMESPACE