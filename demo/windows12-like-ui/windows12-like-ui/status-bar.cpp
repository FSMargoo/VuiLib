#include "status-bar.h"

StatusPercentBar::StatusPercentBar(Core::VUIObject *Parent) : VUIObject(Parent)
{
	Start = 0;
	End	  = 0;
}
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
	: VUIObject(TargetObject), Interpolator(0.01, Core::VInterpolatorFunctional::AccelerateDecelerateInterpolator)
{
	AnimationPlaying = false;
}

void StatusPercentBarAnimation::Play()
{
	if (!AnimationPlaying)
	{
		AnimationPlaying = true;

		Timer.Start(16);
	}
}
void StatusPercentBarAnimation::Stop()
{
	AnimationPlaying = false;
}

void StatusPercentBarAnimation::CheckFrame()
{
	if (Timer.End() && AnimationPlaying)
	{
		Timer.Start(16);

		if (Interpolator.IsEnd())
		{
			Interpolator.Reset();
		}

		auto Start = 1.6f * Interpolator.GetOneFrame() - 0.4f;
		auto End   = Start <= 0.6f ? Start + 0.4f : 1.f;

		((StatusPercentBar *)GetParent())->SetRange(Start, End);
	}
}

bool StatusBarBuilderFunction(const std::wstring &TypeName, const VML::VMLFinder &RootFinder,
							  Core::VUIObject **OriginObject, Core::VUIObject *ParentObject,
							  std::map<std::wstring, VML::VMLPropertyValue> &PropertyValue,
							  VML::VMLControlBuildStatus					*BuildStaus)
{
	if (TypeName == L"status-percent-bar")
	{
		*OriginObject = new StatusPercentBar(ParentObject);

		VML::VMLCommonBuilder CommonBuilder(RootFinder, *OriginObject, PropertyValue, BuildStaus);

		return true;
	}

	return false;
}