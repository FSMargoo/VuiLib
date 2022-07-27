/*
 * VSSBuilder.hpp
 *	@description : VSS Builder
*/

#pragma once

#include "../../VBasicControl"
#include "vssparser.hpp"

VLIB_BEGIN_NAMESPACE

class VVSSVPushButtonBuilder {
private:
	void BuildVSSObject(VPushButton* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet, VPushButtonTheme* PushButtonTheme) {
		VPushButtonTheme* Theme = PushButtonTheme;

		if (TargetControl != nullptr) {
			Theme = TargetControl->GetTheme();
		}

		for (auto& Selector : SelectorSet) {
			if (Selector->GetType() == VVSSSelectorType::ElementSelector &&
				static_cast<VVSSElementSelector*>(Selector)->ElementTag == L"vpushbutton") {
				for (auto& Property : Selector->SelectorProperty) {
					if (Property.first == L"box-shadow") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BoxShadowPixel = PropertyValue.PropertyAsInt;
							}
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BoxShadowColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"box-shadow-color") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BoxShadowColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"box-shadow-radius") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BoxShadowPixel = PropertyValue.PropertyAsInt;
							}
						}
					}
					if (Property.first == L"border-style") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::StringValue) {
								if (PropertyValue.PropertyAsString == L"solid") {
									Theme->BorderStyle = VPenStyle::SolidStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash") {
									Theme->BorderStyle = VPenStyle::DashStyle;
								}
								if (PropertyValue.PropertyAsString == L"dot") {
									Theme->BorderStyle = VPenStyle::DotStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash-dot-dot") {
									Theme->BorderStyle = VPenStyle::DashDotDotStyle;
								}
								if (PropertyValue.PropertyAsString == L"none") {
									Theme->BorderThinkness = 0.f;
								}
							}
						}
					}
					if (Property.first == L"border-width") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BorderThinkness = PropertyValue.PropertyAsInt;
							}
						}
					}
					if (Property.first == L"border-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->LineColor = PropertyValue.PropertyAsColorValue;
								Theme->CurrentLineColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"border-radius") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->Radius = { PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt };
							}
						}
					}
					if (Property.first == L"border") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::StringValue) {
								if (PropertyValue.PropertyAsString == L"solid") {
									Theme->BorderStyle = VPenStyle::SolidStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash") {
									Theme->BorderStyle = VPenStyle::DashStyle;
								}
								if (PropertyValue.PropertyAsString == L"dot") {
									Theme->BorderStyle = VPenStyle::DotStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash-dot-dot") {
									Theme->BorderStyle = VPenStyle::DashDotDotStyle;
								}
								if (PropertyValue.PropertyAsString == L"none") {
									Theme->BorderThinkness = 0.f;
								}
							}
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BorderThinkness = PropertyValue.PropertyAsInt;
							}
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->LineColor = PropertyValue.PropertyAsColorValue;
								Theme->CurrentLineColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}

					if (Property.first == L"text-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->TextColor = PropertyValue.PropertyAsColorValue;
								Theme->CurrentTextColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"background-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BackgroundColor = PropertyValue.PropertyAsColorValue;
								Theme->CurrentBackgroundColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"opacity") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::DoubleValue) {
								TargetControl->SetTransparency(PropertyValue.PropertyAsDouble * 255);
							}
						}
					}

					if (Property.first == L"text-size") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->FontSize = PropertyValue.PropertyAsInt;
							}
						}
					}

					if (Property.first == L"font-family") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::StringValue) {
								delete Theme->FontFamily;

								Theme->FontFamily = new VFontFamily(PropertyValue.PropertyAsString);
							}
						}
					}
				}
			}
			if (Selector->GetType() == VVSSSelectorType::FakeClassSelector &&
				static_cast<VVSSFakeClassSelector*>(Selector)->ElementTag == L"vpushbutton" &&
				static_cast<VVSSFakeClassSelector*>(Selector)->ClassTag == L"hover") {
				for (auto& Property : Selector->SelectorProperty) {
					if (Property.first == L"border-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->OnHoverLineColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"text-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->OnHoverTextColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"background-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->OnHoverBackgroundColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
				}
			}
			if (Selector->GetType() == VVSSSelectorType::FakeClassSelector &&
				static_cast<VVSSFakeClassSelector*>(Selector)->ElementTag == L"vpushbutton" &&
				static_cast<VVSSFakeClassSelector*>(Selector)->ClassTag == L"active") {
				for (auto& Property : Selector->SelectorProperty) {
					if (Property.first == L"border-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->OnClickedLineColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"text-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->OnClickedTextColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"background-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->OnClickedBackgroundColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
				}
			}
		}

		if (TargetControl != nullptr) { TargetControl->Update(); }
	}

