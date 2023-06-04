#pragma once

#include "../../fmt/vfmt.h"
#include "../uibasic/vtimer.h"
#include "vabstractbutton.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

class VTextLabel : public VAbstractButton, public VFMTDevice
{
private:
	VTextLabelTheme *Theme;

	bool AutoSize = false;

private:
	VRect GetRegion() override;

private:
	VBasicTimer				AnimationFrameTimer;
	VAnimationInterpolator *Interpolator;

	VLabelStatusTheme OldTheme;
	VLabelStatusTheme TargetTheme;

	bool InAnimation = false;

public:
	void LeftClickedDown() override;
	void LeftClickedUp() override;
	void GotMouseFocus() override;
	void LostMouseFocus() override;

public:
	void			 SetTheme(VTextLabelTheme *Theme);
	VTextLabelTheme *GetTheme();

public:
	VTextLabel(VUIObject *Parent);
	VTextLabel(int Width, int Height, VUIObject *Parent);
	VTextLabel(int Width, int Height, const VString &PlainText, VUIObject *Parent);
	~VTextLabel();

public:
	void SetPlainText(const VString &PlainText);
	void SetTextColor(const VColor &TextColor);
	void SetBackgroundColor(const VColor &BackgroundColor);
	void SetRadius(const VPoint &Radius);
	void SetTextSize(const int &TextSize);
	void SetLineAlignment(const Core::VFontAlignment &Alignment);
	void SetParagraphAlignment(const Core::VFontParagraphAlignment &Alignment);
	void SetAutoSize(const bool &Status);

public:
	VString GetPlainText() const;
	bool	GetAutoSizeStatus() const;

public:
	void ResizeByText(const VString &Text);
	void ResizeByText();

public:
	void OnPaint(VCanvasPainter *Painter) override;

public:
	void CheckFrame() override;

public:
	void Output(const VString &String) override;
};

} // namespace Core

VLIB_END_NAMESPACE