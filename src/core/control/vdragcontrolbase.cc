#include "../../../include/core/control/vdragcontrolbase.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VDragControlBase::VDragControlBase(VUIObject* Parent) : VUIObject(Parent) {

}
void VDragControlBase::MouseLeftClicked(VMouseClickedFlag ClickedFlag) {
    switch (ClickedFlag) {
        case VMouseClickedFlag::Down: {
            UserInDrag = true;
            CallWidgetLockFocusID();

            break;
        }
        case VMouseClickedFlag::Up: {
            UserInDrag = false;
            CallWidgetUnlockFocusID();

            break;
        }
    }
}
void VDragControlBase::OnMessage(Core::VMessage *Message) {
    if (UserInDrag) {
        switch (Message->GetType()) {
            case VMessageType::MouseClickedMessage: {
                VMouseClickedMessage* ClickedMessage = static_cast<VMouseClickedMessage*>(Message);
                if (!ClickedMessage->MousePosition.InsideRectangle(GetRegion())) {
                    UserInDrag = false;
                    CallWidgetUnlockFocusID();
                } else {
                    MouseDragged.Emit();
                }

                break;
            }
            case VMessageType::MouseMoveMessage: {
                VMouseMoveMessage* MouseMoveMessage = static_cast<VMouseMoveMessage*>(Message);

                MouseDragged.Emit();
                MouseDraggedPosition.Emit(MouseMoveMessage->MousePosition.X, MouseMoveMessage->MousePosition.Y);

                break;
            }
            case VMessageType::KillFocusMessage: {
                UserInDrag = false;
                CallWidgetUnlockFocusID();

                break;
            }
        }
    }
}

VDragControlBaseOnPushButton::VDragControlBaseOnPushButton(VUIObject* Parent)
    : VPushButton(Parent) {

}
VDragControlBaseOnPushButton::VDragControlBaseOnPushButton(const int& Width, const int& Height, VUIObject* Parent)
    : VPushButton(Width, Height, Parent) {

}
void VDragControlBaseOnPushButton::MouseLeftClicked(VMouseClickedFlag ClickedFlag) {
    VPushButton::MouseLeftClicked(ClickedFlag);
    switch (ClickedFlag) {
        case VMouseClickedFlag::Down: {
            UserInDrag = true;
            CallWidgetLockFocusID();

            BeginDrag();

            break;
        }
        case VMouseClickedFlag::Up: {
            UserInDrag = false;
            CallWidgetUnlockFocusID();
            
            VPushButton::LosedMouseFocus();

            EndDrag();

            break;
        }
    }
}
void VDragControlBaseOnPushButton::OnMessage(Core::VMessage *Message) {
    if (UserInDrag) {
        switch (Message->GetType()) {
            case VMessageType::MouseClickedMessage: {
                VMouseClickedMessage* ClickedMessage = static_cast<VMouseClickedMessage*>(Message);
                if (!ClickedMessage->MousePosition.InsideRectangle(GetRegion())) {
                    UserInDrag = false;
                    CallWidgetUnlockFocusID();

                    CallWidgetSendMessage(Message);

                    VPushButton::LosedMouseFocus();

                    EndDrag();
                } else {
                    MouseDragged.Emit(ClickedMessage->MousePosition.X, ClickedMessage->MousePosition.Y);
                }

                break;
            }
            case VMessageType::MouseMoveMessage: {
                VMouseMoveMessage* MouseMoveMessage = static_cast<VMouseMoveMessage*>(Message);
                MouseDragged.Emit(MouseMoveMessage->MousePosition.X, MouseMoveMessage->MousePosition.Y);

                break;
            }
            case VMessageType::KillFocusMessage: {
                UserInDrag = false;
                CallWidgetUnlockFocusID();

                VPushButton::LosedMouseFocus();

                EndDrag();

                break;
            }
        }
    }
}

}

VLIB_END_NAMESPACE