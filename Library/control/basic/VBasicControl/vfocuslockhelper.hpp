/*
 * VFocusLockHelper.hpp
 *	@description : Provide a Helper Class
*/

#pragma once

#include "vuiobject.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VFocusLockHelper class:
 *	@description : Provide a Helper Class
*/
class VFocusLockHelper : public VUIObject {
private:
	bool LockHelperInFocus = false;

public:
	virtual void FocusLocked() {

	}
	virtual void FocusUnlocked() {

	}

public:
	VSignal<> LockedFocus;
	VSignal<> UnlockedFocus;

public:
	void MouseLeftClicked(VMouseClickedFlag ClickedFlag) override {
		switch (ClickedFlag) {
		case VMouseClickedFlag::Down: {
			LockGlobalFocusID(Kernel()->GlobalID);

			LockHelperInFocus = true;

			FocusLocked();

			LockedFocus.Emit();

			break;
		}
		case VMouseClickedFlag::Up: {
			UnlockGlobalFocusID();

			LockHelperInFocus = false;

			FocusUnlocked();
			UnlockedFocus.Emit();

			break;
		}
		}
	}

	void DealyMessage(VMessage* Message) override {
		if (LockHelperInFocus == true) {
			switch (Message->GetType()) {
			case VMessageType::MouseClickedMessage: {
				VMouseClickedMessage* ClickedMessage = static_cast<VMouseClickedMessage*>(Message);

				if (ClickedMessage->MousePosition.InsideRect(Surface()->Rect) == false) {
					UnlockGlobalFocusID();

					LockHelperInFocus = false;
					UnlockedFocus.Emit();
				}

				break;
			}
			}
		}
	}
};

VLIB_END_NAMESPACE