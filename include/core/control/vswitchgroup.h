#pragma once

#include "../object/vuiobject.h"
#include "vradiobutton.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

class VSwitchGroup : public VUIObject
{
private:
	std::vector<VRadioButton *> RadioSet;

private:
	void ElementOnTrigger(VAbstractButton *TriggerObject);

public:
	VSwitchGroup(VUIObject *Parent);

public:
	void AddObject(VRadioButton *RadioObject);
};

} // namespace Core

VLIB_END_NAMESPACE