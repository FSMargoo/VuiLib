#include "../../include/vml/vmlwidget.h"

#include <direct.h>

VLIB_BEGIN_NAMESPACE

namespace VML {
    void VMLMainWindow::SortVMLAstNode(std::vector<VMLNode>& Nodes) {
        std::sort(Nodes.begin(), Nodes.end(), [](VMLNode Left, VMLNode Right) -> bool {
            return Left.ChildrenSequence < Right.ChildrenSequence;
        });
    }
    VMLMainWindow::VMLMainWindow(Core::VApplication* Parent, const bool& Sizble)
            : VMainWindow(0, 0, Parent, Sizble) {

    }
    VMLMainWindow::VMLMainWindow(const int& Width, const int& Height, Core::VApplication* Parent, const bool& Sizble)
            : VMainWindow(Width, Height, Parent, Sizble) {    }
    VMLMainWindow::~VMLMainWindow() {
        for (auto& MetaFunction : MetaFunctionList) {
            delete MetaFunction.second;
        }

        MetaFunctionList.clear();

        VUIObject::~VUIObject();
    }
    VMLWidgetLoadResult VMLMainWindow::LoadVML(const std::wstring VML, VMLParserParseMode StringMode) {
        VMLParser Parser(VML, StringMode);

        return LoadVML(Parser.ParseVML());
    }

    VMLFinder VMLMainWindow::GetRootFinder() {
        return VMLFinder(nullptr, ObjectList);
    }

