#include "../../../include/core/control/vabstractbutton.h"

VLIB_BEGIN_NAMESPACE

namespace Core {
    void VAbstractButton::MouseLeftClicked(const VMouseClickedFlag& ClickedFlag) {
        switch (ClickedFlag) {
            case VMouseClickedFlag::Down: {
                LeftClickedDown();
                ButtonOnActive.Emit();
    
                break;
            }
            case VMouseClickedFlag::Up: {
                LeftClickedUp();
                ButtonPushed.Emit();
                _PtrButtonPushed.Emit(this);
    
                break;
            }
        }
    }
}

VLIB_END_NAMESPACE