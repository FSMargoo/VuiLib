#pragma once

#include "../vbasiccore.h"
#include "../render/vcolor.h"

#include <cmath>
#include <functional>

VLIB_BEGIN_NAMESPACE

namespace Core {

namespace VInterpolatorFunctional {
	double AnticipateInterpolator(double x);
	double AccelerateDecelerateInterpolator(double x);
	double AccelerateInterpolator(double x);
	double AnticipateOvershootInterpolator(double x);
	double DecelerateInterpolator(double x);
	double LinearInterpolator(double x);
	double OvershootInterpolator(double x);
	double CycleInterpolator(double x);
}

enum class VInterpolatorType {
	AnticipateInterpolator,           // x * x* ((2 + 1)* x - 2)
	AccelerateDecelerateInterpolator, // (cos((x+ 1) * 3.14) / 2.0) + 0.5
	AccelerateInterpolator,           // pow(x, 2)
	AnticipateOvershootInterpolator,  // 0.5*2*x * 2*x * ((3 + 1) * 2*x - 3)             0 < x < 0.5
									  // 0.5 * ((2 * x - 2) * (2 * x - 2) * ((3 + 1) * (2 * x - 2) + 3) + 2)  0.5 <= x < 1.0
	DecelerateInterpolator,           // 1.0 - pow((1.0 - x), 2 * 1)
	LinearInterpolator,               // x
	OvershootInterpolator,            // (x-1) * (x-1) * ((2+ 1) *( x-1) + 2)+1
	CycleInterpolator                 // sin(2 * 1 * Pai * x)
};

class VAnimationInterpolator {
 private:
	double                        InterpolatorDuration;
	std::function<double(double)> InterpolatorFunction;

 private:
	 void _SetInterpolator(const VInterpolatorType& Interpolator);

 public:
	double                        LocalX = 0;

 public:
	 VAnimationInterpolator(const double& Dx, const VInterpolatorType& Interpolator);
	 VAnimationInterpolator(const double& Dx, std::function<double(double)> Interpolator);

 public:
	 bool IsEnd() const;

	 void SetInterpolator(const VInterpolatorType& Interpolator);

	 double GetOneFrame();

	 void Reset();

	 void SetDeltaX(const double& Dx);
};

class VColorInterpolator : public VAnimationInterpolator {
 private:
	 VColor TargetColor;
	 VColor SourceColor;

 public:
	 VColorInterpolator(const unsigned int& Duraction, const VInterpolatorType& Interpolator);

 public:
	 void Start(const VColor& Target, const VColor& Source);

	 VColor GetFrame();

 public:
     static VColor GetTheColorByPercent(const VColor& OldColor, const VColor& NewColor, const double& Percent);
};
class VPositionInterpolator : public VAnimationInterpolator {
 private:
	 VPoint TargetPosition;
	 VPoint SourcePosition;

 public:
	 VPositionInterpolator(const unsigned int& Duraction, const VPoint& Target, const VPoint& Source,
		 const VInterpolatorType& Interpolator);

	 void Start(const VPoint& Target, const VPoint& Source);

	 VPoint GetFrame();
};

}

VLIB_END_NAMESPACE