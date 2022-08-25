#pragma once

#include "../core/vcontrol.h"
#include "../vss/vssbuilder.h"

#include "vml.h"
#include "vmlbuilder.h"

#include <algorithm>

VLIB_BEGIN_NAMESPACE

namespace VML {
    class VMLWidget : public Core::VMainWindow {
    protected:
        std::vector<VMLObject*> ObjectList;

    private:
        VMLFinder GetRootFinder();

    private:
        void SortVMLAstNode(std::vector<VMLNode>& Nodes);

    public:
        VMLWidget(const int& Width, const int& Height, Core::VApplication* Parent, const bool& Sizble = true);
        VMLWidget(Core::VApplication* Parent, const bool& Sizble = true);

    public:
        VMLWidgetLoadResult LoadVML(const std::wstring VML, VMLParserParseMode StringMode);
        VMLWidgetLoadResult LoadVML(VMLParserResult VMLAstTree, VUIObject* UIParent = nullptr);
        VMLWidgetLoadResult LoadVML(std::map<std::wstring, VMLNode> VMLAstTree, VMLWidgetVMLObjectList* ObjectCacheList, VUIObject* UIParent = nullptr);

    public:
        void SetStyleSheet(VSS::VSSParserResult VSSParserResult, std::vector<VMLObject*> List);
        void SetStyleSheet(VSS::VSSParserResult VSSParserResult);

    public:
        VMLFinder Get(const std::wstring& ChildrenId);
        VMLFinder operator[](const std::wstring& ChildrenId);
    };
}

VLIB_END_NAMESPACE

