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

protected:
	/**
	 * When the control received the repaint message, the virtual function will
	 * be called for drawing
	 * @param Surface The surface will be created by the parent class and provide it
	 * to this sub object.
	 */
	virtual void OnPaint(const sk_sp<VSurface> &Surface) = 0;
	/**
	 * The virtual function that to be called when the property was changed
	 */
	virtual void OnPropertyChange() {

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
	void GetPropertyValue(const std::string &Name) {
		return static_cast<Type*>(GetProperty(Name).GetValue());
	}

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

protected:
	VPropertyList _propertyList;

protected:
	VObject                 *_parent;
	std::vector<VObject *>   _childList;
};