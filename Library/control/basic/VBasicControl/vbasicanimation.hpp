/*
 * VBasicAnimation.hpp
 *	@description  : The Basic Animation Interface
*/

#pragma once
#pragma warning(disable : 26451)

#include "vuiobject.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VInterpolatorFunctional Functional:
 *	@description  : The Interpolator Functional
*/
namespace VInterpolatorFunctional {
	double AnticipateInterpolator(double x) {
		return x * x * (static_cast<double>(2 + 1) * x - 2);
	}
	double AccelerateDecelerateInterpolator(double x) {
		return (cos((x + 1) * 3.14) / 2.0) + 0.5;
	}
	double AccelerateInterpolator(double x) {
		return pow(x, 2);
	}
	double AnticipateOvershootInterpolator(double x) {
		if (x <= 0.5) {
			return 0.5 * 2 * x * 2 * x * (static_cast<double>(3 + 1) * 2 * x - 3);
		}
		else {
			return 0.5 * ((2 * x - 2) * (2 * x - 2) * (static_cast<double>(3 + 1) * (2 * x - 2) + 3) + 2);
		}
	}
	double DecelerateInterpolator(double x) {
		return 1.0 - pow((1.0 - x), 2 * 1);
	}
	double LinearInterpolator(double x) {
		return x;
	}
	double OvershootInterpolator(double x) {
		return (x - 1) * (x - 1) * (static_cast<double>(2 + 1) * (x - 1) + 2) + 1;
	}
	double CycleInterpolator(double x) {
		return sin(2 * 1 * 3.14 * x);
	}
}

/*
 * VInterpolatorType enum class:
 *	@description  : The Interpolator Type
*/
enum class VInterpolatorType {
	AnticipateInterpolator,           // x * x* ((2 + 1)* x - 2)
	AccelerateDecelerateInterpolator, // (cos((x+ 1) * π) / 2.0) + 0.5
	AccelerateInterpolator,           // pow(x, 2)
	AnticipateOvershootInterpolator,  // 0.5*2*x * 2*x * ((3 + 1) * 2*x - 3)             （0 < x < 0.5）
									  // 0.5 * ((2 * x - 2) * (2 * x - 2) * ((3 + 1) * (2 * x - 2) + 3) + 2)  （0.5 <= x < 1.0）
	DecelerateInterpolator,           // 1.0 - pow((1.0 - x), 2 * 1)
	LinearInterpolator,               // x
	OvershootInterpolator,            // (x-1) * (x-1) * ((2+ 1) *( x-1) + 2)+1
	CycleInterpolator                 // sin(2 * 1* π * x)
};

/*
 * VAnimationInterpolator class:
 *	@description  : The Basic Interpolator of Animation
*/
class VAnimationInterpolator {
private:
	double                        InterpolatorDuration;
	std::function<double(double)> InterpolatorFunction;

public:
	double                        LocalX = 0;

public:
	/*
	 * Build up
	*/

	VAnimationInterpolator(double Dx, VInterpolatorType Interpolator) {
		InterpolatorDuration = Dx;

		using namespace VInterpolatorFunctional;

		switch (Interpolator) {
		case VInterpolatorType::AnticipateInterpolator:
			InterpolatorFunction = AnticipateInterpolator;

			break;
		case VInterpolatorType::AccelerateDecelerateInterpolator:
			InterpolatorFunction = AccelerateDecelerateInterpolator;

			break;
		case VInterpolatorType::AccelerateInterpolator:
			InterpolatorFunction = AccelerateInterpolator;

			break;
		case VInterpolatorType::AnticipateOvershootInterpolator:
			InterpolatorFunction = AnticipateOvershootInterpolator;

			break;
		case VInterpolatorType::DecelerateInterpolator:
			InterpolatorFunction = DecelerateInterpolator;

			break;
		case VInterpolatorType::LinearInterpolator:
			InterpolatorFunction = LinearInterpolator;

			break;
		case VInterpolatorType::OvershootInterpolator:
			InterpolatorFunction = OvershootInterpolator;

			break;
		case VInterpolatorType::CycleInterpolator:
			InterpolatorFunction = CycleInterpolator;

			break;
		default:
			InterpolatorFunction = AnticipateInterpolator;

			break;
		}
	}
	VAnimationInterpolator(double Dx, std::function<double(double)> Interpolator) {
		InterpolatorDuration = Dx;
		InterpolatorFunction = Interpolator;
	}

public:
	/*
	 * InterpolatorEnd Functional:
	 *	@description  : Is the InterpolatorEnd End
	*/
	bool  InterpolatorEnd() {
		return (LocalX >= 1 + InterpolatorDuration);
	}

