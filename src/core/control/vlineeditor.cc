#pragma warning(disable : 4018)
#pragma warning(disable : 6530)

#include "../../../include/core/control/vlineeditor.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
VLineEditor::VLineEditor(VUIObject *Parent) : VAbstractButton(Parent)
{
	Theme = new VTextEditorTheme(*(static_cast<VTextEditorTheme *>(GetTargetTheme(VUIThemeType::VTextEditor))));

	Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);
}
VLineEditor::VLineEditor(const int &Width, const int &Height, VUIObject *Parent) : VAbstractButton(Parent)
{
	Theme = new VTextEditorTheme(*(static_cast<VTextEditorTheme *>(GetTargetTheme(VUIThemeType::VTextEditor))));

	Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

	Resize(Width, Height);
}
VLineEditor::VLineEditor(const int &Width, const int &Height, VString PlainText, VUIObject *Parent)
	: VAbstractButton(Parent)
{
	Theme = new VTextEditorTheme(*(static_cast<VTextEditorTheme *>(GetTargetTheme(VUIThemeType::VTextEditor))));

	Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

	InputStringCache = PlainText;

	Resize(Width, Height);
}

void VLineEditor::Move(const int &X, const int &Y)
{
	VUIObject::Move(X, Y);

	if (InTyping)
	{
		CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
						 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);
	}
}

void VLineEditor::CheckFrame()
{
	if (!Interpolator->IsEnd() && InAnimation)
	{
		if (AnimationFrameTimer.End())
		{
			AnimationFrameTimer.Start(16);

			auto AnimationCurvature = Interpolator->GetOneFrame();

			Theme->LocalTheme.BorderThickness =
				TargetTheme.BorderThickness +
				(OldTheme.BorderThickness - TargetTheme.BorderThickness) * (1.f - AnimationCurvature);

			Theme->LocalTheme.TextColor =
				VColorInterpolator::GetTheColorByPercent(OldTheme.TextColor, TargetTheme.TextColor, AnimationCurvature);
			Theme->LocalTheme.BackgroundColor = VColorInterpolator::GetTheColorByPercent(
				OldTheme.BackgroundColor, TargetTheme.BackgroundColor, AnimationCurvature);
			Theme->LocalTheme.BorderColor = VColorInterpolator::GetTheColorByPercent(
				OldTheme.BorderColor, TargetTheme.BorderColor, AnimationCurvature);

			Theme->LocalTheme.Radius.X =
				TargetTheme.Radius.X + (OldTheme.Radius.X - TargetTheme.Radius.X) * (1.f - AnimationCurvature);
			Theme->LocalTheme.Radius.Y =
				TargetTheme.Radius.Y + (OldTheme.Radius.Y - TargetTheme.Radius.Y) * (1.f - AnimationCurvature);

			Update();
		}
	}
	else if (InAnimation)
	{
		InAnimation = false;
	}

	if (CursorTimer.End() && InTyping)
	{
		ShowCursor = !ShowCursor;

		CursorTimer.Start(600);

		Update();
	}
}

