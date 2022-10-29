#pragma once

#include "../vbasiccore.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

    typedef struct VAnimationCurve {
        VAnimationCurve() {  }

        virtual double CurveFunction(const double& X);
    } VLinerCurve;

    struct VEaseInSineCurve : public VAnimationCurve {
        VEaseInSineCurve() {  }

        double CurveFunction(const double& X) override;
    };
    struct VEaseOutSineCurve : public VAnimationCurve {
        VEaseOutSineCurve() {  }

        double CurveFunction(const double& X) override;
    };
    struct VEaseInOutSineCurve : public VAnimationCurve {
        VEaseInOutSineCurve() {  }

        double CurveFunction(const double& X) override;
    };
    struct VEaseInQuadCurve : public VAnimationCurve {
        VEaseInQuadCurve() {  }

        double CurveFunction(const double& X) override;
    };
    struct VEaseOutQuadCurve : public VAnimationCurve {
        VEaseOutQuadCurve() {  }

        double CurveFunction(const double& X) override;
    };
    struct VEaseInOutQuadCurve : public VAnimationCurve {
        VEaseInOutQuadCurve() {  }

        double CurveFunction(const double& X) override;
    };
    struct VEaseInCubicCurve : public VAnimationCurve {
        VEaseInCubicCurve() {  }

        double CurveFunction(const double& X) override;
    };
    struct VEaseOutCubicCurve : public VAnimationCurve {
        VEaseOutCubicCurve() {  }

        double CurveFunction(const double& X) override;
    };
    struct VEaseInOutCubicCurve : public VAnimationCurve {
        VEaseInOutCubicCurve() {  }

        double CurveFunction(const double& X) override;
    };
    struct VEaseInQuartCurve : public VAnimationCurve {
        VEaseInQuartCurve() {  }

        double CurveFunction(const double& X) override;
    };
    struct VEaseOutQuartCurve : public VAnimationCurve {
        VEaseOutQuartCurve() {  }

        double CurveFunction(const double& X) override;
    };
    struct VEaseInOutQuartCurve : public VAnimationCurve {
        VEaseInOutQuartCurve() {  }

        double CurveFunction(const double& X) override;
    };

    enum class VAnimationCurveFlag {
        EaseLinerCurve,
        EaseInSineCurve, EaseOutSineCurve, EaseInOutSineCurve,
        EaseInQuadCurve, EaseOutQuadCurve, EaseInOutQuadCurve,
        EaseInCubicCurve, EaseOutCubicCurve, EaseInOutCubicCurve,
        EaseInQuartCurve, EaseOutQuartCurve, EaseInOutQuartCurve
    };

    struct VAnimationCurveFactory {
        static VAnimationCurve* GetCurve(const VAnimationCurveFlag& Flag);
    };
}

VLIB_END_NAMESPACE