#pragma once

#include "vanimationcurve.h"
#include "../object/vuiobject.h"
#include "../uibasic/vtimer.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

	class VBasicAnimation {
	public:
		VAnimationCurve* AnimationCurve;
		double           AnimationX;
		double           DeltaX;

	public:
		VBasicAnimation(VAnimationCurve* Curve, const int& Duration);
	};

	class VPositionAnimation : public VUIObject {
	private:
		VPoint TargetPoint;
		VPoint SourcePoint;

		VBasicAnimation Animation;

		bool   InAnimation = false;

		VBasicTimer Timer;

	public:
		VPositionAnimation(VUIObject* Parent, VAnimationCurve* Curve, const double& Duration);
		VPositionAnimation(VUIObject* Parent, VAnimationCurve* Curve, const double& Duration, const VPoint& Target);

		void SetTargetPosition(const VPoint& Target);
		void SetCurve(VAnimationCurve* Curve);
		void SetDuration(const int& Duration);

		void Start();
		void Pause();

		void CheckFrame() override;
	};

	class VGeometryAnimation : public VUIObject {
	private:
		VPoint TargetPoint;
		VPoint SourcePoint;

		VBasicAnimation Animation;

		bool   InAnimation = false;

		VBasicTimer Timer;

	public:
		VGeometryAnimation(VUIObject* Parent, VAnimationCurve* Curve, const double& Duration);
		VGeometryAnimation(VUIObject* Parent, VAnimationCurve* Curve, const double& Duration, const VPoint& Target);

		void SetTargetSize(const VPoint& Target);
		void SetCurve(VAnimationCurve* Curve);
		void SetDuration(const int& Duration);

		void Start();
		void Pause();

		void CheckFrame() override;
	};

	class VOpacityAnimation : public VUIObject {
	private:
		int TargetValue;
		int SourceValue;

		VBasicAnimation Animation;

		bool   InAnimation = false;

		VBasicTimer Timer;

	public:
		VOpacityAnimation(VUIObject* Parent, VAnimationCurve* Curve, const double& Duration);
		VOpacityAnimation(VUIObject* Parent, VAnimationCurve* Curve, const double& Duration, const int& Target);

		void SetTargetValue(const int& Target);
		void SetCurve(VAnimationCurve* Curve);
		void SetDuration(const int& Duration);

		void Start();
		void Pause();

		void CheckFrame() override;
	};
}

VLIB_END_NAMESPACE