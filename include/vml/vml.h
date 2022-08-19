#include "../core/vcontrol.h"
#include "../vss/vssbuilder.h"

#include "vmlbuilder.h"
#include "../vss/vssparser.h"

#include <algorithm>

VLIB_BEGIN_NAMESPACE

namespace VML {
    enum class VMLObjectType {
        PushButton, ImageLabel, TextLabel, MainWindow, Layout
    };

    struct VMLObject {
        Core::VUIObject* UIObject;

        std::wstring     VMLID;
        std::wstring     VMLClass;
        std::wstring     VMLDOMID;

        VMLObjectType    VMLType = VMLObjectType::PushButton;

        std::vector<VMLObject*> ChildrenObjects;

        static VMLObjectType      StringToObjectType(const std::wstring& String);
        static const std::wstring ObjectTypeToString(VMLObjectType ObjectType);
        void SetNativeStyleSheet(VSS::VSSBasicSelector* Selector);
        void SetStyleSheet(VSS::VSSBasicSelector* Selector);
    };

    enum class VMLWidgetVMLLoadStats {
        Ok, Failed, InvalidAstTree
    };

    struct VMLWidgetLoadResult {
        VMLWidgetVMLLoadStats Status = VMLWidgetVMLLoadStats::Ok;

        std::wstring           FailedMessage;
    };

    struct VMLWidgetVMLObjectList {
        std::vector<VMLObject*> Objects;
    };

    class VMLFinder {
    private:
        std::vector<VMLObject*> ObjectList;
        VMLObject* OriginObject;

    public:
        VMLFinder(VMLObject* Object, std::vector<VMLObject*> List);

        VMLFinder operator[](const std::wstring& ChildrenId);
        VMLFinder Get(const std::wstring& ChildrenId);
        bool      IsValid();

        template<class Type = VUIObject>
        Type* Get() {
            return static_cast<Type*>(OriginObject->UIObject);
        }

        operator Core::VUIObject* ();
    };

    class VMLWidget : public Core::VMainWindow {
    protected:
        std::vector<VMLObject*> ObjectList;

    private:
        void SortVMLAstNode(std::vector<VMLNode>& Nodes);

    public:
        VMLWidget(const int& Width, const int& Height, Core::VApplication* Parent, const bool& Sizble = true);

    public:
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

