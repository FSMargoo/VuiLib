#pragma once

#include "../core/control/vfakecaption.h"
#include "../core/vcontrol.h"
#include "../core/visual/vvisual.h"
#include "../vss/vssbuilder.h"
#include "vml.h"
#include "vmlbuilder.h"
#include "vmlvariable.h"
#include <algorithm>

VLIB_BEGIN_NAMESPACE

namespace VML
{
class VMLMainWindow : public Core::VMainWindow
{
protected:
	std::vector<VMLObject *>		ObjectList;
	std::vector<VMLBasicVariable *> VariableList;
	std::map<VString, void *>		MetaFunctionList;
	std::vector<std::function<bool(const VString &, const VMLFinder &, Core::VUIObject **, Core::VUIObject *,
								   std::map<VString, VMLPropertyValue> &, VMLControlBuildStatus *)>>
		UserDefineList;

private:
	VMLFinder GetRootFinder();

private:
	void SortVMLAstNode(std::vector<VMLNode> &Nodes);

public:
	VMLMainWindow(const int &Width, const int &Height, Core::VApplication *Parent, const bool &Sizble = true);
	VMLMainWindow(Core::VApplication *Parent, const bool &Sizble = true);
	~VMLMainWindow();

public:
	VMLWidgetLoadResult LoadVML(const VString VML, VMLParserParseMode StringMode);
	VMLWidgetLoadResult LoadVML(VMLParserResult VMLAstTree, VMLObject *UIParent = nullptr);
	VMLWidgetLoadResult LoadVML(std::map<VString, VMLNode> VMLAstTree, VMLWidgetVMLObjectList *ObjectCacheList,
								VMLObject *UIParent = nullptr);

public:
	void SetStyleSheet(VSS::VSSParserResult VSSParserResult, std::vector<VMLObject *> List);
	void SetStyleSheet(VSS::VSSParserResult VSSParserResult);

	HWND GetLocalWinId() override;

public:
	void PushUserDefineFunction(
		std::function<bool(const VString &, const VMLFinder &, Core::VUIObject **, Core::VUIObject *,
						   std::map<VString, VMLPropertyValue> &, VMLControlBuildStatus *)>
			FunctionObject);

public:
	VMLFinder Get(const VString &ChildrenId);
	VMLFinder operator[](const VString &ChildrenId);

public:
	template <class... Parameters>
	inline void RegisterMetaFunction(const VString &FunctionName, void (*Functional)(Parameters...))
	{
		Core::VSignal<Parameters...> *Signal = new Core::VSignal<Parameters...>;
		Signal->Connect(Functional);

		MetaFunctionList.insert(std::pair<VString, void *>(FunctionName, (void *)Signal));
	}
	template <class ObjectType, class... Parameters>
	inline void RegisterMetaFunction(const VString &FunctionName, ObjectType *Object,
									 void (ObjectType::*Functional)(Parameters...))
	{
		Core::VSignal<Parameters...> *Signal = new Core::VSignal<Parameters...>;
		Signal->Connect(Object, Functional);

		MetaFunctionList.insert(std::pair<VString, void *>(FunctionName, (void *)Signal));
	}
};
} // namespace VML

VLIB_END_NAMESPACE
