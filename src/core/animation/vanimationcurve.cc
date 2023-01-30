#pragma once

#include "../../../include/core/animation/vanimationcurve.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
double VAnimationCurve::CurveFunction(const double &X)
{
	return X;
}
double VEaseInSineCurve::CurveFunction(const double &X)
{
	return 1 - cos((X * 3.14) / 2);
}
double VEaseOutSineCurve::CurveFunction(const double &X)
{
	return sin((X * 3.14) / 2);
}
double VEaseInOutSineCurve::CurveFunction(const double &X)
{
	return -(cos(3.14 * X) - 1) / 2;
}
double VEaseInQuadCurve::CurveFunction(const double &X)
{
	return X * X;
}
double VEaseOutQuadCurve::CurveFunction(const double &X)
{
	return 1 - (1 - X) * (1 - X);
}
double VEaseInOutQuadCurve::CurveFunction(const double &X)
{
	return X < 0.5 ? 2 * X * X : 1 - pow(-2 * X + 2, 2) / 2;
}
double VEaseInCubicCurve::CurveFunction(const double &X)
{
	return X * X * X;
}
double VEaseOutCubicCurve::CurveFunction(const double &X)
{
	return 1 - pow(1 - X, 3);
}
double VEaseInOutCubicCurve::CurveFunction(const double &X)
{
	return X < 0.5 ? 4 * X * X * X : 1 - pow(-2 * X + 2, 3) / 2;
}
double VEaseInQuartCurve::CurveFunction(const double &X)
{
	return X * X * X * X;
}
double VEaseOutQuartCurve::CurveFunction(const double &X)
{
	return 1 - pow(1 - X, 4);
}
double VEaseInOutQuartCurve::CurveFunction(const double &X)
{
	return X < 0.5 ? 8 * X * X * X * X : 1 - pow(-2 * X + 2, 4) / 2;
}

VAnimationCurve *VAnimationCurveFactory::GetCurve(const VAnimationCurveFlag &Flag)
{
	switch (Flag)
	{
	case VAnimationCurveFlag::EaseLinerCurve: {
		return new VLinerCurve;
	}
	case VAnimationCurveFlag::EaseInSineCurve: {
		return new VEaseInSineCurve;
	}
	case VAnimationCurveFlag::EaseOutSineCurve: {
		return new VEaseOutSineCurve;
	}
	case VAnimationCurveFlag::EaseInOutSineCurve: {
		return new VEaseInOutSineCurve;
	}
	case VAnimationCurveFlag::EaseOutQuadCurve: {
		return new VEaseOutQuadCurve;
	}
	case VAnimationCurveFlag::EaseInQuadCurve: {
		return new VEaseInQuadCurve;
	}
	case VAnimationCurveFlag::EaseInOutQuadCurve: {
		return new VEaseInOutQuadCurve;
	}
	case VAnimationCurveFlag::EaseInCubicCurve: {
		return new VEaseInCubicCurve;
	}
	case VAnimationCurveFlag::EaseOutCubicCurve: {
		return new VEaseOutCubicCurve;
	}
	case VAnimationCurveFlag::EaseInOutCubicCurve: {
		return new VEaseInOutCubicCurve;
	}
	case VAnimationCurveFlag::EaseInQuartCurve: {
		return new VEaseInQuartCurve;
	}
	case VAnimationCurveFlag::EaseOutQuartCurve: {
		return new VEaseOutQuartCurve;
	}
	case VAnimationCurveFlag::EaseInOutQuartCurve: {
		return new VEaseInOutQuartCurve;
	}
	}
}
} // namespace Core

VLIB_END_NAMESPACE