void VLineEditor::OnMessage(VMessage *Message)
{
	if (Message->GetType() == VMessageType::KillFocusMessage)
	{
		CallWidgetUnlockFocusID();

		InTyping = false;
		LostMouseFocus();

		return;
	}

	if (Message->GetType() == VMessageType::MouseMoveMessage)
	{
		VMouseMoveMessage *MouseMoveMessage = static_cast<VMouseMoveMessage *>(Message);

		if (MouseMoveMessage->MousePosition.InsideRectangle(GetRegion()))
		{
			HCURSOR ArrowCursor = LoadCursor(NULL, IDC_IBEAM);
			SetClassLongPtr(CallWidgetGetHWND(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(ArrowCursor));

			InCursorStyle = true;
		}
		else if (InCursorStyle)
		{
			InCursorStyle = false;

			HCURSOR ArrowCursor = LoadCursor(NULL, IDC_ARROW);
			SetClassLongPtr(CallWidgetGetHWND(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(ArrowCursor));
		}
	}
	if (Message->GetType() == VMessageType::MouseClickedMessage)
	{
		VMouseClickedMessage *MouseMessage = static_cast<VMouseClickedMessage *>(Message);

		if (!MouseMessage->MousePosition.InsideRectangle(GetRegion()) && InTyping)
		{
			if (ShiftOnHold && InSelectMode)
			{
				return;
			}
			if (!ShiftOnHold && InSelectMode)
			{
				InSelectMode = false;

				TextSelectRange.startPosition = 0;
				TextSelectRange.length		  = 0;

				Update();
			}

			CallWidgetUnlockFocusID();

			VMouseClickedMessage *RestoreMessage = new VMouseClickedMessage(*MouseMessage);

			InTyping = false;
			LostMouseFocus();

			CallWidgetSendMessage(RestoreMessage);

			delete RestoreMessage;

			return;
		}
		if (MouseMessage->MousePosition.InsideRectangle(GetRegion()) &&
			MouseMessage->ClickedMethod == VMouseClickedFlag::Down && MouseMessage->ClickedKey == VMouseKeyFlag::Left)
		{
			if (!ShiftOnHold && InSelectMode)
			{
				InSelectMode = false;

				TextSelectRange.startPosition = 0;
				TextSelectRange.length		  = 0;
			}

			IDWriteTextLayout *TextLayout;

			VLIB_REPORT_IF_FAILED(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
				InputStringCache.CStyleString(), InputStringCache.size(), Theme->LabelFont->GetDXObject(), INT_MAX,
				GetHeight(), &TextLayout));

			auto MouseRelativeX =
				MouseMessage->MousePosition.X - GetOriginX() - Theme->LocalTheme.BorderThickness - OffsetX;
			auto MouseRelativeY = MouseMessage->MousePosition.Y - GetOriginY();

			DWRITE_HIT_TEST_METRICS HitTestMetrics;

			BOOL IsTrailingHit;
			BOOL IsInside;

			TextLayout->HitTestPoint(static_cast<FLOAT>(MouseRelativeX), static_cast<FLOAT>(MouseRelativeY),
									 &IsTrailingHit, &IsInside, &HitTestMetrics);

			auto OldCursorPosition = CursorPosition;

			if (CursorPosition != 0)
			{
				CursorPosition	= HitTestMetrics.textPosition;
				CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

				DWRITE_TEXT_METRICS TextMetrics;
				TextLayout->GetMetrics(&TextMetrics);

				OffsetX =
					TextMetrics.widthIncludingTrailingWhitespace < GetWidth()
						? 0
						: static_cast<int>(GetWidth() - TextMetrics.widthIncludingTrailingWhitespace - TextMetrics.top);
				WidthOffset = 0;
			}
			else
			{
				CursorPosition = -1;
				OffsetX		   = 0;

				DWRITE_TEXT_METRICS StringMetrics;

				TextLayout->GetMetrics(&StringMetrics);

				WidthOffset = StringMetrics.top + StringMetrics.width;

				CursorGraphicsX = Theme->LocalTheme.BorderThickness;

				ShowCursor = true;

				CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
								 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);
			}

			VDXObjectSafeFree(&TextLayout);

			Update();
		}
	}
	if (Message->GetType() == VMessageType::KeyClickedMessage && InTyping)
	{
		VKeyClickedMessage *KeyMessage = static_cast<VKeyClickedMessage *>(Message);

		if (KeyMessage->KeyVKCode == VK_LEFT && KeyMessage->KeyStats == VkeyClickedFlag::Down)
		{
			if (!ShiftOnHold && InSelectMode)
			{
				InSelectMode = false;

				Update();

				CursorPosition = TextSelectRange.startPosition - 1;

				TextSelectRange.startPosition = 0;
				TextSelectRange.length		  = 0;

				if (CursorPosition != -1)
				{
					IDWriteTextLayout *TextLayout;

					VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
										  InputStringCache.CStyleString(), InputStringCache.size(),
										  Theme->LabelFont->GetDXObject(), INT_MAX, GetHeight(), &TextLayout),
									  L"Failed to create TextLayout object!");

					DWRITE_HIT_TEST_METRICS HitTestMetrics;

					FLOAT CursorX;
					FLOAT CursorY;

					TextLayout->HitTestTextPosition(CursorPosition, FALSE, &CursorX, &CursorY, &HitTestMetrics);

					CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

					if (CursorGraphicsX > GetWidth())
					{
						OffsetX = (HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top) < GetWidth()
									  ? 0
									  : static_cast<int>(GetWidth() - (HitTestMetrics.width + HitTestMetrics.left +
																	   HitTestMetrics.top));

						WidthOffset = HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top;
					}
					else
					{
						DWRITE_TEXT_METRICS
						StringMetrics;

						TextLayout->GetMetrics(&StringMetrics);

						OffsetX		= 0;
						WidthOffset = StringMetrics.top + StringMetrics.width;
					}

					VDXObjectSafeFree(&TextLayout);
				}
				else
				{
					if (!ShiftOnHold && InSelectMode)
					{
						InSelectMode = false;

						TextSelectRange.startPosition = 0;
						TextSelectRange.length		  = 0;

						Update();
					}

					CursorPosition = -1;
					OffsetX		   = 0;

					IDWriteTextLayout *TextLayout;

					VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
										  InputStringCache.CStyleString(), InputStringCache.size(),
										  Theme->LabelFont->GetDXObject(), INT_MAX, GetHeight(), &TextLayout),
									  L"Failed to create TextLayout object!");

					DWRITE_TEXT_METRICS StringMetrics;

					TextLayout->GetMetrics(&StringMetrics);

					WidthOffset = StringMetrics.top + StringMetrics.width;

					CursorGraphicsX = Theme->LocalTheme.BorderThickness;

					ShowCursor = true;

					VDXObjectSafeFree(&TextLayout);
				}

				CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
								 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);
				Update();

				return;
			}

			if (SelectedUsedKey == VLineEditorUsedKey::None)
			{
				SelectedUsedKey = VLineEditorUsedKey::Left;
			}

			if (CursorPosition - 1 >= -1)
			{
				if (CursorPosition - 1 == -1)
				{
					CursorPosition = -1;
					OffsetX		   = 0;

					IDWriteTextLayout *TextLayout;

					VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
										  InputStringCache.CStyleString(), InputStringCache.size(),
										  Theme->LabelFont->GetDXObject(), INT_MAX, GetHeight(), &TextLayout),
									  L"Failed to create TextLayout object!");

					DWRITE_TEXT_METRICS StringMetrics;

					TextLayout->GetMetrics(&StringMetrics);

					WidthOffset = StringMetrics.top + StringMetrics.width;

					CursorGraphicsX = Theme->LocalTheme.BorderThickness;

					ShowCursor = true;

					VDXObjectSafeFree(&TextLayout);

					CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
									 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

					Update();
				}
				else
				{
					ShowCursor = true;

					Update();

					if (CursorPosition == InputStringCache.size())
					{
						--CursorPosition;
					}

					--CursorPosition;

					IDWriteTextLayout *TextLayout;

					VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
										  InputStringCache.CStyleString(), InputStringCache.size(),
										  Theme->LabelFont->GetDXObject(), INT_MAX, GetHeight(), &TextLayout),
									  L"Failed to create TextLayout object!");

					DWRITE_HIT_TEST_METRICS HitTestMetrics;

					FLOAT CursorX;
					FLOAT CursorY;

					TextLayout->HitTestTextPosition(CursorPosition, FALSE, &CursorX, &CursorY, &HitTestMetrics);

					CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

					if (CursorGraphicsX > GetWidth())
					{
						OffsetX = (HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top) < GetWidth()
									  ? 0
									  : static_cast<int>(GetWidth() - (HitTestMetrics.width + HitTestMetrics.left +
																	   HitTestMetrics.top));

						WidthOffset = HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top;
					}
					else
					{
						DWRITE_TEXT_METRICS
						StringMetrics;

						TextLayout->GetMetrics(&StringMetrics);

						OffsetX		= 0;
						WidthOffset = StringMetrics.top + StringMetrics.width;
					}

					VDXObjectSafeFree(&TextLayout);

					CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
									 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);
				}

				if (InSelectMode)
				{
					if (SelectedUsedKey == VLineEditorUsedKey::Left || SelectedUsedKey == VLineEditorUsedKey::Home)
					{
						TextSelectRange.startPosition = CursorPosition + 1;
						TextSelectRange.length		  = TextSelectRange.length + 1;

						SelectedUsedKey = VLineEditorUsedKey::Left;
					}
					else
					{
						TextSelectRange.length -= 1;

						if (TextSelectRange.length == 0)
						{
							SelectedUsedKey = VLineEditorUsedKey::Left;
						}
					}
				}
			}
		}
		if (KeyMessage->KeyVKCode == VK_RIGHT && KeyMessage->KeyStats == VkeyClickedFlag::Down)
		{
			if (!ShiftOnHold && InSelectMode)
			{
				InSelectMode = false;

				CursorPosition = TextSelectRange.startPosition + TextSelectRange.length;

				TextSelectRange.startPosition = 0;
				TextSelectRange.length		  = 0;

				IDWriteTextLayout *TextLayout;

				VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
									  InputStringCache.CStyleString(), InputStringCache.size(),
									  Theme->LabelFont->GetDXObject(), INT_MAX, GetHeight(), &TextLayout),
								  L"Failed to create TextLayout object!");

				DWRITE_HIT_TEST_METRICS HitTestMetrics;

				FLOAT CursorX;
				FLOAT CursorY;

				TextLayout->HitTestTextPosition(CursorPosition, FALSE, &CursorX, &CursorY, &HitTestMetrics);

				CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

				if (CursorGraphicsX > GetWidth())
				{
					OffsetX = (HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top) < GetWidth()
								  ? 0
								  : static_cast<int>(GetWidth() -
													 (HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top));

					WidthOffset = HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top;
				}
				else
				{
					DWRITE_TEXT_METRICS StringMetrics;

					TextLayout->GetMetrics(&StringMetrics);

					OffsetX		= 0;
					WidthOffset = StringMetrics.top + StringMetrics.width;
				}

				VDXObjectSafeFree(&TextLayout);

				CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
								 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

				Update();

				return;
			}

			if (CursorPosition + 1 <= InputStringCache.size())
			{
				if (SelectedUsedKey == VLineEditorUsedKey::None)
				{
					SelectedUsedKey = VLineEditorUsedKey::Right;
				}
				ShowCursor = true;

				Update();

				++CursorPosition;

				if (InSelectMode)
				{
					if (TextSelectRange.length != 0)
					{
						if (TextSelectRange.startPosition > CursorPosition && TextSelectRange.length != 0)
						{
							TextSelectRange.length += 1;
						}
						else if (SelectedUsedKey == VLineEditorUsedKey::Right ||
								 SelectedUsedKey == VLineEditorUsedKey::End)
						{
							TextSelectRange.length += 1;
						}
						else
						{
							TextSelectRange.startPosition += 1;
							TextSelectRange.length -= 1;

							if (TextSelectRange.length == 0)
							{
								SelectedUsedKey = VLineEditorUsedKey::Right;
							}
						}
					}
					else
					{
						TextSelectRange.startPosition = CursorPosition;
						TextSelectRange.length		  = 1;
					}
				}

				IDWriteTextLayout *TextLayout;

				VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
									  InputStringCache.CStyleString(), InputStringCache.size(),
									  Theme->LabelFont->GetDXObject(), INT_MAX, GetHeight(), &TextLayout),
								  L"Failed to create TextLayout object!");

				DWRITE_HIT_TEST_METRICS HitTestMetrics;

				FLOAT CursorX;
				FLOAT CursorY;

				TextLayout->HitTestTextPosition(CursorPosition, FALSE, &CursorX, &CursorY, &HitTestMetrics);

				DWRITE_TEXT_METRICS TextMetrics;
				TextLayout->GetMetrics(&TextMetrics);

				CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

				OffsetX = (HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top) < GetWidth()
							  ? 0
							  : static_cast<int>(GetWidth() -
												 (HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top));

				if (CursorGraphicsX > GetWidth())
				{
					WidthOffset = HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top;
				}
				else
				{
					DWRITE_TEXT_METRICS StringMetrics;

					TextLayout->GetMetrics(&StringMetrics);

					WidthOffset = StringMetrics.widthIncludingTrailingWhitespace + StringMetrics.top;
				}

				VDXObjectSafeFree(&TextLayout);

				CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
								 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);
			}
		}
		if (KeyMessage->KeyVKCode == VK_DELETE && KeyMessage->KeyStats == VkeyClickedFlag::Down)
		{
			if (InSelectMode && !ShiftOnHold)
			{
				InSelectMode = false;

				InputStringCache.Erase(TextSelectRange.startPosition, TextSelectRange.length);

				CursorPosition = TextSelectRange.startPosition;

				TextSelectRange.startPosition = 0;
				TextSelectRange.length		  = 0;

				ShowCursor = true;

				IDWriteTextLayout *TextLayout;

				VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
									  InputStringCache.CStyleString(), InputStringCache.size(),
									  Theme->LabelFont->GetDXObject(), INT_MAX, GetHeight(), &TextLayout),
								  L"Failed to create TextLayout object!");

				DWRITE_HIT_TEST_METRICS HitTestMetrics;

				FLOAT CursorX;
				FLOAT CursorY;

				TextLayout->HitTestTextPosition(CursorPosition, FALSE, &CursorX, &CursorY, &HitTestMetrics);

				if (CursorPosition != 0)
				{
					CursorPosition	= HitTestMetrics.textPosition;
					CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

					DWRITE_TEXT_METRICS TextMetrics;
					TextLayout->GetMetrics(&TextMetrics);

					OffsetX		= TextMetrics.widthIncludingTrailingWhitespace < GetWidth()
									  ? 0
									  : static_cast<int>(GetWidth() - TextMetrics.widthIncludingTrailingWhitespace -
													 TextMetrics.top);
					WidthOffset = 0;
				}
				else
				{
					CursorPosition = -1;
					OffsetX		   = 0;

					DWRITE_TEXT_METRICS StringMetrics;

					TextLayout->GetMetrics(&StringMetrics);

					WidthOffset = StringMetrics.top + StringMetrics.width;

					CursorGraphicsX = Theme->LocalTheme.BorderThickness;

					ShowCursor = true;
				}

				CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
								 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

				Update();

				TextOnChange.Emit(InputStringCache);

				return;
			}
			if (CursorPosition + 1 < InputStringCache.size())
			{
				ShowCursor = true;

				Update();

				if (CursorPosition > -1)
				{
					InputStringCache.Erase(InputStringCache.begin() + CursorPosition + 1);
				}
				else
				{
					InputStringCache.Erase(InputStringCache.begin());
				}

				TextOnChange.Emit(InputStringCache);
			}
		}
		if (KeyMessage->KeyVKCode == VK_END && KeyMessage->KeyStats == VkeyClickedFlag::Down)
		{
			if (ShiftOnHold && InSelectMode)
			{
				if (CursorPosition != -1)
				{
					TextSelectRange.startPosition = CursorPosition + 1;

					TextSelectRange.length = InputStringCache.size() - CursorPosition - 1;
				}
				else
				{
					TextSelectRange.startPosition = TextSelectRange.length;
					TextSelectRange.length		  = InputStringCache.size() - TextSelectRange.length;
				}

				SelectedUsedKey = VLineEditorUsedKey::End;

				Update();
			}
			if (!ShiftOnHold && InSelectMode)
			{
				InSelectMode = false;

				TextSelectRange.startPosition = 0;
				TextSelectRange.length		  = 0;

				Update();
			}

			CursorPosition = InputStringCache.size() - 1;

			ShowCursor = true;

			IDWriteTextLayout *TextLayout;

			VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
								  InputStringCache.CStyleString(), InputStringCache.size(),
								  Theme->LabelFont->GetDXObject(), INT_MAX, GetHeight(), &TextLayout),
							  L"Failed to create TextLayout object!");

			DWRITE_HIT_TEST_METRICS HitTestMetrics;

			FLOAT CursorX;
			FLOAT CursorY;

			TextLayout->HitTestTextPosition(CursorPosition, FALSE, &CursorX, &CursorY, &HitTestMetrics);

			CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

			DWRITE_TEXT_METRICS TextMetrics;
			TextLayout->GetMetrics(&TextMetrics);

			OffsetX =
				TextMetrics.widthIncludingTrailingWhitespace < GetWidth()
					? 0
					: static_cast<int>(GetWidth() - TextMetrics.widthIncludingTrailingWhitespace - TextMetrics.top);
			WidthOffset = 0;

			VDXObjectSafeFree(&TextLayout);

			CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
							 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

			Update();
		}
		if (KeyMessage->KeyVKCode == 0x56 && KeyMessage->KeyStats == VkeyClickedFlag::Down)
		{
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && IsClipboardFormatAvailable(CF_TEXT) &&
				OpenClipboard(CallWidgetGetHWND()))
			{
				KeyboardUsed = true;

				HANDLE ClipBoardHandle;
				char  *ClipBoardBuffer;

				ClipBoardHandle = GetClipboardData(CF_TEXT);
				ClipBoardBuffer = (char *)GlobalLock(ClipBoardHandle);

				GlobalUnlock(ClipBoardHandle);
				CloseClipboard();

				int		 ConvertedLength = MultiByteToWideChar(CP_ACP, 0, ClipBoardBuffer, -1, NULL, 0);
				wchar_t *WideString		 = new wchar_t[ConvertedLength];

				MultiByteToWideChar(CP_ACP, 0, ClipBoardBuffer, -1, WideString, ConvertedLength);

				VString TestString = InputStringCache;

				if (CursorPosition == -1)
				{
					TestString.Insert(0, WideString);
				}
				else if (CursorPosition < InputStringCache.size())
				{
					TestString.Insert(CursorPosition + 1, WideString);
				}
				else
				{
					TestString.Insert(CursorPosition, WideString);
				}

				IDWriteTextLayout *TextLayout;

				VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
									  TestString.CStyleString(), TestString.size(), Theme->LabelFont->GetDXObject(),
									  INT_MAX, GetHeight(), &TextLayout),
								  L"Failed to create TextLayout object!");

				DWRITE_TEXT_METRICS TestMetrics;
				TextLayout->GetMetrics(&TestMetrics);

				if (TestString.IndexOf(L"\n") == -1)
				{
					InputStringCache = TestString;

					CursorPosition += ConvertedLength - 1;

					ShowCursor = true;

					Update();

					DWRITE_HIT_TEST_METRICS HitTestMetrics;

					FLOAT CursorX;
					FLOAT CursorY;

					TextLayout->HitTestTextPosition(CursorPosition, FALSE, &CursorX, &CursorY, &HitTestMetrics);

					CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

					DWRITE_TEXT_METRICS TextMetrics;
					TextLayout->GetMetrics(&TextMetrics);

					OffsetX = TextMetrics.widthIncludingTrailingWhitespace < GetWidth()
								  ? 0
								  : static_cast<int>(GetWidth() - TextMetrics.widthIncludingTrailingWhitespace -
													 TextMetrics.top);

					delete[] WideString;

					CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness,
									 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

					TextOnChange.Emit(InputStringCache);
				}

				VDXObjectSafeFree(&TextLayout);
			}
		}
		if (KeyMessage->KeyVKCode == 0x41 && KeyMessage->KeyStats == VkeyClickedFlag::Down)
		{
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
			{
				KeyboardUsed = true;
				InSelectMode = true;
				ShiftOnHold	 = false;

				TextSelectRange.length		  = InputStringCache.size();
				TextSelectRange.startPosition = 0;

				CursorPosition = -1;
				OffsetX		   = 0;

				IDWriteTextLayout *TextLayout;

				VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
									  InputStringCache.CStyleString(), InputStringCache.size(),
									  Theme->LabelFont->GetDXObject(), INT_MAX, GetHeight(), &TextLayout),
								  L"Failed to create TextLayout object!");

				DWRITE_TEXT_METRICS StringMetrics;

				TextLayout->GetMetrics(&StringMetrics);

				WidthOffset = StringMetrics.top + StringMetrics.width;

				CursorGraphicsX = Theme->LocalTheme.BorderThickness;

				ShowCursor = true;

				VDXObjectSafeFree(&TextLayout);

				CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
								 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

				Update();

				SelectedUsedKey = VLineEditorUsedKey::None;
			}
		}
		if (KeyMessage->KeyVKCode == 0x43 && KeyMessage->KeyStats == VkeyClickedFlag::Down)
		{
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && InSelectMode && OpenClipboard(CallWidgetGetHWND()))
			{
				KeyboardUsed = true;

				VString ClipString;
				ClipString = InputStringCache.Split(
					TextSelectRange.startPosition > 0 ? TextSelectRange.startPosition - 1 : 0, TextSelectRange.length);

				std::string ANSIString;

				int ConvertedLength =
					WideCharToMultiByte(CP_ACP, 0, ClipString.CStyleString(), -1, NULL, 0, NULL, NULL);

				ANSIString.resize(ConvertedLength);
				WideCharToMultiByte(CP_ACP, 0, ClipString.CStyleString(), -1, &ANSIString[0], (int)ANSIString.size(),
									NULL, NULL);

				HANDLE ClipBoardHandle;
				char  *ClipBoardBuffer;

				EmptyClipboard();

				ClipBoardHandle = GlobalAlloc(GMEM_MOVEABLE, ANSIString.size() + 1);
				ClipBoardBuffer = (char *)GlobalLock(ClipBoardHandle);

				strcpy_s(ClipBoardBuffer, ANSIString.size() + 1, ANSIString.c_str());

				GlobalUnlock(ClipBoardHandle);
				SetClipboardData(CF_TEXT, ClipBoardHandle);

				CloseClipboard();
			}
		}
		if (KeyMessage->KeyVKCode == VK_SHIFT && KeyMessage->KeyStats == VkeyClickedFlag::Down)
		{
			if (!ShiftOnHold)
			{
				InSelectMode = true;
				ShiftOnHold	 = true;

				SelectedUsedKey = VLineEditorUsedKey::None;
			}
		}
		if (KeyMessage->KeyVKCode == VK_SHIFT && KeyMessage->KeyStats == VkeyClickedFlag::Up)
		{
			ShiftOnHold = false;
		}
		if (KeyMessage->KeyVKCode == VK_HOME && KeyMessage->KeyStats == VkeyClickedFlag::Down)
		{
			if (ShiftOnHold && InSelectMode)
			{
				TextSelectRange.startPosition = 0;

				int SelectLength = abs(static_cast<int>(TextSelectRange.length) - CursorPosition);

				TextSelectRange.length =
					SelectLength == InputStringCache.size() + 1 ? SelectLength + 2 : SelectLength + 1;

				SelectedUsedKey = VLineEditorUsedKey::Home;

				Update();
			}
			if (!ShiftOnHold && InSelectMode)
			{
				InSelectMode = false;

				TextSelectRange.startPosition = 0;
				TextSelectRange.length		  = 0;

				Update();
			}

			CursorPosition = -1;
			OffsetX		   = 0;

			IDWriteTextLayout *TextLayout;

			VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
								  InputStringCache.CStyleString(), InputStringCache.size(),
								  Theme->LabelFont->GetDXObject(), INT_MAX, GetHeight(), &TextLayout),
							  L"Failed to create TextLayout object!");

			DWRITE_TEXT_METRICS StringMetrics;

			TextLayout->GetMetrics(&StringMetrics);

			WidthOffset = StringMetrics.top + StringMetrics.width;

			CursorGraphicsX = Theme->LocalTheme.BorderThickness;

			ShowCursor = true;

			VDXObjectSafeFree(&TextLayout);

			CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
							 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

			Update();
		}
	}
	if (Message->GetType() == VMessageType::IMECharMessage && InTyping)
	{
		VIMECharMessage *IMECharMessage = static_cast<VIMECharMessage *>(Message);

		if (KeyboardUsed)
		{
			KeyboardUsed = false;

			return;
		}

		if (IMECharMessage->IMEChar == L'\n' || IMECharMessage->IMEChar == L'\r')
		{
			CallWidgetUnlockFocusID();

			InTyping = false;
			LostMouseFocus();

			return;
		}
		if (IMECharMessage->IMEChar == L'\b')
		{
			if (InSelectMode && !ShiftOnHold)
			{
				InSelectMode = false;

				CursorPosition = TextSelectRange.startPosition;

				InputStringCache.Erase(TextSelectRange.startPosition, TextSelectRange.length);

				TextSelectRange.startPosition = 0;
				TextSelectRange.length		  = 0;

				ShowCursor = true;

				IDWriteTextLayout *TextLayout;

				VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
									  InputStringCache.CStyleString(), InputStringCache.size(),
									  Theme->LabelFont->GetDXObject(), INT_MAX, GetHeight(), &TextLayout),
								  L"Failed to create TextLayout object!");

				DWRITE_HIT_TEST_METRICS HitTestMetrics;

				FLOAT CursorX;
				FLOAT CursorY;

				TextLayout->HitTestTextPosition(CursorPosition, FALSE, &CursorX, &CursorY, &HitTestMetrics);

				if (CursorPosition != 0)
				{
					CursorPosition	= HitTestMetrics.textPosition;
					CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

					DWRITE_TEXT_METRICS TextMetrics;
					TextLayout->GetMetrics(&TextMetrics);

					OffsetX		= TextMetrics.widthIncludingTrailingWhitespace < GetWidth()
									  ? 0
									  : static_cast<int>(GetWidth() - TextMetrics.widthIncludingTrailingWhitespace -
													 TextMetrics.top);
					WidthOffset = 0;
				}
				else
				{
					CursorPosition = -1;
					OffsetX		   = 0;

					DWRITE_TEXT_METRICS StringMetrics;

					TextLayout->GetMetrics(&StringMetrics);

					WidthOffset = StringMetrics.top + StringMetrics.width;

					CursorGraphicsX = Theme->LocalTheme.BorderThickness;

					ShowCursor = true;
				}

				CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
								 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

				Update();

				TextOnChange.Emit(InputStringCache);

				return;
			}
			if (CursorPosition >= 0)
			{
				ShowCursor = true;

				Update();

				InputStringCache.Erase(InputStringCache.begin() + CursorPosition);

				--CursorPosition;

				IDWriteTextLayout *TextLayout;

				VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
									  InputStringCache.CStyleString(), InputStringCache.size(),
									  Theme->LabelFont->GetDXObject(), INT_MAX, GetHeight(), &TextLayout),
								  L"Failed to create TextLayout object!");

				DWRITE_HIT_TEST_METRICS HitTestMetrics;

				FLOAT CursorX;
				FLOAT CursorY;

				TextLayout->HitTestTextPosition(CursorPosition, FALSE, &CursorX, &CursorY, &HitTestMetrics);

				CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

				DWRITE_TEXT_METRICS TextMetrics;
				TextLayout->GetMetrics(&TextMetrics);

				OffsetX =
					TextMetrics.widthIncludingTrailingWhitespace < GetWidth()
						? 0
						: static_cast<int>(GetWidth() - TextMetrics.widthIncludingTrailingWhitespace - TextMetrics.top);

				VDXObjectSafeFree(&TextLayout);

				CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
								 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

				TextOnChange.Emit(InputStringCache);
			}

			return;
		}

		if (InSelectMode)
		{
			if (TextSelectRange.length != 0)
			{
				InSelectMode = false;

				InputStringCache.Erase(TextSelectRange.startPosition, TextSelectRange.length);

				CursorPosition = TextSelectRange.startPosition;

				TextSelectRange.startPosition = 0;
				TextSelectRange.length		  = 0;
			}
			else
			{
				TextSelectRange.startPosition = 0;
				TextSelectRange.length		  = 0;
			}
		}

		if (CursorPosition == -1)
		{
			InputStringCache.Insert(InputStringCache.begin(), IMECharMessage->IMEChar);
		}
		else if (CursorPosition < InputStringCache.size())
		{
			InputStringCache.Insert(InputStringCache.begin() + CursorPosition + 1, IMECharMessage->IMEChar);
		}
		else
		{
			InputStringCache.Insert(InputStringCache.begin() + CursorPosition, IMECharMessage->IMEChar);
		}

		IDWriteTextLayout *TestTextLayout;

		VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
							  InputStringCache.CStyleString(), InputStringCache.size(), Theme->LabelFont->GetDXObject(),
							  INT_MAX, GetHeight(), &TestTextLayout),
						  L"Failed to create TextLayout object!");

		DWRITE_TEXT_METRICS TestMetrics;
		TestTextLayout->GetMetrics(&TestMetrics);

		++CursorPosition;

		ShowCursor = true;

		DWRITE_HIT_TEST_METRICS HitTestMetrics;

		FLOAT CursorX;
		FLOAT CursorY;

		TestTextLayout->HitTestTextPosition(CursorPosition, FALSE, &CursorX, &CursorY, &HitTestMetrics);

		CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

		DWRITE_TEXT_METRICS TextMetrics;
		TestTextLayout->GetMetrics(&TextMetrics);

		OffsetX = TextMetrics.widthIncludingTrailingWhitespace < GetWidth()
					  ? 0
					  : static_cast<int>(GetWidth() - TextMetrics.widthIncludingTrailingWhitespace - TextMetrics.top);

		TextOnChange.Emit(InputStringCache);

		CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
						 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

		Update();

		VDXObjectSafeFree(&TestTextLayout);
	}
}

