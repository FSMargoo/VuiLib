#pragma once

#include "../core/vcontrol.h"
#include "../vss/vssbuilder.h"

#include "../vss/vssparser.h"

#include "vmlvariable.h"

#include <algorithm>

#define VML_META_FUNCTION(FunctionName) L#FunctionName, FunctionName
#define VML_CLASS_META_FUNCTION(ThisPtr, FunctionInstance, FunctionName) L#FunctionName, ThisPtr, FunctionInstance

VLIB_BEGIN_NAMESPACE

namespace VML {
    enum class VMLObjectType {
        PushButton, ImageLabel, TextLabel, MainWindow, Layout, ScaleLayout, RadioButton,
        HorizontalSlider, VerticalSlider, FakeCaption, BlurLabel, IconButton, LineEditor,
        SwitchGroup, VerticalScroller, HorizontalScroller, ViewLabel,
        PositionAnimation, GeometryAnimation, OpacityAnimation, CircleView, PolygonView,
        Widget, Canvas, Editor, UserDefine
    };

    struct VMLObject {
        Core::VUIObject* UIObject;

        std::wstring     VMLID;
        std::wstring     VMLClass;
        std::wstring     VMLDOMID;

        Core::VLayout*       VMLNativeLaytout;
        Core::VScaleLayout*  VMLNativeScaleLaytout;

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
        std::vector<VMLObject*>          ObjectList;
        VMLObject* OriginObject;

    public:
        std::vector<VMLBasicVariable* >* VariableList;

    public:
        VMLFinder(VMLObject* Object, std::vector<VMLObject*> List, std::vector<VMLBasicVariable* >* VariableList);

        VMLFinder operator[](const std::wstring& ChildrenId);
        Core::VUIObject* operator->();
        VMLFinder Get(const std::wstring& ChildrenId);
        bool      IsValid();

        template<class Type = Core::VUIObject>
        Type* Get() {
            return static_cast<Type*>(OriginObject->UIObject);
        }

        Core::VLayout*      GetVMLLaytout();
        Core::VScaleLayout* GetVMLScaleLaytout();

        operator Core::VUIObject* ();
    };
}

VLIB_END_NAMESPACE

