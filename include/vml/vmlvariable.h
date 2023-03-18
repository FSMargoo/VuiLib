#pragma once

#include "../core/object/vuiobject.h"

VLIB_BEGIN_NAMESPACE

namespace VML
{
class VMLBasicVariable
{
protected:
	Core::VUIObject *BindObject;

public:
	std::function<void()> Update;

public:
	VMLBasicVariable(Core::VUIObject *TargetObject);
};

class VMLIntVariable : public VMLBasicVariable
{
private:
	int Value;

public:
	VMLIntVariable(Core::VUIObject *TargetObject, const int &IValue);
	VMLIntVariable *operator=(const VMLIntVariable &TargetVariable);
	VMLIntVariable *operator=(const int &TargetVariable);
	bool			operator==(const int &JudgetValue);
	bool			operator!=(const int &JudgetValue);
	bool			operator==(const VMLIntVariable &JudgetValue);
	bool			operator!=(const VMLIntVariable &JudgetValue);
	bool			operator>=(const VMLIntVariable &JudgetValue);
	bool			operator<=(const VMLIntVariable &JudgetValue);
	bool			operator<(const VMLIntVariable &JudgetValue);
	bool			operator>(const VMLIntVariable &JudgetValue);
					operator int();
};
class VMLDoubleVariable : public VMLBasicVariable
{
private:
	double Value;

public:
	VMLDoubleVariable(Core::VUIObject *TargetObject, const double &IValue);
	VMLDoubleVariable *operator=(const VMLDoubleVariable &TargetVariable);
	VMLDoubleVariable *operator=(const double &TargetVariable);
	bool			   operator==(const double &JudgetValue);
	bool			   operator!=(const double &JudgetValue);
	bool			   operator==(const VMLDoubleVariable &JudgetValue);
	bool			   operator!=(const VMLDoubleVariable &JudgetValue);
	bool			   operator>=(const VMLDoubleVariable &JudgetValue);
	bool			   operator<=(const VMLDoubleVariable &JudgetValue);
	bool			   operator<(const VMLDoubleVariable &JudgetValue);
	bool			   operator>(const VMLDoubleVariable &JudgetValue);
					   operator double();
};
class VMLStringVariable : public VMLBasicVariable
{
private:
	VString Value;

public:
	VMLStringVariable(Core::VUIObject *TargetObject, const VString &IValue);
	VMLStringVariable *operator=(const VMLStringVariable &TargetVariable);
	VMLStringVariable *operator=(const VString &TargetVariable);
	bool			   operator==(const VMLStringVariable &JudgetValue);
	bool			   operator!=(const VMLStringVariable &JudgetValue);
	bool			   operator==(const VString &JudgetValue);
	bool			   operator!=(const VString &JudgetValue);
					   operator VString();
};
class VMLBooleanVariable : public VMLBasicVariable
{
private:
	bool Value;

public:
	VMLBooleanVariable(Core::VUIObject *TargetObject, const bool &IValue);
	VMLBooleanVariable *operator=(const VMLBooleanVariable &TargetVariable);
	VMLBooleanVariable *operator=(const bool &TargetVariable);
	bool				operator==(const bool &JudgetValue);
	bool				operator!=(const bool &JudgetValue);
	bool				operator==(const VMLBooleanVariable &JudgetValue);
	bool				operator!=(const VMLBooleanVariable &JudgetValue);
						operator bool();
};
} // namespace VML

VLIB_END_NAMESPACE