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
	CheckLocalFocusMessage, KillFocusMessage, QuitWindowMessage,
	BlurCombinationOnRend
};

typedef class VMessage {
 private:
	 VMessageType MessageType;

 public:
     UINT         Win32ID;
     HWND         MessageTriggerWidget = NULL;

 public:
	 explicit VMessage(VMessageType Type = UnknowMessage);

	 VMessageType GetType() {
	   	 return MessageType;
	 }
} VBasicMessage;

class VBlurCombinationOnRend : public VMessage {
 public:
 	 VRect DirtyRectangle;

 public:
	 explicit VBlurCombinationOnRend(HWND TriggerWidget, const VRect& RepaintRegion);
};

class VFreeSourceMessage : public VMessage {
 public:
 	 VFreeSourceMessage(HWND TriggerWidget);
};

class VMouseMoveMessage : public VMessage {
 public:
	 VPoint MousePosition;

 public:
	 VMouseMoveMessage(HWND TriggerWidget, int X, int Y);
};

class VMouseWheelMessage : public VMessage {
 public:
	 VPoint MousePosition;

	 short  WheelValue;

 public:
	 VMouseWheelMessage(HWND TriggerWidget, int X, int Y, short WheelParameter);
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
	 VMouseClickedMessage(HWND TriggerWidget, int X, int Y, VMouseClickedFlag ClickedFlag, VMouseKeyFlag Key);
};

class VKeyClickedMessage : public VMessage {
 public:
	 byte			 KeyVKCode;
	 bool			 KeyPrevDown;
	 bool			 KeyExtened;
	 VkeyClickedFlag KeyStats;

 public:
	 VKeyClickedMessage(HWND TriggerWidget, byte VKCode, bool PrevDown, bool Extened, VkeyClickedFlag Stats);
};

class VRepaintMessage : public VMessage {
 public:
 	 VRect DirtyRectangle;

 public:
	 explicit VRepaintMessage(HWND TriggerWidget, const VRect& RepaintRegion);
};

class VGetRepaintAeraMessage : public VMessage {
 public:
 	 VRect* RepaintAera;
 
 public:
 	 explicit VGetRepaintAeraMessage(HWND TriggerWidget, VRect& RepaintRegion);
	 ~VGetRepaintAeraMessage();
};

class VIMECharMessage : public VMessage {
 public:
	 wchar_t IMEChar;

 public:
	 explicit VIMECharMessage(HWND TriggerWidget, wchar_t CharInputed);
};

class VCheckFocusMessage : public VMessage {
 public:
	 VPoint	FocusPoint;
	 void*	Object;
	 bool	Click;

 public:
	 explicit VCheckFocusMessage(HWND TriggerWidget, const VPoint& Point, void* MessageObject, const bool& MouseClick = false);
};

class VKillFocusMessage : public VMessage {
 public:
	 VKillFocusMessage(HWND TriggerWidget);
};

class VQuitWindowMessage : public VMessage {
 public:
    VQuitWindowMessage(HWND TriggerWidget);
};

}

VLIB_END_NAMESPACE
