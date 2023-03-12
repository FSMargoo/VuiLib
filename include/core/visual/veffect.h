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

	void  SetRadius(const float &BlurRadius);
	float GetRadius() const;

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
class VGrayScaleEffect : public VBasicEffect
{
public:
	VGrayScaleEffect();

	void ApplyEffect(const VRenderHandle &RenderHandle, Core::VCanvasPainter *Image) override;
	void ApplyEffect(const VRenderHandle &RenderHandle, Core::VImage *Image) override;
};
class VSepiaEffect : public VBasicEffect
{
public:
	VSepiaEffect(const float &SepiaIntensity);

	void ApplyEffect(const VRenderHandle &RenderHandle, Core::VCanvasPainter *Image) override;
	void ApplyEffect(const VRenderHandle &RenderHandle, Core::VImage *Image) override;

private:
	float Intensity;
};
} // namespace Core

VLIB_END_NAMESPACE