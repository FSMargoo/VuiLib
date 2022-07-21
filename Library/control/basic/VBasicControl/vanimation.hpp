/*
 * VAnimation.hpp
 *	@description  : The Animation Interface
*/

#pragma once

#include "vbasicanimation.hpp"
#include "../../../basic/vbasic/vtimer.hpp"

VLIB_BEGIN_NAMESPACE

class VGeomteryAnimation : public VUIObject {
private:
	VSizeInterpolator* Interpolator;
	VTimer             FpsTimer;

	VSize			   TargetSize;

public:
	VGeomteryAnimation(VUIObject* Parent, int Duraction, VInterpolatorType InterpolatorType) : VUIObject(Parent) {
		Interpolator = new VSizeInterpolator(36.f / Duraction, InterpolatorType);
	}

public:
	void SetTargetSize(VSize Target) {
		TargetSize = Target;
	}
	void SetInterpolator(VInterpolatorType InterpolatorType) {
		Interpolator->SetInterpolator(InterpolatorType);
	}
	void SetDuraction(int Duraction) {
		Interpolator->SetDx(36.f / Duraction);
	}

	void Start(VSize NewGeomtery) {
		Interpolator->Start(
			{ Parent()->GetWidth(), Parent()->GetHeight() }, NewGeomtery);

		FpsTimer.Start(36);
	}
	void Start() {
		Interpolator->Start(
			{ Parent()->GetWidth(), Parent()->GetHeight() }, TargetSize);

		FpsTimer.Start(36);
	}

	void CheckFrame() override {
		if (Interpolator->IsAnimationEnd() == false && FpsTimer.End() == true) {
			Parent()->Resize(Interpolator->GetOneFrame());

			FpsTimer.Start(36);
		}
	}
};
class VPositionAnimation : public VUIObject {
private:
	VPositionInterpolator* Interpolator;

	VTimer                 FpsTimer;

	VPoint                 TargetPoint;

public:
	VPositionAnimation(VUIObject* Parent, int Duraction, VInterpolatorType InterpolatorType) : VUIObject(Parent) {
		Interpolator = new VPositionInterpolator(36.f / Duraction, InterpolatorType);
	}

public:
	void Start(VSize NewPosition) {
		Interpolator->Start(
			{ Parent()->GetX(), Parent()->GetY() }, NewPosition);

		FpsTimer.Start(36);
	}
	void Start() {
		Interpolator->Start(
			{ Parent()->GetX(), Parent()->GetY() }, TargetPoint);

		FpsTimer.Start(36);
	}

	void SetTargetPoint(VPoint Target) {
		TargetPoint = Target;
	}
	void SetInterpolator(VInterpolatorType InterpolatorType) {
		Interpolator->SetInterpolator(InterpolatorType);
	}
	void SetDuraction(int Duraction) {
		Interpolator->SetDx(36.f / Duraction);
	}

	void CheckFrame() override {
		if (Interpolator->IsAnimationEnd() == false && FpsTimer.End() == true) {
			Parent()->Move(Interpolator->GetOneFrame());

			FpsTimer.Start(36);
		}
	}
};
class VAlphaAnimation : public VUIObject {
private:
	VAlphaInterpolator* Interpolator;

	VTimer              FpsTimer;

	int                 TargetAlpha = 0;

public:
	VAlphaAnimation(VUIObject* Parent, int Duraction, VInterpolatorType InterpolatorType) : VUIObject(Parent) {
		Interpolator = new VAlphaInterpolator(36.f / Duraction, InterpolatorType);
	}

public:
	void Start(short NewAlpha) {
		Interpolator->Start(Parent()->GetTransparency(), NewAlpha);

		FpsTimer.Start(36);
	}
	void Start() {
		Interpolator->Start(Parent()->GetTransparency(), TargetAlpha);

		FpsTimer.Start(36);
	}

	void SetTargetAlpha(int Target) {
		TargetAlpha = Target;
	}
	void SetInterpolator(VInterpolatorType InterpolatorType) {
		Interpolator->SetInterpolator(InterpolatorType);
	}
	void SetDuraction(int Duraction) {
		Interpolator->SetDx(36.f / Duraction);
	}

	void CheckFrame() override {
		if (Interpolator->IsAnimationEnd() == false && FpsTimer.End() == true) {
			Parent()->SetTransparency(Interpolator->GetOneFrame());

			FpsTimer.Start(36);
		}
	}
};

VLIB_END_NAMESPACE