// License(MIT)
// Athuor: Margoo
#pragma once

#include "../object/vuiobject.h"
#include "../uibasic/vtimer.h"
#include "vabstractbutton.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

class VPushButton : public VAbstractButton
{
protected:
	VPushButtonTheme *Theme;

private:
	VBasicTimer				AnimationFrameTimer;
	VAnimationInterpolator *Interpolator;

	VLabelStatusTheme OldTheme;
	VLabelStatusTheme TargetTheme;

	bool InAnimation = false;

public:
	VRect GetRegion() override;

public:
	void LeftClickedDown() override;
	void LeftClickedUp() override;
	void GotMouseFocus() override;
	void LostMouseFocus() override;

public:
	explicit VPushButton(VUIObject *Parent);
	VPushButton(const int &Width, const int &Height, VUIObject *Parent);
	VPushButton(const int &Width, const int &Height, const VString &PlaneText, VUIObject *Parent);
	~VPushButton() noexcept;

public:
	void		   SetPlaneText(const VString &PlaneText);
	void		   SetTextSize(const int &TextSize);
	const VString &GetPlaneString();

public:
	VPushButtonTheme *GetTheme();
	void			  SetTheme(VPushButtonTheme *Theme);

public:
	void OnPaint(VCanvasPainter *Painter) override;
	void CheckFrame() override;
};

} // namespace Core

VLIB_END_NAMESPACE