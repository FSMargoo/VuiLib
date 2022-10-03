#include "../../../include/core/control/vlineeditor.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VLineEditor::VLineEditor(VUIObject* Parent) : VAbstractButton(Parent) {
    Theme = new VTextEditorTheme(*(static_cast<VTextEditorTheme*>(GetTargetTheme(VUIThemeType::VTextEditor))));

    Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);
}
VLineEditor::VLineEditor(const int& Width, const int& Height, VUIObject* Parent) : VAbstractButton(Parent) {
    Theme = new VTextEditorTheme(*(static_cast<VTextEditorTheme*>(GetTargetTheme(VUIThemeType::VTextEditor))));

    Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

    Resize(Width, Height);
}
VLineEditor::VLineEditor(const int& Width, const int& Height, std::wstring PlaneText, VUIObject* Parent) : VAbstractButton(Parent) {
    Theme = new VTextEditorTheme(*(static_cast<VTextEditorTheme*>(GetTargetTheme(VUIThemeType::VTextEditor))));

    Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

    InputStringCache = PlaneText;

    Resize(Width, Height);
}

void VLineEditor::Move(const int &X, const int &Y) {
    VUIObject::Move(X, Y);

    if (InTyping) {
        CallWidgetSetIME(GetX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX, GetY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);
    }
}

void VLineEditor::CheckFrame() {
    if (!Interpolator->IsEnd() && InAnimation) {
        if (AnimationFrameTimer.End()) {
            AnimationFrameTimer.Start(16);

            auto AnimationCurvature = Interpolator->GetOneFrame();

            Theme->LocalTheme.BorderThickness = TargetTheme.BorderThickness +
                                                (OldTheme.BorderThickness - TargetTheme.BorderThickness) *
                                                (1.f - AnimationCurvature);

            Theme->LocalTheme.TextColor = VColorInterpolator::GetTheColorByPercent(OldTheme.TextColor,
                                                                                   TargetTheme.TextColor,
                                                                                   AnimationCurvature);
            Theme->LocalTheme.BackgroundColor = VColorInterpolator::GetTheColorByPercent(OldTheme.BackgroundColor,
                                                                                         TargetTheme.BackgroundColor,
                                                                                         AnimationCurvature);
            Theme->LocalTheme.BorderColor = VColorInterpolator::GetTheColorByPercent(OldTheme.BorderColor,
                                                                                     TargetTheme.BorderColor,
                                                                                     AnimationCurvature);

            Theme->LocalTheme.Radius.X = TargetTheme.Radius.X +
                                         (OldTheme.Radius.X - TargetTheme.Radius.X) * (1.f - AnimationCurvature);
            Theme->LocalTheme.Radius.Y = TargetTheme.Radius.Y +
                                         (OldTheme.Radius.Y - TargetTheme.Radius.Y) * (1.f - AnimationCurvature);

            Update();
        }
    }
    else if (InAnimation) {
        InAnimation = false;
    }

    if (CursorTimer.End() && InTyping) {
        ShowCursor = !ShowCursor;

        CursorTimer.Start(600);

        Update();
    }
}

