/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge,us files (the "Software"), to deal
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
 * \file vObject.h
 * \brief The base class object for the drawable class in VUILib
 */

#pragma once

#include <include/base/object/vObjectProperty.h>
#include <include/renderer/vRenderTarget.h>
#include <include/base/message/vMessage.h>
#include <include/renderer/vSurface.h>
#include <include/base/vBase.h>

/**
 * The base class object for the drawable class in VUILib
 * Every object that is drawable in the user interface should inherit this class
 * to ensure that it can join the message loop correctly
 */
class VObject {
public:
	/**
	 * Create the object with no parent object
	 */
	VObject();
	/**
	 * Create the object with a specified parent object pointer
	 * @param Parent The parent pointer. The lifetime of the pointer
	 * should be maintained by the user
	 */
	explicit VObject(VObject *Parent);
	virtual ~VObject() = default;

public:
	/**
	 * Get the property by the specified name
	 * @param Name The name of the property
	 * @return The reference of the property instance
	 */
	VObjectProperty& GetProperty(const std::string &Name);

public:
	/**
	 * Set the new parent object, the old pointer in the old parent object
	 * will be removed.
	 * @param Parent The new parent object
	 */
	void SetParent(VObject *Parent);
	/**
	 * Get the parent pointer in const form
	 * @return A VObject pointer referred to the parent object
	 */
	[[nodiscard]] const VObject *GetParent() const;

public:
	/**
	 * Raise this object in the top of the parent list;
	 * To use this method, the parent of the object should not
	 * be nullptr
	 */
	void RaiseTop();
	/**
	 * Raise this object in the lowest of the parent list;
	 * To use this method, the parent of the object should not
	 * be nullptr
	 */
	void RaiseLowest();
	/**
	 * Raise this object under the target object in the parent list;
	 * To use this method, the parent of the object should not be nullptr,
	 * and the target object should have the same parent with this object
	 */
	void RaiseUnder(const VObject *Target);
	/**
	 * Raise this object upper the target object in the parent list;
	 * To use this method, the parent of the object should not be nullptr,
	 * and the target object should have the same parent with this object
	 */
	void RaiseUpper(const VObject *Target);

public:
	/**
	 * Adjust the bound geometry information
	 * @param Width The width of new geometry information
	 * @param Height The height of new geometry information
	 */
	virtual void Resize(const int &Width, const int &Height);
	/**
	 * Adjust the position of the object
	 * @param X The new X axis position
	 * @param Y The new Y axis position
	 */
	void Move(const int &X, const int &Y);
	/**
	 * Set the object visible, it will process messages normally
	 */
	virtual void Show();
	/**
	 * Set the object invisible, it will ignore every meesage
	 */
	virtual void Hide();

public:
	/**
	 * Set the enabled status of the control
	 * @param Status If the status is true, this control will be disabled, nor enabled;
	 * The disabled property won't affect the specified control like the window
	 */
	void SetDisabled(const bool &Status);
	/**
	 * Get the disabled status of the control
	 * @return If the value is true, this control is disabled, nor enabled
	 */
	[[nodiscard]] bool GetDisabled() const;
	/**
	 * Set the X position of the object
	 * @param X The X position of the object
	 */
	void SetX(const int &X);
	/**
	 * Get the X position of the object;
	 * @return The X position of the object
	 */
	const int& GetX() const;
	/**
	 * Set the Y position of the object
	 * @param Y The Y position of the object
	 */
	void SetY(const int &Y);
	/**
	 * Get the Y position of the object;
	 * @return The Y position of the object
	 */
	 const int& GetY() const;

protected:
	/**
	 * Upload the message into the parent object if the parent object
	 * still has the parent object it will still send to the parent object
	 * of the parent object
	 * @param Message The message to be sent, it will be released in the final parent
	 * object
	 */
	void UploadMessage(VBaseMessage *Message);
	/**
	 * Upload repaint message by the current object's geometery information
	 */
	void RepaintOnySelf();

protected:
	/**
	 * When the object received message, this function will be called
	 * @param Message The message to be processed
	 * @param Surface The target surface to be painted on, if there is a repaint
	 * message
	 * @return Whether this message will be cut off, if true, the message will be cut off
	 */
	bool OnMessage(VBaseMessage *Message, sk_sp<VSurface> &Surface);
	/**
	 * When the final parent object received the message, this function will be emitted
	 * @param Message The message to be processed
	 */
	virtual void OnFinalMessage(VBaseMessage *Message) {

	}
	/**
	 * When the control received the repaint message, the virtual function will
	 * be called for drawing
	 * @param Surface The surface will be created by the parent class and provide it
	 * to this sub object.
	 */
	virtual void OnPaint(sk_sp<VSurface> &Surface) = 0;
	/**
	 * The virtual function that to be called when the property was changed
	 */
	virtual void OnPropertyChange() {

	}
	/**
	 * When the mouse moved over this object, this function will be called
	 * @param Message The mouse message
	 * @return If return false, this control will not take over the focus of
	 * the widget, nor it will take over the focus of the widget
	 */
	virtual bool OnMouseMoved(VMouseMovedMessage *Message) {
		return false;
	}
	/**
	 * When the mouse clicked over this object, this function will be called
	 * @param Message The mouse message
	 * @return If return false, this control will not take over the focus of
	 * the widget, nor it will take over the focus of the widget
	 */
	virtual bool OnMouseClicked(VMouseClickedMessage *Message) {
		return false;
	}

protected:
	/**
	 * Add a property to the object
	 * @tparam Type The type of the target value
	 * @param Name The property name
	 * @param Pointer The pointer referred to the pointer
	 */
	template<class Type>
		requires std::is_base_of_v<VPropertyValueBase, Type>
	void RegisterProperty(const std::string &Name, std::unique_ptr<Type> &&Pointer) {
		return RegisterProperty(Name, std::move(reinterpret_cast<std::unique_ptr<VPropertyValueBase>&&>(Pointer)));
	}
	/**
	 * Add a property to the object
	 * @param Name The property name
	 * @param Pointer The pointer referred to the pointer
	 */
	void RegisterProperty(const std::string &Name, std::unique_ptr<VPropertyValueBase> &&Pointer);
	/**
	 * Get the property value in specified type format
	 * @tparam Type The type of the target value
	 * @param Name The property name
	 */
	template<class Type>
	    requires std::is_base_of_v<VPropertyValueBase, Type>
	Type *GetPropertyValue(const std::string &Name) {
		return GetProperty(Name).GetValue()->Cast<Type>();
	}

protected:
	/**
	 * Lock the focus object, which the focus object can't be changed;
	 * This API will keep the current focus object and won't change
	 */
	virtual void LockFocus();
	/**
	 * Release the focus locking status
	 */
	virtual void UnlockFocus();
	/**
	 * Get the focus locking status
	 * @return If the focus is locking, returning true, nor returning false;
	 * Although the focus is locking, the control will still process the user
	 * input if it can
	 */
	virtual bool GetFocusLocking();
	/**
	 * Raise up an object as a focusing object
	 * @param Object The object to be raised up as a focusing object
	 */
	virtual void RaiseUpAsFocus(VObject *Object);
	/**
	 * Get the object in focus
	 * @return The object which is in focus
	 */
	virtual VObject *GetFocusingObject();

private:
	/**
	 * Remove the old pointer in the object parent object
	 * @param OldParent The old parent pointer
	 */
	void RemoveParent(VObject *OldParent);
	/**
	 * Add this object into the new parent object
	 * @param Parent The parent pointer
	 */
	void AdaptParent(VObject *Parent);
	/**
	 * Init the general property of every object
	 */
	void InitGeneralProperty();

protected:
	VPropertyList _propertyList;

protected:
	VRectProperty           *_bound;
	VBooleanProperty        *_visible;
	VBooleanProperty        *_disable;
	VObject                 *_parent;
	std::vector<VObject *>   _childList;
};