#pragma once

#include "../animation/vanimation.h"
#include "vabstractbutton.h"
#include "vpushbutton.h"
#include "vtextlabel.h"
#include "vviewlabel.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
struct VDropContextBase
{
	virtual bool IsGroup() = 0;

	VUIObject *Instance = nullptr;
	VString	   Text;

	VSignal<VDropContextBase *> OnTrigger;
};
struct VDropContext : public VDropContextBase
{
	bool IsGroup() override;
};
struct VDropGroup : public VDropContextBase
{
	bool IsGroup() override;
};

class VDropDownContenxt : public VUIObject
{
public:
	explicit VDropDownContenxt(VUIObject *Object);
	explicit VDropDownContenxt(const int &Width, const int &Height, VUIObject *Parent);
	~VDropDownContenxt();

public:
	void OnPaint(VCanvasPainter *Painter) override;

public:
	void SetLayout();

public:
	void Resize(const int &Width, const int &Height) override;

public:
	VDropDownContextTheme *GetTheme();
	void				   SetTheme(VDropDownContextTheme *TargetTheme);

public:
	std::vector<VDropContextBase *> Context;

private:
	void SetDropContextView();
	void ButtonSignalEmit(VAbstractButton *Button);

private:
	VViewLabel			  *ViewLabel;
	VDropDownContextTheme *Theme;
};
class VDropDown : public VAbstractButton
{
public:
	void LeftClickedDown() override;
	void LeftClickedUp() override;
	void GotMouseFocus() override;
	void LostMouseFocus() override;

public:
	explicit VDropDown(VUIObject *Parent);
	VDropDown(const int &Width, const int &Height, VUIObject *Parent);
	~VDropDown() noexcept;

public:
	void Resize(const int &Width, const int &Height) override;
	void Move(const int &X, const int &Y);

public:
	VRect GetRegion() override;

public:
	VDropDownTheme *GetTheme();
	void			SetTheme(VDropDownTheme *Theme);

public:
	void OnPaint(VCanvasPainter *Painter) override;
	void CheckFrame() override;

public:
	void AddContext(VDropContextBase *Context);

public:
	void OnMessage(VMessage *Message) override;

public:
	VDropDownContenxt *GetComplex();

public:
	VSignal<VDropContextBase *> ContextChanged;

private:
	void ContextOnClicked(VDropContextBase *Context);
	void ContextDialogAnimationEnd();
	void ComplexReset();

private:
	VBasicTimer				AnimationFrameTimer;
	VAnimationInterpolator *Interpolator;

	VDropDownContenxt  *Complex;
	VPositionAnimation *ComplexPositionAnimation;
	VOpacityAnimation  *ComplexOpacityAnimation;

	VDropContextBase *OnSelectContext;

	VLabelStatusTheme OldTheme;
	VLabelStatusTheme TargetTheme;

	bool InAnimation = false;
	bool InSelect	 = false;

	VDropDownTheme *Theme;
};
} // namespace Core

VLIB_END_NAMESPACE