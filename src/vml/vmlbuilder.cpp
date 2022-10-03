#include "../../include/vml/vmlbuilder.h"

VLIB_BEGIN_NAMESPACE

namespace VML {
    bool VMLCommonBuilder::CheckNativeCallParameter(std::vector<VMLPropertyValue> Properties,
                                  std::vector<VMLPropertyType>  PropertiesType) {
        for (int PropertiesCount = 0; PropertiesCount != Properties.size(); ++PropertiesCount) {
            if (Properties[PropertiesCount].PropertyType != PropertiesType[PropertiesCount]) {
                return false;
            }
        }

        return true;
    }
    void VMLCommonBuilder::Builder(Core::VUIObject* Object,
                 const int& X, const int& Y,
                 const int& Width,
                 const int& Height,
                 const int& Transparency,
                 const bool& Visble,
                 const Core::VLayoutMode& VerticalLayoutMode,
                 const Core::VLayoutMode& HorizontalLayoutMode,
                 const double& VerticalLayoutPercent, const double& HorizontalLayoutPercent,
                 const int& RelativeX, const int& RelativeY,
                 const int& XMiddleOffset, const int& YMiddleOffset,
                 const double& WidthRatio, const double& HeightRatio,
                 const VMLFinder& ObjectFinder) {
        Core::VLayout* _NativeLayout = new Core::VLayout(Object, Object->GetParent());
        _NativeLayout->SetHorizontalLayoutMode(HorizontalLayoutMode);
        _NativeLayout->SetVerticalLayoutMode(VerticalLayoutMode);
        _NativeLayout->SetVerticalLayoutPercent(VerticalLayoutPercent);
        _NativeLayout->SetHorizontalLayoutPercent(HorizontalLayoutPercent);
        _NativeLayout->SetRelativeX(RelativeX);
        _NativeLayout->SetRelativeY(RelativeY);
        _NativeLayout->SetXMiddleOffset(XMiddleOffset);
        _NativeLayout->SetYMiddleOffset(YMiddleOffset);
        Core::VScaleLayout* _NativeScaleLayout = new Core::VScaleLayout(Object, Object->GetParent());
        _NativeScaleLayout->SetWidthScalePercent(WidthRatio);
        _NativeScaleLayout->SetHeightScalePercent(HeightRatio);

        if (!Object->IsWidget()) {
            Object->Move(X, Y);
        }

        Object->Resize(Width, Height);
        Object->SetTransparency(Transparency);

        if (!Visble) {
            Object->Hide();
        }
    }

    void VMLCommonBuilder::AnalyzeProperty(const VMLFinder& RootFinder,
                                           Core::VUIObject* Object,
                                           std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                           VMLControlBuildStatus* BuildStatus) {
        int X = 0;
        int Y = 0;
        int Height = 0;
        int Width = 0;
        int Transparency = 255;
        bool Visble = true;

        Core::VLayoutMode  VerticalLayoutMode   = Core::VLayoutMode::LayoutModeDontUse;
        Core::VLayoutMode  HorizontalLayoutMode = Core::VLayoutMode::LayoutModeDontUse;

        Core::VUIObject* LayoutParent;

        double       VerticalLayoutPercent   = 0.f;
        double       HorizontalLayoutPercent = 0.f;

        int          RelativeX = 0;
        int          RelativeY = 0;

        int          XMiddleOffset = 0;
        int          YMiddleOffset = 0;

        double       WidthRatio  = 0.f;
        double       HeightRatio = 0.f;

        BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Ok;

        for (auto &ElementProperty: PropertyValueList) {
            if (ElementProperty.first == L"x") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue &&
                    ElementProperty.second.PropertyType != VMLPropertyType::NativeCall) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"x\" Property must match the type \"int\" or \"native call functional\"";

                    return;
                }

