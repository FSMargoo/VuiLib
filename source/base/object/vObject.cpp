/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file vObject.cpp
 * \brief The base class object for the drawable class in VUILib
 */

#include <include/base/object/vObject.h>

VObject::VObject() : _parent(nullptr) {

}
VObject::VObject(VObject *Parent) : _parent(Parent) {
	AdaptParent(Parent);
}
void VObject::SetParent(VObject *Parent) {
	if (Parent == _parent) {
		throw std::logic_error("The parent pointer should changed!");
	}
	RemoveParent(Parent);
	_parent = Parent;
	AdaptParent(Parent);
}
const VObject *VObject::GetParent() const {
	return _parent;
}
void VObject::RaiseTop() {
	if (_parent != nullptr) {
		_parent->_childList.erase(std::remove_if(
		                                  _parent->_childList.begin(),
		                                  _parent->_childList.end(),
		                                    [this](VObject *Object) {
			                                    return Object == this;
		                                    }),
		                          _parent->_childList.end());
		_parent->_childList.push_back(this);
	}
	else {
		throw std::logic_error("The parent pointer should not be nullptr!");
	}
}
void VObject::RaiseLowest() {
	if (_parent != nullptr) {
		_parent->_childList.erase(std::remove_if(
		                                  _parent->_childList.begin(),
		                                  _parent->_childList.end(),
		                                  [this](VObject *Object) {
			                                  return Object == this;
		                                  }),
		                          _parent->_childList.end());
		_parent->_childList.insert(_parent->_childList.begin(), this);
	}
	else {
		throw std::logic_error("The parent pointer should not be nullptr!");
	}
}
void VObject::RaiseUnder(const VObject *Target) {
	if (_parent != nullptr && Target->GetParent() == _parent) {
		_parent->_childList.erase(std::remove_if(
		                                  _parent->_childList.begin(),
		                                  _parent->_childList.end(),
		                                  [this](VObject *Object) {
			                                  return Object == this;
		                                  }),
		                          _parent->_childList.end());
		auto where = _parent->_childList.begin();
		for (auto& object : _parent->_childList) {
			if (object == Target) {
				break;
			}

			++where;
		}
		_parent->_childList.insert(where, this);
	}
	else {
		throw std::logic_error("The parent pointer should not be nullptr, and the target object should have the same parent with this object!");
	}
}
void VObject::RaiseUpper(const VObject *Target) {
	if (_parent != nullptr && Target->GetParent() == _parent) {
		_parent->_childList.erase(std::remove_if(
		                                  _parent->_childList.begin(),
		                                  _parent->_childList.end(),
		                                  [this](VObject *Object) {
			                                  return Object == this;
		                                  }),
		                          _parent->_childList.end());
		auto where = _parent->_childList.begin();
		for (auto& object : _parent->_childList) {
			++where;

			if (object == Target) {
				break;
			}
		}
		_parent->_childList.insert(where, this);
	}
	else {
		throw std::logic_error("The parent pointer should not be nullptr, and the target object should have the same parent with this object!");
	}
}
VObjectProperty& VObject::GetProperty(const std::string &Name) {
	return _propertyList.find(Name)->second;
}
void VObject::RegisterProperty(const std::string &Name, std::unique_ptr<VPropertyValueBase> &&Pointer) {
	if (_propertyList.find(Name) != _propertyList.end()) {
		throw std::logic_error("VProperty should not be redefined");
	}

	_propertyList.insert({ Name, VObjectProperty(Name, std::move(Pointer)) });
}
void VObject::RemoveParent(VObject *OldParent) {
	if (OldParent != nullptr) {
		OldParent->_childList.erase(std::remove_if(
		                                    OldParent->_childList.begin(),
		                                    OldParent->_childList.end(),
		                                    [this](VObject *Object) {
			                                    return Object == this;
		                                    }),
		                            OldParent->_childList.end());
	}
}
void VObject::AdaptParent(VObject *Parent) {
	if (Parent != nullptr) {
		Parent->_childList.push_back(this);
	}
}