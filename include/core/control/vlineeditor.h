#pragma once

#include "vabstractbutton.h"
#include "../uibasic/vtimer.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

class VLineEditor : public VAbstractButton {
private:
    VTextEditorTheme* Theme;

private:
    VBasicTimer             AnimationFrameTimer;
    VAnimationInterpolator* Interpolator;

    VLabelStatusTheme       OldTheme;
    VLabelStatusTheme       TargetTheme;

    bool                    InAnimation = false;

    unsigned int            TextPosition = 0;

    bool                    InTyping = false;
    bool                    InCursorStyle = false;

private:
    VBasicTimer             CursorTimer;
    bool                    ShowCursor = false;
    bool                    KeyboardUsed = false;

private:
    std::wstring            InputStringCache;
    int                     CursorPosition = 0;
    int                     CursorGraphicsX = 0;

public:
    VLineEditor(VUIObject* Parent);

public:
    VSignal<const std::wstring&> TextOnChange;

public:
    void OnPaint(VCanvasPainter* Painter) override;
    void CheckFrame() override;
    void OnMessage(VMessage* Message) override;

public:
    VTextEditorTheme* GetTheme();

public:
    void SetPlaneText(const std::wstring& PlaneText);
    const std::wstring GetPlaneText();
    const int          GetCurrentCursorPosition();

public:
    void LeftClickedDown() override;
    void LeftClickedUp() override;
    void GotMouseFocus() override;
    void LosedMouseFocus() override;
};

}

VLIB_END_NAMESPACE