void VLineEditor::LeftClickedDown()
{
	InAnimation = true;

	OldTheme	= Theme->LocalTheme;
	TargetTheme = Theme->ActiveTheme;

	Interpolator->Reset();
	AnimationFrameTimer.Start(0);

	StartedType.Emit();

	CallWidgetSetFocusID(ObjectKernel.GlobalID);
	CallWidgetLockFocusID();
	CallWidgetSetIME(GetOriginX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
					 GetOriginY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

	ShowCursor = true;

	CursorTimer.Start(600);

	InTyping = true;

	Update();
}
void VLineEditor::LeftClickedUp()
{
	if (!InTyping)
	{
		InAnimation = true;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->OnHoverTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);

		Update();
	}
}
void VLineEditor::GotMouseFocus()
{
	if (!InTyping)
	{
		InAnimation = true;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->OnHoverTheme;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);

		Update();
	}
}
void VLineEditor::LostMouseFocus()
{
	if (!InTyping)
	{
		InAnimation = true;

		ShiftOnHold	 = false;
		InSelectMode = false;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->StaticTheme;

		TextSelectRange.length		  = 0;
		TextSelectRange.startPosition = 0;

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);

		EndType.Emit();

		Update();
	}
}

void VLineEditor::SetPlainText(const VString &PlainText)
{
	InputStringCache = PlainText;

	CursorPosition	= 0;
	CursorGraphicsX = 0;
	OffsetX			= 0;

	Update();
}

