#include "../../../include/core/uibasic/vsmarttimer.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VSmartTimer::VSmartTimer(Core::VApplication* Parent) : VUIObject(Parent) {

}
void VSmartTimer::Start(const unsigned long& TimerDuraction) {
    if (!StartClock) {
        VBasicTimer::Start(TimerDuraction);

        StartClock = true;
    } else {
        VLIB_REPORT_ERROR(L"VSmartTimer start as twice!");
    }
}
void VSmartTimer::CheckFrame() {
    if (End() && StartClock) {
        StartClock = false;

        OnTime.Emit();
    }
}

}

VLIB_END_NAMESPACE