#include <utility>

#include "../../../include/core/uibasic/vuiinterpolator.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

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

VAnimationInterpolator::VAnimationInterpolator(const double& Dx, const VInterpolatorType& Interpolator) {
	InterpolatorDuration = Dx;

	_SetInterpolator(Interpolator);
}
void VAnimationInterpolator::_SetInterpolator(const VInterpolatorType& Interpolator) {
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
VAnimationInterpolator::VAnimationInterpolator(const double& Dx, std::function<double(double)> Interpolator) {
	InterpolatorDuration = Dx;
	InterpolatorFunction = std::move(Interpolator);
}
bool VAnimationInterpolator::IsEnd() const {
	return (LocalX >= 1 + InterpolatorDuration);
}
void VAnimationInterpolator::SetInterpolator(const VInterpolatorType& Interpolator) {
	_SetInterpolator(Interpolator);
}
double VAnimationInterpolator::GetOneFrame() {
	if (IsEnd() == true) {
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
void VAnimationInterpolator::Reset() {
	LocalX = 0;
}
void VAnimationInterpolator::SetDeltaX(const double& Dx) {
	InterpolatorDuration = Dx;
}

VColorInterpolator::VColorInterpolator(const unsigned int& Duraction, const VInterpolatorType& Interpolator)
	: VAnimationInterpolator(16.f / Duraction, Interpolator),
	TargetColor(0, 0, 0, 0), SourceColor(0, 0, 0, 0) {
}
void VColorInterpolator::Start(const VColor& Target, const VColor& Source) {
	Reset();

	TargetColor = Target;
	SourceColor = Source;
}
VColor VColorInterpolator::GetTheColorByPercent(const VColor& SourceColor, const VColor& TargetColor, const double& Percent) {
    float ResultR = TargetColor.GetR() + (SourceColor.GetR() - TargetColor.GetR()) * (1.f - Percent);
    float ResultG = TargetColor.GetG() + (SourceColor.GetG() - TargetColor.GetG()) * (1.f - Percent);
    float ResultB = TargetColor.GetB() + (SourceColor.GetB() - TargetColor.GetB()) * (1.f - Percent);
    float ResultA = TargetColor.GetA() + (SourceColor.GetA() - TargetColor.GetA()) * (1.f - Percent);

    return VColor(ResultR, ResultG, ResultB, ResultA);
}
VColor VColorInterpolator::GetFrame() {
	double DeltaX = GetOneFrame();

	float ResultR = TargetColor.GetR() + (SourceColor.GetR() - TargetColor.GetR()) * (1.f - DeltaX);
	float ResultG = TargetColor.GetG() + (SourceColor.GetG() - TargetColor.GetG()) * (1.f - DeltaX);
	float ResultB = TargetColor.GetB() + (SourceColor.GetB() - TargetColor.GetB()) * (1.f - DeltaX);
	float ResultA = TargetColor.GetA() + (SourceColor.GetA() - TargetColor.GetA()) * (1.f - DeltaX);

	return VColor(ResultR, ResultG, ResultB, ResultA);
}

VPositionInterpolator::VPositionInterpolator(const unsigned int& Duraction, const VPoint& Target, const VPoint& Source, 
	const VInterpolatorType& Interpolator)
	: VAnimationInterpolator(16.f / Duraction, Interpolator), TargetPosition(0, 0), SourcePosition(0, 0) {
	
}
void VPositionInterpolator::Start(const VPoint& Target, const VPoint& Source) {
	Reset();

	TargetPosition = Target;
	SourcePosition = Source;
}
VPoint VPositionInterpolator::GetFrame() {
	double DeltaX = GetOneFrame();

	int ResultX = TargetPosition.X + (SourcePosition.X - TargetPosition.X) * (1.f - DeltaX);
	int ResultY = TargetPosition.Y + (SourcePosition.Y - TargetPosition.Y) * (1.f - DeltaX);

	return VPoint{ ResultX, ResultY };
}

}

VLIB_END_NAMESPACE