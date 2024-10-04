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

#include <include/base/message/vMessage.h>
#include <include/base/object/vObjectProperty.h>
#include <include/base/vBase.h>
#include <include/renderer/vRenderTarget.h>
#include <include/renderer/vSurface.h>

class VPushButton;

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
	VObjectProperty &GetProperty(const OString &Name);
	/**
	 * Judge whether the property is existed
	 * @param Name The name of the property
	 * @return If the property exist, return true, nor false
	 */
	bool HasProperty(const OString &Name) const;

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
	void SetX(const int X);
	/**
	 * Get the X position of the object;
	 * @return The X position of the object
	 */
	const int GetX() const;
	/**
	 * Set the Y position of the object
	 * @param Y The Y position of the object
	 */
	void SetY(const int Y);
	/**
	 * Get the Y position of the object;
	 * @return The Y position of the object
	 */
	const int GetY() const;
	/**
	 * Set the width of the object
	 * @param Get The width of the object
	 */
	void SetWidth(const int Width);
	/**
	 * Get the width of the object;
	 * @return The width of the object
	 */
	const int GetWidth() const;
	/**
	 * Set the height of the object
	 * @param Height The height of the object
	 */
	void SetHeight(const int Height);
	/**
	 * Get the height of the object;
	 * @return The height of the object
	 */
	const int GetHeight() const;

public:
	/**
	 * Set the opacity of the control, the control whose opacity value is 0.f won't be
	 * removed from the message objects like the Hide method
	 * @param Value The value of the opacity
	 */
	void SetOpacity(const float &Value);

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
	 * Upload repaint message by the current object's geometry information
	 */
	void RepaintOnSelf();

public:
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
	virtual void OnPaint(sk_sp<SkSurface> &Surface) = 0;
	/**
	 * The virtual function that to be called when the property was changed
	 */
	virtual void OnPropertyChange() {
	}
	/**
	 * The virtual function that to be called when the layout needs to be rearranged
	 */
	virtual void OnLayoutRearrange() {
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
	 * When the mouse moved out the bound of this object, this function will be called;
	 * This function dose not allows any return value, since it shouldn't change the focus
	 * object in this function
	 * @param Message The mouse message
	 */
	virtual void OnMouseLeft(VMouseMovedMessage *Message) {
		return;
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

public:
	/**
	 * Lock the size of the object, which not allows any layout to arrange
	 */
	void LockSizing();
	/**
	 * Lock the size of the object, which allows any layout to arrange
	 */
	void UnlockSizing();

private:
	/**
	 * When the object received a repaint message that this object can deal with,
	 * this function will be called
	 * @param Message The pointer of the repaint message
	 * @param Surface The surface reference
	 */
	virtual void OnRepaintMessage(VRepaintMessage *Message, sk_sp<SkSurface> &Surface);
	/**
	 * When the object received a general message, this function will be called
	 * @param Message The general message pointer
	 * @return If the object can handle this message and determined to cut off it from
	 * spreading to another object, the return value will be true nor false
	 */
	virtual bool OnGeneralMessage(VBaseMessage *Message);

public:
	/**
	 * Add a property to the object
	 * @tparam Type The type of the target value
	 * @param Name The property name
	 * @param Pointer The pointer referred to the pointer
	 */
	template <class Type>
		requires std::is_base_of_v<VPropertyValueBase, Type>
	void RegisterProperty(const OString &Name, std::unique_ptr<Type> &&Pointer) {
		return RegisterProperty(Name, std::move(reinterpret_cast<std::unique_ptr<VPropertyValueBase> &&>(Pointer)));
	}
	/**
	 * Add a property to the object
	 * @param Name The property name
	 * @param Pointer The pointer referred to the pointer
	 */
	void RegisterProperty(const OString &Name, std::unique_ptr<VPropertyValueBase> &&Pointer);
	/**
	 * Get the property value in specified type format
	 * @tparam Type The type of the target value
	 * @param Name The property name
	 */
	template <class Type>
		requires std::is_base_of_v<VPropertyValueBase, Type>
	Type *GetPropertyValue(const OString &Name) {
		return GetProperty(Name).GetValue()->Cast<Type>();
	}
	/**
	 * Set the specified property with specified value
	 * @tparam Type The type of the property
	 * @tparam ValueType The type of the property value
	 * @param Name The name of the property
	 * @param Value The value to be set
	 */
	template <class Type, class ValueType>
		requires std::is_base_of_v<VPropertyValueBase, Type>
	void SetPropertyValue(const OString &Name, const ValueType &Value) {
		if (HasProperty(Name)) {
			auto &property						  = GetProperty(Name);
			property._value->Cast<Type>()->_value = Value;
		}
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
	bool _onHover;

protected:
	VPropertyList _propertyList;

protected:
	VBooleanProperty	  *_userSpecifiedSize;
	VRectProperty		  *_bound;
	VBooleanProperty	  *_visible;
	VBooleanProperty	  *_disable;
	VFloatProperty		  *_opacity;
	VObject				  *_parent;
	std::vector<VObject *> _childList;

/**
 * PN means "property name"
 */
public:
	static constexpr const char *PN_UserSpecifiedSize = "user-specified-size";
	static constexpr const char *PN_Visible			  = "visible";
	static constexpr const char *PN_Bound			  = "bound";
	static constexpr const char *PN_Disable			  = "disable";
	static constexpr const char *PN_Opacity			  = "opacity";
};