void VLineEditor::OnMessage(VMessage* Message) {
    if (Message->GetType() == VMessageType::KillFocusMessage) {
        CallWidgetUnlockFocusID();

        InTyping = false;
        LosedMouseFocus();

        return;
    }
    if (Message->GetType() == VMessageType::MouseMoveMessage) {
        VMouseMoveMessage* MouseMoveMessage = static_cast<VMouseMoveMessage*>(Message);

        if (MouseMoveMessage->MousePosition.InsideRectangle(GetRegion())) {
            HCURSOR ArrowCursor = LoadCursor(NULL, IDC_IBEAM);
            SetClassLongPtr(GetHWnd(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(ArrowCursor));

            InCursorStyle = true;
        }
        else if (InCursorStyle) {
            InCursorStyle = false;

            HCURSOR ArrowCursor = LoadCursor(NULL, IDC_ARROW);
            SetClassLongPtr(GetHWnd(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(ArrowCursor));
        }
    }
    if (Message->GetType() == VMessageType::MouseClickedMessage) {
        VMouseClickedMessage* MouseMessage = static_cast<VMouseClickedMessage*>(Message);

        if (!MouseMessage->MousePosition.InsideRectangle(GetRegion()) && InTyping) {
            CallWidgetUnlockFocusID();

            VMouseClickedMessage* RestoreMessage = new VMouseClickedMessage(*MouseMessage);

            InTyping = false;
            LosedMouseFocus();

            CallWidgetSendMessage(RestoreMessage);

            delete RestoreMessage;

            return;
        }
        if (MouseMessage->MousePosition.InsideRectangle(GetRegion()) &&
            MouseMessage->ClickedMethod == VMouseClickedFlag::Down && MouseMessage->ClickedKey == VMouseKeyFlag::Left) {
            IDWriteTextLayout* TextLayout;

            VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
                    InputStringCache.c_str(),
                    InputStringCache.size(),
                    Theme->LabelFont->GetDXObject(),
                        INT_MAX,
                    GetHeight(),
                    &TextLayout
            ), L"Failed to create TextLayout object!");

            auto MouseRelativeX = MouseMessage->MousePosition.X - GetX() - Theme->LocalTheme.BorderThickness - OffsetX;
            auto MouseRelativeY = MouseMessage->MousePosition.Y - GetY();

            DWRITE_HIT_TEST_METRICS HitTestMetrics;

            BOOL IsTrailingHit;
            BOOL IsInside;

            TextLayout->HitTestPoint(static_cast<FLOAT>(MouseRelativeX), static_cast<FLOAT>(MouseRelativeY),
                                     &IsTrailingHit, &IsInside, &HitTestMetrics);

            CursorPosition  = HitTestMetrics.textPosition;
            CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

            DWRITE_TEXT_METRICS TextMetrics;
            TextLayout->GetMetrics(&TextMetrics);

            OffsetX = TextMetrics.widthIncludingTrailingWhitespace < GetWidth() ?
                      0 :static_cast<int>(GetWidth() - TextMetrics.widthIncludingTrailingWhitespace - TextMetrics.top);
            WidthOffset = 0;

            VDXObjectSafeFree(&TextLayout);

            Update();
        }
    }
    if (Message->GetType() == VMessageType::KeyClickedMessage && InTyping) {
        VKeyClickedMessage* KeyMessage = static_cast<VKeyClickedMessage*>(Message);

        if (KeyMessage->KeyVKCode == VK_LEFT && KeyMessage->KeyStats == VkeyClickedFlag::Down) {
            if (CursorPosition - 1 >= 0) {
                ShowCursor = true;

                Update();

                --CursorPosition;

                IDWriteTextLayout* TextLayout;

                VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
                        InputStringCache.c_str(),
                        InputStringCache.size(),
                        Theme->LabelFont->GetDXObject(),
                        INT_MAX,
                        GetHeight(),
                        &TextLayout
                ), L"Failed to create TextLayout object!");

                DWRITE_HIT_TEST_METRICS HitTestMetrics;

                FLOAT CursorX;
                FLOAT CursorY;

                TextLayout->HitTestTextPosition(CursorPosition, FALSE, &CursorX, &CursorY, &HitTestMetrics);

                CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

                if (CursorGraphicsX > GetWidth()) {
                    OffsetX = (HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top) < GetWidth() ?
                              0 : static_cast<int>(GetWidth() -
                                                   (HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top));

                    WidthOffset = HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top;
                }
                else {
                    DWRITE_TEXT_METRICS StringMetrics;

                    TextLayout->GetMetrics(&StringMetrics);

                    OffsetX = 0;
                    WidthOffset = StringMetrics.top + StringMetrics.width;
                }

                VDXObjectSafeFree(&TextLayout);

                CallWidgetSetIME(GetX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX, GetY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);
            }
        }
        if (KeyMessage->KeyVKCode == VK_RIGHT && KeyMessage->KeyStats == VkeyClickedFlag::Down) {
            if (CursorPosition + 1 <= InputStringCache.size()) {
                ShowCursor = true;

                Update();

                ++CursorPosition;

                IDWriteTextLayout* TextLayout;

                VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
                        InputStringCache.c_str(),
                        InputStringCache.size(),
                        Theme->LabelFont->GetDXObject(),
                        INT_MAX,
                        GetHeight(),
                        &TextLayout
                ), L"Failed to create TextLayout object!");

                DWRITE_HIT_TEST_METRICS HitTestMetrics;

                FLOAT CursorX;
                FLOAT CursorY;

                TextLayout->HitTestTextPosition(CursorPosition, FALSE, &CursorX, &CursorY, &HitTestMetrics);

                DWRITE_TEXT_METRICS TextMetrics;
                TextLayout->GetMetrics(&TextMetrics);

                CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

                OffsetX = (HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top) < GetWidth() ?
                          0 : static_cast<int>(GetWidth() -
                                               (HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top));

                if (CursorGraphicsX > GetWidth()) {
                    WidthOffset = HitTestMetrics.width + HitTestMetrics.left + HitTestMetrics.top;
                }
                else {
                    DWRITE_TEXT_METRICS StringMetrics;

                    TextLayout->GetMetrics(&StringMetrics);

                    WidthOffset = StringMetrics.widthIncludingTrailingWhitespace + StringMetrics.top;
                }

                VDXObjectSafeFree(&TextLayout);

                CallWidgetSetIME(GetX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX, GetY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);
            }
        }
        if (KeyMessage->KeyVKCode == VK_DELETE && KeyMessage->KeyStats == VkeyClickedFlag::Down) {
            if (CursorPosition + 1 < InputStringCache.size()) {
                ShowCursor = true;

                Update();

                if (CursorPosition > -1) {
                    InputStringCache.erase(InputStringCache.begin() + CursorPosition + 1);
                }
                else {
                    InputStringCache.erase(InputStringCache.begin());
                }

                TextOnChange.Emit(InputStringCache);
            }
        }
        if (KeyMessage->KeyVKCode == VK_END && KeyMessage->KeyStats == VkeyClickedFlag::Down) {
            CursorPosition = InputStringCache.size() - 1;

            ShowCursor = true;

            IDWriteTextLayout* TextLayout;

            VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
                    InputStringCache.c_str(),
                    InputStringCache.size(),
                    Theme->LabelFont->GetDXObject(),
                    INT_MAX,
                    GetHeight(),
                    &TextLayout
            ), L"Failed to create TextLayout object!");

            DWRITE_HIT_TEST_METRICS HitTestMetrics;

            FLOAT CursorX;
            FLOAT CursorY;

            TextLayout->HitTestTextPosition(CursorPosition, FALSE, &CursorX, &CursorY, &HitTestMetrics);

            CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

            DWRITE_TEXT_METRICS TextMetrics;
            TextLayout->GetMetrics(&TextMetrics);

            OffsetX = TextMetrics.widthIncludingTrailingWhitespace < GetWidth() ?
                      0 :static_cast<int>(GetWidth() - TextMetrics.widthIncludingTrailingWhitespace - TextMetrics.top);
            WidthOffset = 0;

            VDXObjectSafeFree(&TextLayout);

            CallWidgetSetIME(GetX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX, GetY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

            Update();
        }
        if (KeyMessage->KeyVKCode == 0x56 && KeyMessage->KeyStats == VkeyClickedFlag::Down) {
            if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && IsClipboardFormatAvailable(CF_TEXT) && OpenClipboard(GetHWnd())) {
                KeyboardUsed = true;

                HANDLE ClipBoardHandle;
                char*  ClipBoardBuffer;

                ClipBoardHandle = GetClipboardData(CF_TEXT);
                ClipBoardBuffer = (char*)GlobalLock(ClipBoardHandle);

                GlobalUnlock(ClipBoardHandle);
                CloseClipboard();

                int ConvertedLength = MultiByteToWideChar(CP_ACP, 0, ClipBoardBuffer, -1, NULL, 0);
                wchar_t* WideString = new wchar_t[ConvertedLength];

                MultiByteToWideChar(CP_ACP, 0, ClipBoardBuffer, -1, WideString, ConvertedLength);

                std::wstring TestString = InputStringCache;

                if (CursorPosition == -1) {
                    TestString.insert(0, WideString);
                }
                else if (CursorPosition < InputStringCache.size()) {
                    TestString.insert(CursorPosition + 1, WideString);
                }
                else {
                    TestString.insert(CursorPosition, WideString);
                }

                IDWriteTextLayout *TextLayout;

                VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
                        TestString.c_str(),
                        TestString.size(),
                        Theme->LabelFont->GetDXObject(),
                        INT_MAX,
                        GetHeight(),
                        &TextLayout
                ), L"Failed to create TextLayout object!");

                DWRITE_TEXT_METRICS TestMetrics;
                TextLayout->GetMetrics(&TestMetrics);

                if (TestString.find(L"\n") == -1) {
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

                    OffsetX = TextMetrics.widthIncludingTrailingWhitespace < GetWidth() ?
                            0 :static_cast<int>(GetWidth() - TextMetrics.widthIncludingTrailingWhitespace - TextMetrics.top);

                    delete[] WideString;

                    CallWidgetSetIME(GetX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness,
                                     GetY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

                    TextOnChange.Emit(InputStringCache);
                }

                VDXObjectSafeFree(&TextLayout);
            }
        }
        if (KeyMessage->KeyVKCode == VK_HOME && KeyMessage->KeyStats == VkeyClickedFlag::Down) {
            CursorPosition = -1;
            OffsetX        = 0;

            IDWriteTextLayout* TextLayout;

            VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
                    InputStringCache.c_str(),
                    InputStringCache.size(),
                    Theme->LabelFont->GetDXObject(),
                    INT_MAX,
                    GetHeight(),
                    &TextLayout
            ), L"Failed to create TextLayout object!");

            DWRITE_TEXT_METRICS StringMetrics;

            TextLayout->GetMetrics(&StringMetrics);

            WidthOffset = StringMetrics.top + StringMetrics.width;

            CursorGraphicsX = Theme->LocalTheme.BorderThickness;

            ShowCursor = true;

            VDXObjectSafeFree(&TextLayout);

            CallWidgetSetIME(GetX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX, GetY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

            Update();
        }
    }
    if (Message->GetType() == VMessageType::IMECharMessage && InTyping) {
        VIMECharMessage* IMECharMessage = static_cast<VIMECharMessage*>(Message);

        if (KeyboardUsed) {
            KeyboardUsed = false;

            return;
        }

        if (IMECharMessage->IMEChar == L'\n' || IMECharMessage->IMEChar == L'\r') {
            CallWidgetUnlockFocusID();

            InTyping = false;
            LosedMouseFocus();

            return;
        }
        if (IMECharMessage->IMEChar == L'\b') {
            if (CursorPosition >= 0) {
                ShowCursor = true;

                Update();

                InputStringCache.erase(InputStringCache.begin() + CursorPosition);

                --CursorPosition;

                IDWriteTextLayout* TextLayout;

                VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
                        InputStringCache.c_str(),
                        InputStringCache.size(),
                        Theme->LabelFont->GetDXObject(),
                        INT_MAX,
                        GetHeight(),
                        &TextLayout
                ), L"Failed to create TextLayout object!");

                DWRITE_HIT_TEST_METRICS HitTestMetrics;

                FLOAT CursorX;
                FLOAT CursorY;

                TextLayout->HitTestTextPosition(CursorPosition, FALSE, &CursorX, &CursorY, &HitTestMetrics);

                CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

                DWRITE_TEXT_METRICS TextMetrics;
                TextLayout->GetMetrics(&TextMetrics);

                OffsetX = TextMetrics.widthIncludingTrailingWhitespace < GetWidth() ?
                          0 : static_cast<int>(GetWidth() - TextMetrics.widthIncludingTrailingWhitespace - TextMetrics.top);

                VDXObjectSafeFree(&TextLayout);

                CallWidgetSetIME(GetX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX, GetY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

                TextOnChange.Emit(InputStringCache);
            }

            return;
        }

        std::wstring TestString = InputStringCache;

        if (CursorPosition == -1) {
            TestString.insert(TestString.begin(), IMECharMessage->IMEChar);
        }
        else if (CursorPosition < InputStringCache.size()) {
            TestString.insert(TestString.begin() + CursorPosition + 1, IMECharMessage->IMEChar);
        }
        else {
            TestString.insert(TestString.begin() + CursorPosition, IMECharMessage->IMEChar);
        }

        IDWriteTextLayout* TestTextLayout;

        VLIB_CHECK_REPORT(VDirectXWriteFactory.GetInstance()->CreateTextLayout(
                TestString.c_str(),
                TestString.size(),
                Theme->LabelFont->GetDXObject(),
                INT_MAX,
                GetHeight(),
                &TestTextLayout
        ), L"Failed to create TextLayout object!");

        DWRITE_TEXT_METRICS TestMetrics;
        TestTextLayout->GetMetrics(&TestMetrics);

        InputStringCache = TestString;

        ++CursorPosition;

        ShowCursor = true;

        DWRITE_HIT_TEST_METRICS HitTestMetrics;

        FLOAT CursorX;
        FLOAT CursorY;

        TestTextLayout->HitTestTextPosition(CursorPosition, FALSE, &CursorX, &CursorY, &HitTestMetrics);

        CursorGraphicsX = HitTestMetrics.width + HitTestMetrics.left;

        DWRITE_TEXT_METRICS TextMetrics;
        TestTextLayout->GetMetrics(&TextMetrics);

        OffsetX = TextMetrics.widthIncludingTrailingWhitespace < GetWidth() ?
                  0 :static_cast<int>(GetWidth() - TextMetrics.widthIncludingTrailingWhitespace - TextMetrics.top);

        TextOnChange.Emit(InputStringCache);

        CallWidgetSetIME(GetX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX,
                         GetY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

        Update();

        VDXObjectSafeFree(&TestTextLayout);
    }
}

