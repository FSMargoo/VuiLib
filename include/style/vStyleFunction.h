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
 * \file vStyleFunction.h
 * \brief The function method to store the style sheet
 */

#pragma once

#include <include/renderer/vColorFactory.h>
#include <include/base/object/vObject.h>

using VStyleProperty = VObjectProperty;

/**
 * The style function store class for the style system in VUILib
 */
class VStyleFunction final {
public:
	VStyleFunction() = default;

public:
	/**
	 * Get the property by the specified name
	 * @param Name The name of the property
	 * @return The reference of the property instance
	 */
	VStyleProperty& GetProperty(const OString &Name);

protected:
	/**
	 * Add a property to the object
	 * @tparam Type The type of the target value
	 * @param Name The property name
	 * @param Pointer The pointer referred to the pointer
	 */
	template<class Type>
	    requires std::is_base_of_v<VPropertyValueBase, Type>
	void RegisterProperty(const OString &Name, std::unique_ptr<Type> &&Pointer) {
		return RegisterProperty(Name, std::move(reinterpret_cast<std::unique_ptr<VPropertyValueBase>&&>(Pointer)));
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
	template<class Type>
	    requires std::is_base_of_v<VPropertyValueBase, Type>
	Type* GetPropertyValue(const OString &Name) {
		return GetProperty(Name).GetValue()->Cast<Type>();
	}

private:
	friend class VRadixStyleFactory;
	friend class VApplication;

protected:
	VPropertyList _propertyList;
};