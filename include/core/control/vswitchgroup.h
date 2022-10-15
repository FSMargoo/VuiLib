#pragma once

#include "vradiobutton.h"
#include "../object/vuiobject.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

class VSwitchGroup : public VUIObject {
private:
    std::vector<VRadioButton*> RadioSet;

private:
    void ElementOnTrigger(VAbstractButton* TriggerObject);

public:
    VSwitchGroup(VUIObject* Parent);

public:
    void AddObject(VRadioButton* RadioObject);
};

}

VLIB_END_NAMESPACE