const VString VLineEditor::GetPlainText()
{
	return InputStringCache;
}
const int VLineEditor::GetCurrentCursorPosition()
{
	return CursorPosition;
}
void VLineEditor::SetLeadText(const VString &Text)
{
	LeadText = Text;
}

VTextEditorTheme *VLineEditor::GetTheme()
{
	return Theme;
}

void VLineEditor::OnPaint(Core::VCanvasPainter *Painter)
{
	Painter->BeginDraw();

	VSolidBrush BackgroundBrush(Theme->LocalTheme.BackgroundColor, CallWidgetGetRenderHandle());
	VPenBrush	PenBrush(Theme->LocalTheme.BorderColor, CallWidgetGetRenderHandle(), Theme->LocalTheme.BorderThickness);
	VSolidBrush TextBrush(Theme->LocalTheme.TextColor, CallWidgetGetRenderHandle());
	VPenBrush	CursorBrush(Theme->CursorColor, CallWidgetGetRenderHandle());
	VPenBrush	SelectedColor(Theme->OnSelectedColor, CallWidgetGetRenderHandle());

	VCanvasPainter TextCanvas(GetWidth() - Theme->LocalTheme.BorderThickness * 2, GetHeight(),
							  CallWidgetGetRenderHandle());

	Painter->FillRoundedRectangle({static_cast<int>(Theme->LocalTheme.BorderThickness),
								   static_cast<int>(Theme->LocalTheme.BorderThickness),
								   GetWidth() - static_cast<int>(Theme->LocalTheme.BorderThickness),
								   GetHeight() - static_cast<int>(Theme->LocalTheme.BorderThickness)},
								  Theme->LocalTheme.Radius, &PenBrush, &BackgroundBrush);

	IDWriteTextLayout *TextLayout;

	if (!InputStringCache.empty())
	{
		VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
							  InputStringCache.CStyleString(), InputStringCache.size(), Theme->LabelFont->GetDXObject(),
							  INT_MAX, GetHeight(), &TextLayout) != S_OK,
						  L"Failed to create TextLayout object!");

		if (InSelectMode)
		{
			TextLayout->SetDrawingEffect(SelectedColor.GetDxBrush(), TextSelectRange);
		}
	}
	else
	{
		VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(LeadText.CStyleString(), LeadText.size(),
																			   Theme->LabelFont->GetDXObject(), INT_MAX,
																			   GetHeight(), &TextLayout) != S_OK,
						  L"Failed to create TextLayout object!");
	}

	TextCanvas.BeginDraw();
	TextCanvas.Clear(VColor(0.f, 0.f, 0.f, 0.f));
	if (!InputStringCache.empty())
	{
		TextCanvas.GetDXObject()->DrawTextLayout({static_cast<float>(OffsetX), 0}, TextLayout, TextBrush.GetDxBrush());
	}
	else
	{
		VSolidBrush TextBrush(Theme->StaticTheme.TextColor, CallWidgetGetRenderHandle());
		TextCanvas.GetDXObject()->DrawTextLayout({static_cast<float>(OffsetX), 0}, TextLayout, TextBrush.GetDxBrush());
	}
	TextCanvas.EndDraw();

	Painter->DrawCanvas({static_cast<int>(Theme->LocalTheme.BorderThickness), 0,
						 static_cast<int>(GetWidth() - Theme->LocalTheme.BorderThickness), GetHeight()},
						&TextCanvas, {0, 0, GetWidth(), GetHeight()}, 1.f);

	if (InTyping && ShowCursor)
	{
		Painter->DrawLine({static_cast<int>(CursorGraphicsX + OffsetX + Theme->LocalTheme.BorderThickness),
						   GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2},
						  {static_cast<int>(CursorGraphicsX + OffsetX + Theme->LocalTheme.BorderThickness),
						   GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2 + Theme->LabelFont->GetTextSize()},
						  &CursorBrush);
	}

	Painter->EndDraw();
}

} // namespace Core

VLIB_END_NAMESPACE