public:
	VVSSVPushButtonBuilder(VPushButton* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet, VPushButtonTheme* PushButtonTheme) {
		BuildVSSObject(TargetControl, SelectorSet, PushButtonTheme);
	}
};
class VVSSVTextLabelBuilder {
private:
	void BuildVSSObject(VTextLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet, VTextLabelTheme* TextLabelTheme) {
		VTextLabelTheme* Theme = TextLabelTheme;

		if (TargetControl != nullptr) {
			Theme = TargetControl->GetTheme();
		}

		for (auto& Selector : SelectorSet) {
			if (Selector->GetType() == VVSSSelectorType::ElementSelector &&
				static_cast<VVSSElementSelector*>(Selector)->ElementTag == L"vtextlabel") {
				for (auto& Property : Selector->SelectorProperty) {
					if (Property.first == L"box-shadow") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BoxShadowPixel = PropertyValue.PropertyAsInt;
							}
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BoxShadowColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"box-shadow-color") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BoxShadowColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"box-shadow-radius") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BoxShadowPixel = PropertyValue.PropertyAsInt;
							}
						}
					}

					if (Property.first == L"border-style") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::StringValue) {
								if (PropertyValue.PropertyAsString == L"solid") {
									Theme->BorderStyle = VPenStyle::SolidStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash") {
									Theme->BorderStyle = VPenStyle::DashStyle;
								}
								if (PropertyValue.PropertyAsString == L"dot") {
									Theme->BorderStyle = VPenStyle::DotStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash-dot-dot") {
									Theme->BorderStyle = VPenStyle::DashDotDotStyle;
								}
								if (PropertyValue.PropertyAsString == L"none") {
									Theme->BorderThinkness = 0.f;
								}
							}
						}
					}
					if (Property.first == L"border-width") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BorderThinkness = PropertyValue.PropertyAsInt;
							}
						}
					}
					if (Property.first == L"border-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->LineColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"border-radius") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->Radius = { PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt };
							}
						}
					}
					if (Property.first == L"border") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::StringValue) {
								if (PropertyValue.PropertyAsString == L"solid") {
									Theme->BorderStyle = VPenStyle::SolidStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash") {
									Theme->BorderStyle = VPenStyle::DashStyle;
								}
								if (PropertyValue.PropertyAsString == L"dot") {
									Theme->BorderStyle = VPenStyle::DotStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash-dot-dot") {
									Theme->BorderStyle = VPenStyle::DashDotDotStyle;
								}
								if (PropertyValue.PropertyAsString == L"none") {
									Theme->BorderThinkness = 0.f;
								}
							}
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BorderThinkness = PropertyValue.PropertyAsInt;
							}
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->LineColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}

					if (Property.first == L"text-align") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::StringValue) {
								if (PropertyValue.PropertyAsString == L"near") {
									Theme->FontFormat->SetAlignment(VStringAlignment::AlignmentNear);
								}
								if (PropertyValue.PropertyAsString == L"far") {
									Theme->FontFormat->SetAlignment(VStringAlignment::AlignmentFar);
								}
								if (PropertyValue.PropertyAsString == L"center") {
									Theme->FontFormat->SetAlignment(VStringAlignment::AlignmentCenter);
								}
							}
						}
					}
					if (Property.first == L"text-line-align") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::StringValue) {
								if (PropertyValue.PropertyAsString == L"near") {
									Theme->FontFormat->SetLineAlignment(VStringAlignment::AlignmentNear);
								}
								if (PropertyValue.PropertyAsString == L"far") {
									Theme->FontFormat->SetLineAlignment(VStringAlignment::AlignmentFar);
								}
								if (PropertyValue.PropertyAsString == L"center") {
									Theme->FontFormat->SetLineAlignment(VStringAlignment::AlignmentCenter);
								}
							}
						}
					}

					if (Property.first == L"text-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->TextColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"background-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BackgroundColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"opacity") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::DoubleValue) {
								TargetControl->SetTransparency(PropertyValue.PropertyAsDouble * 255);
							}
						}
					}

					if (Property.first == L"text-size") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->FontSize = PropertyValue.PropertyAsInt;
							}
						}
					}
					if (Property.first == L"font-family") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::StringValue) {
								delete Theme->FontFamily;

								Theme->FontFamily = new VFontFamily(PropertyValue.PropertyAsString);
							}
						}
					}
				}
			}
		}

		if (TargetControl != nullptr) { TargetControl->Update(); }
	}

