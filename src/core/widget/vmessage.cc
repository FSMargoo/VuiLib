#include "../../../include/core/widget/vmessage.h"

VLIB_BEGIN_NAMESPACE

namespace Core {
VMessage::VMessage(VMessageType Type)
	: MessageType(Type) {

}
VFreeSourceMessage::VFreeSourceMessage()
	: VMessage(VMessageType::FreeResourceMessage) {

}
VMouseMoveMessage::VMouseMoveMessage(int X, int Y)
	: VMessage(VMessageType::MouseMoveMessage) {
	MousePosition.X = X;
	MousePosition.Y = Y;
}
VMouseWheelMessage::VMouseWheelMessage(int X, int Y, short WheelParameter)
	: VMessage(VMessageType::MouseWheelMessage) {
	MousePosition.X = X;
	MousePosition.Y = Y;

	WheelValue = WheelParameter;
}
VMouseClickedMessage::VMouseClickedMessage(int X, int Y, VMouseClickedFlag ClickedFlag, VMouseKeyFlag Key)
	: VMessage(VMessageType::MouseClickedMessage) {
	MousePosition.X = X;
	MousePosition.Y = Y;

	ClickedMethod = ClickedFlag;
	ClickedKey = Key;
}
VKeyClickedMessage::VKeyClickedMessage(byte VKCode, bool PrevDown, bool Extened, VkeyClickedFlag Stats)
	: VMessage(VMessageType::KeyClickedMessage) {
	KeyVKCode = VKCode;
	KeyPrevDown = PrevDown;
	KeyExtened = Extened;
	KeyStats = Stats;
}
VRepaintMessage::VRepaintMessage(const VRect& RepaintRegion)
	: VMessage(VMessageType::RepaintMessage) {
	DirtyRectangle = RepaintRegion;
}
VGetRepaintAeraMessage::VGetRepaintAeraMessage(VRect RepaintRegion)
	: VMessage(VMessageType::GetRepaintAeraMessage) {
	RepaintAera = &RepaintRegion;
}
VIMECharMessage::VIMECharMessage(wchar_t CharInputed)
	: VMessage(VMessageType::IMECharMessage) {
	IMEChar = CharInputed;
}
VCheckFocusMessage::VCheckFocusMessage(const VPoint& Point)
	: VMessage(VMessageType::CheckLocalFocusMessage) {
	FocusPoint = Point;
}
VKillFocusMessage::VKillFocusMessage()
	: VMessage(VMessageType::KillFocusMessage) {
}

}
VLIB_END_NAMESPACE