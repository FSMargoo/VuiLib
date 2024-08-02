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
 * \file vObjectProperty.h
 * \brief The property class for VObject class
 */

#pragma once

#include <include/base/geometry/vPoint.h>
#include <include/base/geometry/vRect.h>
#include <include/base/vBase.h>

#include <unordered_map>
#include <string>
#include <memory>

/**
 * The type of the property
 */
enum class VPropertyType {
	Int, Long, Float, String, Rect, Point
};

/**
 * The base class of the property value
 */
class VPropertyValueBase {
protected:
	/**
	 * Construct the property by the property type
	 * @param Type
	 */
	explicit VPropertyValueBase(const VPropertyType &Type);

private:
	VPropertyType _type;
};
/**
 * The int property of the property value
 */
class VIntProperty : public VPropertyValueBase {
public:
	VIntProperty();
	explicit VIntProperty(const int &Value);

private:
	int _value;
};
/**
 * The long property of the property value
 */
class VLongProperty : public VPropertyValueBase {
public:
	VLongProperty();
	explicit VLongProperty(const long &Value);

private:
	long _value;
};
/**
 * The float property of the property value
 */
class VFloatProperty : public VPropertyValueBase {
public:
	VFloatProperty();
	explicit VFloatProperty(const float &Value);

private:
	float _value;
};
/**
 * The string property of the property value
 */
class VStringProperty : public VPropertyValueBase {
public:
	VStringProperty();
	explicit VStringProperty(const std::string &Value);

private:
	std::string _value;
};
/**
 * The rect property of the property value
 */
class VRectProperty : public VPropertyValueBase {
public:
	VRectProperty();
	explicit VRectProperty(const VRect &Value);

private:
	VRect _value;
};
/**
 * The point property of the property value
 */
class VPointProperty : public VPropertyValueBase {
public:
	VPointProperty();
	explicit VPointProperty(const VPoint &Value);

private:
	VPoint _value;
};

/**
 * The property class of the object
 */
class VObjectProperty {
public:
	/**
	 * Construct the property by the property name
	 * @param Name The name of the property
	 * @param Value The pointer to the property value format in std::unique_ptr
	 */
	VObjectProperty(const std::string &Name, const std::unique_ptr<VPropertyValueBase> &Value);

public:
	[[nodiscard]] std::string GetName() const;
	std::unique_ptr<VPropertyValueBase>& GetValue();

private:
	std::string                         _name;
	std::unique_ptr<VPropertyValueBase> _value;
};

using VPropertyList = std::unordered_map<std::string, VObjectProperty>;