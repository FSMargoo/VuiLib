#pragma once

#include "../signal/vsignal.h"
#include "../vbasiccore.h"

#include <ctime>

VLIB_BEGIN_NAMESPACE

namespace Core
{

class VBasicTimer
{
private:
	time_t		  LastTime;
	unsigned long Duraction;

public:
	VBasicTimer();

	virtual void Start(const unsigned long &TimerDuraction);
	virtual bool End();
};

class VTimeHelper
{
public:
	static void Sleep(int MS)
	{
		MsgWaitForMultipleObjectsEx(0, nullptr, MS, QS_ALLPOSTMESSAGE, MWMO_INPUTAVAILABLE);
	}
};

} // namespace Core

VLIB_END_NAMESPACE