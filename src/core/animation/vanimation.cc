#pragma once

#include "../../../include/core/animation/vanimation.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

VBasicAnimation::VBasicAnimation(Core::VAnimationCurve *Curve, const int &Duration)
{
	AnimationCurve = Curve;

	AnimationX = 0;
	DeltaX	   = 16.f / (double)Duration;
}

VPositionAnimation::VPositionAnimation(Core::VUIObject *Parent, Core::VAnimationCurve *Curve, const double &Duration)
	: Animation(Curve, Duration), VUIObject(Parent)
{
}

VPositionAnimation::VPositionAnimation(VUIObject *Parent, VAnimationCurve *Curve, const double &Duration,
									   const VPoint &Target)
	: Animation(Curve, Duration), VUIObject(Parent)
{
	TargetPoint = Target;
}

void VPositionAnimation::SetTargetPosition(const VPoint &Target)
{
	TargetPoint = Target;
}
void VPositionAnimation::SetCurve(VAnimationCurve *Curve)
{
	if (Animation.AnimationCurve != nullptr)
	{
		delete Animation.AnimationCurve;
	}

	Animation.AnimationCurve = Curve;
}
void VPositionAnimation::SetDuration(const int &Duration)
{
	Animation.DeltaX = 16.f / double(Duration);
}

void VPositionAnimation::Start()
{
	SourcePoint = {GetParent()->GetX(), GetParent()->GetY()};

	InAnimation			 = true;
	Animation.AnimationX = 0;

	Timer.Start(12);
}

void VPositionAnimation::Pause()
{
	InAnimation = false;
}

void VPositionAnimation::CheckFrame()
{
	if (Timer.End())
	{
		Timer.Start(12);

		if (InAnimation && Animation.AnimationX < 1)
		{
			double Percent = Animation.AnimationCurve->CurveFunction(Animation.AnimationX);

			GetParent()->Move(SourcePoint.X + (TargetPoint.X - SourcePoint.X) * Percent,
							  SourcePoint.Y + (TargetPoint.Y - SourcePoint.Y) * Percent);

			Animation.AnimationX += Animation.DeltaX;

			if (Animation.AnimationX >= 1.f)
			{
				GetParent()->Move(TargetPoint.X, TargetPoint.Y);
			}
		}
		else if (InAnimation)
		{
			InAnimation = false;
		}
	}
}

VGeometryAnimation::VGeometryAnimation(Core::VUIObject *Parent, Core::VAnimationCurve *Curve, const double &Duration)
	: Animation(Curve, Duration), VUIObject(Parent)
{
}

VGeometryAnimation::VGeometryAnimation(VUIObject *Parent, VAnimationCurve *Curve, const double &Duration,
									   const VPoint &Target)
	: Animation(Curve, Duration), VUIObject(Parent)
{
	TargetPoint = Target;
}

void VGeometryAnimation::SetTargetSize(const VPoint &Target)
{
	TargetPoint = Target;
}
void VGeometryAnimation::SetCurve(VAnimationCurve *Curve)
{
	Animation.AnimationCurve = Curve;
}
void VGeometryAnimation::SetDuration(const int &Duration)
{
	Animation.DeltaX = 16.f / double(Duration);
}

void VGeometryAnimation::Start()
{
	SourcePoint			 = {GetParent()->GetWidth(), GetParent()->GetHeight()};
	InAnimation			 = true;
	Animation.AnimationX = 0;
}

void VGeometryAnimation::Pause()
{
	InAnimation = false;
}

void VGeometryAnimation::CheckFrame()
{
	if (Timer.End())
	{
		Timer.Start(12);

		if (InAnimation && Animation.AnimationX < 1)
		{
			double Percent = Animation.AnimationCurve->CurveFunction(Animation.AnimationX);

			GetParent()->Resize(SourcePoint.X + (TargetPoint.X - SourcePoint.X) * Percent,
								SourcePoint.Y + (TargetPoint.Y - SourcePoint.Y) * Percent);

			Animation.AnimationX += Animation.DeltaX;
		}
		else if (InAnimation)
		{
			InAnimation = false;
		}
	}
}

VOpacityAnimation::VOpacityAnimation(Core::VUIObject *Parent, Core::VAnimationCurve *Curve, const double &Duration)
	: Animation(Curve, Duration), VUIObject(Parent)
{
}

VOpacityAnimation::VOpacityAnimation(VUIObject *Parent, VAnimationCurve *Curve, const double &Duration,
									 const int &Target)
	: Animation(Curve, Duration), VUIObject(Parent)
{
	TargetValue = Target;
}

void VOpacityAnimation::SetTargetValue(const int &Target)
{
	TargetValue = Target;
}
void VOpacityAnimation::SetCurve(VAnimationCurve *Curve)
{
	if (Animation.AnimationCurve != nullptr)
	{
		delete Animation.AnimationCurve;
	}
	Animation.AnimationCurve = Curve;
}
void VOpacityAnimation::SetDuration(const int &Duration)
{
	Animation.DeltaX = 16.f / double(Duration);
}

void VOpacityAnimation::Start()
{
	SourceValue = GetParent()->GetTransparency();
	InAnimation = true;

	Timer.Start(12);

	Animation.AnimationX = 0;
}

void VOpacityAnimation::Pause()
{
	InAnimation = false;
}

void VOpacityAnimation::CheckFrame()
{
	if (Timer.End())
	{
		Timer.Start(12);

		if (InAnimation && Animation.AnimationX < 1)
		{
			double Percent = Animation.AnimationCurve->CurveFunction(Animation.AnimationX);

			GetParent()->SetTransparency((SourceValue + (TargetValue - SourceValue) * Percent));

			Animation.AnimationX += Animation.DeltaX;

			if (Animation.AnimationX >= 1)
			{
				GetParent()->SetTransparency(TargetValue);
			}
		}
		else if (InAnimation)
		{
			InAnimation = false;
		}
	}
}

} // namespace Core

VLIB_END_NAMESPACE