#pragma once

#include "../object/vapplication.h"
#include "../signal/vsignal.h"
#include "vtimer.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
class VSmartTimer : public VBasicTimer, public VUIObject
{
private:
	bool StartClock = false;

public:
	VSignal<> OnTime;

public:
	explicit VSmartTimer(VUIObject *Parent);

	void Start(const unsigned long &TimerDuraction) override;

	void CheckFrame() override;
};
} // namespace Core

VLIB_END_NAMESPACE