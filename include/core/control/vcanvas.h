#pragma once

#include "../object/vuiobject.h"
#include "../uibasic/vsmarttimer.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
class VCanvas : public VUIObject
{
public:
	void SetFps(const int &Fps);

public:
	void OnPaint(VCanvasPainter *Painter) override;

public:
	VCanvas(VUIObject *Parent);
	VCanvas(const int &Width, const int &Height, VUIObject *Parent, const int &Fps = 0);
	VCanvas(const int &Fps, VUIObject *Parent);

private:
	void OnTime();

private:
	Core::VSmartTimer UpdateTimer;
	int				  FlushDealy;

public:
	VSignal<Core::VCanvasPainter *, const VRenderHandle &> OnPaintSignal;

public:
	std::function<void(Core::VCanvasPainter *, const VRenderHandle &)> PaintFunction;
};
} // namespace Core

VLIB_END_NAMESPACE