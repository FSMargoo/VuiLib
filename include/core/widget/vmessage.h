// License(MIT)
// Athuor: Margoo
// This is Message in vuilib
#pragma once

#include "../vbasiccore.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

enum VMessageType {
	UnknowMessage, MouseMoveMessage, MouseClickedMessage,
	KeyClickedMessage, RepaintMessage, GetRepaintAeraMessage,
	IMECharMessage, MouseWheelMessage, FreeResourceMessage,
	CheckLocalFocusMessage, KillFocusMessage
};

typedef class VMessage {
 private:
	 VMessageType MessageType;

 public:
	 explicit VMessage(VMessageType Type = UnknowMessage);

	 VMessageType GetType() {
	   	 return MessageType;
	 }
} VBasicMessage;

class VFreeSourceMessage : public VMessage {
 public:
 	 VFreeSourceMessage();
};

class VMouseMoveMessage : public VMessage {
 public:
	 VPoint MousePosition;

 public:
	 VMouseMoveMessage(int X, int Y);
};

class VMouseWheelMessage : public VMessage {
 public:
	 VPoint MousePosition;

	 short  WheelValue;

 public:
	 VMouseWheelMessage(int X, int Y, short WheelParameter);
};

typedef enum VMouseClickedFlag {
	Down, Up
} VkeyClickedFlag;
enum VMouseKeyFlag {
	Middle, Left, Right
};

class VMouseClickedMessage : public VMessage {
 public:
	 VPoint			  MousePosition;

	 VMouseClickedFlag ClickedMethod;
	 VMouseKeyFlag     ClickedKey;

 public:
	 VMouseClickedMessage(int X, int Y, VMouseClickedFlag ClickedFlag, VMouseKeyFlag Key);
};

class VKeyClickedMessage : public VMessage {
 public:
	 byte			KeyVKCode;
	 bool			KeyPrevDown;
	 bool			KeyExtened;
	 VkeyClickedFlag KeyStats;

 public:
	 VKeyClickedMessage(byte VKCode, bool PrevDown, bool Extened, VkeyClickedFlag Stats);
};

class VRepaintMessage : public VMessage {
 public:
 	 VRect DirtyRectangle;

 public:
	 explicit VRepaintMessage(const VRect& RepaintRegion);
};

class VGetRepaintAeraMessage : public VMessage {
 public:
 	 VRect* RepaintAera;
 
 public:
 	 explicit VGetRepaintAeraMessage(VRect& RepaintRegion);
};

class VIMECharMessage : public VMessage {
 public:
	 wchar_t IMEChar;

 public:
	 explicit VIMECharMessage(wchar_t CharInputed);
};

class VCheckFocusMessage : public VMessage {
 public:
	 VPoint FocusPoint;

 public:
	 explicit VCheckFocusMessage(const VPoint& Point);
};

class VKillFocusMessage : public VMessage {
 public:
	 VKillFocusMessage();
};
}

VLIB_END_NAMESPACE