	void  SetInterpolator(VInterpolatorType Interpolator) {
		using namespace VInterpolatorFunctional;

		switch (Interpolator) {
		case VInterpolatorType::AnticipateInterpolator:
			InterpolatorFunction = AnticipateInterpolator;

			break;
		case VInterpolatorType::AccelerateDecelerateInterpolator:
			InterpolatorFunction = AccelerateDecelerateInterpolator;

			break;
		case VInterpolatorType::AccelerateInterpolator:
			InterpolatorFunction = AccelerateInterpolator;

			break;
		case VInterpolatorType::AnticipateOvershootInterpolator:
			InterpolatorFunction = AnticipateOvershootInterpolator;

			break;
		case VInterpolatorType::DecelerateInterpolator:
			InterpolatorFunction = DecelerateInterpolator;

			break;
		case VInterpolatorType::LinearInterpolator:
			InterpolatorFunction = LinearInterpolator;

			break;
		case VInterpolatorType::OvershootInterpolator:
			InterpolatorFunction = OvershootInterpolator;

			break;
		case VInterpolatorType::CycleInterpolator:
			InterpolatorFunction = CycleInterpolator;

			break;
		default:
			InterpolatorFunction = AnticipateInterpolator;

			break;
		}
	}

	double GetOneFrame() {
		if (InterpolatorEnd() == true) {
			return 1.f;
		}

		if (LocalX <= 1) {
			double Result = InterpolatorFunction(LocalX);
			LocalX += InterpolatorDuration;

			return Result;
		}
		else {
			LocalX += InterpolatorDuration;
			return 1.f;
		}

		return 0.f;
	}

	void Reset() {
		LocalX = 0;
	}

	void SetDx(double Dx) {
		InterpolatorDuration = Dx;
	}
};



class VBasicInterpolator {
protected:
	bool                   AnimationStart = false;
	VAnimationInterpolator Interpolator;

public:
	VBasicInterpolator(double Dx, VInterpolatorType Type)
		: Interpolator(Dx, Type) {

	}

public:
	/*
	 * IsAnimationEnd Functional:
	 *	@description  : Get the Stats of Animation
	*/
	bool IsAnimationEnd() {
		if (AnimationStart == true) {
			if (Interpolator.InterpolatorEnd() == true) {
				AnimationStart = false;

				return true;
			}
			else {
				return false;
			}
		}
		else {
			return true;
		}
	}

	/*
	 * SetDx Functional:
	 *	@description  : Set the Delta X
	*/
	void SetDx(double Dx) {
		Interpolator.SetDx(Dx);
	}
};

/*
 * VColorInterpolator class:
 *	@description  : The Interpolator of VColor
*/
class VColorInterpolator : public VBasicInterpolator {
private:
	VColor TargetColor;
	VColor SourceColor;

public:
	/*
	 * Build up Functional
	*/

	VColorInterpolator(double Dx, VInterpolatorType Interpolator)
		: VBasicInterpolator(Dx, Interpolator) {

	}
	void Start(VColor Source, VColor Target) {
		TargetColor = Target;
		SourceColor = Source;

		AnimationStart = true;

		Interpolator.Reset();
	}

	/*
	 * GetOneFrame Functional:
	 *	@description  : Get One Color
	*/
	VColor GetOneFrame() {
		auto InterpolatorFrame = Interpolator.GetOneFrame();

		// TargetPoint.x + (SourcePoint.x - TargetPoint.x) * (1.f - InterpolatorFrame);

		int ResultR = TargetColor.GetRed()   + (SourceColor.GetRed()   - TargetColor.GetRed()   ) * (1.f - InterpolatorFrame);
		int ResultG = TargetColor.GetGreen()   + (SourceColor.GetGreen()   - TargetColor.GetGreen()   ) * (1.f - InterpolatorFrame);
		int ResultB = TargetColor.GetBlue() + (SourceColor.GetBlue() - TargetColor.GetBlue()) * (1.f - InterpolatorFrame);
		int ResultA = TargetColor.GetAlpha() + (SourceColor.GetAlpha() - TargetColor.GetAlpha()) * (1.f - InterpolatorFrame);

		return VColor(ResultR, ResultG, ResultB, ResultA);
	}

