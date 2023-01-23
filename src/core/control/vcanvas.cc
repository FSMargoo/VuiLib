#include "../../../include/core/control/vcanvas.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
void VCanvas::SetFps(const int &Fps)
{
	if (Fps == 0)
	{
		FlushDealy = 0;
	}
	else
	{
		FlushDealy = 1000 / Fps;
	}

	if (FlushDealy != 0)
	{
		UpdateTimer.Start(FlushDealy);
	}
}
void VCanvas::OnPaint(VCanvasPainter *Painter)
{
	if (PaintFunction != NULL)
	{
		PaintFunction(Painter, CallWidgetGetRenderHandle());
	}

	OnPaintSignal.Emit(Painter, CallWidgetGetRenderHandle());

	if (FlushDealy != 0)
	{
		UpdateTimer.Start(FlushDealy);
	}
}
void VCanvas::OnTime()
{
	Update();
}

VCanvas::VCanvas(const int &Width, const int &Height, VUIObject *Parent, const int &Fps)
	: VUIObject(Parent), UpdateTimer(Parent)
{
	Resize(Width, Height);
	SetFps(Fps);

	UpdateTimer.OnTime.Connect(this, &VCanvas::OnTime);
}
VCanvas::VCanvas(VUIObject *Parent) : VUIObject(Parent), UpdateTimer(Parent)
{
	FlushDealy = 0;

	UpdateTimer.OnTime.Connect(this, &VCanvas::OnTime);
}
VCanvas::VCanvas(const int &Fps, VUIObject *Parent) : VUIObject(Parent), UpdateTimer(Parent)
{
	SetFps(Fps);

	UpdateTimer.OnTime.Connect(this, &VCanvas::OnTime);
}
} // namespace Core

VLIB_END_NAMESPACE