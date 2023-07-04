/*!
**************************************
*        # C/C++代码混淆器 #
*
*          --- 狐狸の窝 ---
*  Copyright (C) https://foxzzz.com
**************************************
*/
#define J20D LocalTextLayout->HitTestTextRange(SelectRange.startPosition, SelectRange.length, 0, OffsetY, &HitTestMetrics[0],
#define J20C                                                                                                           \
	if (Message->GetType() == VMessageType::KeyClickedMessage && UserInOperating && Message->Win32ID == WM_KEYDOWN)
#define J20B                                                                                                           \
	void VEditorCaret::SetCaretSelectionByMousePosition(const int &X, const int &Y, IDWriteTextLayout *TextLayout)
#define J20A                                                                                                           \
	Theme = new VTextEditorTheme(*(static_cast<VTextEditorTheme *>(GetTargetTheme(VUIThemeType::VTextEditor))));
#define J209 Painter->FillRoundedRectangle(VRect(Theme->LocalTheme.BorderThickness, Theme->LocalTheme.BorderThickness,
#define J208 GetWidth() - Theme->LocalTheme.BorderThickness * 2, GetHeight() - Theme->LocalTheme.BorderThickness * 2,
#define J207 LocalTextLayout->HitTestTextRange(SelectRange.startPosition, SelectRange.length, 0, 0, 0, 0, &HitCount);
#define J206 VColorInterpolator::GetTheColorByPercent(OldTheme.TextColor, TargetTheme.TextColor, AnimationCurvature);
#define J205 D2D1::Point2F(OffsetX, OffsetY + Theme->LabelFont->GetTextSize() + TopMargin), LeadingTextLayout.Get(),
#define J204 void VEditorCaret::SetCaretByMousePosition(const int &X, const int &Y, IDWriteTextLayout *TextLayout)
#define J203 auto Result = CaretCopy.GetCaretPosition(LocalTextLayout.Get()).Y + Theme->LabelFont->GetTextSize() +
#define J202 D2D1::Point2F(OffsetX, OffsetY + Theme->LabelFont->GetTextSize() + TopMargin), LocalTextLayout.Get(),
#define J201 TextLayout->HitTestTextPosition(NextLinePosition, FALSE, &UnusedParemeter, &CaretY, &HitTestMetrics);
#define J200 Theme->LabelFont->SetParagraphAlignment(VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
#define J1FF OffsetY -= (CaretPosition.Y + Theme->LabelFont->GetTextSize() * 2 + 4 - (-OffsetY + GetHeight()));
#define J1FE VEditor::VEditor(VUIObject *Parent) : VAbstractButton(Parent), CaretTimer(this), ShowCaret(false)
#define J1FD TextLayout->HitTestTextPosition(LinePosition, FALSE, &UnusedParemeter, &CaretY, &HitTestMetrics);
#define J1FC TargetTheme.Radius.Y + (OldTheme.Radius.Y - TargetTheme.Radius.Y) * (1.f - AnimationCurvature);
#define J1FB TargetTheme.Radius.X + (OldTheme.Radius.X - TargetTheme.Radius.X) * (1.f - AnimationCurvature);
#define J1FA VSolidBrush FillRectangleBrush(Theme->OnSelectedBackgroundColor, CallWidgetGetRenderHandle());
#define J1F9 SetClassLongPtr(CallWidgetGetHWND(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(ArrowCursor));
#define J1F8 Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);
#define J1F7 VSolidBrush BackgroundBrush(Theme->LocalTheme.BackgroundColor, CallWidgetGetRenderHandle());
#define J1F6 UINT StringCount = static_cast<UINT32>(wcsnlen(CString, MemoryByteSize / sizeof(wchar_t)));
#define J1F5 TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);
#define J1F4 if (CaretPosition.Y + Theme->LabelFont->GetTextSize() * 2 + 4 >= -OffsetY + GetHeight())
#define J1F3 OffsetY + Theme->LabelFont->GetTextSize() + 4 + TopMargin + (TopMargin == 0 ? 0 : -2));
#define J1F2 (OldTheme.BorderThickness - TargetTheme.BorderThickness) * (1.f - AnimationCurvature);
#define J1F1 TextLayout->HitTestTextPosition(CaretStart, FALSE, &CaretX, &CaretY, &HitTestMetrics);
#define J1F0 TextLayout->HitTestPoint(CaretX, CaretY, &IsTrailingHit, &IsInside, &HitTestMetrics);
#define J1EF HitTestMetric.top += OffsetY + Theme->LabelFont->GetTextSize() + (TopMargin / 3 * 4);
#define J1EE TextLayout->HitTestTextPosition(CaretStart, FALSE, &CaretX, &CaretY, &CaretMetrics);
#define J1ED InEditingText.CStyleString(), InEditingText.size(), Theme->LabelFont->GetDXObject(),
#define J1EC LocalTextLayout->SetDrawingEffect(SelectedBrush.GetDxBrush(), Caret.GetTextRange());
#define J1EB InEditingText.Insert(InEditingText.begin() + Caret.CaretStart + ActuallyStringCount,
#define J1EA TextLayout->HitTestTextPosition(CaretEnd, FALSE, &CaretX, &CaretY, &HitTestMetrics);
#define J1E9 VSolidBrush LeadingTextBrush(Theme->PlaceHolderColor, CallWidgetGetRenderHandle());
#define J1E8 HGLOBAL ClipboardHandle = GlobalAlloc(GMEM_DDESHARE | GMEM_ZEROINIT, DataByteSize);
#define J1E7 TextLayout->HitTestTextPosition(CaretEnd, FALSE, &CaretX, &CaretY, &CaretMetrics);
#define J1E6 if (Message->GetType() == VMessageType::MouseMoveMessage && InMouseDragSelecting)
#define J1E5 VPenBrush	BorderBrush(Theme->LocalTheme.BorderColor, CallWidgetGetRenderHandle(),
#define J1E4 VSolidBrush TextBrush(Theme->LocalTheme.TextColor, CallWidgetGetRenderHandle());
#define J1E3 GetOriginY() + CaretPosition.Y + Theme->LabelFont->GetTextSize() + TopMargin);
#define J1E2 if (MouseMessage->ClickedKey == Left && MouseMessage->ClickedMethod == Down &&
#define J1E1 if (Message->GetType() == VMessageType::KillFocusMessage && !UserInOperating)
#define J1E0 Theme->LocalTheme.BackgroundColor = VColorInterpolator::GetTheColorByPercent(
#define J1DF VPenBrush SelectedBrush(Theme->OnSelectedColor, CallWidgetGetRenderHandle());
#define J1DE LocalTextLayout->SetDrawingEffect(std::get<0>(Effect), std::get<1>(Effect));
#define J1DD void VEditorCaret::CaretSelectionTurnLineHead(IDWriteTextLayout *TextLayout)
#define J1DC OldTheme.BackgroundColor, TargetTheme.BackgroundColor, AnimationCurvature);
#define J1DB if (IMECharMessage->IMEChar != L'\b' && IMECharMessage->IMEChar != L'\x13')
#define J1DA VPenBrush CaretBrush(Theme->CursorColor, CallWidgetGetRenderHandle(), 1.f);
#define J1D9 TextLayout->HitTestPoint(X, Y, &IsTrailingHit, &IsInside, &HitTestMetrics);
#define J1D8 void VEditorCaret::CaretSelectionTurnLineEnd(IDWriteTextLayout *TextLayout)
#define J1D7 if (Message->GetType() == VMessageType::IMECharMessage && UserInOperating)
#define J1D6 LeadText.CStyleString(), LeadText.size(), Theme->LabelFont->GetDXObject(),
#define J1D5 if (MouseMessage->ClickedKey == Left && MouseMessage->ClickedMethod == Up)
#define J1D4 InEditingText.Insert(InEditingText.Begin() + Caret.CaretStart, Character);
#define J1D3 InEditingText.Erase(Caret.CaretStart, Caret.CaretEnd - Caret.CaretStart);
#define J1D2 Theme->LocalTheme.BorderColor = VColorInterpolator::GetTheColorByPercent(
#define J1D1 if (OperationCacheMax > 0 && OperationCacheMax < OldStringSet.size() + 1)
#define J1D0 LocalTextLayout->SetFontStyle(std::get<0>(Effect), std::get<1>(Effect));
#define J1CF VEditor::VEditor(const int &Width, const int &Height, VUIObject *Parent)
#define J1CE void VEditorCaret::CaretSelectionTurnDown(IDWriteTextLayout *TextLayout)
#define J1CD void VEditorCaret::CaretSelectionTurnUp(IDWriteTextLayout *TextLayout)
#define J1CC if (KeyPressResetTimer.End() && time(NULL) - LastKeyPressTime >= 1000)
#define J1CB InEditingText.Insert(InEditingText.begin() + Caret.CaretStart, L'\n');
#define J1CA auto HitTestMetrics = std::vector<DWRITE_HIT_TEST_METRICS>(HitCount);
#define J1C9 if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && !OldStringSet.empty())
#define J1C8 auto MouseWheelMessage = static_cast<VMouseWheelMessage *>(Message);
#define J1C7 VPoint VEditorCaret::GetCaretPosition(IDWriteTextLayout *TextLayout)
#define J1C6 Caret.SetCaretSelectionByMousePosition(X, Y, LocalTextLayout.Get());
#define J1C5 Painter->SolidRectangle(VRect(HitTestMetric.left, HitTestMetric.top,
#define J1C4 memcpy(LockMemory, &CString[TextRange.startPosition], DataByteSize);
#define J1C3 auto Result = VDirectXWriteFactory.GetInstance()->CreateTextLayout(
#define J1C2 Theme->LabelFont->SetTextSize(Theme->LabelFont->GetTextSize() - 1);
#define J1C1 if (Metrics.height + Theme->LabelFont->GetTextSize() > GetHeight())
#define J1C0 OldTheme.BorderColor, TargetTheme.BorderColor, AnimationCurvature);
#define J1BF Theme->LabelFont->SetTextSize(Theme->LabelFont->GetTextSize() + 1);
#define J1BE void VEditorCaret::CaretTurnLineHead(IDWriteTextLayout *TextLayout)
#define J1BD const wchar_t *CString = reinterpret_cast<const wchar_t *>(Memory);
#define J1BC auto CaretPosition = Caret.GetCaretPosition(LocalTextLayout.Get());
#define J1BB void VEditorCaret::CaretTurnLineEnd(IDWriteTextLayout *TextLayout)
#define J1BA auto MouseMoveMessage = static_cast<VMouseMoveMessage *>(Message);
#define J1B9 if (MouseWheelMessage->MousePosition.InsideRectangle(GetRegion()))
#define J1B8 InEditingText.Erase(InEditingText.begin() + Caret.CaretStart - 1);
#define J1B7 if (MouseMoveMessage->MousePosition.InsideRectangle(GetRegion()))
#define J1B6 CaretStart = HitTestMetrics.textPosition + HitTestMetrics.length;
#define J1B5 auto MouseMessage = static_cast<VMouseClickedMessage *>(Message);
#define J1B4 return !(ASCIICode >= 0 && ASCIICode <= 31 && ASCIICode == 127);
#define J1B3 Painter->DrawLine(CaretPosition, CaretEndPosition, &CaretBrush);
#define J1B2 CaretEndPosition.Offset(0, Theme->LabelFont->GetTextSize() - 2);
#define J1B1 size_t DataByteSize = sizeof(wchar_t) * (TextRange.length + 1);
#define J1B0 void VEditorCaret::CaretTurnDown(IDWriteTextLayout *TextLayout)
#define J1AF if (Message->GetType() == VMessageType::CheckLocalFocusMessage)
#define J1AE HGLOBAL ClipboardDataHandle = GetClipboardData(CF_UNICODETEXT);
#define J1AD auto *CheckFocus = static_cast<VCheckFocusMessage *>(Message);
#define J1AC else if (KeyMessage->KeyVKCode == L'Z' && EnableOperationBack)
#define J1AB InEditingText.Erase(InEditingText.Begin() + Caret.CaretStart);
#define J1AA void VEditor::DrawSelectedRange(Core::VCanvasPainter *Painter)
#define J1A9 if (SetClipboardData(CF_UNICODETEXT, ClipboardHandle) != NULL)
#define J1A8 auto IMECharMessage = static_cast<VIMECharMessage *>(Message);
#define J1A7 auto CaretY = Caret.GetCaretPosition(LocalTextLayout.Get()).Y;
#define J1A6 : VAbstractButton(Parent), CaretTimer(this), ShowCaret(false)
#define J1A5 auto KeyMessage = static_cast<VKeyClickedMessage *>(Message);
#define J1A4 void VEditorCaret::CaretTurnUp(IDWriteTextLayout *TextLayout)
#define J1A3 !MouseMessage->MousePosition.InsideRectangle(GetRegion()) &&
#define J1A2 Microsoft::WRL::ComPtr<IDWriteTextLayout> LeadingTextLayout;
#define J1A1 void VEditor::SetAllowFontSizeDragStatus(const bool &Status)
#define J1A0 UINT32 LineCount = static_cast<UINT32>(LineMetrics.size());
#define J19F if (Message->GetType() == VMessageType::MouseClickedMessage)
#define J19E CaretTimer.OnTime.Connect(this, &VEditor::ResetCaretStatus);
#define J19D UINT32 LineCount = static_cast<UINT32>(LineMetrics.size());
#define J19C NextLinePosition = LinePosition + LineMetrics[Line].length;
#define J19B Caret.SetCaretByMousePosition(X, Y, LocalTextLayout.Get());
#define J19A Theme->LocalTheme.Radius, &BorderBrush, &BackgroundBrush);
#define J199 if (Message->GetType() == VMessageType::MouseWheelMessage)
#define J198 TextBeforeChange.Connect(this, &VEditor::CacheOperation);
#define J197 if (Message->GetType() == VMessageType::MouseMoveMessage)
#define J196 void VEditor::Resize(const int &Width, const int &Height)
#define J195 MouseMessage->MousePosition.InsideRectangle(GetRegion()))
#define J194 void VEditor::SetOperationBackStatus(const bool &Status)
#define J193 size_t MemoryByteSize = GlobalSize(ClipboardDataHandle);
#define J192 Caret.CaretSelectionTurnLineHead(LocalTextLayout.Get());
#define J191 bool VEditor::IsASCIICharacter(const wchar_t &ASCIICode)
#define J190 Caret.CaretSelectionTurnLineEnd(LocalTextLayout.Get());
#define J18F static_cast<UINT32>(HitTestMetrics.size()), &HitCount);
#define J18E for (auto Count = UINT32(0); Count < HitCount; ++Count)
#define J18D auto AnimationCurvature = Interpolator->GetOneFrame();
#define J18C void VEditor::SetLeadingText(const VString &PlainText)
#define J18B const wchar_t *CString = InEditingText.CStyleString();
#define J18A void VEditor::SetCaret(const VEditorCaret &CaretValue)
#define J189 VDirectXWriteFactory.GetInstance()->CreateTextLayout(
#define J188 void VEditor::AddCharaceter(const wchar_t &Character)
#define J187 void VEditor::SetMaxOperationCache(const int &Status)
#define J186 HitTestMetrics.textPosition += HitTestMetrics.length;
#define J185 SetPlainText(OldStringSet[OldStringSet.size() - 1]);
#define J184 Caret.CaretSelectionTurnDown(LocalTextLayout.Get());
#define J183 CallWidgetSetIME(GetOriginX() + CaretPosition.X + 5,
#define J182 void VEditor::SetPlainText(const VString &PlainText)
#define J181 void VEditor::SetAllowEditStatus(const bool &Status)
#define J180 void VEditor::CacheOperation(const VString &OldText)
#define J17F void *Memory = GlobalLock(ClipboardDataHandle);
#define J17E return VPoint(CaretMetrics.left, CaretMetrics.top);
#define J17D return -(Result - Theme->LabelFont->GetTextSize());
#define J17C void VEditor::SetTopMargin(const int &TargetMargin)
#define J17B else if (KeyMessage->KeyVKCode == 'X' && AllowEdit)
#define J17A DWRITE_TEXT_RANGE TextRange = Caret.GetTextRange();
#define J179 if (CheckFocus->Object != this && UserInOperating)
#define J178 CheckInput.Emit(IMECharMessage->IMEChar, &Result);
#define J177 Caret.CaretSelectionTurnUp(LocalTextLayout.Get());
#define J176 for (UINT Count = 0; Count < StringCount; ++Count)
#define J175 else if (HitTestMetrics.textPosition < CaretStart)
#define J174 HCURSOR ArrowCursor = LoadCursor(NULL, IDC_ARROW);
#define J173 void VEditor::SetLeftMargin(const int &LeftMargin)
#define J172 HCURSOR ArrowCursor = LoadCursor(NULL, IDC_IBEAM);
#define J171 GetHeight() - Theme->LocalTheme.BorderThickness),
#define J170 else if (CaretEnd + 1 <= CacheTargetPtr->size())
#define J16F bool VEditor::GetAllowFontSizeDragStatus() const
#define J16E if (SelectMode == VEditorCaretSelectMode::Right)
#define J16D CaretCopy.CaretStart = InEditingText.size() + 1;
#define J16C if (SelectRange.length > 0 && Caret.InSelecting)
#define J16B CallWidgetGetFocusID() == ObjectKernel.GlobalID)
#define J16A void *LockMemory = GlobalLock(ClipboardHandle);
#define J169 Caret.CaretTurnLineHead(LocalTextLayout.Get());
#define J168 GetWidth() - Theme->LocalTheme.BorderThickness,
#define J167 if (SelectMode == VEditorCaretSelectMode::Left)
#define J166 PushNewCharacter.Emit(IMECharMessage->IMEChar);
#define J165 CaretCopy.CaretEnd = InEditingText.size() + 1;
#define J164 Caret.CaretTurnLineEnd(LocalTextLayout.Get());
#define J163 if (Theme->LabelFont->GetTextSize() - 1 >= 12)
#define J162 DWRITE_TEXT_RANGE VEditorCaret::GetTextRange()
#define J161 void VEditor::OnPaint(VCanvasPainter *Painter)
#define J160 if (Theme->LabelFont->GetTextSize() + 1 <= 82)
#define J15F return DWRITE_TEXT_RANGE{Start, End - Start};
#define J15E std::vector<DWRITE_LINE_METRICS> LineMetrics;
#define J15D if (CaretStart + 1 <= CacheTargetPtr->size())
#define J15C if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
#define J15B if (Caret.CaretStart < InEditingText.Size())
#define J15A LocalTextLayout->SetUnderline(true, Effect);
#define J159 bool VEditor::GetOperationBackStatus() const
#define J158 CheckInput.Emit(CString[Count], &AllowFlag);
#define J157 void VEditorCaret::CaretSelectionTurnRight()
#define J156 CallWidgetSetFocusID(ObjectKernel.GlobalID);
#define J155 void VEditorCaret::CaretSelectionTurnLeft()
#define J154 auto Y = MouseMoveMessage->MousePosition.Y;
#define J153 else if (KeyMessage->KeyVKCode == VK_PRIOR)
#define J152 OldStringSet.erase(OldStringSet.end() - 1);
#define J151 if (CaretEnd + 1 <= CacheTargetPtr->size())
#define J150 if (LinePosition == CacheTargetPtr->Size())
#define J14F if (HitTestMetrics.textPosition > CaretEnd)
#define J14E IDWriteTextLayout *VEditor::GetTextLayout()
#define J14D if (LinePosition >= CacheTargetPtr->size())
#define J14C Caret.CaretTurnDown(LocalTextLayout.Get());
#define J14B auto HitTestMetric = HitTestMetrics[Count];
#define J14A SelectMode = VEditorCaretSelectMode::Right;
#define J149 SelectMode = VEditorCaretSelectMode::Right;
#define J148 auto X = MouseMoveMessage->MousePosition.X;
#define J147 if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
#define J146 else if (KeyMessage->KeyVKCode == VK_RIGHT)
#define J145 SelectMode = VEditorCaretSelectMode::Left;
#define J144 LineMetrics.resize(TextMetrics.lineCount);
#define J143 void VEditor::OnMessage(VMessage *Message)
#define J142 else if (KeyMessage->KeyVKCode == VK_NEXT)
#define J141 else if (IMECharMessage->IMEChar == L'\b')
#define J140 if (!Interpolator->IsEnd() && InAnimation)
#define J13F else if (KeyMessage->KeyVKCode == VK_LEFT)
#define J13E HitTestMetric.top + HitTestMetric.height),
#define J13D else if (KeyMessage->KeyVKCode == VK_HOME)
#define J13C else if (KeyMessage->KeyVKCode == VK_DOWN)
#define J13B SelectMode = VEditorCaretSelectMode::Left;
#define J13A Caret.CaretTurnUp(LocalTextLayout.Get());
#define J139 OldStringSet.erase(OldStringSet.begin());
#define J138 void VEditor::SetDeltaY(const int &Delta)
#define J137 else if (KeyMessage->KeyVKCode == VK_END)
#define J136 HitTestMetric.left + HitTestMetric.width,
#define J135 LinePosition -= LineMetrics[Line].length;
#define J134 LinePosition += LineMetrics[Line].length;
#define J133 int VEditor::GetMaxOperationCache() const
#define J132 CaretStart = HitTestMetrics.textPosition;
#define J131 Caret.CaretStart += ActuallyStringCount;
#define J130 auto SelectRange = Caret.GetTextRange();
#define J12F bool VEditor::GetAllowEditStatus() const
#define J12E if (GetWidth() == 0 || GetHeight() == 0)
#define J12D else if (KeyMessage->KeyVKCode == VK_UP)
#define J12C if (KeyMessage->KeyVKCode == VK_DELETE)
#define J12B DWRITE_HIT_TEST_METRICS HitTestMetrics;
#define J12A CaretStart = max(CaretStart, CaretEnd);
#define J129 if (IMECharMessage->IMEChar != L'\x13')
#define J128 if (Theme->OnSelectedColor.GetA() != 0)
#define J127 AddCharaceter(IMECharMessage->IMEChar);
#define J126 auto X = MouseMessage->MousePosition.X;
#define J125 CaretEnd = HitTestMetrics.textPosition;
#define J124 Painter->GetDXObject()->DrawTextLayout(
#define J123 auto Y = MouseMessage->MousePosition.Y;
#define J122 void VEditorCaret::SetValidCaretRange()
#define J121 if (GetWidth() > 0 && GetHeight() > 0)
#define J120 if (MouseWheelMessage->WheelValue < 0)
#define J11F Caret = CaretSet[CaretSet.size() - 1];
#define J11E Caret.CacheTargetPtr = &InEditingText;
#define J11D auto CaretEndPosition = CaretPosition;
#define J11C VString VEditor::GetLedingText() const
#define J11B LocalTextLayout->GetMetrics(&Metrics);
#define J11A else if (KeyMessage->KeyVKCode == 'A')
#define J119 else if (KeyMessage->KeyVKCode == 'C')
#define J118 else if (KeyMessage->KeyVKCode == 'V')
#define J117 Y -= Theme->LabelFont->GetTextSize();
#define J116 TextLayout->GetMetrics(&TextMetrics);
#define J115 case VEditorCaretSelectMode::Right: {
#define J114 VString VEditor::GetPlainText() const
#define J113 VTextEditorTheme *VEditor::GetTheme()
#define J112 DWRITE_HIT_TEST_METRICS CaretMetrics;
#define J111 TextBeforeChange.Emit(InEditingText);
#define J110 DWRITE_TEXT_METRICS TextMetrics;
#define J10F CaretEnd = CacheTargetPtr->size();
#define J10E case VEditorCaretSelectMode::Left: {
#define J10D CaretStart = CacheTargetPtr->size();
#define J10C CaretStart += HitTestMetrics.length;
#define J10B void VEditorCaret::CaretSelectAll()
#define J10A if (Caret.InSelecting && AllowEdit)
#define J109 Caret.CaretEnd = Caret.CaretStart;
#define J108 void VEditorCaret::CaretTurnRight()
#define J107 Theme->LocalTheme.BorderThickness =
#define J106 Theme->LocalTheme.BorderThickness);
#define J105 CaretSet.erase(CaretSet.end() - 1);
#define J104 for (auto &Effect : UnderlineStyle)
#define J103 TextClicked.Emit(Caret.CaretStart);
#define J102 void VEditor::ResetOffsetYByCaret()
#define J101 /* CaretPosition.Y += TopMargin;*/
#define J100 TextOnChange.Emit(GetPlainText());
#define JFF	 void VEditorCaret::CaretTurnLeft()
#define JFE	 Caret.CaretEnd = Caret.CaretStart;
#define JFD	 TargetTheme = Theme->OnHoverTheme;
#define JFC	 GlobalUnlock(ClipboardDataHandle);
#define JFB	 if (NextLinePosition > CaretStart)
#define JFA	 CaretEnd = CacheTargetPtr->size();
#define JF9 LeadingTextLayout.GetAddressOf());
#define JF8 auto MaxOffset = GetMaxOffsetY();
#define JF7 LinePosition = NextLinePosition;
#define JF6 CaretSet.erase(CaretSet.begin());
#define JF5 TextOnChange.Emit(InEditingText);
#define JF4 void VEditorCaret::CaretPageEnd()
#define JF3 TargetTheme = Theme->ActiveTheme;
#define JF2 TargetTheme = Theme->StaticTheme;
#define JF1 TextClicked.Emit(Caret.CaretEnd);
#define JF0 VUIObject::Resize(Width, Height);
#define JEF if (LinePosition > CaretPosition)
#define JEE if (ObjectVisual.Stats == Hidden)
#define JED LocalTextLayout.GetAddressOf());
#define JEC for (; Line < LineCount; ++Line)
#define JEB Theme->LabelFont->GetTextSize();
#define JEA Caret.CaretSelectionTurnRight();
#define JE9 std::swap(CaretEnd, CaretStart);
#define JE8 void VEditorCaret::CaretPageUp()
#define JE7 OldStringSet.push_back(OldText);
#define JE6 if (CaretPosition.Y <= -OffsetY)
#define JE5 VEditorCaret VEditor::GetCaret()
#define JE4 void VEditorCaret::ResetSelect()
#define JE3 if (NextLinePosition > CaretEnd)
#define JE2 void VEditor::ResetCaretStatus()
#define JE1 if (ClipboardDataHandle != NULL)
#define JE0 BOOL IsTrailingHit = FALSE;
#define JDF LeadingTextBrush.GetDxBrush());
#define JDE bool VEditor::ClearSelectArea()
#define JDD if (Line >= LineMetrics.size())
#define JDC Caret.CaretSelectionTurnLeft();
#define JDB void VEditor::ResetTextLayout()
#define JDA for (auto &Effect : TextEffect)
#define JD9 void VEditor::LeftClickedDown()
#define JD8 void VEditor::DeleteCharacter()
#define JD7 int CaretPosition = CaretStart;
#define JD6 CaretStart = LinePosition - 1;
#define JD5 if (LinePosition > CaretStart)
#define JD4 HitTestMetric.left += OffsetX;
#define JD3 GlobalUnlock(ClipboardHandle);
#define JD2 if (Caret.CaretStart - 1 >= 0)
#define JD1 LastKeyPressTime = time(NULL);
#define JD0 KeyPressResetTimer.Start(250);
#define JCF AnimationFrameTimer.Start(16);
#define JCE void VEditor::WriteClipboard()
#define JCD for (auto &Effect : TextStyle)
#define JCC Line = LineMetrics.size() - 1;
#define JCB if (AnimationFrameTimer.End())
#define JCA InMouseDragSelecting = false;
#define JC9 Theme->LocalTheme.TextColor =
#define JC8 return LocalTextLayout.Get();
#define JC7 void VEditor::GotMouseFocus()
#define JC6 void VEditor::CopyClipboard()
#define JC5 else if (CaretStart - 1 >= 0)
#define JC4 void VEditor::BackCharacter()
#define JC3 EnableOperationBack = Status;
#define JC2 OldTheme = Theme->LocalTheme;
#define JC1 if (HitTestMetric.width == 0)
#define JC0 CaretPosition.Offset(OffsetX,
#define JBF AnimationFrameTimer.Start(0);
#define JBE TargetTheme.BorderThickness +
#define JBD int ActuallyStringCount = 0;
#define JBC BOOL IsInside = FALSE;
#define JBB InMouseDragSelecting = true;
#define JBA Theme->LocalTheme.Radius.Y =
#define JB9 if (ClipboardHandle != NULL)
#define JB8 GlobalFree(ClipboardHandle);
#define JB7 Theme->LocalTheme.Radius.X =
#define JB6 DWRITE_TEXT_METRICS Metrics;
#define JB5 int VEditor::GetMaxOffsetY()
#define JB4 UINT32 NextLinePosition = 0;
#define JB3 CaretEnd = LinePosition - 1;
#define JB2 if (!InEditingText.empty())
#define JB1 if (CaretStart != CaretEnd)
#define JB0 EnableOperationBack = true;
#define JAF void VEditor::ScrollToEnd()
#define JAE InEditingText = PlainText;
#define JAD if (CaretStart == CaretEnd)
#define JAC if (CaretStart >= CaretEnd)
#define JAB auto HitCount = UINT32(0);
#define JAA Caret.InSelecting = false;
#define JA9 return EnableOperationBack;
#define JA8 BOOL IsTrailingHit = FALSE;
#define JA7 OperationCacheMax = Status;
#define JA6 void VEditor::SetScroller()
#define JA5 if (CaretEnd <= CaretStart)
#define JA4 OffsetY = -CaretPosition.Y;
#define JA3 DragResetFontSize = Status;
#define JA2 DrawSelectedRange(Painter);
#define JA1 UINT32 Start = CaretStart;
#define JA0 if (CaretY >= GetHeight())
#define J9F CaretSet.push_back(Caret);
#define J9E Caret.InSelecting = false;
#define J9D if (TextRange.length <= 0)
#define J9C void VEditor::CheckFrame()
#define J9B OffsetY = GetMaxOffsetY();
#define J9A if (CaretEnd < CaretStart)
#define J99 FLOAT UnusedParemeter = 0;
#define J98 UserInOperating = false;
#define J97 if (CaretStart > CaretEnd)
#define J96 DragResetFontSize = false;
#define J95 CaretStart = LinePosition;
#define J94 void VEditor::InitEditor()
#define J93 TopMargin = TargetMargin;
#define J92 return OperationCacheMax;
#define J91 bool AllowFlag = true;
#define J90 auto CaretCopy = Caret;
#define J8F if (Line > LineCount - 1)
#define J8E BOOL IsInside = FALSE;
#define J8D switch (Caret.SelectMode)
#define J8C return DragResetFontSize;
#define J8B CaretPosition = CaretEnd;
#define J8A HitTestMetric.width = 10;
#define J89 UINT32 LinePosition = 0;
#define J88 UINT32 LinePosition = 0;
#define J87 LocalTextLayout.Reset();
#define J86 Core::VUIObject::Hide();
#define J85 TextBrush.GetDxBrush());
#define J84 if (CaretStart - 1 >= 0)
#define J83 CaretEnd = CaretStart;
#define J82 if (OffsetY < MaxOffset)
#define J81 UserInOperating = false;
#define J80 CaretEnd = LinePosition;
#define J7F Caret.CaretSelectAll();
#define J7E ClipboardHandle = NULL;
#define J7D Caret.CaretTurnRight();
#define J7C if (Character != L'\r')
#define J7B if (LockMemory != NULL)
#define J7A ShowCaret = !ShowCaret;
#define J79 FirstKeyPress = false;
#define J78 UserInOperating = true;
#define J77 UINT32 End = CaretEnd;
#define J76 LeadText = PlainText;
#define J75 if (!DragResetFontSize)
#define J74 CaretTimer.Start(1000);
#define J73 OperationCacheMax = 20;
#define J72 if (ClearSelectArea())
#define J71 if (Line >= LineCount)
#define J70 CaretStart = CaretEnd;
#define J6F Caret.CaretTurnLeft();
#define J6E CaretEnd = CaretStart;
#define J6D if (CheckFocus->Click)
#define J6C if (CaretEnd - 1 >= 0)
#define J6B ++ActuallyStringCount;
#define J6A CaretTimer.Start(500);
#define J69 if (AllowFlag == true)
#define J68 InAnimation = true;
#define J67 Interpolator->Reset();
#define J66 ResetOffsetYByCaret();
#define J65 return InEditingText;
#define J64 &FillRectangleBrush);
#define J63 Caret.CaretPageEnd();
#define J62 Caret.CaretStart = 0;
#define J61 FirstKeyPress = true;
#define J60 Line = LineCount - 1;
#define J5F OffsetX = LeftMargin;
#define J5E Painter->BeginDraw();
#define J5D SetValidCaretRange();
#define J5C if (OpenClipboard(0))
#define J5B ShowCaret = false;
#define J5A UsedComboKey = false;
#define J59 else if (InAnimation)
#define J58 } /* namespace Core*/
#define J57 if (EmptyClipboard())
#define J56 Caret.CaretEnd = 0;
#define J55 InAnimation = false;
#define J54 if (UserInOperating)
#define J53 Caret.ResetSelect();
#define J52 Caret.CaretPageUp();
#define J51 void VEditor::Hide()
#define J50 if (LocalTextLayout)
#define J4F OffsetY = MaxOffset;
#define J4E VLIB_BEGIN_NAMESPACE
#define J4D if (CString == NULL)
#define J4C FLOAT CaretX = 0;
#define J4B UsedComboKey = true;
#define J4A FLOAT CaretY = 0;
#define J49 InAnimation = true;
#define J48 InAnimation = true;
#define J47 InSelecting = true;
#define J46 ShowCaret = false;
#define J45 if (Memory != NULL)
#define J44 UINT32 Line = 0;
#define J43 bool Result = true;
#define J42 switch (SelectMode)
#define J41 Caret = CaretValue;
#define J40 Painter->EndDraw();
#define J3F AllowEdit = Status;
#define J3E FLOAT CaretX = 0.f;
#define J3D FLOAT CaretY = 0.f;
#define J3C ClearSelectArea();
#define J3B ResetTextLayout();
#define J3A UINT32 Line = 0;
#define J39 ShowCaret = false;
#define J38 ShowCaret = true;
#define J37 if (IsTrailingHit)
#define J36 if (FirstKeyPress)
#define J35 VLIB_END_NAMESPACE
#define J34 OffsetY -= YDelta;
#define J33 OffsetY += YDelta;
#define J32 DeleteCharacter();
#define J31 if (UsedComboKey)
#define J30 ShowCaret = true;
#define J2F return AllowEdit;
#define J2E AllowEdit = true;
#define J2D OffsetY = CaretY;
#define J2C WriteClipboard();
#define J2B if (!InSelecting)
#define J2A CloseClipboard();
#define J29 CaretStart -= 1;
#define J28 CaretStart += 1;
#define J27 CString[Count]);
#define J26 BackCharacter();
#define J25 if (OffsetY > 0)
#define J24 CopyClipboard();
#define J23 return LeadText;
#define J22 YDelta = Delta;
#define J21 CaretStart = 0;
#define J20 CaretEnd -= 1;
#define J1F CaretEnd += 1;
#define J1E namespace Core
#define J1D if (ShowCaret)
#define J1C TopMargin = 0;
#define J1B if (Line <= 0)
#define J1A SetScroller();
#define J19 OffsetX = 5;
#define J18 YDelta = 25;
#define J17 if (AllowEdit)
#define J16 return false;
#define J15 return Caret;
#define J14 InitEditor();
#define J13 ++CaretStart;
#define J12 X -= OffsetX;
#define J11 return Theme;
#define J10 Y -= OffsetY;
#define JF	CaretY += 12;
#define JE	OffsetY = 0;
#define JD	X -= GetX();
#define JC	return true;
#define JB	Y -= GetY();
#define JA	if (!Result)
#define J9	default: {
#define J8	return 0;
#define J7	Update();
#define J6	--Line;
#define J5	++Line;
#define J4	return;
#define J3	break;
#define J2	else
#define J1	{
#define J0	}
#pragma warning(disable : 4018)
#pragma warning(disable : 26451)
#include "../../../include/core/control/veditor.h"
#include <time.h>
#define J20E		  J4E J1E J1 J1CD J1 J47 JAD J1 J110 J15E
#define J243		  J116 J144 J1F5 J44 J89 JB4 J1A0 JEC J1 JF7
#define J210		  J19C JFB J1 J3 J0 J0 J8F J1 J60 J0
#define J211		  J1B J1 J4 J0 J6 J135 J12B J4C J4A J99
#define J212		  J8E JA8 J1F1 J1FD J1F0 J132 J37 J1 J10D J0
#define J213		  J47 J13B J5D J4 J0 J2 J1 J42 J1 J10E
#define J248		  J110 J15E J116 J144 J1F5 J44 J89 JB4 J1A0 JEC
#define J249		  J1 JF7 J19C JFB J1 J3 J0 J0 J8F J1
#define J216		  J60 J0 J1B J1 J4 J0 J6 J135 J12B J4C
#define J217		  J4A J99 J8E JA8 J1F1 J1FD J1F0 J132 J9A J1
#define J218		  J14A J0 J47 J5D J4 J0 J115 J110 J15E J116
#define J219		  J144 J1F5 J44 J89 JB4 J1A0 JEC J1 JF7 J19C
#define J21A		  JE3 J1 J3 J0 J0 J8F J1 J60 J0 J1B
#define J21B		  J1 J4 J0 J6 J135 J12B J4C J4A J99 J8E
#define J21C		  JA8 J1EA J1FD J1F0 J125 J37 J1 JFA J0 J9A
#define J21D		  J1 J145 J0 J5D J4 J0 J0 J0 J0 J1CE
#define J21E		  J1 J47 JAD J1 J110 J15E J116 J144 J1F5 J44
#define J224		  J89 JB4 J1A0 JEC J1 JF7 J19C JFB J1 J3
#define J225		  J0 J0 J8F J1 J60 J0 J134 J5 JDD J1
#define J226		  J4 J0 J12B J4C J4A J99 J8E JA8 J1F1 J1FD
#define J222		  J1F0 J125 J37 J1 JFA J0 J14A J4 J0 J2
#define J223		  J1 J42 J1 J10E J110 J15E J116 J144 J1F5 J44
#define J224		  J89 JB4 J1A0 JEC J1 JF7 J19C JFB J1 J3
#define J225		  J0 J0 J8F J1 J60 J0 J134 J5 JDD J1
#define J226		  J4 J0 J12B J4C J4A J99 J8E JA8 J1F1 J1FD
#define J227		  J1F0 J132 J97 J1 J14A J0 J5D J4 J0 J115
#define J248		  J110 J15E J116 J144 J1F5 J44 J89 JB4 J1A0 JEC
#define J229		  J1 JF7 J19C JE3 J1 J3 J0 J0 J8F J1
#define J24A		  J60 J0 J134 J5 JDD J1 J4 J0 J12B J4C
#define J22B		  J4A J99 J8E JA8 J1EA J1FD J1F0 J125 J5D J4
#define J22C		  J0 J0 J0 J0 J155 J1 J47 JAD J1 J145
#define J22D		  J84 J1 J29 J0 J4 J0 J2 J1 J42 J1
#define J22E		  J10E J84 J1 J29 J0 J4 J0 J115 J6C J1
#define J22F		  J20 JA5 J1 J14A J0 J0 J4 J0 J0 J0
#define J230		  J5D J0 J157 J1 J47 JAD J1 J14A J151 J1
#define J231		  J1F J0 J4 J0 J2 J1 J42 J1 J10E J15D
#define J232		  J1 J28 JAC J1 J14A J0 J0 J4 J0 J115
#define J233		  J151 J1 J1F J0 J4 J0 J0 J0 J5D J0
#define J234		  JE4 J1 J21 J83 J0 J1A4 J1 JAD J1 J110
#define J23E		  J15E J116 J144 J1F5 J44 J89 JB4 J1A0 JEC J1
#define J236		  JF7 J19C JFB J1 J3 J0 J0 J8F J1 J60
#define J240		  J0 J1B J1 J4 J0 J6 J135 J12B J4C J4A
#define J238		  J99 J8E JA8 J1F1 J1FD JF J1F0 J132 J6E J4
#define J239		  J0 J2 J1 J42 J1 J10E J110 J15E J116 J144
#define J23A		  J1F5 J44 J89 JB4 J1A0 JEC J1 JF7 J19C JFB
#define J23B		  J1 J3 J0 J0 J8F J1 J60 J0 J1B J1
#define J23C		  J4 J0 J6 J135 J12B J4C J4A J99 J8E JA8
#define J23D		  J1F1 J1FD JF J1F0 J132 J6E J4 J0 J115 J110
#define J23E		  J15E J116 J144 J1F5 J44 J89 JB4 J1A0 JEC J1
#define J23F		  JF7 J19C JE3 J1 J3 J0 J0 J8F J1 J60
#define J240		  J0 J1B J1 J4 J0 J6 J135 J12B J4C J4A
#define J241		  J99 J8E JA8 J1EA J1FD JF J1F0 J125 J70 J4
#define J242		  J0 J0 J0 J0 J1B0 J1 JAD J1 J110 J15E
#define J243		  J116 J144 J1F5 J44 J89 JB4 J1A0 JEC J1 JF7
#define J244		  J19C JFB J1 J134 J3 J0 J0 J8F J1 J60
#define J245		  J0 J134 J5 JDD J1 J4 J0 J12B J4C J4A
#define J246		  J99 J8E JA8 J1F1 J201 JF J1F0 J132 J37 J1
#define J247		  J10C J0 J6E J4 J0 J2 J1 J42 J1 J10E
#define J248		  J110 J15E J116 J144 J1F5 J44 J89 JB4 J1A0 JEC
#define J249		  J1 JF7 J19C JFB J1 J3 J0 J0 J8F J1
#define J24A		  J60 J0 J134 J5 JDD J1 J4 J0 J12B J4C
#define J24B		  J4A J99 J8E JA8 J1F1 J1FD JF J1F0 J132 J6E
#define J24C		  J4 J0 J115 J110 J15E J116 J144 J1F5 J44 J89
#define J24D		  JB4 J1A0 JEC J1 JF7 J19C JE3 J1 J3 J0
#define J24E		  J0 J8F J1 J60 J0 J134 J5 JDD J1 J4
#define J24F		  J0 J12B J4C J4A J99 J8E JA8 J1EA J1FD JF
#define J250		  J1F0 J132 J6E J4 J0 J0 J0 J0 J1BE J1
#define J251		  JB1 J1 J12A J83 J0 J110 J15E J116 J144 J1F5
#define J252		  J3A J88 J19D JEC J1 J134 JD5 J1 J3 J0
#define J253		  J0 J71 J1 J60 J0 J135 J95 J83 J0 J1BB
#define J254		  J1 JB1 J1 J12A J83 J0 J110 J15E J116 J144
#define J255		  J1F5 J3A J88 J19D JEC J1 J134 JD5 J1 J3
#define J256		  J0 J0 JD6 J14D J1 J13 J0 J6E J0 J1DD
#define J257		  J1 J110 J15E J116 J144 J1F5 JD7 J16E J1 J8B
#define J258		  J0 J3A J88 J19D JEC J1 J134 JEF J1 J3
#define J259		  J0 J0 JDD J1 JCC J0 J135 J16E J1 J80
#define J25A		  J0 J2 J1 J95 J0 J47 J97 J1 J145 J0
#define J25B		  J5D J0 J1D8 J1 J110 J15E J116 J144 J1F5 JD7
#define J25C		  J16E J1 J8B J0 J3A J88 J19D JEC J1 J134
#define J25D		  JEF J1 J3 J0 J0 J16E J1 JB3 J150 J1
#define J25E		  J80 J0 J0 J2 J1 JD6 J150 J1 J95 J0
#define J25F		  J0 J47 J97 J1 J14A J0 J5D J0 J10B J1
#define J260		  J21 J10F J47 J149 J0 JE8 J1 J21 J83 J0
#define J261		  JF4 J1 J10D J83 J0 JFF J1 JB1 J1 J6E
#define J262		  J0 JC5 J1 J29 J6E J0 J0 J108 J1 JB1
#define J263		  J1 J70 J0 J170 J1 J1F J70 J0 J0 J204
#define J264		  J1 JE0 JBC J12B J1D9 J37 J1 J1B6 J0 J2
#define J265		  J1 J132 J0 J6E J0 J20B J1 JE0 JBC J12B
#define J266		  J1D9 J37 J1 J186 J0 J2B J1 J14F J1 J70
#define J267		  J47 J14A J125 J0 J175 J1 J47 J145 J132 J0
#define J268		  J2 J1 J125 J0 J0 J2 J1 J14F J1 J167
#define J269		  J1 J70 J14A J125 J0 J2 J1 J125 J0 J0
#define J26A		  J175 J1 J16E J1 J6E J145 J132 J0 J2 J1
#define J26B		  J132 J0 J0 J2 J1 J16E J1 J125 J0 J2
#define J26C		  J1 J132 J0 J0 J0 J0 J122 J1 J9A J1
#define J26D		  JE9 J0 J0 J1C7 J1 J112 J3E J3D J2B J1
#define J26E		  J1EE J0 J2 J1 J42 J1 J10E J1EE J3 J0
#define J26F		  J115 J1E7 J3 J0 J0 J0 J17E J0 J162 J1
#define J270		  J9A J1 JE9 J0 J77 JA1 J15F J0 J1FE J1
#define J271		  J14 J0 J1CF J1A6 J1 JF0 J14 J0 J94 J1
#define J272		  J20A J1F8 J200 J61 J5A J19 J18 J1C J81 J11E
#define J273		  JAA JCA J96 J3B J2E JB0 J73 J74 J19E J198
#define J274		  J0 J180 J1 J1D1 J1 J139 JF6 J0 JE7 J9F
#define J275		  J0 JE2 J1 J54 J1 J7A J0 J2 J1 J39
#define J276		  J0 J6A J7 J0 JDB J1 J87 J121 J1 J1C3
#define J277		  J1ED J208 JED JDA J1 J1DE J0 JCD J1 J1D0
#define J278		  J0 J104 J1 J15A J0 J7 J0 J0 J12F J1
#define J279		  J2F J0 J181 J1 J3F J0 J16F J1 J8C J0
#define J27A		  J1A1 J1 JA3 J0 J159 J1 JA9 J0 J194 J1
#define J27B		  JC3 J0 J133 J1 J92 J0 J187 J1 JA7 J0
#define J27C		  JAF J1 J9B J0 J182 J1 JAE J62 J56 J3B
#define J27D		  J100 J0 J18C J1 J76 J62 J56 J3B J100 J0
#define J27E		  J1AA J1 JAB J130 J16C J1 J207 J1CA J20D J18F
#define J27F		  J1FA J18E J1 J14B JD4 J1EF JC1 J1 J8A J0
#define J280		  J1C5 J136 J13E J64 J0 J0 J0 J161 J1 J12E
#define J281		  J1 J4 J0 J5E J1E4 J1F7 J1DF J1DA J1E5 J106
#define J282		  J128 J1 J1EC J0 J209 J168 J171 J19A JB2 J1
#define J283		  JA2 J124 J202 J85 J0 J2 J1 J1E9 J1A2 J189
#define J284		  J1D6 J208 JF9 J124 J205 JDF J0 J1D J1 J1BC
#define J285		  JC0 J1F3 J11D J1B2 J1B3 J0 J40 J3B J0 JD9
#define J286		  J1 J49 J78 J38 JC2 JF3 J67 JBF J156 J7
#define J287		  J0 JC7 J1 J48 JC2 JFD J67 JBF J156 J7
#define J288		  J0 J138 J1 J22 J0 J173 J1 J5F J0 J17C
#define J289		  J1 J93 J7 J0 J9C J1 J1CC J1 J61 J0
#define J28A		  J54 J1 J1BC J183 J1E3 J0 J140 J1 JCB J1
#define J28B		  JCF J18D J107 JBE J1F2 JC9 J206 J1E0 J1DC J1D2
#define J28C		  J1C0 JB7 J1FB JBA J1FC J7 J0 J0 J59 J1
#define J28D		  J55 J0 J0 JC4 J1 J17 J1 J72 J1 J4
#define J28E		  J0 JD2 J1 J1B8 J0 J6F J3B J0 J0 JD8
#define J28F		  J1 J17 J1 J72 J1 J4 J0 J15B J1 J1AB
#define J290		  J0 J3B J0 J0 J188 J1 J17 J1 J3C J7C
#define J291		  J1 J1D4 J7D J3B J0 J2 J1 J1CB J7D J3B
#define J292		  J0 J0 J0 J102 J1 J1A7 JA0 J1 J2D J0
#define J293		  J2 J1 JE J0 J0 JDE J1 J10A J1 J9E
#define J294		  J1D3 JFE JC J0 J16 J0 J191 J1 J1B4 J0
#define J295		  JA6 J1 J50 J1 J1BC J101 JE6 J1 JA4 J0
#define J296		  J1F4 J1 J1FF J0 J0 J0 JB5 J1 JB6 J11B
#define J297		  J90 J16D J165 J203 JEB J1C1 J1 J17D J0 J2
#define J298		  J1 J8 J0 J0 JC6 J1 J17A J9D J1 J4
#define J299		  J0 J5C J1 J57 J1 J1B1 J1E8 JB9 J1 J16A
#define J29A		  J7B J1 J18B J1C4 JD3 J1A9 J1 J7E J0 J0
#define J29B		  JB8 J0 J0 J2A J0 J0 JCE J1 J5C J1
#define J29C		  J1AE JE1 J1 J193 J17F J1BD J4D J1 J4 J0
#define J29D		  J1F6 J45 J1 J91 JBD J176 J1 J158 J69 J1
#define J29E		  J1EB J27 J6B J0 J0 J131 JFE JFC J0 J0
#define J29F		  J3B J2A J0 J0 J113 J1 J11 J0 JE5 J1
#define J2A0		  J15 J0 J18A J1 J41 J1A J7 J0 J14E J1
#define J2A1		  JC8 J0 J114 J1 J65 J0 J11C J1 J23 J0
#define J2A2		  J196 J1 JF0 J3B J1A J0 J51 J1 J86 J98
#define J2A3		  J5B JCA J68 J62 J109 JC2 JF2 J53 J67 JBF
#define J2A4		  J174 J1F9 J7 J0 J143 J1 JEE J1 J4 J0
#define J2A5		  J1E1 J1 J98 J5B JCA J68 J62 J109 JC2 JF2
#define J2A6		  J53 J67 JBF J174 J1F9 J7 J0 J1AF J1 J1AD
#define J2A7		  J179 J1 J6D J1 J81 J46 J49 J62 J109 JC2
#define J2A8		  JF2 J67 JBF J7 J0 J174 J1F9 J0 J0 J19F
#define J2A9		  J1 J1B5 J1E2 J1A3 J16B J1 J81 J46 J49 JC2
#define J2AA		  JF2 J67 JBF J7 J0 J1D5 J1 JCA J0 J1E2
#define J2AB		  J195 J1 J126 J123 JD JB J12 J117 J10 JBB
#define J2AC		  J147 J1 J19B J103 J0 J2 J1 J1C6 J7 J8D
#define J2AD		  J1 J10E J103 J3 J0 J9 JF1 J3 J0 J0
#define J2AE		  J0 J0 J0 J197 J1 J1BA J1B7 J1 J172 J1F9
#define J2AF		  J0 J2 J1 J174 J1F9 J0 J0 J1E6 J1 J1BA
#define J2B0		  J148 J154 JD JB J12 J10 J117 J1C6 J7 J0
#define J2B1		  J20C J1 J1A5 J12C J1 J111 J32 JF5 J0 J1AC
#define J2B2		  J1 J1C9 J1 J4B J185 J11F J152 J105 JF5 J0
#define J2B3		  J0 J13D J1 J147 J1 J169 J0 J2 J1 J192
#define J2B4		  J0 J0 J137 J1 J147 J1 J164 J0 J2 J1
#define J2B5		  J190 J0 J0 J153 J1 JE J52 J0 J142 J1
#define J2B6		  J63 J0 J11A J1 J15C J1 J4B J7F J66 J0
#define J2B7		  J0 J119 J1 J15C J1 J4B J24 J0 J0 J17B
#define J2B8		  J1 J15C J1 J4B J111 J3C J24 JF5 J0 J0
#define J2B9		  J118 J1 J15C J1 J111 J111 J4B J3C J2C JF5
#define J2BA		  J0 J0 J12D J1 J147 J1 J13A J0 J2 J1
#define J2BB		  J177 J0 J0 J13C J1 J147 J1 J14C J0 J2
#define J2BC		  J1 J184 J0 J0 J146 J1 J147 J1 J7D J0
#define J2BD		  J2 J1 JEA J0 J0 J13F J1 J147 J1 J6F
#define J2BE		  J0 J2 J1 JDC J0 J0 J2 J1 J4 J0
#define J2BF		  J6A J30 J1A J7 J0 J199 J1 J1C8 J1B9 J1
#define J2C0		  J120 J1 J75 J1 J34 JF8 J82 J1 J4F J0
#define J2C1		  J0 J2 J1 J15C J1 J163 J1 J1C2 J0 J0
#define J2C2		  J2 J1 J34 JF8 J82 J1 J4F J0 J0 J0
#define J2C3		  J0 J2 J1 J75 J1 J33 J25 J1 JE J0
#define J2C4		  J0 J2 J1 J15C J1 J160 J1 J1BF J0 J0
#define J2C5		  J2 J1 J33 J25 J1 JE J0 J0 J0 J0
#define J2C6		  J0 J3B J7 J0 J1D7 J1 J1A8 J31 J1 J5A
#define J2C7		  J4 J0 J43 J178 JA J1 J4 J0 J1DB J1
#define J2C8		  J111 J127 J0 J141 J1 J36 J1 J79 JD1 JD0
#define J2C9		  J0 J111 J26 J0 J129 J1 J100 J166 J0 J7
#define J2CA		  J0 J0 J58 J35
#define J2CB		  J20E J243 J210 J211 J212 J213 J248 J249 J216 J217
#define J2CC		  J218 J219 J21A J21B J21C J21D J21E J224 J225 J226
#define J2CD		  J222 J223 J224 J225 J226 J227 J248 J229 J24A J22B
#define J2CE		  J22C J22D J22E J22F J230 J231 J232 J233 J234 J23E
#define J2CF		  J236 J240 J238 J239 J23A J23B J23C J23D J23E J23F
#define J2D0		  J240 J241 J242 J243 J244 J245 J246 J247 J248 J249
#define J2D1		  J24A J24B J24C J24D J24E J24F J250 J251 J252 J253
#define J2D2		  J254 J255 J256 J257 J258 J259 J25A J25B J25C J25D
#define J2D3		  J25E J25F J260 J261 J262 J263 J264 J265 J266 J267
#define J2D4		  J268 J269 J26A J26B J26C J26D J26E J26F J270 J271
#define J2D5		  J272 J273 J274 J275 J276 J277 J278 J279 J27A J27B
#define J2D6		  J27C J27D J27E J27F J280 J281 J282 J283 J284 J285
#define J2D7		  J286 J287 J288 J289 J28A J28B J28C J28D J28E J28F
#define J2D8		  J290 J291 J292 J293 J294 J295 J296 J297 J298 J299
#define J2D9		  J29A J29B J29C J29D J29E J29F J2A0 J2A1 J2A2 J2A3
#define J2DA		  J2A4 J2A5 J2A6 J2A7 J2A8 J2A9 J2AA J2AB J2AC J2AD
#define J2DB		  J2AE J2AF J2B0 J2B1 J2B2 J2B3 J2B4 J2B5 J2B6 J2B7
#define J2DC		  J2B8 J2B9 J2BA J2BB J2BC J2BD J2BE J2BF J2C0 J2C1
#define J2DD		  J2C2 J2C3 J2C4 J2C5 J2C6 J2C7 J2C8 J2C9 J2CA
#define J2DE		  J2CB J2CC J2CD J2CE J2CF J2D0 J2D1 J2D2 J2D3 J2D4
#define J2DF		  J2D5 J2D6 J2D7 J2D8 J2D9 J2DA J2DB J2DC J2DD
#define J2E0		  J2DE J2DF
#define J2E1(__FOX__) __FOX__
J2E1(J2E0)