	/*
	 * IsAnimationEnd Functional:
	 *	@description  : Get the Stats of Animation
	*/
	bool IsAnimationEnd() {
		if (AnimationStart == true) {
			if (Interpolator.InterpolatorEnd() == true) {
				AnimationStart = false;

				return true;
			}
			else {
				return false;
			}
		}
		else {
			return true;
		}
	}
};

/*
 * VPositionInterpolator class:
 *	@description  : The Interpolator of Position
*/
class VPositionInterpolator : public VBasicInterpolator {
private:
	VPoint TargetPoint;
	VPoint SourcePoint;

public:
	VPositionInterpolator(double Dx, VInterpolatorType Interpolator)
		: VBasicInterpolator(Dx, Interpolator) {

	}

	void SetTargetPoint(VPoint Target) {
		TargetPoint = Target;
	}
	void SetInterpolator(VInterpolatorType InterpolatorType) {
		Interpolator.SetInterpolator(InterpolatorType);
	}
	void SetDx(double Dx) {
		Interpolator.SetDx(Dx);
	}

	void Start(VPoint Source, VPoint Target) {
		TargetPoint = Target;
		SourcePoint = Source;

		AnimationStart = true;

		Interpolator.Reset();
	}

	/*
	 * GetOneFrame Functional:
	 *	@description  : Get One Point
	*/
	VPoint GetOneFrame() {
		double InterpolatorFrame = Interpolator.GetOneFrame();

		int ResultX = TargetPoint.x + (SourcePoint.x - TargetPoint.x) * (1.f - InterpolatorFrame);
		int ResultY = TargetPoint.y + (SourcePoint.y - TargetPoint.y) * (1.f - InterpolatorFrame);

		return { ResultX, ResultY };
	}
};

/*
 * VAlphaInterpolator class:
 *	@description  : The Interpolator of Transparency
*/
class VAlphaInterpolator : public VBasicInterpolator {
private:
	short TargetAlpha = 0;
	short SourceAlpha = 0;

public:
	VAlphaInterpolator(double Dx, VInterpolatorType Interpolator)
		: VBasicInterpolator(Dx, Interpolator) {

	}

	void SetTargetAlpha(int Target) {
		TargetAlpha = Target;
	}
	void SetInterpolator(VInterpolatorType InterpolatorType) {
		Interpolator.SetInterpolator(InterpolatorType);
	}
	void SetDx(double Dx) {
		Interpolator.SetDx(Dx);
	}

	void Start(short Source, short Target) {
		TargetAlpha = Target;
		SourceAlpha = Source;

		AnimationStart = true;

		Interpolator.Reset();
	}

	/*
	 * GetOneFrame Functional:
	 *	@description  : Get One Alpha
	*/
	short GetOneFrame() {
		double InterpolatorFrame = Interpolator.GetOneFrame();
		
		return TargetAlpha + (SourceAlpha - TargetAlpha) * (1.f - InterpolatorFrame);
	}
};

/*
 * VSizeInterpolator class:
 *	@description  : The Interpolator of Size
*/
class VSizeInterpolator : public VBasicInterpolator {
private:
	VSize SourceSize;
	VSize TargetSize;

public:
	VSizeInterpolator(double Dx, VInterpolatorType Interpolator)
		: VBasicInterpolator(Dx, Interpolator) {

	}
	void Start(VSize Source, VSize Target) {
		TargetSize = Target;
		SourceSize = Source;

		AnimationStart = true;

		Interpolator.Reset();
	}

	void SetTargetSize(VSize Target) {
		TargetSize = Target;
	}
	void SetInterpolator(VInterpolatorType InterpolatorType) {
		Interpolator.SetInterpolator(InterpolatorType);
	}
	void SetDx(double Dx) {
		Interpolator.SetDx(Dx);
	}

	/*
	 * GetOneFrame Functional:
	 *	@description  : Get One Alpha
	*/
	VSize GetOneFrame() {
		double InterpolatorFrame = Interpolator.GetOneFrame();

		int TargetWidth  = TargetSize.x + (SourceSize.x - TargetSize.x) * (1.f - InterpolatorFrame);
		int TargetHeight = TargetSize.y + (SourceSize.y - TargetSize.y) * (1.f - InterpolatorFrame);

		return { TargetWidth, TargetHeight };
	}
};

VLIB_END_NAMESPACE