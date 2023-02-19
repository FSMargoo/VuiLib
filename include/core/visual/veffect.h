#pragma once

#include "../render/vpainter.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
class VBasicEffect
{
public:
	VBasicEffect();

	virtual void ApplyEffect(const VRenderHandle &RenderHandle, Core::VCanvasPainter *Image) = 0;
	virtual void ApplyEffect(const VRenderHandle &RenderHandle, Core::VImage *Image)		 = 0;
};
class VGassuinBlurEffect : public VBasicEffect
{
public:
	VGassuinBlurEffect(const float &BlurRadius);

	void ApplyEffect(const VRenderHandle &RenderHandle, Core::VCanvasPainter *Image) override;
	void ApplyEffect(const VRenderHandle &RenderHandle, Core::VImage *Image) override;

private:
	float Radius;
};
class VExposureEffect : public VBasicEffect
{
public:
	VExposureEffect(const float &ExposureValue);

	void ApplyEffect(const VRenderHandle &RenderHandle, Core::VCanvasPainter *Image) override;
	void ApplyEffect(const VRenderHandle &RenderHandle, Core::VImage *Image) override;

private:
	float Exposure;
};
} // namespace Core

VLIB_END_NAMESPACE