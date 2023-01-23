#pragma once

#include "../core/vcontrol.h"
#include "../vss/vssbuilder.h"

#include "../core/visual/vvisual.h"

#include "../core/control/vfakecaption.h"

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
	std::map<std::wstring, void *>	MetaFunctionList;
	std::vector<std::function<bool(const std::wstring &, const VMLFinder &, Core::VUIObject **, Core::VUIObject *,
								   std::map<std::wstring, VMLPropertyValue> &, VMLControlBuildStatus *)>>
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
	VMLWidgetLoadResult LoadVML(const std::wstring VML, VMLParserParseMode StringMode);
	VMLWidgetLoadResult LoadVML(VMLParserResult VMLAstTree, VMLObject *UIParent = nullptr);
	VMLWidgetLoadResult LoadVML(std::map<std::wstring, VMLNode> VMLAstTree, VMLWidgetVMLObjectList *ObjectCacheList,
								VMLObject *UIParent = nullptr);

public:
	void SetStyleSheet(VSS::VSSParserResult VSSParserResult, std::vector<VMLObject *> List);
	void SetStyleSheet(VSS::VSSParserResult VSSParserResult);

	HWND GetLocalWinId() override;

public:
	void PushUserDefineFunction(
		std::function<bool(const std::wstring &, const VMLFinder &, Core::VUIObject **, Core::VUIObject *,
						   std::map<std::wstring, VMLPropertyValue> &, VMLControlBuildStatus *)>
			FunctionObject);

public:
	VMLFinder Get(const std::wstring &ChildrenId);
	VMLFinder operator[](const std::wstring &ChildrenId);

public:
	template <class... Parameters>
	inline void RegisterMetaFunction(const std::wstring &FunctionName, void (*Functional)(Parameters...))
	{
		Core::VSignal<Parameters...> *Signal = new Core::VSignal<Parameters...>;
		Signal->Connect(Functional);

		MetaFunctionList.insert(std::pair<std::wstring, void *>(FunctionName, (void *)Signal));
	}
	template <class ObjectType, class... Parameters>
	inline void RegisterMetaFunction(const std::wstring &FunctionName, ObjectType *Object,
									 void (ObjectType::*Functional)(Parameters...))
	{
		Core::VSignal<Parameters...> *Signal = new Core::VSignal<Parameters...>;
		Signal->Connect(Object, Functional);

		MetaFunctionList.insert(std::pair<std::wstring, void *>(FunctionName, (void *)Signal));
	}
};
} // namespace VML

VLIB_END_NAMESPACE