public:
	VVSSVTextLabelBuilder(VTextLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet, VTextLabelTheme* Theme) {
		BuildVSSObject(TargetControl, SelectorSet, Theme);
	}
};
class VVSSVImageLabelBuilder {
private:
	void BuildVSSObject(VImageLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet, VImageLabelTheme* ImageLabelTheme) {
		VImageLabelTheme* Theme = ImageLabelTheme;
		if (TargetControl != nullptr) {
			Theme = TargetControl->GetTheme();
		}

		for (auto& Selector : SelectorSet) {
			if (Selector->GetType() == VVSSSelectorType::ElementSelector &&
				static_cast<VVSSElementSelector*>(Selector)->ElementTag == L"vimagelabel") {
				for (auto& Property : Selector->SelectorProperty) {
					if (Property.first == L"box-shadow") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BoxShadowPixel = PropertyValue.PropertyAsInt;
							}
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BoxShadowColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"box-shadow-color") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BoxShadowColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"box-shadow-radius") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BoxShadowPixel = PropertyValue.PropertyAsInt;
							}
						}
					}

					if (Property.first == L"border-radius") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->Radius = { PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt };
							}
						}
					}
					if (Property.first == L"no-repeat") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::BooleanValue) {
								Theme->ImageRepeat = PropertyValue.PropertyAsBool;
							}
						}
					}
					if (Property.first == L"opacity") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::DoubleValue) {
								TargetControl->SetTransparency(PropertyValue.PropertyAsDouble * 255);
							}
						}
					}
				}
			}
		}

		if (TargetControl != nullptr) { TargetControl->Update(); }
	}

public:
	VVSSVImageLabelBuilder(VImageLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet, VImageLabelTheme* ImageLabelTheme) {
		BuildVSSObject(TargetControl, SelectorSet, ImageLabelTheme);
	}
};
class VVSSVBlurLabelBuilder {
private:
	void BuildVSSObject(VBlurLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet, VBlurLabelTheme* BlurLabelTheme) {
		VBlurLabelTheme* Theme = BlurLabelTheme;
		if (Theme != nullptr) {
			Theme = TargetControl->GetTheme();
		}

		for (auto& Selector : SelectorSet) {
			if (Selector->GetType() == VVSSSelectorType::ElementSelector &&
				static_cast<VVSSElementSelector*>(Selector)->ElementTag == L"vblurlabel") {
				for (auto& Property : Selector->SelectorProperty) {
					if (Property.first == L"border-radius") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->Radius = { PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt };
							}
						}
					}
					if (Property.first == L"blur-radius") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BlurRadius = PropertyValue.PropertyAsInt;
							}
						}
					}
					if (Property.first == L"mix-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->MixedColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"opacity") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::DoubleValue) {
								TargetControl->SetTransparency(PropertyValue.PropertyAsDouble * 255);
							}
						}
					}
				}
			}
		}

		if (TargetControl != nullptr) { TargetControl->Update(); }
	}

