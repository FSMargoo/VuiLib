#include "status-bar.h"

StatusPercentBar::StatusPercentBar(Core::VUIObject *Parent, const int &Width, const int &Height) : VUIObject(Parent)
{
	Start = 0;
	End	  = 0;

	Resize(Width, Height);
}

void StatusPercentBar::OnPaint(Core::VCanvasPainter *Painter)
{
	Core::VSolidBrush *Brush = new Core::VSolidBrush(Core::VColor::White, CallWidgetGetRenderHandle());

	Painter->BeginDraw();
	Painter->SolidRoundedRectangle(Core::VRect(GetWidth() * Start, 0, GetWidth() * End, GetHeight()),
								   Core::VPoint(GetHeight() / 2, GetHeight()), Brush);
	Painter->EndDraw();
}
void StatusPercentBar::SetRange(const double &RangeStart, const double &RangeEnd)
{
	Start = RangeStart;
	End	  = RangeEnd;

	Update();
}

StatusPercentBarAnimation::StatusPercentBarAnimation(StatusPercentBar *TargetObject)
	: VUIObject(TargetObject), Interpolator(0.1, Core::VInterpolatorFunctional::AccelerateInterpolator)
{
	AnimationPlaying = false;
}

void StatusPercentBarAnimation::Play()
{
	if (AnimationPlaying)
	{
		AnimationPlaying = true;

		Timer.Start(16);
	}
}

void StatusPercentBarAnimation::CheckFrame()
{
	if (Timer.End())
	{
		if (Interpolator.IsEnd())
		{
			Interpolator.Reset();
		}

		auto Start = 0.8f * Interpolator.GetOneFrame();

		((StatusPercentBar *)GetParent())->SetRange(Start, Start + 0.2f);
	}
}