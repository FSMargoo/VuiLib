#pragma once

#include "../../../include/core/uibasic/vtimer.h"
#include "../../../include/vml/vmlwidget.h"

class StatusPercentBar : public Core::VUIObject
{
public:
	StatusPercentBar(Core::VUIObject *Parent);
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
	void Stop();
	void CheckFrame() override;

public:
	Core::VSignal<>	  EndAnimation;
	Core::VBasicTimer Timer;

private:
	Core::VAnimationInterpolator Interpolator;
	bool						 AnimationPlaying;
};

bool StatusBarBuilderFunction(const std::wstring &, const VML::VMLFinder &RootFinder, Core::VUIObject **,
							  Core::VUIObject *, std::map<std::wstring, VML::VMLPropertyValue> &,
							  VML::VMLControlBuildStatus *);