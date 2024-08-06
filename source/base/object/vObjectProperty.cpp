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
 * \file vObjectProperty.cpp
 * \brief The property class for VObject class
 */

#include <include/base/object/vObjectProperty.h>

VPropertyValueBase::VPropertyValueBase(VPropertyType Type) : _type(Type) {

}
VIntProperty::VIntProperty()
    : VPropertyValueBase(VPropertyType::Int), _value(0) {

}
VIntProperty::VIntProperty(const int &Value)
    : VPropertyValueBase(VPropertyType::Int), _value(Value) {

}
VLongProperty::VLongProperty()
    : VPropertyValueBase(VPropertyType::Long), _value(0) {

}
VLongProperty::VLongProperty(const long &Value)
    : VPropertyValueBase(VPropertyType::Long), _value(Value) {

}
VFloatProperty::VFloatProperty()
    : VPropertyValueBase(VPropertyType::Float), _value(0.f) {

}
VFloatProperty::VFloatProperty(const float &Value)
    : VPropertyValueBase(VPropertyType::Long), _value(Value) {

}
VBooleanProperty::VBooleanProperty()
    : VPropertyValueBase(VPropertyType::Boolean), _value() {

}
VBooleanProperty::VBooleanProperty(const bool &Value)
    : VPropertyValueBase(VPropertyType::Boolean), _value(Value) {

}
VStringProperty::VStringProperty()
    : VPropertyValueBase(VPropertyType::String), _value() {

}
VStringProperty::VStringProperty(const std::string &Value)
    : VPropertyValueBase(VPropertyType::String), _value(Value) {

}
VRectProperty::VRectProperty()
    : VPropertyValueBase(VPropertyType::Rect), _value() {

}
VRectProperty::VRectProperty(const VRect &Value)
    : VPropertyValueBase(VPropertyType::Rect), _value(Value) {

}
VPointProperty::VPointProperty()
    : VPropertyValueBase(VPropertyType::Point), _value() {

}
VPointProperty::VPointProperty(const VPoint &Value)
    : VPropertyValueBase(VPropertyType::Point), _value(Value) {

}
VColorProperty::VColorProperty()
    : VPropertyValueBase(VPropertyType::Color), _value() {

}
VColorProperty::VColorProperty(const SkColor &Value)
    : VPropertyValueBase(VPropertyType::Color), _value(Value) {

}
VObjectProperty::VObjectProperty(const std::string &Name, std::unique_ptr<VPropertyValueBase> &&Value)
	: _name(Name), _value(std::move(Value)) {

}
std::string VObjectProperty::GetName() const {
	return _name;
}
std::unique_ptr<VPropertyValueBase>& VObjectProperty::GetValue() {
	return _value;
}