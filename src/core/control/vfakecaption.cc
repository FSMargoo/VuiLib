#include "../../../include/core/control/vfakecaption.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VFakeCaption::VFakeCaption(const int& Width, const int& Height, VUIObject* Parent) : VUIObject(Parent) {
    Resize(Width, Height);
}
VFakeCaption::VFakeCaption(VUIObject* Parent) : VUIObject(Parent) {

}
void VFakeCaption::OnMessage(VMessage* Message) {
    if (Message->GetType() == VMessageType::MouseClickedMessage) {
        VMouseClickedMessage* MouseClicekdMessage = static_cast<VMouseClickedMessage*>(Message);

        if (MouseClicekdMessage->ClickedKey == VMouseKeyFlag::Left &&
            MouseClicekdMessage->ClickedMethod == VMouseClickedFlag::Down &&
            MouseClicekdMessage->MousePosition.InsideRectangle(GetRegion()) &&
            CallWidgetGetLockingStatus() == false) {
            PostMessage(GetLocalWinId(), WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(MouseClicekdMessage->MousePosition.X, MouseClicekdMessage->MousePosition.Y));
        }
    }
}

}

VLIB_END_NAMESPACE