void VLineEditor::LeftClickedDown() {
    InAnimation = true;

    OldTheme = Theme->LocalTheme;
    TargetTheme = Theme->ActiveTheme;

    Interpolator->Reset();
    AnimationFrameTimer.Start(0);

    StartedType.Emit();

    CallWidgetSetFocusID(ObjectKernel.GlobalID);
    CallWidgetLockFocusID();
    CallWidgetSetIME(GetX() + CursorGraphicsX + Theme->LocalTheme.BorderThickness + OffsetX, GetY() + GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2);

    ShowCursor = true;

    CursorTimer.Start(600);

    InTyping = true;

    Update();
}
void VLineEditor::LeftClickedUp() {
    if (!InTyping) {
        InAnimation = true;

        OldTheme = Theme->LocalTheme;
        TargetTheme = Theme->OnHoverTheme;

        Interpolator->Reset();
        AnimationFrameTimer.Start(0);

        Update();
    }
}
void VLineEditor::GotMouseFocus() {
    if (!InTyping) {
        InAnimation = true;

        OldTheme = Theme->LocalTheme;
        TargetTheme = Theme->OnHoverTheme;

        Interpolator->Reset();
        AnimationFrameTimer.Start(0);

        Update();
    }
}
void VLineEditor::LosedMouseFocus() {
    if (!InTyping) {
        InAnimation = true;

        OldTheme = Theme->LocalTheme;
        TargetTheme = Theme->StaticTheme;

        Interpolator->Reset();
        AnimationFrameTimer.Start(0);

        EndType.Emit();

        Update();
    }
}

