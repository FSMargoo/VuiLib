#include "../../include/vml/vml.h"

VLIB_BEGIN_NAMESPACE

namespace VML {
    VMLObjectType VMLObject::StringToObjectType(const std::wstring &String) {
        VMLObjectType Type = VMLObjectType::PushButton;

        if (String == L"pushbutton") {
            return VMLObjectType::PushButton;
        }
        if (String == L"textlabel") {
            return VMLObjectType::TextLabel;
        }
        if (String == L"imagelabel") {
            return VMLObjectType::ImageLabel;
        }
        if (String == L"mainwindow") {
            return VMLObjectType::MainWindow;
        }
        if (String == L"layout") {
            return VMLObjectType::Layout;
        }
        if (String == L"scalelayout") {
            return VMLObjectType::ScaleLayout;
        }
        if (String == L"radiobutton") {
            return VMLObjectType::RadioButton;
        }

        return Type;
    }
    const std::wstring VMLObject::ObjectTypeToString(VMLObjectType ObjectType) {
        switch (ObjectType) {
            case VMLObjectType::PushButton: {
                return L"pushbutton";
            }
            case VMLObjectType::TextLabel: {
                return L"textlabel";
            }
            case VMLObjectType::ImageLabel: {
                return L"imagelabel";
            }
            case VMLObjectType::MainWindow: {
                return L"mainwindow";
            }
            case VMLObjectType::RadioButton: {
                return L"radiobutton";
            }
            case VMLObjectType::ScaleLayout: {
                return L"scalelayout";
            }
        }

        return L"";
    }
    void VMLObject::SetNativeStyleSheet(VSS::VSSBasicSelector* Selector) {
        switch (VMLType) {
            case VMLObjectType::PushButton: {
                VSS::VSSVPushButtonBuilder Builder(static_cast<Core::VPushButton*>(UIObject), std::vector<VSS::VSSBasicSelector*>{Selector}, nullptr);

                break;
            }
            case VMLObjectType::TextLabel: {
                VSS::VSSVTextLabelBuilder Builder(static_cast<Core::VTextLabel*>(UIObject), std::vector<VSS::VSSBasicSelector*>{Selector}, nullptr);

                break;
            }
            case VMLObjectType::ImageLabel: {
                VSS::VSSVImageLabelBuilder Builder(static_cast<Core::VImageLabel*>(UIObject), std::vector<VSS::VSSBasicSelector*>{Selector}, nullptr);

                break;
            }
            case VMLObjectType::RadioButton: {
                VSS::VSSVRadioButtonBuilder Builder(static_cast<Core::VRadioButton*>(UIObject), std::vector<VSS::VSSBasicSelector*>{Selector}, nullptr);

                break;
            }
            case VMLObjectType::MainWindow: {
                VSS::VSSVMainWindowBuilder Builder(static_cast<Core::VMainWindow*>(UIObject), std::vector<VSS::VSSBasicSelector*>{Selector});

                break;
            }
        }
    }
    void VMLObject::SetStyleSheet(VSS::VSSBasicSelector* Selector) {
        switch (Selector->GetType()) {
            case VSS::VSSSelectorType::ClassSelector: {
                if (static_cast<VSS::VSSClassSelector*>(Selector)->ClassTag == VMLClass) {
                    VSS::VSSElementSelector* ElementSelector = new VSS::VSSElementSelector;

                    ElementSelector->ElementTag = ObjectTypeToString(VMLType);
                    ElementSelector->SelectorProperty = Selector->SelectorProperty;

                    SetNativeStyleSheet(ElementSelector);

                    delete ElementSelector;
                }

                break;
            }
            case VSS::VSSSelectorType::ElementSelector: {
                if (StringToObjectType(static_cast<VSS::VSSElementSelector*>(Selector)->ElementTag) == VMLType &&
                    VMLClass.empty()) {
                    SetNativeStyleSheet(Selector);
                }

                break;
            }
            case VSS::VSSSelectorType::ClassWithFakeClassSelector: {
                if (static_cast<VSS::VSSClassWithFakeClassSelector*>(Selector)->ClassTag == VMLClass) {
                    VSS::VSSFakeClassSelector* FakeClassSelector = new VSS::VSSFakeClassSelector;

                    FakeClassSelector->ElementTag = ObjectTypeToString(VMLType);
                    FakeClassSelector->SelectorProperty = Selector->SelectorProperty;
                    FakeClassSelector->ClassTag = static_cast<VSS::VSSClassWithFakeClassSelector*>(Selector)->FakeClassTag;

                    SetNativeStyleSheet(FakeClassSelector);

                    delete FakeClassSelector;
                }

                break;
            }
            case VSS::VSSSelectorType::GenericSelector: {
                SetNativeStyleSheet(Selector);

                break;
            }
            case VSS::VSSSelectorType::IDSelector: {
                if (static_cast<VSS::VSSIDSelector*>(Selector)->IDTag == VMLDOMID) {
                    VSS::VSSElementSelector* ElementSelector = new VSS::VSSElementSelector;

                    ElementSelector->ElementTag = ObjectTypeToString(VMLType);
                    ElementSelector->SelectorProperty = Selector->SelectorProperty;

                    SetNativeStyleSheet(ElementSelector);

                    delete ElementSelector;
                }

                break;
            }
            case VSS::VSSSelectorType::FakeClassSelector: {
                if (StringToObjectType(static_cast<VSS::VSSFakeClassSelector*>(Selector)->ElementTag) == VMLType &&
                    VMLClass.empty()) {
                    SetNativeStyleSheet(Selector);
                }

                break;
            }
            default:
                break;
        }

        for (auto& Objects : ChildrenObjects) {
            Objects->SetStyleSheet(Selector);
        }
    }
    VMLFinder::VMLFinder(VML::VMLObject *Object, std::vector<VMLObject *> List) {
        ObjectList = List;
        OriginObject = Object;
    }

    VMLFinder VMLFinder::operator[](const std::wstring& ChildrenId) {
        for (auto& Object : ObjectList) {
            if (Object->VMLID == ChildrenId) {
                return VMLFinder(Object, Object->ChildrenObjects);
            }
        }

        return VMLFinder(nullptr, std::vector<VMLObject*>());
    }
    VMLFinder VMLFinder::Get(const std::wstring& ChildrenId) {
        for (auto& Object : ObjectList) {
            if (Object->VMLID == ChildrenId) {
                return VMLFinder(Object, Object->ChildrenObjects);
            }
        }

        return VMLFinder(nullptr, std::vector<VMLObject*>());
    }
    bool      VMLFinder::IsValid() {
        return OriginObject != nullptr;
    }
    VMLFinder::operator Core::VUIObject* () {
        return OriginObject->UIObject;
    }
}

VLIB_END_NAMESPACE