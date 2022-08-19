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
    void VMLWidget::SortVMLAstNode(std::vector<VMLNode>& Nodes) {
        std::sort(Nodes.begin(), Nodes.end(), [](VMLNode Left, VMLNode Right) -> bool {
            return Left.ChildrenSequence < Right.ChildrenSequence;
        });
    }
    VMLWidget::VMLWidget(const int& Width, const int& Height, Core::VApplication* Parent, const bool& Sizble)
            : VMainWindow(Width, Height, Parent, Sizble) {    }
    VMLWidgetLoadResult VMLWidget::LoadVML(VMLParserResult VMLAstTree, VUIObject* UIParent) {
        if (VMLAstTree.ParserStatus == VMLParserStatus::Error) {
            std::wstring ASTError;

            for (auto& ErrorString : VMLAstTree.ErrorInfo) {
                ASTError.append(ErrorString.ErrorString + L"At Line (" + std::to_wstring(ErrorString.Line) + L")\n");
            }

            return { VMLWidgetVMLLoadStats::InvalidAstTree, L"Received an Invalid Ast Tree\nAST Error : \n" + ASTError };
        }
        if (VMLAstTree.ParserStatus == VMLParserStatus::Failed) {
            return { VMLWidgetVMLLoadStats::InvalidAstTree, L"Received an Invalid Ast Tree\nAST Error : " + VMLAstTree.ErrorInfo[0].ErrorString };
        }

        auto LoadResult = LoadVML(VMLAstTree.Nodes, nullptr, UIParent);

        Update({ 0, 0, GetWidth(), GetHeight() });

        return LoadResult;
    }
    VMLWidgetLoadResult VMLWidget::LoadVML(std::map<std::wstring, VMLNode> VMLAstTree, VMLWidgetVMLObjectList* ObjectCacheList, VUIObject* UIParent) {
        VMLWidgetLoadResult Result;
        Result.Status = VMLWidgetVMLLoadStats::Ok;

        if (UIParent == nullptr) {
            UIParent = this;
        }

        std::vector<VMLNode> VMLAstOrderlyNodes;
        for (auto& Element : VMLAstTree) {
            VMLAstOrderlyNodes.push_back(Element.second);
        }

        std::sort(VMLAstOrderlyNodes.begin(), VMLAstOrderlyNodes.end(), [](VMLNode Left, VMLNode Right) -> bool {
            return Left.ChildrenSequence < Right.ChildrenSequence;
        });

        for (auto& Element : VMLAstOrderlyNodes) {
            std::wstring StyleSheetString;
            VMLObject* VMLObject = new struct VMLObject;
            VMLObject->VMLID = Element.NodeTag;

            if (Element.PropertyExsit(L"type")) {
                VMLPropertyValue ElementProperty = Element.GetProperty(L"type");

                if (ElementProperty.PropertyType == VMLPropertyType::StringValue) {
                    if (ElementProperty.PropertyAsString == L"pushbutton") {
                        Core::VPushButton* PushButton = new Core::VPushButton(UIParent);
                        VMLObject->UIObject = PushButton;
                        VMLObject->VMLType = VMLObjectType::PushButton;

                        VMLControlBuildStatus BuildStatus;
                        VMLPushButtonBuilder  Builder(PushButton, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"imagelabel") {
                        Core::VImageLabel* ImageLabel = new Core::VImageLabel(0, 0, nullptr, UIParent);
                        VMLObject->UIObject = ImageLabel;
                        VMLObject->VMLType = VMLObjectType::ImageLabel;

                        VMLControlBuildStatus  BuildStatus;
                        VMLImageLabelBuilder   Builder(ImageLabel, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"textlabel") {
                        Core::VTextLabel* TextLabel = new Core::VTextLabel(UIParent);
                        VMLObject->UIObject = TextLabel;
                        VMLObject->VMLType = VMLObjectType::TextLabel;

                        VMLControlBuildStatus BuildStatus;
                        VMLTextLabelBuilder   Builder(TextLabel, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"layout") {
                        if (UIParent->GetParent()->IsApplication() == true) {
                            continue;
                        }

                        Core::VLayout* Layout = new Core::VLayout(UIParent, UIParent->GetParent());
                        VMLObject->UIObject = Layout;
                        VMLObject->VMLType = VMLObjectType::Layout;

                        VMLControlBuildStatus BuildStatus;
                        VMLLayoutBuilder      Builder(Layout, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"mainwindow") {
                        VMLObject->UIObject = this;
                        VMLObject->VMLType = VMLObjectType::MainWindow;

                        VMLControlBuildStatus BuildStatus;
                        VMLMainWindowBuilder  Builder(this, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else {
                        delete VMLObject;

                        return { VMLWidgetVMLLoadStats::Failed, L"Element \"" + Element.NodeTag + L"\" Don't Owns an Valid Type" };
                    }
                }
                else {
                    delete VMLObject;

                    return { VMLWidgetVMLLoadStats::Failed, L"Element Type Must Use String Value" };
                }

                VMLWidgetVMLObjectList* ChildList = new VMLWidgetVMLObjectList;
                LoadVML(Element.ChildrenNodes, ChildList, VMLObject->UIObject);

                for (auto& ChildObject : ChildList->Objects) {
                    VMLObject->ChildrenObjects.push_back(ChildObject);
                }
            }
            if (Element.PropertyExsit(L"id")) {
                VMLPropertyValue ElementId = Element.GetProperty(L"id");

                if (ElementId.PropertyType == VMLPropertyType::StringValue) {
                    VMLObject->VMLDOMID = ElementId.PropertyAsString;
                }
                else {
                    delete VMLObject;

                    return { VMLWidgetVMLLoadStats::Failed, L"Id Must Use String Value" };
                }
            }
            if (Element.PropertyExsit(L"class")) {
                VMLPropertyValue ElementId = Element.GetProperty(L"class");

                if (ElementId.PropertyType == VMLPropertyType::StringValue) {
                    VMLObject->VMLClass = ElementId.PropertyAsString;
                }
                else {
                    delete VMLObject;

                    return { VMLWidgetVMLLoadStats::Failed, L"Class Must Use String Value" };
                }
            }
            if (Element.PropertyExsit(L"style-sheet")) {
                VMLPropertyValue ElementStyleSheet = Element.GetProperty(L"style-sheet");

                if (ElementStyleSheet.PropertyType == VMLPropertyType::StringValue) {
                    StyleSheetString.append(ElementStyleSheet.PropertyAsString + L"\n");
                }
                else {
                    delete VMLObject;

                    return { VMLWidgetVMLLoadStats::Failed, L"Class Must Use String Value" };
                }
            }
            if (Element.PropertyExsit(L"style-sheet-src")) {
                VMLPropertyValue ElementStyleSheet = Element.GetProperty(L"style-sheet-src");

                if (ElementStyleSheet.PropertyType == VMLPropertyType::StringValue &&
                    VKits::VParserHelper::FileExist(ElementStyleSheet.PropertyAsString)) {
                    StyleSheetString.append(VKits::VParserHelper::ReadFromFile(ElementStyleSheet.PropertyAsString) + L"\n");
                }
                else {
                    delete VMLObject;

                    return { VMLWidgetVMLLoadStats::Failed, L"Class Must Use String Value" };
                }
            }

            if (ObjectCacheList != nullptr) {
                ObjectCacheList->Objects.push_back(VMLObject);

                if (!StyleSheetString.empty()) {
                    VSS::VSSParser Parser(StyleSheetString);
                    SetStyleSheet(Parser.ParseVSS(), ObjectCacheList->Objects);
                }
            }
            else {
                ObjectList.push_back(VMLObject);

                if (!StyleSheetString.empty()) {
                    VSS::VSSParser Parser(StyleSheetString);
                    SetStyleSheet(Parser.ParseVSS(), ObjectList);
                }
            }
        }

        return Result;
    }
    void VMLWidget::SetStyleSheet(VSS::VSSParserResult VSSParserResult, std::vector<VMLObject*> List) {
        for (auto& Selector : VSSParserResult.SelectorSet) {
            if (Selector->GetType() == VSS::VSSSelectorType::ElementSelector ||
                Selector->GetType() == VSS::VSSSelectorType::FakeClassSelector) {
                std::wstring ElementTag = static_cast<VSS::VSSElementSelector*>(Selector)->ElementTag;

                if (ElementTag == L"vpushbutton") {
                    Core::VPushButtonTheme* PushButtonTheme = new Core::VPushButtonTheme(*(
                            static_cast<Core::VPushButtonTheme*>(GetTargetTheme(Core::VUIThemeType::VPushButton)))
                    );

                    VSS::VSSVPushButtonBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector*>{Selector}, PushButtonTheme);

                    static_cast<Core::VApplication*>(GetParent())->SetTheme(PushButtonTheme);
                }
                if (ElementTag == L"vimagelabel") {
                    Core::VImageLabelTheme* ImageLabelTheme = new Core::VImageLabelTheme(*(
                            static_cast<Core::VImageLabelTheme*>(GetTargetTheme(Core::VUIThemeType::VImageLabel)))
                    );

                    VSS::VSSVImageLabelBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector*>{Selector}, ImageLabelTheme);

                    static_cast<Core::VApplication*>(GetParent())->SetTheme(ImageLabelTheme);
                }
                if (ElementTag == L"vtextlabel") {
                    Core::VTextLabelTheme* TextLabelTheme = new Core::VTextLabelTheme(*(static_cast<Core::VTextLabelTheme*>(
                            GetTargetTheme(Core::VUIThemeType::VTextLabel)))
                    );

                    VSS::VSSVTextLabelBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector*>{Selector}, TextLabelTheme);

                    static_cast<Core::VApplication*>(GetParent())->SetTheme(TextLabelTheme);
                }
            }

            for (auto& Object : List) {
                Object->SetStyleSheet(Selector);
            }
        }
    }
    void VMLWidget::SetStyleSheet(VSS::VSSParserResult VSSParserResult) {
        SetStyleSheet(VSSParserResult, ObjectList);

        for (int Count = 0; Count < VSSParserResult.SelectorSet.size(); ++Count) {
            delete VSSParserResult.SelectorSet.at(Count);
        }

        VSSParserResult.SelectorSet.clear();
    }
    VMLFinder VMLWidget::Get(const std::wstring& ChildrenId) {
        for (auto& Object : ObjectList) {
            if (Object->VMLID == ChildrenId) {
                return VMLFinder(Object, Object->ChildrenObjects);
            }
        }

        return VMLFinder(nullptr, std::vector<VMLObject*>());
    }
    VMLFinder VMLWidget::operator[](const std::wstring& ChildrenId) {
        for (auto& Object : ObjectList) {
            if (Object->VMLID == ChildrenId) {
                return VMLFinder(Object, Object->ChildrenObjects);
            }
        }

        return VMLFinder(nullptr, std::vector<VMLObject*>());
    }
}

VLIB_END_NAMESPACE