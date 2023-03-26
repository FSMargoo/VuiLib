// License(MIT)
// Athuor: Margoo
// The widget (VMainWindow)
#pragma once

#include "../object/vuiobject.h"
#include "../uibasic/vtimer.h"
#include "vabstractbutton.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

class VRadioButton : public VAbstractButton
{
private:
	VRadioButtonTheme *Theme;

private:
	VBasicTimer				AnimationFrameTimer;
	VAnimationInterpolator *Interpolator;

	VLabelStatusTheme OldTheme;
	VLabelStatusTheme TargetTheme;

	bool InAnimation = false;
	bool LockBack	 = false;

private:
	VRect GetRegion() override;

public:
	void SwitchStatusIgnoreLockBack();

public:
	void LeftClickedDown() override;
	void GotMouseFocus() override;
	void LostMouseFocus() override;

public:
	explicit VRadioButton(VUIObject *Parent);
	VRadioButton(const int &Width, const int &Height, VUIObject *Parent);
	~VRadioButton() noexcept;

public:
	void SetSwitchStatus(const bool &Status);
	void SetPlainText(const VString &Text);
	void SetLockBackStatus(const bool &LockBack);
	bool GetSwitchStatus() const;
	bool GetLockBackStatus() const;

public:
	VRadioButtonTheme *GetTheme();
	void			   SetTheme(VRadioButtonTheme *Theme);

public:
	void OnPaint(VCanvasPainter *Painter) override;
	void CheckFrame() override;
};

} // namespace Core

VLIB_END_NAMESPACE