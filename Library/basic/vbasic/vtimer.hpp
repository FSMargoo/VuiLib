/*
 * VTimer.hpp
 *	@description : Timer
 *	@birth		 : 2022/6.11
*/

#pragma once

#include "vbase.hpp"
#include "../../control/basic/VBasicControl/vapplication.hpp"

#include <time.h>

VLIB_BEGIN_NAMESPACE

/*
 * VTimer class:
 *	@description  : The Basic Timer
*/
class VTimer {
private:
	time_t        ClockTimer;
	unsigned long Duraction;

public:
	/*
	 * Build up Functional
	*/

	VTimer() {
		Duraction  = 0;
		ClockTimer = 0;
	}

	/*
	 * Start Functional:
	 *	@description  : Start Clock
	*/
	virtual void Start(unsigned long TimerDuraction) {
		ClockTimer = clock();

		Duraction  = TimerDuraction;
	}
	/*
	 * End Functional:
	 *	@description  : Timer End
	*/
	bool End() {
		return (clock() - ClockTimer >= Duraction);
	}
};

/*
 * VSmartTimer class:
 *	@description  : A Timer Own Signal
*/
class VSmartTimer : public VTimer, public VUIObject {
private:
	bool ClockStart = false;

public:
	VSignal<> TimerOnTime;

public:
	VSmartTimer(VApplication* Parent) : VUIObject(Parent) {

	}

public:
	void Start(unsigned long TimerDuraction) override {
		VTimer::Start(TimerDuraction);

		ClockStart = true;
	}

	void CheckFrame() override {
		if (ClockStart == true) {
			if (End() == true) {
				ClockStart = false;

				TimerOnTime.Emit();
			}
		}
	}
};

VLIB_END_NAMESPACE