public:
	VVSSVBlurLabelBuilder(VBlurLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet, VBlurLabelTheme* BlurLabelTheme) {
		BuildVSSObject(TargetControl, SelectorSet, BlurLabelTheme);
	}
};
class VVSSVIconButtonBuilder {
private:
	void BuildVSSObject(VIconButton* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet, VIconButtonTheme* IconButtonTheme) {
		VIconButtonTheme* Theme = IconButtonTheme;
		if (TargetControl != nullptr) {
			Theme = TargetControl->GetTheme();
		}

		for (auto& Selector : SelectorSet) {
			if (Selector->GetType() == VVSSSelectorType::ElementSelector &&
				static_cast<VVSSElementSelector*>(Selector)->ElementTag == L"viconbutton") {
				for (auto& Property : Selector->SelectorProperty) {
					if (Property.first == L"box-shadow") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BoxShadowPixel = PropertyValue.PropertyAsInt;
							}
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BoxShadowColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"box-shadow-color") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BoxShadowColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"box-shadow-radius") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BoxShadowPixel = PropertyValue.PropertyAsInt;
							}
						}
					}
					if (Property.first == L"border-style") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::StringValue) {
								if (PropertyValue.PropertyAsString == L"solid") {
									Theme->BorderStyle = VPenStyle::SolidStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash") {
									Theme->BorderStyle = VPenStyle::DashStyle;
								}
								if (PropertyValue.PropertyAsString == L"dot") {
									Theme->BorderStyle = VPenStyle::DotStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash-dot-dot") {
									Theme->BorderStyle = VPenStyle::DashDotDotStyle;
								}
								if (PropertyValue.PropertyAsString == L"none") {
									Theme->BorderThinkness = 0.f;
								}
							}
						}
					}
					if (Property.first == L"border-width") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BorderThinkness = PropertyValue.PropertyAsInt;
							}
						}
					}
					if (Property.first == L"border-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->LineColor = PropertyValue.PropertyAsColorValue;
								Theme->CurrentLineColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"border-radius") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->Radius = { PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt };
							}
						}
					}
					if (Property.first == L"border") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::StringValue) {
								if (PropertyValue.PropertyAsString == L"solid") {
									Theme->BorderStyle = VPenStyle::SolidStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash") {
									Theme->BorderStyle = VPenStyle::DashStyle;
								}
								if (PropertyValue.PropertyAsString == L"dot") {
									Theme->BorderStyle = VPenStyle::DotStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash-dot-dot") {
									Theme->BorderStyle = VPenStyle::DashDotDotStyle;
								}
								if (PropertyValue.PropertyAsString == L"none") {
									Theme->BorderThinkness = 0.f;
								}
							}
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BorderThinkness = PropertyValue.PropertyAsInt;
							}
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->LineColor = PropertyValue.PropertyAsColorValue;
								Theme->CurrentLineColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}

					if (Property.first == L"background-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BackgroundColor = PropertyValue.PropertyAsColorValue;
								Theme->CurrentBackgroundColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"opacity") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::DoubleValue) {
								TargetControl->SetTransparency(PropertyValue.PropertyAsDouble * 255);
							}
						}
					}
				}
			}
			if (Selector->GetType() == VVSSSelectorType::FakeClassSelector &&
				static_cast<VVSSFakeClassSelector*>(Selector)->ElementTag == L"viconbutton" &&
				static_cast<VVSSFakeClassSelector*>(Selector)->ClassTag == L"hover") {
				for (auto& Property : Selector->SelectorProperty) {
					if (Property.first == L"border-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->OnHoverLineColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"background-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->OnHoverBackgroundColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
				}
			}
			if (Selector->GetType() == VVSSSelectorType::FakeClassSelector &&
				static_cast<VVSSFakeClassSelector*>(Selector)->ElementTag == L"viconbutton" &&
				static_cast<VVSSFakeClassSelector*>(Selector)->ClassTag == L"active") {
				for (auto& Property : Selector->SelectorProperty) {
					if (Property.first == L"border-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->OnClickedLineColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"background-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->OnClickedBackgroundColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
				}
			}
		}

		if (TargetControl != nullptr) { TargetControl->Update(); }
	}

