#include "../../include/vml/vmlbuilder.h"

VLIB_BEGIN_NAMESPACE

namespace VML {
    void
    VMLCommonBuilder::Builder(Core::VUIObject *Object, const int &X, const int &Y, const int &Width, const int &Height,
                               const int &Transparency, const bool &Visble) {
        if (!Object->IsWidget()) {
            Object->Move(X, Y);
        }

        Object->Resize(Width, Height);
        Object->SetTransparency(Transparency);

        if (!Visble) {
            Object->Hide();
        }
    }

    void VMLCommonBuilder::AnalyzeProperty(Core::VUIObject *Object,
                                            std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                            VMLControlBuildStatus *BuildStatus) {
        int X = 0;
        int Y = 0;
        int Height = 0;
        int Width = 0;
        int Transparency = 255;
        bool Visble = true;

        BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Ok;

        for (auto &ElementProperty: PropertyValueList) {
            if (ElementProperty.first == L"x") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"x\" Property must match the type \"int\"";

                    return;
                }

                X = ElementProperty.second.PropertyAsInt;
            }
            if (ElementProperty.first == L"y") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"y\" Property must match the type \"int\"";

                    return;
                }

                Y = ElementProperty.second.PropertyAsInt;
            }
            if (ElementProperty.first == L"width") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"width\" Property must match the type \"int\"";

                    return;
                }

                Width = ElementProperty.second.PropertyAsInt;
            }
            if (ElementProperty.first == L"height") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"height\" Property must match the type \"int\"";

                    return;
                }

                Height = ElementProperty.second.PropertyAsInt;
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

        VMLCommonBuilder::Builder(Object, X, Y, Width, Height, Transparency, Visble);
    }

    void VMLPushButtonBuilder::Builder(Core::VPushButton *PushButton, const std::wstring &PlaneText, const int &TextSize) {
        PushButton->SetPlaneText(PlaneText);

        if (TextSize != 0) {
            PushButton->SetTextSize(TextSize);
        }
    }

    void VMLPushButtonBuilder::AnalyzeProperty(Core::VPushButton *Object,
                                                 std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                                 VMLControlBuildStatus *BuildStatus) {
        VMLCommonBuilder::AnalyzeProperty(Object, PropertyValueList, BuildStatus);

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

    VMLPushButtonBuilder::VMLPushButtonBuilder(Core::VPushButton *Object,
                                                   std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                                   VMLControlBuildStatus *BuildStatus)
            : VMLCommonBuilder(Object, PropertyValueList, BuildStatus) {
        AnalyzeProperty(Object, PropertyValueList, BuildStatus);
    }

    void VMLImageLabelBuilder::Builder(Core::VImageLabel *ImageLabel, Core::VImage *Image) {
        ImageLabel->SetImage(Image);
    }

    void VMLImageLabelBuilder::AnalyzeProperty(Core::VImageLabel *Object,
                                                std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                                VMLControlBuildStatus *BuildStatus) {
        VMLCommonBuilder::AnalyzeProperty(Object, PropertyValueList, BuildStatus);

        Core::VImage *Image = nullptr;

        for (auto &ElementProperty: PropertyValueList) {
            if (ElementProperty.first == L"image-path") {
                if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue) {
                    BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
                    BuildStatus->FailedReason = L"\"image-path\" Property Must Match the Type \"string\"";

                    return;
                }

                Image = new Core::VImage(ElementProperty.second.PropertyAsString,
                                         Object->CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget());
            }
        }

        Builder(Object, Image);
    }

    VMLImageLabelBuilder::VMLImageLabelBuilder(Core::VImageLabel *Object,
                                                 std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                                 VMLControlBuildStatus *BuildStatus)
            : VMLCommonBuilder(Object, PropertyValueList, BuildStatus) {
        AnalyzeProperty(Object, PropertyValueList, BuildStatus);
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
                                       const Core::VFontAlignment &Alignment,
                                       const Core::VFontParagraphAlignment &LineAlignment) {
        TextLabel->SetPlaneText(PlaneText);

        if (TextSize != 0) {
            TextLabel->SetTextSize(TextSize);
        }

        TextLabel->SetLineAlignment(Alignment);
        TextLabel->SetParagraphAlignment(LineAlignment);
    }

    void VMLTextLabelBuilder::AnalyzeProperty(Core::VTextLabel *Object,
                                               std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                               VMLControlBuildStatus *BuildStatus) {
        VMLCommonBuilder::AnalyzeProperty(Object, PropertyValueList, BuildStatus);

        std::wstring Text;
        int TextSize = 0;

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
        }

        Builder(Object, Text, TextSize, Alignment, LineAlignment);
    }

    VMLTextLabelBuilder::VMLTextLabelBuilder(Core::VTextLabel *Object,
                                               std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                               VMLControlBuildStatus *BuildStatus)
            : VMLCommonBuilder(Object, PropertyValueList, BuildStatus) {
        AnalyzeProperty(Object, PropertyValueList, BuildStatus);
    }

    void VMLMainWindowBuilder::Builder(Core::VMainWindow *MainWindow, const int &Width, const int &Height,
                                        const std::wstring &Title, const bool &Sizble) {
        MainWindow->Resize(Width, Height);

        if (!Title.empty()) {
            MainWindow->SetTitle(Title);
        }

        MainWindow->SetSizble(Sizble);
    }

    void VMLMainWindowBuilder::AnalyzeProperty(Core::VMainWindow *Object,
                                                std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                                VMLControlBuildStatus *BuildStatus) {
        int Width = 0;
        int Height = 0;

        std::wstring Title;

        bool Sizble = true;

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
        }

        Builder(Object, Width, Height, Title, Sizble);
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

    void VMLLayoutBuilder::AnalyzeProperty(Core::VLayout* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                         VMLControlBuildStatus* BuildStatus) {
        VMLCommonBuilder::AnalyzeProperty(Object, PropertyValueList, BuildStatus);

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
    VMLLayoutBuilder::VMLLayoutBuilder(Core::VLayout* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
            VMLControlBuildStatus* BuildStatus)
            : VMLCommonBuilder(Object, PropertyValueList, BuildStatus) {
                AnalyzeProperty(Object, PropertyValueList, BuildStatus);
    }

    void VMLRadioButtonBuilder::Builder(Core::VRadioButton* RadioButton, const bool& Status) {
        RadioButton->SetSwitchStatus(Status);
    }
    void VMLRadioButtonBuilder::AnalyzeProperty(Core::VRadioButton *RadioButton,
                                                std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                                VML::VMLControlBuildStatus *BuildStatus) {
        VMLCommonBuilder::AnalyzeProperty(RadioButton, PropertyValueList, BuildStatus);

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
    VMLRadioButtonBuilder::VMLRadioButtonBuilder(Core::VRadioButton* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                                 VMLControlBuildStatus* BuildStatus)
            : VMLCommonBuilder(Object, PropertyValueList, BuildStatus) {
        AnalyzeProperty(Object, PropertyValueList, BuildStatus);
    }
}

VLIB_END_NAMESPACE