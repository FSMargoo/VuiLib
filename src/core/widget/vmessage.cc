#include "../../../include/core/widget/vmessage.h"

VLIB_BEGIN_NAMESPACE

namespace Core {
VMessage::VMessage(VMessageType Type)
	: MessageType(Type) {

}
VFreeSourceMessage::VFreeSourceMessage(HWND TriggerWidget)
	: VMessage(VMessageType::FreeResourceMessage) {
    MessageTriggerWidget = TriggerWidget;
}
VMouseMoveMessage::VMouseMoveMessage(HWND TriggerWidget, int X, int Y)
	: VMessage(VMessageType::MouseMoveMessage) {
	MousePosition.X = X;
	MousePosition.Y = Y;

    MessageTriggerWidget = TriggerWidget;
}
VMouseWheelMessage::VMouseWheelMessage(HWND TriggerWidget, int X, int Y, short WheelParameter)
	: VMessage(VMessageType::MouseWheelMessage) {
	MousePosition.X = X;
	MousePosition.Y = Y;

	WheelValue = WheelParameter;

    MessageTriggerWidget = TriggerWidget;
}
VMouseClickedMessage::VMouseClickedMessage(HWND TriggerWidget, int X, int Y, VMouseClickedFlag ClickedFlag, VMouseKeyFlag Key)
	: VMessage(VMessageType::MouseClickedMessage) {
	MousePosition.X = X;
	MousePosition.Y = Y;

	ClickedMethod = ClickedFlag;
	ClickedKey = Key;

    MessageTriggerWidget = TriggerWidget;
}
VKeyClickedMessage::VKeyClickedMessage(HWND TriggerWidget, byte VKCode, bool PrevDown, bool Extened, VkeyClickedFlag Stats)
	: VMessage(VMessageType::KeyClickedMessage) {
	KeyVKCode = VKCode;
	KeyPrevDown = PrevDown;
	KeyExtened = Extened;
	KeyStats = Stats;

    MessageTriggerWidget = TriggerWidget;
}
VRepaintMessage::VRepaintMessage(HWND TriggerWidget, const VRect& RepaintRegion)
	: VMessage(VMessageType::RepaintMessage) {
	DirtyRectangle = RepaintRegion;

    MessageTriggerWidget = TriggerWidget;
}
VGetRepaintAeraMessage::VGetRepaintAeraMessage(HWND TriggerWidget, VRect& RepaintRegions)
	: VMessage(VMessageType::GetRepaintAeraMessage) {
	RepaintAera = &RepaintRegions;

    MessageTriggerWidget = TriggerWidget;
}
VIMECharMessage::VIMECharMessage(HWND TriggerWidget, wchar_t CharInputed)
	: VMessage(VMessageType::IMECharMessage) {
	IMEChar = CharInputed;

    MessageTriggerWidget = TriggerWidget;
}
VCheckFocusMessage::VCheckFocusMessage(HWND TriggerWidget, const VPoint& Point)
	: VMessage(VMessageType::CheckLocalFocusMessage) {
	FocusPoint = Point;

    MessageTriggerWidget = TriggerWidget;
}
VKillFocusMessage::VKillFocusMessage(HWND TriggerWidget)
	: VMessage(VMessageType::KillFocusMessage) {
    MessageTriggerWidget = TriggerWidget;
}

VQuitWindowMessage::VQuitWindowMessage(HWND TriggerWidget)
    : VMessage(VMessageType::QuitWindowMessage) {
    MessageTriggerWidget = TriggerWidget;
}

}
VLIB_END_NAMESPACE