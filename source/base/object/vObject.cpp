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
	InitGeneralProperty();
}
VObject::VObject(VObject *Parent) : _parent(Parent) {
	AdaptParent(Parent);

	InitGeneralProperty();
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

		RepaintOnySelf();
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

		RepaintOnySelf();
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

		RepaintOnySelf();
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

		RepaintOnySelf();
	}
	else {
		throw std::logic_error("The parent pointer should not be nullptr, and the target object should have the same parent with this object!");
	}
}
void VObject::Resize(const int &Width, const int &Height) {
	_bound->_value.Resize(Width, Height);

	RepaintOnySelf();
}
void VObject::Move(const int &X, const int &Y) {
	auto width  = _bound->_value.GetWidth();
	auto height = _bound->_value.GetHeight();

	_bound->_value = { X, Y, X + width, Y + height };

	RepaintOnySelf();
}
void VObject::Show() {
	_visible->_value = true;

	RepaintOnySelf();
}
void VObject::Hide() {
	_visible->_value = false;

	RepaintOnySelf();
}
void VObject::SetDisabled(const bool &Status) {
	_disable->_value = Status;
}
bool VObject::GetDisabled() const {
	return _disable->_value;
}
void VObject::SetX(const int &X) {
	_bound->_value.SetLeft(X);
}
void VObject::SetY(const int &Y) {
	_bound->_value.SetTop(Y);
}
const int& VObject::GetX() const {
	return _bound->_value.GetLeft();
}
const int& VObject::GetY() const {
	return _bound->_value.GetTop();
}
void VObject::UploadMessage(VBaseMessage *Message) {
	if (_parent != nullptr) {
		_parent->UploadMessage(Message);
	}
	else {
		OnFinalMessage(Message);
	}
}
void VObject::RepaintOnySelf() {
	auto message = new VRepaintMessage(nullptr, _bound->_value);
	UploadMessage(message);
}
bool VObject::OnMessage(VBaseMessage *Message, sk_sp<VSurface> &Surface) {
	if (Message->GetType() == VMessageType::Repaint) {
		OnRepaintMessage(Message->Cast<VRepaintMessage>(), Surface);
	}
	else {
		return OnGeneralMessage(Message);
	}

	return false;
}
void VObject::OnRepaintMessage(VRepaintMessage *Message, sk_sp<VSurface> &Surface) {
	const auto bound    = _bound->_value;
	auto repaintMessage = Message->Cast<VRepaintMessage>();
	if (Surface == nullptr) {
		throw std::logic_error("Surface should not be nullptr!");
	}
	/**
	  * Draw the context the relative surface
	  */
	auto subSurface = sk_make_sp<VSurface, const int &, const int &>(bound.GetWidth(), bound.GetHeight());

	OnPaint(subSurface);

	for (auto& object : _childList) {
		const auto leftTop = object->_bound->_value.GetLeftTopPoint();
		Surface->GetNativeSurface()->draw(subSurface->GetNativeSurface()->getCanvas(), leftTop.GetX(), leftTop.GetY());
	}

	auto point = bound.GetLeftTopPoint();
	Surface->GetNativeSurface()->draw(subSurface->GetNativeSurface()->getCanvas(), point.GetX(), point.GetY());
}
bool VObject::OnGeneralMessage(VBaseMessage *Message) {
	const auto bound = _bound->_value;
	/**
	 * Sent the message to children object first
	 */
	sk_sp<VSurface> nullSurface = nullptr;
	for (auto& object : _childList) {
		auto result = object->OnMessage(Message, nullSurface);
		if (result) {
			return true;
		}
	}
	switch (Message->GetType()) {
		case VMessageType::Repaint: {
			return false;
		}
		case VMessageType::MouseMove: {
			auto mouseMoveMessage = Message->Cast<VMouseMovedMessage>();
			if (bound.IsPointInside(mouseMoveMessage->Point)) {
				auto result = OnMouseMoved(mouseMoveMessage);
				if (result) {
					RaiseUpAsFocus(this);
				}

				_onHover = true;

				return result;
			}
			else if (_onHover) {
				_onHover = false;
				OnMouseLeft(mouseMoveMessage);
			}

			break;
		}
		case VMessageType::MouseClicked: {
			auto mouseClickedMessage = Message->Cast<VMouseClickedMessage>();
			if (bound.IsPointInside(mouseClickedMessage->Point)) {
				auto result = OnMouseClicked(mouseClickedMessage);
				if (result) {
					RaiseUpAsFocus(this);
				}

				return result;
			}

			break;
		}
	}

	return false;
}
void VObject::LockFocus() {
	if (_parent != nullptr) {
		return _parent->LockFocus();
	}
	else {
		throw std::logic_error("Object should have a legal parent objects");
	}
}
void VObject::UnlockFocus() {
	if (_parent != nullptr) {
		return _parent->UnlockFocus();
	}
	else {
		throw std::logic_error("Object should have a legal parent objects");
	}
}
bool VObject::GetFocusLocking() {
	if (_parent != nullptr) {
		return _parent->GetFocusLocking();
	}
	else {
		throw std::logic_error("Object should have a legal parent objects");
	}
}
void VObject::RaiseUpAsFocus(VObject *Object) {
	if (_parent != nullptr) {
		return _parent->RaiseUpAsFocus(Object);
	}
	else {
		throw std::logic_error("Object should have a legal parent objects");
	}
}
VObject *VObject::GetFocusingObject() {
	if (_parent != nullptr) {
		return _parent->GetFocusingObject();
	}
	else {
		throw std::logic_error("Object parent dose not suitable");
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
		RepaintOnySelf();
	}
}
void VObject::AdaptParent(VObject *Parent) {
	if (Parent != nullptr) {
		Parent->_childList.push_back(this);

		RepaintOnySelf();
	}
}
void VObject::InitGeneralProperty() {
	auto visible = std::make_unique<VBooleanProperty>(true);
	auto disable = std::make_unique<VBooleanProperty>(false);
	auto bound = std::make_unique<VRectProperty>();

	RegisterProperty("visible", std::move(visible));
	RegisterProperty("bound", std::move(bound));
	RegisterProperty("disable", std::move(bound));

	_bound   = GetPropertyValue<VRectProperty>("bound");
	_visible = GetPropertyValue<VBooleanProperty>("visible");
	_disable = GetPropertyValue<VBooleanProperty>("disable");
	_onHover = false;
}