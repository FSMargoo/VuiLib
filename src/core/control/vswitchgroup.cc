#include "../../../include/core/control/vswitchgroup.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VSwitchGroup::VSwitchGroup(VUIObject* Parent) : VUIObject(Parent) {

}

void VSwitchGroup::ElementOnTrigger(Core::VAbstractButton* TriggerObject) {
    for (auto Element : RadioSet) {
        if (Element != TriggerObject && Element->GetSwitchStatus() == true) {
            ((VRadioButton*)Element)->LeftClickedDown();
        }
    }
}

void VSwitchGroup::AddObject(Core::VRadioButton* RadioButton) {
    RadioSet.push_back(RadioButton);
    RadioButton->_PtrButtonPushed.Connect(this, &VSwitchGroup::ElementOnTrigger);
}

}

VLIB_END_NAMESPACE