    VMLWidgetLoadResult VMLMainWindow::LoadVML(VMLParserResult VMLAstTree, VMLObject* UIParent) {
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
    VMLWidgetLoadResult VMLMainWindow::LoadVML(std::map<std::wstring, VMLNode> VMLAstTree, VMLWidgetVMLObjectList* ObjectCacheList, VMLObject* UIParent) {
        VMLWidgetLoadResult Result;
        Result.Status = VMLWidgetVMLLoadStats::Ok;
        bool                AlreadyCreateMainWindow = false;

        if (UIParent == nullptr) {
            UIParent = new VMLObject;

            UIParent->UIObject = this;
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
                        Core::VPushButton* PushButton = new Core::VPushButton(UIParent->UIObject);
                        VMLObject->UIObject = PushButton;
                        VMLObject->VMLType = VMLObjectType::PushButton;

                        VMLControlBuildStatus BuildStatus;
                        VMLPushButtonBuilder  Builder(GetRootFinder(), PushButton, Element.NodeValue, &BuildStatus);

                        if (Element.PropertyExsit(L"on-clicked")) {
                            if (Element.NodeValue[L"on-clicked"].PropertyType == VMLPropertyType::NativeCall) {
                                auto NativeCallName = Element.NodeValue[L"on-clicked"].NativeCallMethodName;

                                if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end()) {
                                    PushButton->ButtonPushed.Connect((Core::VSignal<>*)MetaFunctionList[NativeCallName], &Core::VSignal<>::Emit);
                                }
                            }
                        }

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"imagelabel") {
                        Core::VImageLabel* ImageLabel = new Core::VImageLabel(0, 0, nullptr, UIParent->UIObject);
                        VMLObject->UIObject = ImageLabel;
                        VMLObject->VMLType = VMLObjectType::ImageLabel;

                        VMLControlBuildStatus  BuildStatus;
                        VMLImageLabelBuilder   Builder(GetRootFinder(), ImageLabel, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"textlabel") {
                        Core::VTextLabel* TextLabel = new Core::VTextLabel(UIParent->UIObject);
                        VMLObject->UIObject = TextLabel;
                        VMLObject->VMLType = VMLObjectType::TextLabel;

                        VMLControlBuildStatus BuildStatus;
                        VMLTextLabelBuilder   Builder(GetRootFinder(), TextLabel, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"fake-caption") {
                        Core::VFakeCaption* FakeCaption = new Core::VFakeCaption(UIParent->UIObject);
                        VMLObject->UIObject = FakeCaption;
                        VMLObject->VMLType = VMLObjectType::FakeCaption;

                        VMLControlBuildStatus BuildStatus;
                        VMLCommonBuilder      Builder(GetRootFinder(), FakeCaption, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"layout") {
                        if (UIParent->UIObject->GetParent()->IsApplication() == true) {
                            continue;
                        }

                        Core::VLayout* Layout = new Core::VLayout(UIParent->UIObject, UIParent->UIObject->GetParent());
                        VMLObject->UIObject = Layout;
                        VMLObject->VMLType = VMLObjectType::Layout;

                        VMLControlBuildStatus BuildStatus;
                        VMLLayoutBuilder      Builder(GetRootFinder(), Layout, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"scalelayout") {
                        if (UIParent->UIObject->GetParent()->IsApplication() == true) {
                            continue;
                        }

                        Core::VScaleLayout* Layout = new Core::VScaleLayout(UIParent->UIObject, UIParent->UIObject->GetParent());
                        VMLObject->UIObject = Layout;
                        VMLObject->VMLType = VMLObjectType::Layout;

                        VMLControlBuildStatus BuildStatus;
                        VMLScaleLayoutBuilder Builder(GetRootFinder(), Layout, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"text-size-layout") {
                        if (UIParent->UIObject->GetParent()->IsApplication() == true) {
                            continue;
                        }

                        if (UIParent->VMLType == VMLObjectType::TextLabel) {
                            Core::VTextSizeLayout *Layout = new Core::VTextSizeLayout(static_cast<Core::VTextLabel*>(UIParent->UIObject),
                                                                                   UIParent->UIObject->GetParent());
                            VMLObject->UIObject = Layout;
                            VMLObject->VMLType = VMLObjectType::Layout;

                            VMLControlBuildStatus BuildStatus;
                            VMLTextSizeLayoutBuilder Builder(GetRootFinder(), Layout, Element.NodeValue, &BuildStatus);

                            if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                                Result.Status = VMLWidgetVMLLoadStats::Failed;
                                Result.FailedMessage =
                                        L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" +
                                        BuildStatus.FailedReason + L"\"";

                                return Result;
                            }
                        }
                        else {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage =
                                    L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"\'text-size-layout\' should match parent \'text-label\'\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"horizontal-slider") {
                        Core::VSliderHorizontal* SliderHorizontal = new Core::VSliderHorizontal(UIParent->UIObject);
                        VMLObject->UIObject = SliderHorizontal;
                        VMLObject->VMLType = VMLObjectType::HorizontalSlider;

                        VMLControlBuildStatus BuildStatus;
                        VMLSliderHorizontalBuilder Builder(GetRootFinder(), SliderHorizontal, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"vertical-slider") {
                        Core::VSliderVertical* SliderHorizontal = new Core::VSliderVertical(UIParent->UIObject);
                        VMLObject->UIObject = SliderHorizontal;
                        VMLObject->VMLType = VMLObjectType::VerticalSlider;

                        VMLControlBuildStatus BuildStatus;
                        VMLSliderVerticalBuilder Builder(GetRootFinder(), SliderHorizontal, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"mainwindow") {
                        VMLObject->UIObject = this;
                        VMLObject->VMLType = VMLObjectType::MainWindow;

                        AlreadyCreateMainWindow = true;

                        VMLControlBuildStatus BuildStatus;
                        VMLMainWindowBuilder  Builder(this, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"radiobutton") {
                        if (UIParent->VMLType != VMLObjectType::SwitchGroup) {
                            Core::VRadioButton *RadioButton = new Core::VRadioButton(UIParent->UIObject);
                            VMLObject->UIObject = RadioButton;
                            VMLObject->VMLType = VMLObjectType::RadioButton;

                            VMLControlBuildStatus BuildStatus;
                            VMLRadioButtonBuilder Builder(GetRootFinder(), RadioButton, Element.NodeValue,
                                                          &BuildStatus);

                            if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                                Result.Status = VMLWidgetVMLLoadStats::Failed;
                                Result.FailedMessage =
                                        L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" +
                                        BuildStatus.FailedReason + L"\"";

                                return Result;
                            }
                        }
                        else {
                            Core::VRadioButton *RadioButton = new Core::VRadioButton(UIParent->UIObject->GetParent());
                            VMLObject->UIObject = RadioButton;
                            VMLObject->VMLType = VMLObjectType::RadioButton;

                            ((Core::VSwitchGroup*)UIParent->UIObject)->AddObject(RadioButton);

                            VMLControlBuildStatus BuildStatus;
                            VMLRadioButtonBuilder Builder(GetRootFinder(), RadioButton, Element.NodeValue,
                                                          &BuildStatus);

                            if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                                Result.Status = VMLWidgetVMLLoadStats::Failed;
                                Result.FailedMessage =
                                        L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" +
                                        BuildStatus.FailedReason + L"\"";

                                return Result;
                            }
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"blurlabel") {
                        Core::VBlurLabel* BlurLabel = new Core::VBlurLabel(UIParent->UIObject);

                        VMLObject->UIObject = BlurLabel;
                        VMLObject->VMLType = VMLObjectType::BlurLabel;

                        VMLControlBuildStatus BuildStatus;
                        VMLCommonBuilder      Builder(GetRootFinder(), BlurLabel, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"iconbutton") {
                        Core::VIconButton* BlurLabel = new Core::VIconButton(UIParent->UIObject);

                        VMLObject->UIObject = BlurLabel;
                        VMLObject->VMLType = VMLObjectType::IconButton;

                        VMLControlBuildStatus BuildStatus;
                        VMLIconButtonBuilder  Builder(GetRootFinder(), BlurLabel, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"lineeditor") {
                        Core::VLineEditor* LineEditor = new Core::VLineEditor(UIParent->UIObject);

                        VMLObject->UIObject = LineEditor;
                        VMLObject->VMLType = VMLObjectType::LineEditor;

                        VMLControlBuildStatus BuildStatus;
                        VMLLineEditorBuilder  Builder(GetRootFinder(), LineEditor, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"switchgroup") {
                        Core::VSwitchGroup* SwitchGroup = new Core::VSwitchGroup(UIParent->UIObject);

                        VMLObject->UIObject = SwitchGroup;
                        VMLObject->VMLType = VMLObjectType::SwitchGroup;
                    }
                    else if (ElementProperty.PropertyAsString == L"vertical-scroller") {
                        Core::VScrollerVertical* VerticalScroller = new Core::VScrollerVertical(UIParent->UIObject);

                        VMLObject->UIObject = VerticalScroller;
                        VMLObject->VMLType = VMLObjectType::VerticalScroller;

                        VMLControlBuildStatus BuildStatus;
                        VMLVerticalScrollerBuilder  Builder(GetRootFinder(), VerticalScroller, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"horizontal-scroller") {
                        Core::VScrollerHorizontal* VerticalScroller = new Core::VScrollerHorizontal(UIParent->UIObject);

                        VMLObject->UIObject = VerticalScroller;
                        VMLObject->VMLType = VMLObjectType::VerticalScroller;

                        VMLControlBuildStatus BuildStatus;
                        VMLHorizontalScrollerBuilder Builder(GetRootFinder(), VerticalScroller, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"viewlabel") {
                        Core::VViewLabel* ViewLabel = new Core::VViewLabel(UIParent->UIObject);

                        VMLObject->UIObject = ViewLabel;
                        VMLObject->VMLType = VMLObjectType::ViewLabel;

                        VMLControlBuildStatus BuildStatus;
                        VMLViewLabelBuilder Builder(GetRootFinder(), ViewLabel, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"geometry-animation") {
                        Core::VGeometryAnimation* Animation = new Core::VGeometryAnimation(UIParent->UIObject, nullptr, 0.f);

                        VMLObject->UIObject = Animation;
                        VMLObject->VMLType = VMLObjectType::GeometryAnimation;

                        VMLControlBuildStatus BuildStatus;
                        VMLGeometryAnimationBuilder Builder(GetRootFinder(), Animation, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"position-animation") {
                        Core::VPositionAnimation* Animation = new Core::VPositionAnimation(UIParent->UIObject, nullptr, 0.f);

                        VMLObject->UIObject = Animation;
                        VMLObject->VMLType = VMLObjectType::PositionAnimation;

                        VMLControlBuildStatus BuildStatus;
                        VMLPositionAnimationBuilder Builder(GetRootFinder(), Animation, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"opacity-animation") {
                        Core::VOpacityAnimation* Animation = new Core::VOpacityAnimation(UIParent->UIObject, nullptr, 0.f);

                        VMLObject->UIObject = Animation;
                        VMLObject->VMLType = VMLObjectType::OpacityAnimation;

                        VMLControlBuildStatus BuildStatus;
                        VMLOpacityAnimationBuilder Builder(GetRootFinder(), Animation, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"circle-view") {
                        Core::VCircleView* CircleView = new Core::VCircleView(UIParent->UIObject);

                        VMLObject->UIObject = CircleView;
                        VMLObject->VMLType = VMLObjectType::CircleView;

                        VMLControlBuildStatus BuildStatus;
                        VMLCommonBuilder Builder(GetRootFinder(), CircleView, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"polygon-view") {
                        Core::VPolygonView* PolygonView = new Core::VPolygonView(UIParent->UIObject);

                        VMLObject->UIObject = PolygonView;
                        VMLObject->VMLType = VMLObjectType::PolygonView;

                        VMLControlBuildStatus BuildStatus;
                        VMLPolygonViewBuilder Builder(GetRootFinder(), PolygonView, Element.NodeValue, &BuildStatus);

                        if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                            Result.Status = VMLWidgetVMLLoadStats::Failed;
                            Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                            return Result;
                        }
                    }
                    else if (ElementProperty.PropertyAsString == L"widget") {
                        if (AlreadyCreateMainWindow) {
                            Core::VWidget *Widget = new Core::VWidget(0, 0, static_cast<Core::VApplication*>(GetParent()), L"WidgetFromVML");

                            VMLObject->UIObject = Widget;
                            VMLObject->VMLType = VMLObjectType::Widget;

                            VMLControlBuildStatus BuildStatus;
                            VMLWidgetBuilder Builder(Widget, Element.NodeValue, &BuildStatus);

                            if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                                Result.Status = VMLWidgetVMLLoadStats::Failed;
                                Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                                return Result;
                            }
                        }
                        else if (UIParent->VMLType == VMLObjectType::MainWindow) {
                            Core::VWidget *Widget = new Core::VWidget(0, 0, static_cast<Core::VMainWindow*>(UIParent->UIObject), L"WidgetFromVML");

                            VMLObject->UIObject = Widget;
                            VMLObject->VMLType = VMLObjectType::Widget;

                            VMLControlBuildStatus BuildStatus;
                            VMLWidgetBuilder Builder(Widget, Element.NodeValue, &BuildStatus);

                            if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                                Result.Status = VMLWidgetVMLLoadStats::Failed;
                                Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                                return Result;
                            }
                        }
                        else if (UIParent->VMLType == VMLObjectType::Widget) {
                            Core::VWidget *Widget = new Core::VWidget(0, 0, static_cast<Core::VWidget*>(GetParent()), L"WidgetFromVML");

                            VMLObject->UIObject = Widget;
                            VMLObject->VMLType = VMLObjectType::Widget;

                            VMLControlBuildStatus BuildStatus;
                            VMLWidgetBuilder Builder(Widget, Element.NodeValue, &BuildStatus);

                            if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok) {
                                Result.Status = VMLWidgetVMLLoadStats::Failed;
                                Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

                                return Result;
                            }
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
                LoadVML(Element.ChildrenNodes, ChildList, VMLObject);

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

                    return { VMLWidgetVMLLoadStats::Failed, L"id must use string value" };
                }
            }
            if (Element.PropertyExsit(L"class")) {
                VMLPropertyValue ElementId = Element.GetProperty(L"class");

                if (ElementId.PropertyType == VMLPropertyType::StringValue) {
                    VMLObject->VMLClass = ElementId.PropertyAsString;
                }
                else {
                    delete VMLObject;

                    return { VMLWidgetVMLLoadStats::Failed, L"class must use string value" };
                }
            }
            if (Element.PropertyExsit(L"style-sheet")) {
                VMLPropertyValue ElementStyleSheet = Element.GetProperty(L"style-sheet");

                if (ElementStyleSheet.PropertyType == VMLPropertyType::StringValue) {
                    StyleSheetString.append(ElementStyleSheet.PropertyAsString + L"\n");
                }
                else {
                    delete VMLObject;

                    return { VMLWidgetVMLLoadStats::Failed, L"style-sheet must use string value" };
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

                    return { VMLWidgetVMLLoadStats::Failed, L"style-sheet-src must use string value (or value dosen't exsits)" };
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
    void VMLMainWindow::SetStyleSheet(VSS::VSSParserResult VSSParserResult, std::vector<VMLObject*> List) {
        for (auto& Selector : VSSParserResult.SelectorSet) {
            if (Selector->GetType() == VSS::VSSSelectorType::ElementSelector ||
                Selector->GetType() == VSS::VSSSelectorType::FakeClassSelector) {
                std::wstring ElementTag = static_cast<VSS::VSSElementSelector*>(Selector)->ElementTag;

                if (ElementTag == L"pushbutton") {
                    Core::VPushButtonTheme* PushButtonTheme = new Core::VPushButtonTheme(*(
                            static_cast<Core::VPushButtonTheme*>(GetTargetTheme(Core::VUIThemeType::VPushButton)))
                    );

                    VSS::VSSVPushButtonBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector*>{Selector}, PushButtonTheme);

                    static_cast<Core::VApplication*>(GetParent())->SetTheme(PushButtonTheme);
                }
                if (ElementTag == L"imagelabel") {
                    Core::VImageLabelTheme* ImageLabelTheme = new Core::VImageLabelTheme(*(
                            static_cast<Core::VImageLabelTheme*>(GetTargetTheme(Core::VUIThemeType::VImageLabel)))
                    );

                    VSS::VSSVImageLabelBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector*>{Selector}, ImageLabelTheme);

                    static_cast<Core::VApplication*>(GetParent())->SetTheme(ImageLabelTheme);
                }
                if (ElementTag == L"textlabel") {
                    Core::VTextLabelTheme* TextLabelTheme = new Core::VTextLabelTheme(*(static_cast<Core::VTextLabelTheme*>(
                            GetTargetTheme(Core::VUIThemeType::VTextLabel)))
                    );

                    VSS::VSSVTextLabelBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector*>{Selector}, TextLabelTheme);

                    static_cast<Core::VApplication*>(GetParent())->SetTheme(TextLabelTheme);
                }
                if (ElementTag == L"radiobutton") {
                    Core::VRadioButtonTheme* TextLabelTheme = new Core::VRadioButtonTheme(*(static_cast<Core::VRadioButtonTheme*>(
                            GetTargetTheme(Core::VUIThemeType::VRadioButton)))
                    );

                    VSS::VSSVRadioButtonBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector*>{Selector}, TextLabelTheme);

                    static_cast<Core::VApplication*>(GetParent())->SetTheme(TextLabelTheme);
                }
                if (ElementTag == L"horizontal-slider") {
                    Core::VSliderTheme* SliderTheme = new Core::VSliderTheme(*(static_cast<Core::VSliderTheme*>(
                            GetTargetTheme(Core::VUIThemeType::VSlider)))
                    );

                    VSS::VSSVSliderHorizontalBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector*>{Selector}, SliderTheme);

                    static_cast<Core::VApplication*>(GetParent())->SetTheme(SliderTheme);
                }
                if (ElementTag == L"vertical-slider") {
                    Core::VSliderTheme* SliderTheme = new Core::VSliderTheme(*(static_cast<Core::VSliderTheme*>(
                            GetTargetTheme(Core::VUIThemeType::VSlider)))
                    );

                    VSS::VSSVSliderHorizontalBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector*>{Selector}, SliderTheme);

                    static_cast<Core::VApplication*>(GetParent())->SetTheme(SliderTheme);
                }
                if (ElementTag == L"horizontal-scroller") {
                    Core::VViewScrollerTheme* SliderTheme = new Core::VViewScrollerTheme(*(static_cast<Core::VViewScrollerTheme*>(
                            GetTargetTheme(Core::VUIThemeType::VViewLabel)))
                    );

                    VSS::VSSHorizontalScrollerBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector*>{Selector}, SliderTheme);

                    static_cast<Core::VApplication*>(GetParent())->SetTheme(SliderTheme);
                }
                if (ElementTag == L"vertical-scroller") {
                    Core::VViewScrollerTheme* SliderTheme = new Core::VViewScrollerTheme(*(static_cast<Core::VViewScrollerTheme*>(
                            GetTargetTheme(Core::VUIThemeType::VViewLabel)))
                    );

                    VSS::VSSHorizontalScrollerBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector*>{Selector}, SliderTheme);

                    static_cast<Core::VApplication*>(GetParent())->SetTheme(SliderTheme);
                }
                if (ElementTag == L"view-label") {
                    Core::VViewLabelTheme* SliderTheme = new Core::VViewLabelTheme(*(static_cast<Core::VViewLabelTheme*>(
                            GetTargetTheme(Core::VUIThemeType::VViewLabel)))
                    );

                    VSS::VSSViewLabelBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector*>{Selector}, SliderTheme);

                    static_cast<Core::VApplication*>(GetParent())->SetTheme(SliderTheme);
                }
            }

            for (auto& Object : List) {
                Object->SetStyleSheet(Selector);
            }
        }
    }
    void VMLMainWindow::SetStyleSheet(VSS::VSSParserResult VSSParserResult) {
        SetStyleSheet(VSSParserResult, ObjectList);

        for (int Count = 0; Count < VSSParserResult.SelectorSet.size(); ++Count) {
            delete VSSParserResult.SelectorSet.at(Count);
        }

        VSSParserResult.SelectorSet.clear();
    }
    VMLFinder VMLMainWindow::Get(const std::wstring& ChildrenId) {
        for (auto& Object : ObjectList) {
            if (Object->VMLID == ChildrenId) {
                return VMLFinder(Object, Object->ChildrenObjects);
            }
        }

        return VMLFinder(nullptr, std::vector<VMLObject*>());
    }
    VMLFinder VMLMainWindow::operator[](const std::wstring& ChildrenId) {
        for (auto& Object : ObjectList) {
            if (Object->VMLID == ChildrenId) {
                return VMLFinder(Object, Object->ChildrenObjects);
            }
        }

        return VMLFinder(nullptr, std::vector<VMLObject*>());
    }

    HWND VMLMainWindow::GetLocalWinId() {
        return Core::VMainWindow::CallWidgetGetHWND();
    }
}

VLIB_END_NAMESPACE