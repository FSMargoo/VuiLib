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
#include <include/renderer/vColorFactory.h>

#include <memory>
#include <string>
#include <unordered_map>

/**
 * The type of the property
 */
enum class VPropertyType {
	Int,
	Long,
	Float,
	String,
	Boolean,
	Rect,
	Point,
	Color
};

/**
 * The base class of the property value
 */
class VPropertyValueBase {
public:
	/**
	 * Convert this pointer to the specified type, which is not always safe to call
	 * @tparam Type The type to be converted which is base of VPropertyValueBase
	 * @return The converted pointer
	 */
	template <class Type>
		requires std::is_base_of_v<VPropertyValueBase, Type>
	Type *Cast() {
		return static_cast<Type *>(this);
	}

protected:
	/**
	 * Construct the property by the property type
	 * @param Type
	 */
	explicit VPropertyValueBase(VPropertyType Type);

protected:
	virtual size_t GetID() {
		return typeid(this).hash_code();
	}

public:
	VPropertyType _type;
};
/**
 * The int property of the property value
 */
class VIntProperty : public VPropertyValueBase {
public:
	VIntProperty();
	explicit VIntProperty(const int &Value);

public:
	/**
	 * Get the property value
	 * @return The property value
	 */
	auto GetValue() const {
		return _value;
	}

public:
	int _value;
};
/**
 * The long property of the property value
 */
class VLongProperty : public VPropertyValueBase {
public:
	VLongProperty();
	explicit VLongProperty(const long &Value);

public:
	/**
	 * Get the property value
	 * @return The property value
	 */
	auto GetValue() const {
		return _value;
	}

public:
	long _value;
};
/**
 * The float property of the property value
 */
class VFloatProperty : public VPropertyValueBase {
public:
	VFloatProperty();
	explicit VFloatProperty(const float &Value);

public:
	/**
	 * Get the property value
	 * @return The property value
	 */
	auto GetValue() const {
		return _value;
	}

public:
	float _value;
};
/**
 * The boolean property of the property value
 */
class VBooleanProperty : public VPropertyValueBase {
public:
	VBooleanProperty();
	explicit VBooleanProperty(const bool &Value);

public:
	/**
	 * Get the property value
	 * @return The property value
	 */
	auto GetValue() const {
		return _value;
	}

public:
	bool _value;
};
/**
 * The string property of the property value
 */
class VStringProperty : public VPropertyValueBase {
public:
	VStringProperty();
	explicit VStringProperty(const OString &Value);

public:
	/**
	 * Get the property value
	 * @return The property value
	 */
	auto GetValue() const {
		return _value;
	}

public:
	OString _value;
};
/**
 * The rect property of the property value
 */
class VRectProperty : public VPropertyValueBase {
public:
	VRectProperty();
	explicit VRectProperty(const VRect &Value);

public:
	/**
	 * Get the property value
	 * @return The property value
	 */
	auto GetValue() const {
		return _value;
	}

public:
	VRect _value;
};
/**
 * The point property of the property value
 */
class VPointProperty : public VPropertyValueBase {
public:
	VPointProperty();
	explicit VPointProperty(const VPoint &Value);

public:
	/**
	 * Get the property value
	 * @return The property value
	 */
	auto GetValue() const {
		return _value;
	}

public:
	VPoint _value;
};
/**
 * The color property of the property value
 */
class VColorProperty : public VPropertyValueBase {
public:
	VColorProperty();
	explicit VColorProperty(const SkColor &Value);

public:
	/**
	 * Get the property value
	 * @return The property value
	 */
	auto GetValue() const {
		return _value;
	}

public:
	SkColor _value;
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
	VObjectProperty(const OString &Name, std::unique_ptr<VPropertyValueBase> &&Value);

public:
	[[nodiscard]] OString				 GetName() const;
	std::unique_ptr<VPropertyValueBase> &GetValue();

public:
	OString								_name;
	std::unique_ptr<VPropertyValueBase> _value;
};

using VPropertyList = std::unordered_map<OString, VObjectProperty>;

/**
 * The register interface of the property register
 */
class VPropertyRegisterInterface {
public:
	/**
	 * Add a property to the object
	 * @param Name The property name
	 * @param Pointer The pointer referred to the pointer
	 */
	virtual void RegisterProperty(const OString &Name, std::unique_ptr<VPropertyValueBase> &&Pointer) = 0;
};

/**
 * A register wrapper for a property, when an object provides a property input in its
 * children objects, it can use the register to provide a way to register a property easily
 * @tparam PropertyType The property type of the property generator
 * @tparam ValueType The property value of the property type
 */
template<class PropertyType, class ValueType>
	requires std::is_base_of_v<VPropertyValueBase, PropertyType>
class VPropertyRegister {
public:
	explicit VPropertyRegister(const OString &Name) : _name(Name) {

	}

public:
	/**
	 * Register the property by the specified interface base class
	 * @param Interface The interface pointer
	 * @param Value The value of the property
	 */
	void Register(VPropertyRegisterInterface *Interface, const ValueType &Value) {
		auto property = std::make_unique<PropertyType>(Value);
		Interface->RegisterProperty(_name, std::move(property));
	}

private:
	OString _name;

};