void VLineEditor::SetPlaneText(const std::wstring& PlaneText) {
    InputStringCache = PlaneText;

    CursorPosition  = 0;
    CursorGraphicsX = 0;
    OffsetX         = 0;

    Update();
}

const std::wstring VLineEditor::GetPlaneText() {
    return InputStringCache;
}
const int         VLineEditor::GetCurrentCursorPosition() {
    return CursorPosition;
}

VTextEditorTheme* VLineEditor::GetTheme() {
    return Theme;
}

void VLineEditor::OnPaint(Core::VCanvasPainter *Painter) {
    Painter->BeginDraw();

    VSolidBrush BackgroundBrush(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(),
                                Theme->LocalTheme.BackgroundColor);
    VPenBrush   PenBrush(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(),
                         Theme->LocalTheme.BorderColor, Theme->LocalTheme.BorderThickness);
    VSolidBrush TextBrush(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(),
                          Theme->LocalTheme.TextColor);
    VPenBrush   CursorBrush(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(),
                            Theme->CursorColor);

    VCanvasPainter TextCanvas(GetWidth() - Theme->LocalTheme.BorderThickness * 2, GetHeight(),
                          CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget());

    Painter->FillRoundedRectangle(
            {
                    static_cast<int>(Theme->LocalTheme.BorderThickness),
                    static_cast<int>(Theme->LocalTheme.BorderThickness),
                    GetWidth() - static_cast<int>(Theme->LocalTheme.BorderThickness), GetHeight() - static_cast<int>(Theme->LocalTheme.BorderThickness)
            },
            Theme->LocalTheme.Radius, &PenBrush, &BackgroundBrush);

    TextCanvas.BeginDraw();
    TextCanvas.Clear(VColor(0.f, 0.f, 0.f, 0.f));
    TextCanvas.DrawString(InputStringCache,
                        {
                                OffsetX,
                                0,
                                GetWidth() + WidthOffset, GetHeight()
                        }, Theme->LabelFont, &TextBrush);
    TextCanvas.EndDraw();

    Painter->DrawCanvas({ static_cast<int>(Theme->LocalTheme.BorderThickness), 0,
                          static_cast<int>(GetWidth() - Theme->LocalTheme.BorderThickness), GetHeight() },
                        &TextCanvas, { 0, 0, GetWidth(), GetHeight() }, 1.f);

    if (InTyping && ShowCursor) {
        Painter->DrawLine({static_cast<int>(CursorGraphicsX + OffsetX + Theme->LocalTheme.BorderThickness),
                           GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2},
                          {static_cast<int>(CursorGraphicsX + OffsetX + Theme->LocalTheme.BorderThickness),
                                     GetHeight() / 2 - Theme->LabelFont->GetTextSize() / 2 + Theme->LabelFont->GetTextSize()},
                          &CursorBrush);
    }

    Painter->EndDraw();
}

}

VLIB_END_NAMESPACE