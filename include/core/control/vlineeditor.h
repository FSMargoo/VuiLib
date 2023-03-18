#pragma once

#include "../uibasic/vtimer.h"
#include "vabstractbutton.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

enum class VLineEditorUsedKey
{
	Left,
	Right,
	Home,
	End,
	None
};

class VLineEditor : public VAbstractButton
{
private:
	VTextEditorTheme *Theme;

private:
	VBasicTimer				AnimationFrameTimer;
	VAnimationInterpolator *Interpolator;

	VLabelStatusTheme OldTheme;
	VLabelStatusTheme TargetTheme;

	VString LeadText;

	bool InAnimation = false;

	unsigned int TextPosition = 0;

	bool InTyping	   = false;
	bool InCursorStyle = false;

private:
	VBasicTimer CursorTimer;
	bool		ShowCursor	 = false;
	bool		KeyboardUsed = false;

private:
	VString InputStringCache;
	int		CursorPosition	= 0;
	int		CursorGraphicsX = 0;

	int OffsetX		= 0;
	int WidthOffset = 0;

	DWRITE_TEXT_RANGE TextSelectRange;

	VLineEditorUsedKey SelectedUsedKey;

	bool ShiftOnHold  = false;
	bool InSelectMode = false;

public:
	VLineEditor(VUIObject *Parent);
	VLineEditor(const int &Width, const int &Height, VUIObject *Parent);
	VLineEditor(const int &Width, const int &Height, VString PlaneText, VUIObject *Parent);

public:
	void Move(const int &X, const int &Y) override;

public:
	VSignal<const VString &> TextOnChange;
	VSignal<>				 StartedType;
	VSignal<>				 EndType;

public:
	void OnPaint(VCanvasPainter *Painter) override;
	void CheckFrame() override;
	void OnMessage(VMessage *Message) override;

public:
	VTextEditorTheme *GetTheme();

public:
	void		  SetPlaneText(const VString &PlaneText);
	const VString GetPlaneText();
	const int	  GetCurrentCursorPosition();

	void SetLeadText(const VString &Text);

public:
	void LeftClickedDown() override;
	void LeftClickedUp() override;
	void GotMouseFocus() override;
	void LostMouseFocus() override;
};

} // namespace Core

VLIB_END_NAMESPACE