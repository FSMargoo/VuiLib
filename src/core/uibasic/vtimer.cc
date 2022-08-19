#include "../../../include/core/uibasic/vtimer.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VBasicTimer::VBasicTimer() {
    LastTime  = 0;
    Duraction = 0;
}
void VBasicTimer::Start(const unsigned long& TimerDuraction) {
    LastTime   = clock();
    Duraction  = TimerDuraction;
}
bool VBasicTimer::End() {
    return clock() - LastTime >= Duraction;
}
}

VLIB_END_NAMESPACE