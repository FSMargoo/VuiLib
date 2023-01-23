#include "../../../include/core/uibasic/vsmarttimer.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

VSmartTimer::VSmartTimer(Core::VUIObject *Parent) : VUIObject(Parent)
{
}
void VSmartTimer::Start(const unsigned long &TimerDuraction)
{
	if (!StartClock)
	{
		VBasicTimer::Start(TimerDuraction);

		StartClock = true;
	}
	else
	{
		VBasicTimer::Start(TimerDuraction);
	}
}
void VSmartTimer::CheckFrame()
{
	if (End() && StartClock)
	{
		StartClock = false;

		OnTime.Emit();
	}
}

} // namespace Core

VLIB_END_NAMESPACE