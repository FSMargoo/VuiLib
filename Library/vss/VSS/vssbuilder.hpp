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
		std::vector<VVSSBasicSelector*> SelectorSet) {
		VPushButtonTheme* Theme = new VPushButtonTheme;

		Theme->PlaneString = TargetControl->GetPlaneText();

		for (auto& Selector : SelectorSet) {
			if (Selector->GetType() == VVSSSelectorType::ElementSelector &&
				static_cast<VVSSElementSelector*>(Selector)->ElementTag == L"vpushbutton") {
				for (auto& Property : Selector->SelectorProperty) {
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
				static_cast<VVSSFakeClassSelector*>(Selector)->ClassTag   == L"hover") {
				for (auto& Property : Selector->SelectorProperty) {
					if (Property.first == L"border-color") {
						for (auto& PropertyValue : Property.second.ValueList) {
							if (PropertyValue.Type == VVSSPropertyType::ColorValue) {
								Theme->OnHoverLineColor        = PropertyValue.PropertyAsColorValue;
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
								Theme->OnHoverBackgroundColor    = PropertyValue.PropertyAsColorValue;
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

		TargetControl->SetTheme(Theme);

		delete Theme;
	}

public:
	VVSSVPushButtonBuilder(VPushButton* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet) {
		BuildVSSObject(TargetControl, SelectorSet);
	}
};
class VVSSVTextLabelBuilder {
private:
	void BuildVSSObject(VTextLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet) {
		VTextLabelTheme* Theme = new VTextLabelTheme;

		Theme->PlaneString = TargetControl->GetPlaneText();

		for (auto& Selector : SelectorSet) {
			if (Selector->GetType() == VVSSSelectorType::ElementSelector &&
				static_cast<VVSSElementSelector*>(Selector)->ElementTag == L"vtextlabel") {
				for (auto& Property : Selector->SelectorProperty) {
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

		TargetControl->SetTheme(Theme);

		delete Theme;
	}

public:
	VVSSVTextLabelBuilder(VTextLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet) {
		BuildVSSObject(TargetControl, SelectorSet);
	}
};
class VVSSVImageLabelBuilder {
private:
	void BuildVSSObject(VImageLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet) {
		VImageLabelTheme* Theme = new VImageLabelTheme;

		Theme->Image = TargetControl->GetImage();

		for (auto& Selector : SelectorSet) {
			if (Selector->GetType() == VVSSSelectorType::ElementSelector &&
				static_cast<VVSSElementSelector*>(Selector)->ElementTag == L"vimagelabel") {
				for (auto& Property : Selector->SelectorProperty) {
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

		TargetControl->SetTheme(Theme);

		delete Theme;
	}

public:
	VVSSVImageLabelBuilder(VImageLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet) {
		BuildVSSObject(TargetControl, SelectorSet);
	}
};
class VVSSBlurLabelBuilder {
private:
	void BuildVSSObject(VBlurLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet) {
		VBlurLabelTheme* Theme = new VBlurLabelTheme;

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

		TargetControl->SetTheme(Theme);

		delete Theme;
	}

public:
	VVSSBlurLabelBuilder(VBlurLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet) {
		BuildVSSObject(TargetControl, SelectorSet);
	}
};
class VVSSVIconButtonBuilder {
private:
	void BuildVSSObject(VIconButton* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet) {
		VIconButtonTheme* Theme = new VIconButtonTheme;

		Theme->IconImage = TargetControl->GetIcon();

		for (auto& Selector : SelectorSet) {
			if (Selector->GetType() == VVSSSelectorType::ElementSelector &&
				static_cast<VVSSElementSelector*>(Selector)->ElementTag == L"viconbutton") {
				for (auto& Property : Selector->SelectorProperty) {
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

		TargetControl->SetTheme(Theme);

		delete Theme;
	}

public:
	VVSSVIconButtonBuilder(VIconButton* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet) {
		BuildVSSObject(TargetControl, SelectorSet);
	}
};
class VVSSVViewLabelBuilder {
private:
	void BuildVSSObject(VViewLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet) {
		VViewLabelTheme* Theme = new VViewLabelTheme;

		for (auto& Selector : SelectorSet) {
			if (Selector->GetType() == VVSSSelectorType::ElementSelector &&
				static_cast<VVSSElementSelector*>(Selector)->ElementTag == L"vviewlabel") {
				for (auto& Property : Selector->SelectorProperty) {
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

		TargetControl->SetTheme(Theme);

		delete Theme;
	}

public:
	VVSSVViewLabelBuilder(VViewLabel* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet) {
		BuildVSSObject(TargetControl, SelectorSet);
	}
};
class VVSSVLineEditorBuilder {
private:
	void BuildVSSObject(VLineEditor* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet) {
		VLineEditorTheme* Theme = new VLineEditorTheme;

		for (auto& Selector : SelectorSet) {
			if (Selector->GetType() == VVSSSelectorType::ElementSelector &&
				static_cast<VVSSElementSelector*>(Selector)->ElementTag == L"vlineeditor") {
				for (auto& Property : Selector->SelectorProperty) {
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

		TargetControl->SetTheme(Theme);

		delete Theme;
	}

public:
	VVSSVLineEditorBuilder(VLineEditor* TargetControl,
		std::vector<VVSSBasicSelector*> SelectorSet) {
		BuildVSSObject(TargetControl, SelectorSet);
	}

};

VLIB_END_NAMESPACE