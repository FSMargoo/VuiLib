/*
 * VBlurLabel.hpp
 *	@description  : A Blur Label
*/

#pragma once

#include "vuiobject.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VControlGroup class:
 *	@description  : A Group Container Of Control
*/
class VControlGroup {
private:
	std::vector<VUIObject*> ObjectContainer;

public:
	/*
	 * Build Up
	*/

	VControlGroup() {

	}

	/*
	 * AddObject Functional:
	 *	@description  : Add an Object Into This Group
	*/
	void AddObject(VUIObject* Object) {
		ObjectContainer.push_back(Object);
	}

	/*
	 * Hide Functional:
	 *	@description  : Hide All the Object in This Group
	*/
	void Hide() {
		for (auto& Object : ObjectContainer) {
			Object->Hide();
		}
	}
	/*
	 * Show Functional:
	 *	@description  : Show All the Object in This Group
	*/
	void Show() {
		for (auto& Object : ObjectContainer) {
			Object->Show();
		}
	}

	/*
	 * Begin Functional:
	 *	@description   : Get the Begin Iterator
	*/
	auto Begin() -> decltype(ObjectContainer.begin())  {
		return ObjectContainer.begin();
	}
	/*
	 * End Functional:
	 *	@description  : Get the End Iterator
	*/
	auto End() -> decltype(ObjectContainer.end()) {
		return ObjectContainer.end();
	}
};

VLIB_END_NAMESPACE