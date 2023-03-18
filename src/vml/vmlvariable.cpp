#include "../../include/vml/vmlvariable.h"

VLIB_BEGIN_NAMESPACE

namespace VML
{
VMLBasicVariable::VMLBasicVariable(Core::VUIObject *TargetObject) : BindObject(TargetObject)
{
}
VMLIntVariable::VMLIntVariable(Core::VUIObject *TargetObject, const int &IValue)
	: VMLBasicVariable(TargetObject), Value(IValue)
{
}
VMLIntVariable *VMLIntVariable::operator=(const VMLIntVariable &TargetVariable)
{
	Value = TargetVariable.Value;
	BindObject->Update();
	Update();

	return this;
}
VMLIntVariable *VMLIntVariable::operator=(const int &TargetVariable)
{
	Value = TargetVariable;
	BindObject->Update();
	Update();

	return this;
}
bool VMLIntVariable::operator!=(const int &JudgetValue)
{
	return Value != JudgetValue;
}
bool VMLIntVariable::operator==(const int &JudgetValue)
{
	return Value == JudgetValue;
}
bool VMLIntVariable::operator!=(const VMLIntVariable &JudgetValue)
{
	return Value != JudgetValue.Value;
}
bool VMLIntVariable::operator==(const VMLIntVariable &JudgetValue)
{
	return Value == JudgetValue.Value;
}
bool VMLIntVariable::operator>=(const VMLIntVariable &JudgetValue)
{
	return Value >= JudgetValue.Value;
}
bool VMLIntVariable::operator<=(const VMLIntVariable &JudgetValue)
{
	return Value >= JudgetValue.Value;
}
bool VMLIntVariable::operator<(const VMLIntVariable &JudgetValue)
{
	return Value < JudgetValue.Value;
}
bool VMLIntVariable::operator>(const VMLIntVariable &JudgetValue)
{
	return Value > JudgetValue.Value;
}
VMLIntVariable::operator int()
{
	return Value;
}

VMLDoubleVariable::VMLDoubleVariable(Core::VUIObject *TargetObject, const double &IValue)
	: VMLBasicVariable(TargetObject), Value(IValue)
{
}
VMLDoubleVariable *VMLDoubleVariable::operator=(const VMLDoubleVariable &TargetVariable)
{
	Value = TargetVariable.Value;
	BindObject->Update();
	Update();

	return this;
}
VMLDoubleVariable *VMLDoubleVariable::operator=(const double &TargetVariable)
{
	Value = TargetVariable;
	BindObject->Update();
	Update();

	return this;
}
bool VMLDoubleVariable::operator!=(const double &JudgetValue)
{
	return Value != JudgetValue;
}
bool VMLDoubleVariable::operator==(const double &JudgetValue)
{
	return Value == JudgetValue;
}
bool VMLDoubleVariable::operator!=(const VMLDoubleVariable &JudgetValue)
{
	return Value != JudgetValue.Value;
}
bool VMLDoubleVariable::operator==(const VMLDoubleVariable &JudgetValue)
{
	return Value == JudgetValue.Value;
}
bool VMLDoubleVariable::operator>=(const VMLDoubleVariable &JudgetValue)
{
	return Value >= JudgetValue.Value;
}
bool VMLDoubleVariable::operator<=(const VMLDoubleVariable &JudgetValue)
{
	return Value >= JudgetValue.Value;
}
bool VMLDoubleVariable::operator<(const VMLDoubleVariable &JudgetValue)
{
	return Value < JudgetValue.Value;
}
bool VMLDoubleVariable::operator>(const VMLDoubleVariable &JudgetValue)
{
	return Value > JudgetValue.Value;
}
VMLDoubleVariable::operator double()
{
	return Value;
}

VMLStringVariable::VMLStringVariable(Core::VUIObject *TargetObject, const VString &IValue)
	: VMLBasicVariable(TargetObject), Value(IValue)
{
}
VMLStringVariable *VMLStringVariable::operator=(const VMLStringVariable &TargetVariable)
{
	Value = TargetVariable.Value;
	BindObject->Update();
	Update();

	return this;
}
VMLStringVariable *VMLStringVariable::operator=(const VString &TargetVariable)
{
	Value = TargetVariable;
	BindObject->Update();
	Update();

	return this;
}
bool VMLStringVariable::operator!=(const VString &JudgetValue)
{
	return Value != JudgetValue;
}
bool VMLStringVariable::operator==(const VString &JudgetValue)
{
	return Value == JudgetValue;
}
bool VMLStringVariable::operator!=(const VMLStringVariable &JudgetValue)
{
	return Value != JudgetValue.Value;
}
bool VMLStringVariable::operator==(const VMLStringVariable &JudgetValue)
{
	return Value == JudgetValue.Value;
}
VMLStringVariable::operator VString()
{
	return Value;
}

VMLBooleanVariable::VMLBooleanVariable(Core::VUIObject *TargetObject, const bool &IValue)
	: VMLBasicVariable(TargetObject), Value(IValue)
{
}
VMLBooleanVariable *VMLBooleanVariable::operator=(const VMLBooleanVariable &TargetVariable)
{
	Value = TargetVariable.Value;
	BindObject->Update();
	Update();

	return this;
}
VMLBooleanVariable *VMLBooleanVariable::operator=(const bool &TargetVariable)
{
	Value = TargetVariable;
	BindObject->Update();
	Update();

	return this;
}
bool VMLBooleanVariable::operator!=(const bool &JudgetValue)
{
	return Value != JudgetValue;
}
bool VMLBooleanVariable::operator==(const bool &JudgetValue)
{
	return Value == JudgetValue;
}
bool VMLBooleanVariable::operator!=(const VMLBooleanVariable &JudgetValue)
{
	return Value != JudgetValue.Value;
}
bool VMLBooleanVariable::operator==(const VMLBooleanVariable &JudgetValue)
{
	return Value == JudgetValue.Value;
}
VMLBooleanVariable::operator bool()
{
	return Value;
}
} // namespace VML

VLIB_END_NAMESPACE