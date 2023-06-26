#pragma once

#include "../object/vuiobject.h"
#include "../uibasic/vtimer.h"
#include "vanimationcurve.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

class VBasicAnimation
{
public:
	VAnimationCurve *AnimationCurve;
	double			 AnimationX;
	double			 DeltaX;

public:
	VBasicAnimation(VAnimationCurve *Curve, const int &Duration);
};

class VPositionAnimation : public VUIObject
{
private:
	VPoint TargetPoint;
	VPoint SourcePoint;

	VBasicAnimation Animation;

	bool InAnimation = false;

	VBasicTimer Timer;

public:
	VPositionAnimation(VUIObject *Parent, VAnimationCurve *Curve, const double &Duration);
	VPositionAnimation(VUIObject *Parent, VAnimationCurve *Curve, const double &Duration, const VPoint &Target);

	void SetTargetPosition(const VPoint &Target);
	void SetCurve(VAnimationCurve *Curve);
	void SetDuration(const int &Duration);

	void Start();
	void Pause();

	void CheckFrame() override;

public:
	VSignal<> AnimationEnd;
};

class VGeometryAnimation : public VUIObject
{
private:
	VPoint TargetPoint;
	VPoint SourcePoint;

	VBasicAnimation Animation;

	bool InAnimation = false;

	VBasicTimer Timer;

public:
	VGeometryAnimation(VUIObject *Parent, VAnimationCurve *Curve, const double &Duration);
	VGeometryAnimation(VUIObject *Parent, VAnimationCurve *Curve, const double &Duration, const VPoint &Target);

	void SetTargetSize(const VPoint &Target);
	void SetCurve(VAnimationCurve *Curve);
	void SetDuration(const int &Duration);

	void Start();
	void Pause();

	void CheckFrame() override;

public:
	VSignal<> AnimationEnd;
};

class VOpacityAnimation : public VUIObject
{
private:
	double TargetValue;
	double SourceValue;

	VBasicAnimation Animation;

	bool InAnimation = false;

	VBasicTimer Timer;

public:
	VOpacityAnimation(VUIObject *Parent, VAnimationCurve *Curve, const double &Duration);
	VOpacityAnimation(VUIObject *Parent, VAnimationCurve *Curve, const double &Duration, const double &Target);

	void SetTargetValue(const double &Target);
	void SetCurve(VAnimationCurve *Curve);
	void SetDuration(const int &Duration);

	void Start();
	void Pause();

	void CheckFrame() override;

public:
	VSignal<> AnimationEnd;
};
} // namespace Core

VLIB_END_NAMESPACE