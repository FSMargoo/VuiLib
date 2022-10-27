#pragma once

#include "../object/vuiobject.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

class VAbstractButton : public VUIObject {
 public:
     VSignal<> ButtonPushed;
     VSignal<VAbstractButton*> _PtrButtonPushed;
     VSignal<> ButtonOnActive;

 public:
     virtual void LeftClickedUp()   {  }
     virtual void LeftClickedDown() {  }

 public:
     void MouseLeftClicked(VMouseClickedFlag ClickedFlag) override;

 public:
    VAbstractButton(VUIObject* Parent) : VUIObject(Parent) {  }
};

}

VLIB_END_NAMESPACE