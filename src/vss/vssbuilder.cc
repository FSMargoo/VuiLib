#include "../../include/vss/vssbuilder.h"

VLIB_BEGIN_NAMESPACE

namespace VSS {
    Core::VFontAlignment VSSParserHelper::StringToAlignment(const std::wstring& String) {
        if (String == L"center") {
            return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER;
        }
        if (String == L"left") {
            return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_TRAILING;
        }
        if (String == L"right") {
            return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_LEADING;
        }
        if (String == L"justify") {
            return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
        }

        return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER;
    }
    Core::VFontParagraphAlignment VSSParserHelper::StringToParagraphAlignment(const std::wstring& String) {
        if (String == L"top") {
            return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
        }
        if (String == L"middle") {
            return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
        }
        if (String == L"bottom") {
            return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_FAR;
        }

        return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
    }

    void VSSVPushButtonBuilder::BuildVSSObject(Core::VPushButton *TargetControl,
                                               std::vector<VSSBasicSelector *> SelectorSet,
                                               Core::VPushButtonTheme *PushButtonTheme) {
        Core::VPushButtonTheme *Theme = PushButtonTheme;

        if (TargetControl != nullptr) {
            Theme = TargetControl->GetTheme();
        }

        for (auto &Selector: SelectorSet) {
            if (Selector->GetType() == VSSSelectorType::ElementSelector &&
                static_cast<VSSElementSelector *>(Selector)->ElementTag == L"pushbutton") {
                for (auto &Property: Selector->SelectorProperty) {
                    // Common Property ( Not fade switch supported )
                    if (Property.first == L"opacity") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::DoubleValue) {
                                TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
                            }
                        }
                    }
                    if (Property.first == L"font-family") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::StringValue) {
                                Theme->LabelFont->SetFamilyName(PropertyValue.PropertyAsString);
                            }
                        }
                    }
                    if (Property.first == L"font-size") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->LabelFont->SetTextSize(PropertyValue.PropertyAsInt);
                            }
                        }
                    }
                    if (Property.first == L"text-align") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::StringValue) {
                                Theme->LabelFont->SetLineAlignment(VSSParserHelper::StringToAlignment(PropertyValue.PropertyAsString));
                            }
                        }
                    }
                    if (Property.first == L"vertical-align") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::StringValue) {
                                Theme->LabelFont->SetLineAlignment(
                                        VSSParserHelper::StringToAlignment(PropertyValue.PropertyAsString));
                            }
                        }
                    }

                    if (Property.first == L"background-color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->StaticTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }
                    if (Property.first == L"color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->StaticTheme.TextColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }
                    if (Property.first == L"border-color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }

                    if (Property.first == L"border-width") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
                            }
                        }
                    }
                    if (Property.first == L"border-radius") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->StaticTheme.Radius = { PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
                            }
                        }
                    }

                    if (Property.first == L"border") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
                            }
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
                            }
                        }
                    }
                }
            }
            if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
                static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"pushbutton" &&
                static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"hover") {
                for (auto &Property: Selector->SelectorProperty) {
                    if (Property.first == L"background-color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->OnHoverTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }
                    if (Property.first == L"color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->OnHoverTheme.TextColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }
                    if (Property.first == L"border-color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }

                    if (Property.first == L"border-width") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
                            }
                        }
                    }
                    if (Property.first == L"border-radius") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->OnHoverTheme.Radius = { PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
                            }
                        }
                    }

                    if (Property.first == L"border") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
                            }
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
                            }
                        }
                    }
                }
            }
            if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
                static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"pushbutton" &&
                static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"active") {
                for (auto &Property: Selector->SelectorProperty) {
                    if (Property.first == L"background-color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->ActiveTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }
                    if (Property.first == L"color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->ActiveTheme.TextColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }
                    if (Property.first == L"border-color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }

                    if (Property.first == L"border-width") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
                            }
                        }
                    }
                    if (Property.first == L"border-radius") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->ActiveTheme.Radius = { PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
                            }
                        }
                    }

                    if (Property.first == L"border") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
                            }
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
                            }
                        }
                    }
                }
            }
        }

        Theme->LocalTheme = Theme->StaticTheme;

        if (TargetControl != nullptr) { TargetControl->Update(); }
    }
    VSSVPushButtonBuilder::VSSVPushButtonBuilder(Core::VPushButton *TargetControl,
                                                 std::vector<VSSBasicSelector *> SelectorSet,
                                                 Core::VPushButtonTheme *PushButtonTheme) {
           BuildVSSObject(TargetControl, SelectorSet, PushButtonTheme);
    }

    void VSSVTextLabelBuilder::BuildVSSObject(Core::VTextLabel *TargetControl,
                                              std::vector<VSSBasicSelector *> SelectorSet,
                                              Core::VTextLabelTheme *TextLabelTheme) {
        Core:: VTextLabelTheme *Theme = TextLabelTheme;

        if (TargetControl != nullptr) {
            Theme = TargetControl->GetTheme();
        }

        for (auto &Selector: SelectorSet) {
            if (Selector->GetType() == VSSSelectorType::ElementSelector &&
                static_cast<VSSElementSelector *>(Selector)->ElementTag == L"textlabel") {
                for (auto &Property: Selector->SelectorProperty) {
                    // Common Property ( Not fade switch supported )
                    if (Property.first == L"opacity") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::DoubleValue) {
                                TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
                            }
                        }
                    }
                    if (Property.first == L"font-family") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::StringValue) {
                                Theme->LabelFont->SetFamilyName(PropertyValue.PropertyAsString);
                            }
                        }
                    }
                    if (Property.first == L"font-size") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->LabelFont->SetTextSize(PropertyValue.PropertyAsInt);
                            }
                        }
                    }
                    if (Property.first == L"text-align") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::StringValue) {
                                Theme->LabelFont->SetLineAlignment(VSSParserHelper::StringToAlignment(PropertyValue.PropertyAsString));
                            }
                        }
                    }
                    if (Property.first == L"vertical-align") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::StringValue) {
                                Theme->LabelFont->SetLineAlignment(
                                        VSSParserHelper::StringToAlignment(PropertyValue.PropertyAsString));
                            }
                        }
                    }

                    if (Property.first == L"background-color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->StaticTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }
                    if (Property.first == L"color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->StaticTheme.TextColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }
                    if (Property.first == L"border-color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }

                    if (Property.first == L"border-width") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
                            }
                        }
                    }
                    if (Property.first == L"border-radius") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->StaticTheme.Radius = { PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
                            }
                        }
                    }

                    if (Property.first == L"border") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
                            }
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
                            }
                        }
                    }
                }

                Theme->LocalTheme   = Theme->StaticTheme;
                Theme->OnHoverTheme = Theme->StaticTheme;
                Theme->ActiveTheme  = Theme->StaticTheme;
            }
            if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
                static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"textlabel" &&
                static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"hover") {
                for (auto &Property: Selector->SelectorProperty) {
                    if (Property.first == L"background-color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->OnHoverTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }
                    if (Property.first == L"color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->OnHoverTheme.TextColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }
                    if (Property.first == L"border-color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }

                    if (Property.first == L"border-width") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
                            }
                        }
                    }
                    if (Property.first == L"border-radius") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->OnHoverTheme.Radius = { PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
                            }
                        }
                    }

                    if (Property.first == L"border") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
                            }
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
                            }
                        }
                    }
                }
            }
            if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
                static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"textlabel" &&
                static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"active") {
                for (auto &Property: Selector->SelectorProperty) {
                    if (Property.first == L"background-color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->ActiveTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }
                    if (Property.first == L"color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->ActiveTheme.TextColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }
                    if (Property.first == L"border-color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
                            }
                        }
                    }

                    if (Property.first == L"border-width") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
                            }
                        }
                    }
                    if (Property.first == L"border-radius") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->ActiveTheme.Radius = { PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
                            }
                        }
                    }

                    if (Property.first == L"border") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
                            }
                            if (PropertyValue.Type == VSSPropertyType::IntValue) {
                                Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
                            }
                        }
                    }
                }
            }
        }

        if (TargetControl != nullptr) { TargetControl->Update(); }
    }
    VSSVTextLabelBuilder::VSSVTextLabelBuilder(Core::VTextLabel *TargetControl,
                                               std::vector<VSSBasicSelector *> SelectorSet,
                                               Core::VTextLabelTheme *Theme) {
        BuildVSSObject(TargetControl, SelectorSet, Theme);
    }
    void VSSVImageLabelBuilder::BuildVSSObject(Core::VImageLabel *TargetControl,
                                               std::vector<VSSBasicSelector *> SelectorSet,
                                               Core::VImageLabelTheme *ImageLabelTheme) {
        Core::VImageLabelTheme *Theme = ImageLabelTheme;
        if (TargetControl != nullptr) {
            Theme = TargetControl->GetTheme();
        }

        for (auto &Selector: SelectorSet) {
            if (Selector->GetType() == VSSSelectorType::ElementSelector &&
                static_cast<VSSElementSelector *>(Selector)->ElementTag == L"imagelabel") {
                for (auto &Property: Selector->SelectorProperty) {
                    if (Property.first == L"opacity") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::DoubleValue) {
                                TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
                            }
                        }
                    }
                }
            }
        }

        if (TargetControl != nullptr) { TargetControl->Update(); }
    }
    VSSVImageLabelBuilder::VSSVImageLabelBuilder(Core::VImageLabel *TargetControl,
                                                 std::vector<VSSBasicSelector *> SelectorSet,
                                                 Core::VImageLabelTheme *ImageLabelTheme) {
        BuildVSSObject(TargetControl, SelectorSet, ImageLabelTheme);
    }
    void VSSVMainWindowBuilder::BuildVSSObject(Core::VMainWindow *TargetControl,
                        std::vector<VSSBasicSelector *> SelectorSet) {
        for (auto &Selector: SelectorSet) {
            if (Selector->GetType() == VSSSelectorType::ElementSelector &&
                static_cast<VSSElementSelector *>(Selector)->ElementTag == L"mainwindow") {
                for (auto &Property: Selector->SelectorProperty) {
                    if (Property.first == L"background-color") {
                        for (auto &PropertyValue: Property.second.ValueList) {
                            if (PropertyValue.Type == VSSPropertyType::ColorValue) {
                                TargetControl->SetBackgroundColor(PropertyValue.PropertyAsColorValue);
                            }
                        }
                    }
                }
            }
        }
    }
    VSSVMainWindowBuilder::VSSVMainWindowBuilder(Core::VMainWindow *TargetControl,
                                                 std::vector<VSSBasicSelector *> SelectorSet) {
        BuildVSSObject(TargetControl, SelectorSet);
    }
}

VLIB_END_NAMESPACE