                if (ElementProperty.second.PropertyType == VMLPropertyType::IntValue) {
                    X = ElementProperty.second.PropertyAsInt;
                } else {
                    if (ElementProperty.second.NativeCallMethodName == L"center") {
                        HorizontalLayoutMode = Core::VLayoutMode::LayoutModeCenter;
                    }
                    if (ElementProperty.second.NativeCallMethodName == L"centeroffset") {
                        HorizontalLayoutMode = Core::VLayoutMode::LayoutModeMiddleOffset;

                        if (ElementProperty.second.NativeCallParameter.size() == 1 &&
                            CheckNativeCallParameter(ElementProperty.second.NativeCallParameter, {VMLPropertyType::IntValue})) {
                            XMiddleOffset = ElementProperty.second.NativeCallParameter[0].PropertyAsInt;
                        }
                    }
                    if (ElementProperty.second.NativeCallMethodName == L"relative") {
                        HorizontalLayoutMode = Core::VLayoutMode::LayoutModeRelative;

                        if (ElementProperty.second.NativeCallParameter.size() == 1 &&
                            CheckNativeCallParameter(ElementProperty.second.NativeCallParameter, {VMLPropertyType::IntValue})) {
                            RelativeX = ElementProperty.second.NativeCallParameter[0].PropertyAsInt;
                        }
                    }
                    if (ElementProperty.second.NativeCallMethodName == L"ratio") {
                        HorizontalLayoutMode = Core::VLayoutMode::LayoutModePercent;

                        if (ElementProperty.second.NativeCallParameter.size() == 1 &&
                            CheckNativeCallParameter(ElementProperty.second.NativeCallParameter, {VMLPropertyType::DoubleValue})) {
                            VerticalLayoutPercent = ElementProperty.second.NativeCallParameter[0].PropertyAsDouble;
                        }
                    }
                }
            }
            if (ElementProperty.first == L"y") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue &&
                    ElementProperty.second.PropertyType != VMLPropertyType::NativeCall) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"y\" Property must match the type \"int\"  or \"native call functional\"";

                    return;
                }

                if (ElementProperty.second.PropertyType == VMLPropertyType::IntValue) {
                    Y = ElementProperty.second.PropertyAsInt;
                } else {
                    if (ElementProperty.second.NativeCallMethodName == L"center") {
                        VerticalLayoutMode = Core::VLayoutMode::LayoutModeCenter;
                    }
                    if (ElementProperty.second.NativeCallMethodName == L"centeroffset") {
                        VerticalLayoutMode = Core::VLayoutMode::LayoutModeMiddleOffset;

                        if (ElementProperty.second.NativeCallParameter.size() == 1 &&
                            CheckNativeCallParameter(ElementProperty.second.NativeCallParameter, {VMLPropertyType::IntValue})) {
                            YMiddleOffset = ElementProperty.second.NativeCallParameter[0].PropertyAsInt;
                        }
                    }
                    if (ElementProperty.second.NativeCallMethodName == L"relative") {
                        VerticalLayoutMode = Core::VLayoutMode::LayoutModeRelative;

                        if (ElementProperty.second.NativeCallParameter.size() == 1 &&
                            CheckNativeCallParameter(ElementProperty.second.NativeCallParameter, {VMLPropertyType::IntValue})) {
                            RelativeY = ElementProperty.second.NativeCallParameter[0].PropertyAsInt;
                        }
                    }
                    if (ElementProperty.second.NativeCallMethodName == L"ratio") {
                        VerticalLayoutMode = Core::VLayoutMode::LayoutModePercent;

                        if (ElementProperty.second.NativeCallParameter.size() == 1 &&
                            CheckNativeCallParameter(ElementProperty.second.NativeCallParameter, {VMLPropertyType::DoubleValue})) {
                            HorizontalLayoutPercent = ElementProperty.second.NativeCallParameter[0].PropertyAsDouble;
                        }
                    }
                }
            }
            if (ElementProperty.first == L"width") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue &&
                    ElementProperty.second.PropertyType != VMLPropertyType::NativeCall) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"width\" Property must match the type \"int\"";

                    return;
                }

                if (ElementProperty.second.PropertyType == VMLPropertyType::IntValue) {
                    Width = ElementProperty.second.PropertyAsInt;
                } else {
                    if (ElementProperty.second.NativeCallMethodName == L"fill") {
                        WidthRatio = 1.f;
                    }
                    if (ElementProperty.second.NativeCallMethodName == L"scale") {
                        HorizontalLayoutMode = Core::VLayoutMode::LayoutModeRelative;

                        for (auto& Property : ElementProperty.second.NativeCallParameter) {
                            if (Property.PropertyType == VMLPropertyType::DoubleValue) {
                                WidthRatio = Property.PropertyAsDouble;
                            }
                        }
                    }
                }
            }
            if (ElementProperty.first == L"height") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue &&
                    ElementProperty.second.PropertyType != VMLPropertyType::NativeCall) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"height\" Property must match the type \"int\" or \"native call functional\"";

                    return;
                }

                if (ElementProperty.second.PropertyType == VMLPropertyType::IntValue) {
                    Height = ElementProperty.second.PropertyAsInt;
                } else {
                    if (ElementProperty.second.NativeCallMethodName == L"fill") {
                        HeightRatio = 1.f;
                    }
                    if (ElementProperty.second.NativeCallMethodName == L"scale") {
                        HorizontalLayoutMode = Core::VLayoutMode::LayoutModeRelative;

                        for (auto& Property : ElementProperty.second.NativeCallParameter) {
                            if (Property.PropertyType == VMLPropertyType::DoubleValue) {
                                HeightRatio = Property.PropertyAsDouble;
                            }
                        }
                    }
                }
            }
            if (ElementProperty.first == L"transparency") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"transparency\" Property must match the type \"int\"";

                    return;
                }

                Transparency = ElementProperty.second.PropertyAsInt;
            }
            if (ElementProperty.first == L"visible") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"visible\" Property Must Match the Type \"boolean\"";

                    return;
                }

                Visble = ElementProperty.second.PropertyAsBool;
            }
        }

        VMLCommonBuilder::Builder(Object, X, Y, Width, Height, Transparency, Visble,
                                  VerticalLayoutMode, HorizontalLayoutMode,
                                  VerticalLayoutPercent, HorizontalLayoutPercent,
                                  RelativeX, RelativeY, XMiddleOffset, YMiddleOffset,
                                  WidthRatio, HeightRatio, RootFinder);
    }

    void VMLPushButtonBuilder::Builder(Core::VPushButton *PushButton, const std::wstring &PlaneText, const int &TextSize) {
        PushButton->SetPlaneText(PlaneText);

        if (TextSize != 0) {
            PushButton->SetTextSize(TextSize);
        }
    }

    void VMLPushButtonBuilder::AnalyzeProperty(const VMLFinder& RootFinder, Core::VPushButton *Object,
                                                 std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                                 VMLControlBuildStatus *BuildStatus) {
        
        int TextSize = 0;
        std::wstring Text;

        for (auto &ElementProperty: PropertyValueList) {
            if (ElementProperty.first == L"font-size") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"font-size\" Property Must Match the Type \"int\"";

                    return;
                }

                TextSize = ElementProperty.second.PropertyAsInt;
            }
            if (ElementProperty.first == L"text") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"text\" Property Must Match the Type \"string\"";

                    return;
                }

                Text = ElementProperty.second.PropertyAsString;
            }
        }

        Builder(Object, Text, TextSize);
    }

    VMLPushButtonBuilder::VMLPushButtonBuilder(const VMLFinder& RootFinder,
                                               Core::VPushButton* Object,
                                               std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                               VMLControlBuildStatus* BuildStatus)
            : VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus) {
        AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
    }

    void VMLImageLabelBuilder::Builder(Core::VImageLabel *ImageLabel, Core::VImage *Image) {
        ImageLabel->SetImage(Image);
    }

    void VMLImageLabelBuilder::AnalyzeProperty(const VMLFinder& RootFinder, Core::VImageLabel* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                               VMLControlBuildStatus* BuildStatus) {
        
        Core::VImage *Image = nullptr;

        for (auto &ElementProperty: PropertyValueList) {
            if (ElementProperty.first == L"src") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"src\" Property Must Match the Type \"string\"";

                    return;
                }

                Image = new Core::VImage(ElementProperty.second.PropertyAsString,
                                         Object->CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget());
            }
        }

        Builder(Object, Image);
    }

    VMLImageLabelBuilder::VMLImageLabelBuilder(const VMLFinder& RootFinder, Core::VImageLabel* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                               VMLControlBuildStatus* BuildStatus)
            : VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus) {
        AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
    }

    Core::VFontAlignment VMLTextLabelBuilder::ConvertAlignment(std::wstring AlignmentString) {
        if (AlignmentString == L"alignment-near") {
            return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_LEADING;
        }
        if (AlignmentString == L"alignment-center") {
            return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER;
        }
        if (AlignmentString == L"alignment-far") {
            return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_TRAILING;
        }

        return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_LEADING;
    }

    Core::VFontParagraphAlignment VMLTextLabelBuilder::ConvertParagraphAlignment(std::wstring ParagraphString) {
        if (ParagraphString == L"alignment-near") {
            return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
        }
        if (ParagraphString == L"alignment-center") {
            return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
        }
        if (ParagraphString == L"alignment-far") {
            return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_FAR;
        }
        return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
    }

    void VMLTextLabelBuilder::Builder(Core::VTextLabel *TextLabel, const std::wstring &PlaneText, const int &TextSize,
                                       const Core::VFontAlignment &Alignment, const Core::VFontParagraphAlignment &LineAlignment,
                                       const bool& AutoSize) {
        TextLabel->SetPlaneText(PlaneText);

        if (TextSize != 0) {
            TextLabel->SetTextSize(TextSize);
        }

        TextLabel->SetLineAlignment(Alignment);
        TextLabel->SetParagraphAlignment(LineAlignment);
        TextLabel->SetAutoSize(AutoSize);
    }

    void VMLTextLabelBuilder::AnalyzeProperty(const VMLFinder& RootFinder, Core::VTextLabel *Object,
                                               std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                               VMLControlBuildStatus *BuildStatus) {
        
        std::wstring Text;
        int  TextSize = 0;
        bool AutoSize = false;

        Core::VFontAlignment Alignment = Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_LEADING;
        Core::VFontParagraphAlignment LineAlignment = Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_NEAR;

        for (auto &ElementProperty: PropertyValueList) {
            if (ElementProperty.first == L"text") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"text\" Property Must Match the Type \"string\"";

                    return;
                }

                Text = ElementProperty.second.PropertyAsString;
            }
            if (ElementProperty.first == L"font-size") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"font-size\" Property Must Match the Type \"int\"";

                    return;
                }

                TextSize = ElementProperty.second.PropertyAsInt;
            }
            if (ElementProperty.first == L"alignment") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"alignment\" Property Must Match the Type \"alignment-string\"";

                    return;
                }

                Alignment = ConvertAlignment(ElementProperty.second.PropertyAsString);
            }
            if (ElementProperty.first == L"line-alignment") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"line-alignment\" Property Must Match the Type \"alignment-string\"";

                    return;
                }

                LineAlignment = ConvertParagraphAlignment(ElementProperty.second.PropertyAsString);
            }
            if (ElementProperty.first == L"auto-size") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"auto-size\" Property Must Match the Type \"boolean\"";

                    return;
                }

                AutoSize = ElementProperty.second.PropertyAsBool;
            }
        }

        Builder(Object, Text, TextSize, Alignment, LineAlignment, AutoSize);
    }

    VMLTextLabelBuilder::VMLTextLabelBuilder(const VMLFinder& RootFinder, Core::VTextLabel* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                             VMLControlBuildStatus* BuildStatus)
            : VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus) {
        AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
    }

    void VMLMainWindowBuilder::Builder(Core::VMainWindow *MainWindow, const int &Width, const int &Height,
                                        const std::wstring &Title, const bool &Sizble, const bool &FramelessStatus) {
        MainWindow->Resize(Width, Height);

        if (!Title.empty()) {
            MainWindow->SetTitle(Title);
        }

        MainWindow->SetSizble(Sizble);
        MainWindow->SetFrameless(FramelessStatus);
    }

    void VMLMainWindowBuilder::AnalyzeProperty(Core::VMainWindow *Object,
                                                std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                                VMLControlBuildStatus *BuildStatus) {
        int Width = 0;
        int Height = 0;

        std::wstring Title;

        bool Sizble = true;
        bool FramelessStatus = false;

        for (auto &ElementProperty: PropertyValueList) {
            if (ElementProperty.first == L"width") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"width\" Property Must Match the Type \"int\"";

                    return;
                }

                Width = ElementProperty.second.PropertyAsInt;
            }
            if (ElementProperty.first == L"height") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"height\" Property Must Match the Type \"int\"";

                    return;
                }

                Height = ElementProperty.second.PropertyAsInt;
            }
            if (ElementProperty.first == L"title") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"title\" Property Must Match the Type \"string\"";

                    return;
                }

                Title = ElementProperty.second.PropertyAsString;
            }
            if (ElementProperty.first == L"sizable") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"sizable\" Property Must Match the Type \"boolean\"";

                    return;
                }

                Sizble = ElementProperty.second.PropertyAsBool;
            }
            if (ElementProperty.first == L"frameless") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"frameless\" Property Must Match the Type \"boolean\"";

                    return;
                }

                FramelessStatus = ElementProperty.second.PropertyAsBool;
            }
        }

        Builder(Object, Width, Height, Title, Sizble, FramelessStatus);
    }

    VMLMainWindowBuilder::VMLMainWindowBuilder(Core::VMainWindow *MainWindow,
                                                 std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                                 VMLControlBuildStatus *BuildStatus) {
        AnalyzeProperty(MainWindow, PropertyValueList, BuildStatus);
    }

    void VMLLayoutBuilder::Builder(Core::VLayout* Layout, Core::VLayoutMode VerticalLayoutMode, Core::VLayoutMode HorizontalLayoutMode,
                 double VerticalLayoutPercent, double HorziontalLayoutPercent,
                 int RelativeX, int RelativeY, int XMiddleOffset, int YMiddleOffset) {
        Layout->SetVerticalLayoutMode(VerticalLayoutMode);
        Layout->SetHorizontalLayoutMode(HorizontalLayoutMode);
        Layout->SetVerticalLayoutPercent(VerticalLayoutPercent);
        Layout->SetHorizontalLayoutPercent(HorziontalLayoutPercent);
        Layout->SetRelativeX(RelativeX);
        Layout->SetRelativeY(RelativeY);
        Layout->SetXMiddleOffset(XMiddleOffset);
        Layout->SetXMiddleOffset(YMiddleOffset);
    }

    void VMLLayoutBuilder::AnalyzeProperty(const VMLFinder& RootFinder, Core::VLayout* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                         VMLControlBuildStatus* BuildStatus) {
        
        Core::VLayoutMode  VerticalLayoutMode = Core::VLayoutMode::LayoutModeCenter;
        Core::VLayoutMode  HorizontalLayoutMode = Core::VLayoutMode::LayoutModeCenter;

        double       VerticalLayoutPercent = 0.f;
        double       HorziontalLayoutPercent = 0.f;

        int          RelativeX = 0;
        int          RelativeY = 0;

        int          XMiddleOffset = 0;
        int          YMiddleOffset = 0;

        for (auto& ElementProperty : PropertyValueList) {
            if (ElementProperty.first == L"vertical-percent") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::DoubleValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"vertical-percent\" Property Must Match the Type \"int\"";

                    return;
                }

                VerticalLayoutPercent = ElementProperty.second.PropertyAsDouble;
            }
            if (ElementProperty.first == L"horziontal-percent") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::DoubleValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"horziontal-percent\" Property Must Match the Type \"int\"";

                    return;
                }

                HorziontalLayoutPercent = ElementProperty.second.PropertyAsDouble;
            }
            if (ElementProperty.first == L"relative-x") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"relative-x\" Property Must Match the Type \"int\"";

                    return;
                }

                RelativeX = ElementProperty.second.PropertyAsInt;
            }
            if (ElementProperty.first == L"relative-y") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"relative-y\" Property Must Match the Type \"int\"";

                    return;
                }

                RelativeY = ElementProperty.second.PropertyAsInt;
            }
            if (ElementProperty.first == L"x-middle-offset") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"x-middle-offset\" Property Must Match the Type \"int\"";

                    return;
                }

                XMiddleOffset = ElementProperty.second.PropertyAsInt;
            }
            if (ElementProperty.first == L"y-middle-offset") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"y-middle-offset\" Property Must Match the Type \"int\"";

                    return;
                }

                YMiddleOffset = ElementProperty.second.PropertyAsInt;
            }
            if (ElementProperty.first == L"vertical-layout-mode") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"vertical-layout-mode\" Property Must Match the Type \"string\"";

                    return;
                }

                if (ElementProperty.second.PropertyAsString == L"center") {
                    VerticalLayoutMode = Core::VLayoutMode::LayoutModeCenter;
                }
                if (ElementProperty.second.PropertyAsString == L"far") {
                    VerticalLayoutMode = Core::VLayoutMode::LayoutModeFar;
                }
                if (ElementProperty.second.PropertyAsString == L"percent") {
                    VerticalLayoutMode = Core::VLayoutMode::LayoutModePercent;
                }
                if (ElementProperty.second.PropertyAsString == L"relative") {
                    VerticalLayoutMode = Core::VLayoutMode::LayoutModeRelative;
                }
                if (ElementProperty.second.PropertyAsString == L"relative-bottom") {
                    VerticalLayoutMode = Core::VLayoutMode::LayoutModeRelativeBottom;
                }
                if (ElementProperty.second.PropertyAsString == L"relative-top") {
                    VerticalLayoutMode = Core::VLayoutMode::LayoutModeRelativeTop;
                }
                if (ElementProperty.second.PropertyAsString == L"middle-offset") {
                    VerticalLayoutMode = Core::VLayoutMode::LayoutModeMiddleOffset;
                }
            }
            if (ElementProperty.first == L"horizontal-layout") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"horizontal-layout\" Property Must Match the Type \"string\"";

                    return;
                }

                if (ElementProperty.second.PropertyAsString == L"center") {
                    HorizontalLayoutMode = Core::VLayoutMode::LayoutModeCenter;
                }
                if (ElementProperty.second.PropertyAsString == L"far") {
                    HorizontalLayoutMode = Core::VLayoutMode::LayoutModeFar;
                }
                if (ElementProperty.second.PropertyAsString == L"percent") {
                    HorizontalLayoutMode = Core::VLayoutMode::LayoutModePercent;
                }
                if (ElementProperty.second.PropertyAsString == L"relative") {
                    HorizontalLayoutMode = Core::VLayoutMode::LayoutModeRelative;
                }
                if (ElementProperty.second.PropertyAsString == L"relative-bottom") {
                    HorizontalLayoutMode = Core::VLayoutMode::LayoutModeRelativeBottom;
                }
                if (ElementProperty.second.PropertyAsString == L"relative-top") {
                    HorizontalLayoutMode = Core::VLayoutMode::LayoutModeRelativeTop;
                }
                if (ElementProperty.second.PropertyAsString == L"middle-offset") {
                    HorizontalLayoutMode = Core::VLayoutMode::LayoutModeMiddleOffset;
                }
            }
        }

        Builder(Object, VerticalLayoutMode, HorizontalLayoutMode,
                VerticalLayoutPercent, HorziontalLayoutPercent,
                RelativeX, RelativeY, XMiddleOffset, YMiddleOffset);
    }
    VMLLayoutBuilder::VMLLayoutBuilder(const VMLFinder& RootFinder, Core::VLayout* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
    VMLControlBuildStatus* BuildStatus)
            : VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus) {
                AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
    }

    void VMLRadioButtonBuilder::Builder(Core::VRadioButton* RadioButton, const bool& Status) {
        RadioButton->SetSwitchStatus(Status);
    }
    void VMLRadioButtonBuilder::AnalyzeProperty(const VMLFinder& RootFinder,
                                                Core::VRadioButton *RadioButton,
                                                std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                                VML::VMLControlBuildStatus *BuildStatus) {
        VMLCommonBuilder::AnalyzeProperty(RootFinder, RadioButton, PropertyValueList, BuildStatus);

        bool SwitchStatus = false;

        for (auto& ElementProperty : PropertyValueList) {
            if (ElementProperty.first == L"switch-status") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"switch-status\" Property Must Match the Type \"bool\"";

                    return;
                }

                SwitchStatus = ElementProperty.second.PropertyAsBool;
            }
        }

        Builder(RadioButton, SwitchStatus);
    }
    VMLRadioButtonBuilder::VMLRadioButtonBuilder(const VMLFinder& RootFinder, Core::VRadioButton* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                                 VMLControlBuildStatus* BuildStatus)
            : VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus) {
        AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
    }
    void VMLScaleLayoutBuilder::Builder(Core::VScaleLayout* Layout, const double& ScaleWidthPercent, const double& ScaleHeightPercent) {
        Layout->SetWidthScalePercent(ScaleWidthPercent);
        Layout->SetHeightScalePercent(ScaleHeightPercent);
    }
    void VMLScaleLayoutBuilder::AnalyzeProperty(const VMLFinder& RootFinder, Core::VScaleLayout* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                         VMLControlBuildStatus* BuildStatus) {
        
        double WidthScale = 1.f;
        double HeightScale = 1.f;

        for (auto& ElementProperty : PropertyValueList) {
            if (ElementProperty.first == L"width-scale") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::DoubleValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"width-scale\" Property Must Match the Type \"double\"";

                    return;
                }

                WidthScale = ElementProperty.second.PropertyAsDouble;
            }
            if (ElementProperty.first == L"height-scale") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::DoubleValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"height-scale\" Property Must Match the Type \"double\"";

                    return;
                }

                HeightScale = ElementProperty.second.PropertyAsDouble;
            }
        }

        Builder(Object, WidthScale, HeightScale);
    }
    VMLScaleLayoutBuilder::VMLScaleLayoutBuilder(const VMLFinder& RootFinder,
                                                 Core::VScaleLayout* Object,
                                                 std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                                 VMLControlBuildStatus* BuildStatus)
                                                 : VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus) {
        AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
    }

    void VMLSliderHorizontalBuilder::Builder(Core::VSliderHorizontal* SliderHorizontal, const double& Value) {
        SliderHorizontal->SetValue(Value);
    }
    void VMLSliderHorizontalBuilder::AnalyzeProperty(const VMLFinder& RootFinder, Core::VSliderHorizontal* Horizontal, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                         VMLControlBuildStatus* BuildStatus) {
        VMLCommonBuilder::AnalyzeProperty(RootFinder, Horizontal, PropertyValueList, BuildStatus);

        double Value = 0.f;

        for (auto& ElementProperty : PropertyValueList) {
            if (ElementProperty.first == L"value") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::DoubleValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"value\" Property Must Match the Type \"double\"";

                    return;
                }

                Value = ElementProperty.second.PropertyAsDouble;
            }
        }

        Builder(Horizontal, Value);
    }
    VMLSliderHorizontalBuilder::VMLSliderHorizontalBuilder(const VMLFinder& RootFinder, Core::VSliderHorizontal* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                               VMLControlBuildStatus* BuildStatus)  : VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus) {
        AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
    }

    void VMLSliderVerticalBuilder::Builder(Core::VSliderVertical* SliderHorizontal, const double& Value) {
        SliderHorizontal->SetValue(Value);
    }
    void VMLSliderVerticalBuilder::AnalyzeProperty(const VMLFinder& RootFinder, Core::VSliderVertical* Vertical, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                                     VMLControlBuildStatus* BuildStatus) {
        VMLCommonBuilder::AnalyzeProperty(RootFinder, Vertical, PropertyValueList, BuildStatus);

        double Value = 0.f;

        for (auto& ElementProperty : PropertyValueList) {
            if (ElementProperty.first == L"value") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::DoubleValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"value\" Property Must Match the Type \"double\"";

                    return;
                }

                Value = ElementProperty.second.PropertyAsDouble;
            }
        }

        Builder(Vertical, Value);
    }
    VMLSliderVerticalBuilder::VMLSliderVerticalBuilder(const VMLFinder& RootFinder, Core::VSliderVertical* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                                           VMLControlBuildStatus* BuildStatus)  : VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus) {
        AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
    }

    void VMLIconButtonBuilder::Builder(Core::VIconButton* IconButton, Core::VImage* Image) {
        IconButton->SetIconImage(Image);
    }

    void VMLIconButtonBuilder::AnalyzeProperty(const VMLFinder& RootFinder, Core::VIconButton *Object,
                                               std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                               VMLControlBuildStatus *BuildStatus) {
        Core::VImage* Image = nullptr;

        for (auto &ElementProperty: PropertyValueList) {
            if (ElementProperty.first == L"src") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"src\" Property Must Match the Type \"string\"";

                    return;
                }

                Image = new Core::VImage(ElementProperty.second.PropertyAsString,
                                         Object->CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget());
            }
        }

        Builder(Object, Image);
    }

    VMLIconButtonBuilder::VMLIconButtonBuilder(const VMLFinder& RootFinder,
                                               Core::VIconButton* Object,
                                               std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                               VMLControlBuildStatus* BuildStatus)
            : VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus) {
        AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
    }

    void VMLLineEditorBuilder::Builder(Core::VLineEditor* LineEditor, std::wstring PlaneText) {
        LineEditor->SetPlaneText(PlaneText);
    }
    void VMLLineEditorBuilder::AnalyzeProperty(const VMLFinder& RootFinder, Core::VLineEditor* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                         VMLControlBuildStatus* BuildStatus) {
        std::wstring PlaneText;

        for (auto &ElementProperty: PropertyValueList) {
            if (ElementProperty.first == L"plane-text") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"plane-text\" Property Must Match the Type \"string\"";

                    return;
                }

                PlaneText = ElementProperty.second.PropertyAsString;
            }
        }

        Builder(Object, PlaneText);
    }
    VMLLineEditorBuilder::VMLLineEditorBuilder(const VMLFinder& RootFinder, Core::VLineEditor* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                         VMLControlBuildStatus* BuildStatus)
            : VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus) {
        AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);

    }
}

VLIB_END_NAMESPACE