public:
	VVSSVIconButtonBuilder(VIconButton* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet, VIconButtonTheme* IconButtonTheme) {
		BuildVSSObject(TargetControl, SelectorSet, IconButtonTheme);
	}
};
class VVSSVViewLabelBuilder {
private:
	void BuildVSSObject(VViewLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet, VViewLabelTheme* ViewLabelTheme) {
		VViewLabelTheme* Theme = ViewLabelTheme;
		if (TargetControl != nullptr) {
			Theme = TargetControl->GetTheme();
		}

		for (auto& Selector : SelectorSet) {
			if (Selector->GetType() == VVSSSelectorType::ElementSelector &&
				static_cast<VVSSElementSelector*>(Selector)->ElementTag == L"vviewlabel") {
				for (auto& Property : Selector->SelectorProperty) {
					if (Property.first == L"box-shadow") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BoxShadowPixel = PropertyValue.PropertyAsInt;
							}
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BoxShadowColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"box-shadow-color") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BoxShadowColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"box-shadow-radius") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BoxShadowPixel = PropertyValue.PropertyAsInt;
							}
						}
					}

					if (Property.first == L"border-radius") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->Radius = { PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt };
							}
						}
					}
					if (Property.first == L"background-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BackgroundColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"opacity") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::DoubleValue) {
								TargetControl->SetTransparency(PropertyValue.PropertyAsDouble * 255);
							}
						}
					}
				}
			}
		}

		if (TargetControl != nullptr) { TargetControl->Update(); }
	}

