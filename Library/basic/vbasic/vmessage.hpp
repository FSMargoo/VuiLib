/*
 * VMessage.hpp
 *	@description : VLib's Message Object
 *	@birth		 : 2022/6.5
*/
#pragma once

#include "../../render/vrender/vcanvas.hpp"
#include "vbase.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VMessageType enum:
 *	@description  : Describe The Type Of Message
*/
enum VMessageType {
	UnknowMessage,     MouseMoveMessage,  MouseClickedMessage,
	KeyClickedMessage, RepaintMessage,    GetRepaintAeraMessage,
	IMECharMessage,    MouseWheelMessage, FreeResourceMessage,
	CheckLocalFocusMessage
};

/*
 * VMessage class (alias VBasicMessage):
 *	@description  : The Basic Type Of Message, Each System Message Should Inherit This Class
*/
typedef class VMessage {
private:
	VMessageType MessageType;

public:
	/*
	 * Build up Functional
	*/

	VMessage(VMessageType Type = UnknowMessage)
		: MessageType(Type) {

	}

	/*
	 * GetType functional:
	 *	@description  : Get the Type Of Message
	*/
	VMessageType GetType() {
		return MessageType;
	}
} VBasicMessage;

class VFreeSourceMessage : public VMessage {
public:
	VFreeSourceMessage()
		: VMessage(VMessageType::FreeResourceMessage) {

	}
};

/*
 * VMouseMessage class:
 *	@description  : The Mouse Move Message
*/
class VMouseMoveMessage : public VMessage {
public:
	/*
	 * Mouse Position Information
	*/
	VPoint MousePosition;

public:
	/*
	 * VMouseMoveMessage Build UP Functional
	 *	@description  : Need X, Y parameter
	*/
	VMouseMoveMessage(int X, int Y)
		: VMessage(VMessageType::MouseMoveMessage) {
		MousePosition.x = X;
		MousePosition.y = Y;
	}
};

/*
 * VMouseWheelMessage class:
 *	@description  : The Mouse Wheel Message
*/
class VMouseWheelMessage : public VMessage {
public:
	/*
	 * Mouse Position Information
	*/
	VPoint MousePosition;

	short  WheelValue;

public:
	/*
	 * VMouseMoveMessage Build UP Functional
	 *	@description  : Need X, Y, WheelParameter parameter
	*/
	VMouseWheelMessage(int X, int Y, short WheelParameter)
		: VMessage(VMessageType::MouseWheelMessage) {
		MousePosition.x = X;
		MousePosition.y = Y;

		WheelValue      = WheelParameter;
	}
};

/*
 * VMouseClikedFlag enum:
 *	@description  : The Mouse Clicked Flag ( Up Or Down )
*/
typedef enum VMouseClickedFlag {
	Down, Up
} VkeyClickedFlag;
/*
 * VMouseKeyFlag enum:
 *	@description  : The Mouse Clicked Key Flag
*/
enum VMouseKeyFlag {
	Middle, Left, Right
};

/*
 * VMouseClickedMessage class:
 *	@description  : The Mouse Clicked Message
*/
class VMouseClickedMessage : public VMessage {
public:
	/*
	 * Mouse Position Information
	*/

	VPoint			  MousePosition;

	VMouseClickedFlag ClickedMethod;
	VMouseKeyFlag     ClickedKey;

public:
	/*
	 * VMouseMoveMessage Build UP Functional
	 *	@description  : Need X, Y, ClickedFlag, Key parameter
	*/
	VMouseClickedMessage(int X, int Y,
		VMouseClickedFlag ClickedFlag, VMouseKeyFlag Key)
		: VMessage(VMessageType::MouseClickedMessage) {
		MousePosition.x = X;
		MousePosition.y = Y;

		ClickedMethod = ClickedFlag;
		ClickedKey	  = Key;
	}
};

/*
 * VKeyClickedMessage class:
 *	@description  : Keyboard Clicked Message
*/
class VKeyClickedMessage : public VMessage {
public:
	/*
	 * Key Information
	*/

	byte			KeyVKCode;
	bool			KeyPrevDown;
	bool			KeyExtened;
	VkeyClickedFlag KeyStats;

public:
	VKeyClickedMessage(byte VKCode, bool PrevDown, bool Extened, VkeyClickedFlag Stats)
		: VMessage(VMessageType::KeyClickedMessage)  {
		KeyVKCode   = VKCode;
		KeyPrevDown = PrevDown;
		KeyExtened  = Extened;
		KeyStats    = Stats;
	}
};

/*
 * VRepaintMessage class:
 *	@description  : Provide a Dirty Rectangle For Object To Decide Paint Nor.
*/
class VRepaintMessage : public VMessage {
public:
	VRect DirtyRectangle;

public:
	VRepaintMessage(VRect RepaintRegion)
		: VMessage(VMessageType::RepaintMessage) {
		DirtyRectangle = RepaintRegion;
	}
};

/*
 * VGetRepaintAeraMessage class:
 *	@description  : Get the Finally Repaint Aera
*/
class VGetRepaintAeraMessage : public VMessage {
public:
	/*
	 * RepaintAera ptr VRect:
	 *	@description  : If this Rect Need Change, Use This Reference To Reset the Aera
	*/
	VRect* RepaintAera;

public:
	VGetRepaintAeraMessage(VRect RepaintRegion)
		: VMessage(VMessageType::GetRepaintAeraMessage) {
		RepaintAera = &RepaintRegion;
	}
};

/*
 * VIMECharMessage class:
 *	@description  : When A Window Recive A 
*/
class VIMECharMessage : public VMessage {
public:
	wchar_t IMEChar;

public:
	VIMECharMessage(wchar_t CharInputed)
		: VMessage(VMessageType::IMECharMessage) {
		IMEChar = CharInputed;
	}
};

/*
 * VCheckFocusMessage class:
 *	@description  : Check the Object's Focus
*/
class VCheckFocusMessage : public VMessage {
public:
	VPoint FocusPoint;

public:
	VCheckFocusMessage(VPoint Point)
		: VMessage(VMessageType::CheckLocalFocusMessage) {
		FocusPoint = Point;
	}
};

VLIB_END_NAMESPACE
