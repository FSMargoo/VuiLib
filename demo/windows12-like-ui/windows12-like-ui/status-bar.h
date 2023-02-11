#pragma once

#include "../../../include/core/control/vscrollbar.h"
#include "../../../include/core/uibasic/vtimer.h"

class StatusPercentBar : public Core::VUIObject
{
public:
	StatusPercentBar(Core::VUIObject *Parent, const int &Width, const int &Height);

	void OnPaint(Core::VCanvasPainter *Painter) override;
	void SetRange(const double &RangeStart, const double &RangeEnd);

private:
	double Start;
	double End;
};

class StatusPercentBarAnimation : public Core::VUIObject
{
public:
	StatusPercentBarAnimation(StatusPercentBar *TargetObject);

	void Play();
	void CheckFrame() override;

public:
	Core::VSignal<>	  EndAnimation;
	Core::VBasicTimer Timer;

private:
	Core::VAnimationInterpolator Interpolator;
	bool						 AnimationPlaying;
};