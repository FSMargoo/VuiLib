/*
 * VAbstractButton.hpp
 *	@description : VLib's VAbstract Button
 *	@birth		 : 2022/6.10
*/

#pragma once

#include "vapplication.hpp"

VLIB_BEGIN_NAMESPACE

class VAbstractButton : public VUIObject {
public:
	/*
	 * Signal System
	*/
	VSignal<> ButtonPushed;
	VSignal<> ButtonLeftDown;

public:
	/*
	 * LeftClickedUp virtual Functional:
	 *	@description  : The Left Clicked Up
	*/
	virtual void LeftClickedUp()   {  /* Empty */  }
	/*
	 * LeftClickedDown virtual Functional:
	 *	@description  : The Left Clicked Down
	*/
	virtual void LeftClickedDown() {  /* Empty */ }

public:
	/*
	 * LeftButtonClicked override Functional:
	 *	@description  : Emit Button Pushed Signal
	*/
	void MouseLeftClicked(VMouseClickedFlag ClickedFlag) override {
		switch (ClickedFlag) {
		case VMouseClickedFlag::Down: {
			LeftClickedDown();

			ButtonLeftDown.Emit();

			break;
		}
		case VMouseClickedFlag::Up: {
			LeftClickedUp();

			ButtonPushed.Emit();

			break;
		}
		}
	}

public:
	/*
	 * Build up Functional
	*/
	
	VAbstractButton(VUIObject* Parent) : VUIObject(Parent) {  /*  Empty  */  }
};

VLIB_END_NAMESPACE