public:
	VVSSVViewLabelBuilder(VViewLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet, VViewLabelTheme* ViewLabelTheme) {
		BuildVSSObject(TargetControl, SelectorSet, ViewLabelTheme);
	}
};
class VVSSVLineEditorBuilder {
private:
	void BuildVSSObject(VLineEditor* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet, VLineEditorTheme* LineEditorTheme) {
		VLineEditorTheme* Theme = LineEditorTheme;
		if (TargetControl != nullptr) {
			Theme = TargetControl->GetTheme();
		}

		for (auto& Selector : SelectorSet) {
			if (Selector->GetType() == VVSSSelectorType::ElementSelector &&
				static_cast<VVSSElementSelector*>(Selector)->ElementTag == L"vlineeditor") {
				for (auto& Property : Selector->SelectorProperty) {
					if (Property.first == L"box-shadow") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BoxShadowPixel = PropertyValue.PropertyAsInt;
							}
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BoxShadowColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"box-shadow-color") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BoxShadowColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"box-shadow-radius") {
						Theme->EnableBoxShadow = true;

						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BoxShadowPixel = PropertyValue.PropertyAsInt;
							}
						}
					}
					if (Property.first == L"border-style") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::StringValue) {
								if (PropertyValue.PropertyAsString == L"solid") {
									Theme->BorderStyle = VPenStyle::SolidStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash") {
									Theme->BorderStyle = VPenStyle::DashStyle;
								}
								if (PropertyValue.PropertyAsString == L"dot") {
									Theme->BorderStyle = VPenStyle::DotStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash-dot-dot") {
									Theme->BorderStyle = VPenStyle::DashDotDotStyle;
								}
								if (PropertyValue.PropertyAsString == L"none") {
									Theme->BorderThinkness = 0.f;
								}
							}
						}
					}
					if (Property.first == L"border-width") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BorderThinkness = PropertyValue.PropertyAsInt;
							}
						}
					}
					if (Property.first == L"border-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->LineColor = PropertyValue.PropertyAsColorValue;
								Theme->CurrentLineColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"cursor-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->CursorColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"border-radius") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->Radius = { PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt };
							}
						}
					}
					if (Property.first == L"border") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::StringValue) {
								if (PropertyValue.PropertyAsString == L"solid") {
									Theme->BorderStyle = VPenStyle::SolidStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash") {
									Theme->BorderStyle = VPenStyle::DashStyle;
								}
								if (PropertyValue.PropertyAsString == L"dot") {
									Theme->BorderStyle = VPenStyle::DotStyle;
								}
								if (PropertyValue.PropertyAsString == L"dash-dot-dot") {
									Theme->BorderStyle = VPenStyle::DashDotDotStyle;
								}
								if (PropertyValue.PropertyAsString == L"none") {
									Theme->BorderThinkness = 0.f;
								}
							}
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->BorderThinkness = PropertyValue.PropertyAsInt;
							}
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->LineColor = PropertyValue.PropertyAsColorValue;
								Theme->CurrentLineColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}

					if (Property.first == L"text-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->TextColor = PropertyValue.PropertyAsColorValue;
								Theme->CurrentTextColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"background-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->BackgroundColor = PropertyValue.PropertyAsColorValue;
								Theme->CurrentBackgroundColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"opacity") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::DoubleValue) {
								TargetControl->SetTransparency(PropertyValue.PropertyAsDouble * 255);
							}
						}
					}

					if (Property.first == L"text-size") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::IntValue) {
								Theme->FontSize = PropertyValue.PropertyAsInt;
							}
						}
					}

					if (Property.first == L"font-family") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::StringValue) {
								delete Theme->FontFamily;

								Theme->FontFamily = new VFontFamily(PropertyValue.PropertyAsString);
							}
						}
					}
				}
			}
			if (Selector->GetType() == VVSSSelectorType::FakeClassSelector &&
				static_cast<VVSSFakeClassSelector*>(Selector)->ElementTag == L"vlineeditor" &&
				static_cast<VVSSFakeClassSelector*>(Selector)->ClassTag == L"hover") {
				for (auto& Property : Selector->SelectorProperty) {
					if (Property.first == L"border-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->OnHoverLineColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"background-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->OnHoverBackgroundColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
				}
			}
			if (Selector->GetType() == VVSSSelectorType::FakeClassSelector &&
				static_cast<VVSSFakeClassSelector*>(Selector)->ElementTag == L"vlineeditor" &&
				static_cast<VVSSFakeClassSelector*>(Selector)->ClassTag == L"active") {
				for (auto& Property : Selector->SelectorProperty) {
					if (Property.first == L"border-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->OnClickedLineColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
					if (Property.first == L"background-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->OnClickedBackgroundColor = PropertyValue.PropertyAsColorValue;
							}
						}
					}
				}
			}
		}

		if (TargetControl != nullptr) { TargetControl->Update(); }
	}

public:
	VVSSVLineEditorBuilder(VLineEditor* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet, VLineEditorTheme* LineEditorTheme) {
		BuildVSSObject(TargetControl, SelectorSet, LineEditorTheme);
	}
};
class VVSSVMainWindowBuilder {
private:
	void BuildVSSObject(VMainWindow* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet) {
		for (auto& Selector : SelectorSet) {
			if (Selector->GetType() == VVSSSelectorType::ElementSelector &&
				static_cast<VVSSElementSelector*>(Selector)->ElementTag == L"vmainwindow") {
				for (auto& Property : Selector->SelectorProperty) {
					if (Property.first == L"background-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								TargetControl->SetBackgroundColor(PropertyValue.PropertyAsColorValue);
							}
						}
					}
				}
			}
		}
	}

public:
	VVSSVMainWindowBuilder(VMainWindow* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet) {
		BuildVSSObject(TargetControl, SelectorSet);
	}
};

VLIB_END_NAMESPACE