#include "../../include/vss/vssbuilder.h"

VLIB_BEGIN_NAMESPACE

namespace VSS
{
Core::VFontAlignment VSSParserHelper::StringToAlignment(const VString &String)
{
	if (String == L"center")
	{
		return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER;
	}
	if (String == L"left")
	{
		return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_LEADING;
	}
	if (String == L"right")
	{
		return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_TRAILING;
	}
	if (String == L"justify")
	{
		return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
	}

	return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER;
}
Core::VFontParagraphAlignment VSSParserHelper::StringToParagraphAlignment(const VString &String)
{
	if (String == L"top")
	{
		return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
	}
	if (String == L"middle")
	{
		return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
	}
	if (String == L"bottom")
	{
		return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_FAR;
	}

	return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
}

void VSSVPushButtonBuilder::BuildVSSObject(Core::VPushButton			  *TargetControl,
										   std::vector<VSSBasicSelector *> SelectorSet,
										   Core::VPushButtonTheme		  *PushButtonTheme)
{
	Core::VPushButtonTheme *Theme = PushButtonTheme;

	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"pushbutton")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				// Common Property ( Not fade switch supported )
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}

				if (Property.first == L"font-weight")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->LabelFont->SetTextWidget(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"font-family")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetFamilyName(PropertyValue.PropertyAsString);
						}
					}
				}
				if (Property.first == L"font-size")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->LabelFont->SetTextSize(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"text-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetLineAlignment(
								VSSParserHelper::StringToAlignment(PropertyValue.PropertyAsString));
						}
					}
				}
				if (Property.first == L"vertical-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetParagraphAlignment(
								VSSParserHelper::StringToParagraphAlignment(PropertyValue.PropertyAsString));
						}
					}
				}

				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"pushbutton" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"hover")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"pushbutton" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"active")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
	}

	Theme->LocalTheme = Theme->StaticTheme;

	if (TargetControl != nullptr)
	{
		TargetControl->Update();
	}
}
VSSVPushButtonBuilder::VSSVPushButtonBuilder(Core::VPushButton				*TargetControl,
											 std::vector<VSSBasicSelector *> SelectorSet,
											 Core::VPushButtonTheme			*PushButtonTheme)
{
	BuildVSSObject(TargetControl, SelectorSet, PushButtonTheme);
}

void VSSDropDownContextBuilder::BuildVSSObject(Core::VDropDownContenxt		  *TargetControl,
											   std::vector<VSSBasicSelector *> SelectorSet,
											   Core::VDropDownContextTheme	  *PushButtonTheme)
{
	Core::VDropDownContextTheme *Theme = PushButtonTheme;

	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"drop-down-context")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				// Common Property ( Not fade switch supported )
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ShadowPixel = (double)PropertyValue.PropertyAsInt / 10 * 8;
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ShadowColor = PropertyValue.PropertyAsColorValue;
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}

				if (Property.first == L"item-font-weight")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ItemFont->SetTextWidget(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"item-font-family")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->ItemFont->SetFamilyName(PropertyValue.PropertyAsString);
						}
					}
				}
				if (Property.first == L"item-font-size")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ItemFont->SetTextSize(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"item-text-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->ItemFont->SetLineAlignment(
								VSSParserHelper::StringToAlignment(PropertyValue.PropertyAsString));
						}
					}
				}
				if (Property.first == L"item-vertical-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->ItemFont->SetParagraphAlignment(
								VSSParserHelper::StringToParagraphAlignment(PropertyValue.PropertyAsString));
						}
					}
				}

				if (Property.first == L"group-font-weight")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->GroupFont->SetTextWidget(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"group-font-family")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->GroupFont->SetFamilyName(PropertyValue.PropertyAsString);
						}
					}
				}
				if (Property.first == L"group-font-size")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->GroupFont->SetTextSize(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"group-text-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->GroupFont->SetLineAlignment(
								VSSParserHelper::StringToAlignment(PropertyValue.PropertyAsString));
						}
					}
				}
				if (Property.first == L"group-vertical-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->GroupFont->SetParagraphAlignment(
								VSSParserHelper::StringToParagraphAlignment(PropertyValue.PropertyAsString));
						}
					}
				}

				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ItemColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
	}
}
VSSDropDownContextBuilder::VSSDropDownContextBuilder(Core::VDropDownContenxt		*TargetControl,
													 std::vector<VSSBasicSelector *> SelectorSet,
													 Core::VDropDownContextTheme	*PushButtonTheme)
{
	BuildVSSObject(TargetControl, SelectorSet, PushButtonTheme);
}

void VSSDropDownViewBuilder::BuildVSSObject(Core::VDropDown *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
											Core::VDropDownTheme *PushButtonTheme)
{
	Core::VDropDownTheme *Theme = PushButtonTheme;

	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"drop-down")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				// Common Property ( Not fade switch supported )
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}

				if (Property.first == L"font-weight")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->LabelFont->SetTextWidget(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"font-family")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetFamilyName(PropertyValue.PropertyAsString);
						}
					}
				}
				if (Property.first == L"font-size")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->LabelFont->SetTextSize(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"text-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetLineAlignment(
								VSSParserHelper::StringToAlignment(PropertyValue.PropertyAsString));
						}
					}
				}
				if (Property.first == L"vertical-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetParagraphAlignment(
								VSSParserHelper::StringToParagraphAlignment(PropertyValue.PropertyAsString));
						}
					}
				}

				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"drop-down" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"hover")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"drop-down" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"active")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementSelector &&
			static_cast<VSSFakeElementSelector *>(Selector)->ElementTag == L"drop-down" &&
			static_cast<VSSFakeElementSelector *>(Selector)->FakeElementTag == L"context")
		{
			VSSElementSelector *GrooverSelector = new VSSElementSelector;
			GrooverSelector->ElementTag			= L"drop-down-context";
			GrooverSelector->SelectorProperty	= Selector->SelectorProperty;

			VSSDropDownContextBuilder GrooveBuilder(TargetControl->GetComplex(), {GrooverSelector}, nullptr);

			delete GrooverSelector;
		}
	}

	Theme->LocalTheme = Theme->StaticTheme;

	if (TargetControl != nullptr)
	{
		TargetControl->Update();
	}
}
VSSDropDownViewBuilder::VSSDropDownViewBuilder(Core::VDropDown				  *TargetControl,
											   std::vector<VSSBasicSelector *> SelectorSet,
											   Core::VDropDownTheme			  *PushButtonTheme)
{
	BuildVSSObject(TargetControl, SelectorSet, PushButtonTheme);
}

void VSSVRadioButtonBuilder::BuildVSSObject(Core::VRadioButton			   *TargetControl,
											std::vector<VSSBasicSelector *> SelectorSet,
											Core::VRadioButtonTheme		   *RadioButtonTheme)
{
	Core::VRadioButtonTheme *Theme = RadioButtonTheme;

	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
				static_cast<VSSElementSelector *>(Selector)->ElementTag == L"radiobutton" ||
			(Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			 static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"radiobutton" &&
			 static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"disabled"))
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				// Common Property ( Not fade switch supported )
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}
				if (Property.first == L"font-weight")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->LabelFont->SetTextWidget(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"font-family")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetFamilyName(PropertyValue.PropertyAsString);
						}
					}
				}
				if (Property.first == L"font-size")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->LabelFont->SetTextSize(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"text-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetLineAlignment(
								VSSParserHelper::StringToAlignment(PropertyValue.PropertyAsString));
						}
					}
				}
				if (Property.first == L"vertical-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetParagraphAlignment(
								VSSParserHelper::StringToParagraphAlignment(PropertyValue.PropertyAsString));
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}

				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"radiobutton" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"hover")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"radiobutton" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"enabled")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
	}

	Theme->LocalTheme = Theme->StaticTheme;

	if (TargetControl != nullptr)
	{
		TargetControl->Update();

		if (TargetControl->GetSwitchStatus())
		{
			Theme->LocalTheme = Theme->ActiveTheme;
		}
	}
}
VSSVRadioButtonBuilder::VSSVRadioButtonBuilder(Core::VRadioButton			  *TargetControl,
											   std::vector<VSSBasicSelector *> SelectorSet,
											   Core::VRadioButtonTheme		  *RadioButtonTheme)
{
	BuildVSSObject(TargetControl, SelectorSet, RadioButtonTheme);
}

void VSSVIconButtonBuilder::BuildVSSObject(Core::VIconButton			  *TargetControl,
										   std::vector<VSSBasicSelector *> SelectorSet,
										   Core::VIconButtonTheme		  *IconButtonTheme)
{
	Core::VIconButtonTheme *Theme = IconButtonTheme;

	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"iconbutton")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				// Common Property ( Not fade switch supported )
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}

				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"iconbutton" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"hover")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"iconbutton" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"active")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
	}

	Theme->LocalTheme = Theme->StaticTheme;

	if (TargetControl != nullptr)
	{
		TargetControl->Update();
	}
}
VSSVIconButtonBuilder::VSSVIconButtonBuilder(Core::VIconButton				*TargetControl,
											 std::vector<VSSBasicSelector *> SelectorSet,
											 Core::VIconButtonTheme			*IconButtonTheme)
{
	BuildVSSObject(TargetControl, SelectorSet, IconButtonTheme);
}

void VSSVTextLabelBuilder::BuildVSSObject(Core::VTextLabel *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
										  Core::VTextLabelTheme *TextLabelTheme)
{
	Core::VTextLabelTheme *Theme = TextLabelTheme;

	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"textlabel")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				// Common Property ( Not fade switch supported )
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}
				if (Property.first == L"font-family")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetFamilyName(PropertyValue.PropertyAsString);
						}
					}
				}
				if (Property.first == L"font-weight")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->LabelFont->SetTextWidget(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"font-size")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->LabelFont->SetTextSize(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"text-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetLineAlignment(
								VSSParserHelper::StringToAlignment(PropertyValue.PropertyAsString));
						}
					}
				}
				if (Property.first == L"vertical-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetParagraphAlignment(
								VSSParserHelper::StringToParagraphAlignment(PropertyValue.PropertyAsString));
						}
					}
				}

				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}

			Theme->LocalTheme	= Theme->StaticTheme;
			Theme->OnHoverTheme = Theme->StaticTheme;
			Theme->ActiveTheme	= Theme->StaticTheme;
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"textlabel" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"hover")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"textlabel" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"active")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
	}

	if (TargetControl != nullptr)
	{
		if (TargetControl->GetAutoSizeStatus())
		{
			TargetControl->ResizeByText();
		}

		TargetControl->Update();
	}
}
VSSVTextLabelBuilder::VSSVTextLabelBuilder(Core::VTextLabel *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
										   Core::VTextLabelTheme *Theme)
{
	BuildVSSObject(TargetControl, SelectorSet, Theme);
}
void VSSVImageLabelBuilder::BuildVSSObject(Core::VImageLabel			  *TargetControl,
										   std::vector<VSSBasicSelector *> SelectorSet,
										   Core::VImageLabelTheme		  *ImageLabelTheme)
{
	Core::VImageLabelTheme *Theme = ImageLabelTheme;
	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"imagelabel")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}
				if (Property.first == L"src")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->Image = new Core::VImage(PropertyValue.PropertyAsString,
															TargetControl->CallWidgetGetRenderHandle());
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->BorderRadius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}
			}
		}
	}

	if (TargetControl != nullptr)
	{
		TargetControl->Update();
	}
}
VSSVImageLabelBuilder::VSSVImageLabelBuilder(Core::VImageLabel				*TargetControl,
											 std::vector<VSSBasicSelector *> SelectorSet,
											 Core::VImageLabelTheme			*ImageLabelTheme)
{
	BuildVSSObject(TargetControl, SelectorSet, ImageLabelTheme);
}
void VSSVMainWindowBuilder::BuildVSSObject(Core::VMainWindow			  *TargetControl,
										   std::vector<VSSBasicSelector *> SelectorSet)
{
	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"mainwindow")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetBackgroundColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetRadius({PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt});
						}
					}
				}
			}
		}
	}
}
VSSVMainWindowBuilder::VSSVMainWindowBuilder(Core::VMainWindow				*TargetControl,
											 std::vector<VSSBasicSelector *> SelectorSet)
{
	BuildVSSObject(TargetControl, SelectorSet);
}

void VSSVCircleButtonBuilder::BuildVSSObject(Core::VCircleScrollBarButton	*TargetControl,
											 std::vector<VSSBasicSelector *> SelectorSet,
											 Core::VCircleScrollBarTheme	*CircleButtonTheme)
{
	Core::VCircleScrollBarTheme *Theme = CircleButtonTheme;

	if (TargetControl != nullptr)
	{
		Theme = (Core::VCircleScrollBarTheme *)TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"circlebarbutton")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				// Common Property ( Not fade switch supported )
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}

				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::ClassSelector &&
			static_cast<VSSClassSelector *>(Selector)->TargetElement == L"circlebarbutton" &&
			static_cast<VSSClassSelector *>(Selector)->ClassTag == L"hover")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::ClassSelector &&
			static_cast<VSSClassSelector *>(Selector)->TargetElement == L"circlebarbutton" &&
			static_cast<VSSClassSelector *>(Selector)->ClassTag == L"active")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
	}

	Theme->LocalTheme = Theme->StaticTheme;

	if (TargetControl != nullptr)
	{
		TargetControl->Update();
	}
}
VSSVCircleButtonBuilder::VSSVCircleButtonBuilder(Core::VCircleScrollBarButton	*TargetControl,
												 std::vector<VSSBasicSelector *> SelectorSet,
												 Core::VCircleScrollBarTheme	*RadioButtonTheme)
{
	BuildVSSObject(TargetControl, SelectorSet, RadioButtonTheme);
}

void VSSVSliderHorizontalBuilder::BuildVSSObject(Core::VSliderHorizontal		*TargetControl,
												 std::vector<VSSBasicSelector *> SelectorSet,
												 Core::VSliderTheme				*SliderTheme)
{
	Core::VSliderTheme *Theme = SliderTheme;

	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"horizontal-slider")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"horizontal-slider" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"unselected")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->UnselectedArea.LocalTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->UnselectedArea.LocalTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->UnselectedArea.LocalTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"horizontal-slider" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"selected")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->SelectedArea.LocalTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->SelectedArea.LocalTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->SelectedArea.LocalTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementSelector &&
			static_cast<VSSFakeElementSelector *>(Selector)->ElementTag == L"horizontal-slider" &&
			static_cast<VSSFakeElementSelector *>(Selector)->FakeElementTag == L"groove")
		{
			VSSElementSelector *GrooverSelector = new VSSElementSelector;
			GrooverSelector->ElementTag			= L"circlebarbutton";
			GrooverSelector->SelectorProperty	= Selector->SelectorProperty;

			VSSVCircleButtonBuilder GrooveBuilder(TargetControl->GetScrollBarButtonInstance(), {GrooverSelector},
												  nullptr);

			delete GrooverSelector;
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementWithClassSelector &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ElementTag == L"horizontal-slider" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->FakeElementTag == L"groove" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ClassTag == L"hover")
		{
			VSSClassSelector *GrooverSelector = new VSSClassSelector;
			GrooverSelector->TargetElement	  = L"circlebarbutton";
			GrooverSelector->ClassTag		  = L"hover";
			GrooverSelector->SelectorProperty = Selector->SelectorProperty;

			VSSVCircleButtonBuilder GrooveBuilder(TargetControl->GetScrollBarButtonInstance(), {GrooverSelector},
												  nullptr);

			delete GrooverSelector;
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementWithClassSelector &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ElementTag == L"horizontal-slider" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->FakeElementTag == L"groove" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ClassTag == L"active")
		{
			VSSClassSelector *GrooverSelector = new VSSClassSelector;
			GrooverSelector->TargetElement	  = L"circlebarbutton";
			GrooverSelector->ClassTag		  = L"active";
			GrooverSelector->SelectorProperty = Selector->SelectorProperty;

			VSSVCircleButtonBuilder GrooveBuilder(TargetControl->GetScrollBarButtonInstance(), {GrooverSelector},
												  nullptr);

			delete GrooverSelector;
		}
	}

	if (TargetControl != nullptr)
	{
		TargetControl->Update();
	}
}
VSSVSliderHorizontalBuilder::VSSVSliderHorizontalBuilder(Core::VSliderHorizontal		*TargetControl,
														 std::vector<VSSBasicSelector *> SelectorSet,
														 Core::VSliderTheme				*SliderTheme)
{
	BuildVSSObject(TargetControl, SelectorSet, SliderTheme);
}

void VSSVSliderVerticalBuilder::BuildVSSObject(Core::VSliderVertical		  *TargetControl,
											   std::vector<VSSBasicSelector *> SelectorSet,
											   Core::VSliderTheme			  *SliderTheme)
{
	Core::VSliderTheme *Theme = SliderTheme;

	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"vertical-slider")
		{
			// Common Property ( Not fade switch supported )
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"vertical-slider" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"unselected")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->UnselectedArea.LocalTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->UnselectedArea.LocalTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->UnselectedArea.LocalTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"vertical-slider" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"selected")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->SelectedArea.LocalTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->SelectedArea.LocalTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->SelectedArea.LocalTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementSelector &&
			static_cast<VSSFakeElementSelector *>(Selector)->ElementTag == L"vertical-slider" &&
			static_cast<VSSFakeElementSelector *>(Selector)->FakeElementTag == L"groove")
		{
			VSSElementSelector *GrooverSelector = new VSSElementSelector;
			GrooverSelector->ElementTag			= L"circlebarbutton";
			GrooverSelector->SelectorProperty	= Selector->SelectorProperty;

			VSSVCircleButtonBuilder GrooveBuilder(TargetControl->GetScrollBarButtonInstance(), {GrooverSelector},
												  nullptr);

			delete GrooverSelector;
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementWithClassSelector &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ElementTag == L"vertical-slider" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->FakeElementTag == L"groove" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ClassTag == L"hover")
		{
			VSSClassSelector *GrooverSelector = new VSSClassSelector;
			GrooverSelector->TargetElement	  = L"circlebarbutton";
			GrooverSelector->ClassTag		  = L"hover";
			GrooverSelector->SelectorProperty = Selector->SelectorProperty;

			VSSVCircleButtonBuilder GrooveBuilder(TargetControl->GetScrollBarButtonInstance(), {GrooverSelector},
												  nullptr);

			delete GrooverSelector;
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementWithClassSelector &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ElementTag == L"vertical-slider" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->FakeElementTag == L"groove" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ClassTag == L"active")
		{
			VSSClassSelector *GrooverSelector = new VSSClassSelector;
			GrooverSelector->TargetElement	  = L"circlebarbutton";
			GrooverSelector->ClassTag		  = L"active";
			GrooverSelector->SelectorProperty = Selector->SelectorProperty;

			VSSVCircleButtonBuilder GrooveBuilder(TargetControl->GetScrollBarButtonInstance(), {GrooverSelector},
												  nullptr);

			delete GrooverSelector;
		}
	}

	if (TargetControl != nullptr)
	{
		TargetControl->Update();
	}
}
VSSVSliderVerticalBuilder::VSSVSliderVerticalBuilder(Core::VSliderVertical			*TargetControl,
													 std::vector<VSSBasicSelector *> SelectorSet,
													 Core::VSliderTheme				*SliderTheme)
{
	BuildVSSObject(TargetControl, SelectorSet, SliderTheme);
}
void VSSVBlurLabelBuilder::BuildVSSObject(Core::VBlurLabel *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
										  Core::VBlurLabelTheme *BlurLabelTheme)
{
	Core::VBlurLabelTheme *Theme = BlurLabelTheme;

	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"blurlabel")
		{
			// Common Property ( Not fade switch supported )
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->MixedColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->BlurRadius = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->BorderRadius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}
			}
		}
	}

	if (TargetControl != nullptr)
	{
		TargetControl->Update();
	}
}
VSSVBlurLabelBuilder::VSSVBlurLabelBuilder(Core::VBlurLabel *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
										   Core::VBlurLabelTheme *Theme)
{
	BuildVSSObject(TargetControl, SelectorSet, Theme);
}

void VSSVLineEditorBuilder::BuildVSSObject(Core::VLineEditor			  *TargetControl,
										   std::vector<VSSBasicSelector *> SelectorSet,
										   Core::VTextEditorTheme		  *TextEditorTheme)
{
	Core::VTextEditorTheme *Theme = TextEditorTheme;

	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"lineeditor")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				// Common Property ( Not fade switch supported )
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}
				if (Property.first == L"font-family")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetFamilyName(PropertyValue.PropertyAsString);
						}
					}
				}
				if (Property.first == L"font-size")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->LabelFont->SetTextSize(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"vertical-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetParagraphAlignment(
								VSSParserHelper::StringToParagraphAlignment(PropertyValue.PropertyAsString));
						}
					}
				}

				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"caret-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->CursorColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"caret-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnSelectedColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"lineeditor" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"hover")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"lineeditor" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"active")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementSelector &&
			static_cast<VSSFakeElementSelector *>(Selector)->ElementTag == L"lineeditor" &&
			static_cast<VSSFakeElementSelector *>(Selector)->FakeElementTag == L"selection")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnSelectedColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
			}
		}
	}

	Theme->LocalTheme = Theme->StaticTheme;

	if (TargetControl != nullptr)
	{
		TargetControl->Update();
	}
}
VSSVLineEditorBuilder::VSSVLineEditorBuilder(Core::VLineEditor				*TargetControl,
											 std::vector<VSSBasicSelector *> SelectorSet,
											 Core::VTextEditorTheme			*TextEditorTheme)
{
	BuildVSSObject(TargetControl, SelectorSet, TextEditorTheme);
}

void VSSVEditorBuilder::BuildVSSObject(Core::VEditor *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
									   Core::VTextEditorTheme *TextEditorTheme)
{
	Core::VTextEditorTheme *Theme = TextEditorTheme;

	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"editor")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				// Common Property ( Not fade switch supported )
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}
				if (Property.first == L"font-family")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetFamilyName(PropertyValue.PropertyAsString);
						}
					}
				}
				if (Property.first == L"font-size")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->LabelFont->SetTextSize(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"font-weight")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->LabelFont->SetTextWidget(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"text-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetLineAlignment(
								VSSParserHelper::StringToAlignment(PropertyValue.PropertyAsString));
						}
					}
				}

				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"caret-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->CursorColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"caret-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnSelectedColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"editor" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"hover")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"editor" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"active")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementSelector &&
			static_cast<VSSFakeElementSelector *>(Selector)->ElementTag == L"editor" &&
			static_cast<VSSFakeElementSelector *>(Selector)->FakeElementTag == L"selection")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnSelectedColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnSelectedBackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementSelector &&
			static_cast<VSSFakeElementSelector *>(Selector)->ElementTag == L"editor" &&
			static_cast<VSSFakeElementSelector *>(Selector)->FakeElementTag == L"placeholder")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->PlaceHolderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
			}
		}
	}

	Theme->LocalTheme = Theme->StaticTheme;

	if (TargetControl != nullptr)
	{
		TargetControl->ResetTextLayout();
		TargetControl->Update();
	}
}
VSSVEditorBuilder::VSSVEditorBuilder(Core::VEditor *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
									 Core::VTextEditorTheme *TextEditorTheme)
{
	BuildVSSObject(TargetControl, SelectorSet, TextEditorTheme);
}

void VSSVerticalScrollerBuilder::BuildVSSObject(Core::VScrollerVertical		   *TargetControl,
												std::vector<VSSBasicSelector *> SelectorSet,
												Core::VViewScrollerTheme	   *ScrollerTheme)
{
	Core::VViewScrollerTheme *Theme = ScrollerTheme;

	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"vertical-scroller")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				// Common Property ( Not fade switch supported )
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}
				if (Property.first == L"font-family")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetFamilyName(PropertyValue.PropertyAsString);
						}
					}
				}
				if (Property.first == L"font-size")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->LabelFont->SetTextSize(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"text-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetLineAlignment(
								VSSParserHelper::StringToAlignment(PropertyValue.PropertyAsString));
						}
					}
				}
				if (Property.first == L"vertical-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetParagraphAlignment(
								VSSParserHelper::StringToParagraphAlignment(PropertyValue.PropertyAsString));
						}
					}
				}

				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"vertical-scroller" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"hover")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"vertical-scroller" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"active")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}

		if (Selector->GetType() == VSSSelectorType::FakeElementSelector &&
			static_cast<VSSFakeElementSelector *>(Selector)->ElementTag == L"vertical-scroller" &&
			static_cast<VSSFakeElementSelector *>(Selector)->FakeElementTag == L"groove")
		{
			VSSElementSelector *GrooverSelector = new VSSElementSelector;
			GrooverSelector->ElementTag			= L"scroller-button";
			GrooverSelector->SelectorProperty	= Selector->SelectorProperty;

			VSSScrollerButtonBuilder GrooveBuilder(TargetControl->GetScrollerButtonInstance(), {GrooverSelector},
												   nullptr);

			delete GrooverSelector;
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementWithClassSelector &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ElementTag == L"vertical-scroller" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->FakeElementTag == L"groove" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ClassTag == L"hover")
		{
			VSSClassSelector *GrooverSelector = new VSSClassSelector;
			GrooverSelector->TargetElement	  = L"scroller-button";
			GrooverSelector->ClassTag		  = L"hover";
			GrooverSelector->SelectorProperty = Selector->SelectorProperty;

			VSSScrollerButtonBuilder GrooveBuilder(TargetControl->GetScrollerButtonInstance(), {GrooverSelector},
												   nullptr);

			delete GrooverSelector;
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementWithClassSelector &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ElementTag == L"vertical-scroller" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->FakeElementTag == L"groove" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ClassTag == L"active")
		{
			VSSClassSelector *GrooverSelector = new VSSClassSelector;
			GrooverSelector->TargetElement	  = L"scroller-button";
			GrooverSelector->ClassTag		  = L"active";
			GrooverSelector->SelectorProperty = Selector->SelectorProperty;

			VSSScrollerButtonBuilder GrooveBuilder(TargetControl->GetScrollerButtonInstance(), {GrooverSelector},
												   nullptr);

			delete GrooverSelector;
		}
	}

	Theme->LocalTheme = Theme->StaticTheme;

	if (TargetControl != nullptr)
	{
		TargetControl->Update();
	}
}
VSSVerticalScrollerBuilder::VSSVerticalScrollerBuilder(Core::VScrollerVertical		  *TargetControl,
													   std::vector<VSSBasicSelector *> SelectorSet,
													   Core::VViewScrollerTheme		  *ScrollerTheme)
{
	BuildVSSObject(TargetControl, SelectorSet, ScrollerTheme);
}

void VSSHorizontalScrollerBuilder::BuildVSSObject(Core::VScrollerHorizontal		 *TargetControl,
												  std::vector<VSSBasicSelector *> SelectorSet,
												  Core::VViewScrollerTheme		 *ScrollerTheme)
{
	Core::VViewScrollerTheme *Theme = ScrollerTheme;

	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"horizontal-scroller")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				// Common Property ( Not fade switch supported )
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}
				if (Property.first == L"font-family")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetFamilyName(PropertyValue.PropertyAsString);
						}
					}
				}
				if (Property.first == L"font-size")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->LabelFont->SetTextSize(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"text-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetLineAlignment(
								VSSParserHelper::StringToAlignment(PropertyValue.PropertyAsString));
						}
					}
				}
				if (Property.first == L"vertical-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetParagraphAlignment(
								VSSParserHelper::StringToParagraphAlignment(PropertyValue.PropertyAsString));
						}
					}
				}

				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"horizontal-scroller" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"hover")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"horizontal-scroller" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"active")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementSelector &&
			static_cast<VSSFakeElementSelector *>(Selector)->ElementTag == L"horizontal-scroller" &&
			static_cast<VSSFakeElementSelector *>(Selector)->FakeElementTag == L"groove")
		{
			VSSElementSelector *GrooverSelector = new VSSElementSelector;
			GrooverSelector->ElementTag			= L"scroller-button";
			GrooverSelector->SelectorProperty	= Selector->SelectorProperty;

			VSSScrollerButtonBuilder GrooveBuilder(TargetControl->GetScrollerButtonInstance(), {GrooverSelector},
												   nullptr);

			delete GrooverSelector;
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementWithClassSelector &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ElementTag == L"horizontal-scroller" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->FakeElementTag == L"groove" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ClassTag == L"hover")
		{
			VSSClassSelector *GrooverSelector = new VSSClassSelector;
			GrooverSelector->TargetElement	  = L"scroller-button";
			GrooverSelector->ClassTag		  = L"hover";
			GrooverSelector->SelectorProperty = Selector->SelectorProperty;

			VSSScrollerButtonBuilder GrooveBuilder(TargetControl->GetScrollerButtonInstance(), {GrooverSelector},
												   nullptr);

			delete GrooverSelector;
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementWithClassSelector &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ElementTag == L"horizontal-scroller" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->FakeElementTag == L"groove" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ClassTag == L"active")
		{
			VSSClassSelector *GrooverSelector = new VSSClassSelector;
			GrooverSelector->TargetElement	  = L"scroller-button";
			GrooverSelector->ClassTag		  = L"active";
			GrooverSelector->SelectorProperty = Selector->SelectorProperty;

			VSSScrollerButtonBuilder GrooveBuilder(TargetControl->GetScrollerButtonInstance(), {GrooverSelector},
												   nullptr);

			delete GrooverSelector;
		}
	}

	Theme->LocalTheme = Theme->StaticTheme;

	if (TargetControl != nullptr)
	{
		TargetControl->Update();
	}
}
VSSHorizontalScrollerBuilder::VSSHorizontalScrollerBuilder(Core::VScrollerHorizontal	  *TargetControl,
														   std::vector<VSSBasicSelector *> SelectorSet,
														   Core::VViewScrollerTheme		  *ScrollerTheme)
{
	BuildVSSObject(TargetControl, SelectorSet, ScrollerTheme);
}

void VSSScrollerButtonBuilder::BuildVSSObject(Core::VScrollerPushButton		 *TargetControl,
											  std::vector<VSSBasicSelector *> SelectorSet,
											  Core::VViewScrollerButtonTheme *ScrollerTheme)
{
	Core::VViewScrollerButtonTheme *Theme = ScrollerTheme;

	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"scroller-button")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				// Common Property ( Not fade switch supported )
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}
				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}
				if (Property.first == L"font-family")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetFamilyName(PropertyValue.PropertyAsString);
						}
					}
				}
				if (Property.first == L"font-size")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->LabelFont->SetTextSize(PropertyValue.PropertyAsInt);
						}
					}
				}
				if (Property.first == L"text-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetLineAlignment(
								VSSParserHelper::StringToAlignment(PropertyValue.PropertyAsString));
						}
					}
				}
				if (Property.first == L"vertical-align")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::StringValue)
						{
							Theme->LabelFont->SetParagraphAlignment(
								VSSParserHelper::StringToParagraphAlignment(PropertyValue.PropertyAsString));
						}
					}
				}

				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->StaticTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->StaticTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"scroller-button" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"hover")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->OnHoverTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->OnHoverTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
		if (Selector->GetType() == VSSSelectorType::FakeClassSelector &&
			static_cast<VSSFakeClassSelector *>(Selector)->ElementTag == L"scroller-button" &&
			static_cast<VSSFakeClassSelector *>(Selector)->ClassTag == L"active")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.TextColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}

				if (Property.first == L"border-width")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.Radius = {PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt};
						}
					}
				}

				if (Property.first == L"border")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->ActiveTheme.BorderColor = PropertyValue.PropertyAsColorValue;
						}
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							Theme->ActiveTheme.BorderThickness = PropertyValue.PropertyAsInt;
						}
					}
				}
			}
		}
	}

	Theme->LocalTheme = Theme->StaticTheme;

	if (TargetControl != nullptr)
	{
		TargetControl->Update();
	}
}

VSSScrollerButtonBuilder::VSSScrollerButtonBuilder(Core::VScrollerPushButton	  *TargetControl,
												   std::vector<VSSBasicSelector *> SelectorSet,
												   Core::VViewScrollerButtonTheme *ScrollerTheme)
{
	BuildVSSObject(TargetControl, SelectorSet, ScrollerTheme);
}

void VSSViewLabelBuilder::BuildVSSObject(Core::VViewLabel *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
										 Core::VViewLabelTheme *ViewLabelTheme)
{
	Core::VViewLabelTheme *Theme = ViewLabelTheme;

	if (TargetControl != nullptr)
	{
		Theme = TargetControl->GetTheme();
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"viewlabel")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				// Common Property ( Not fade switch supported )
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}

				if (Property.first == L"opacity")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::DoubleValue)
						{
							TargetControl->SetTransparency(PropertyValue.PropertyAsDouble);
						}
					}
				}

				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->BackgroundColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
				if (Property.first == L"border-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							Theme->BorderColor = PropertyValue.PropertyAsColorValue;
						}
					}
				}
			}
		}

		if (Selector->GetType() == VSSSelectorType::FakeElementSelector &&
			static_cast<VSSFakeElementSelector *>(Selector)->ElementTag == L"viewlabel" &&
			static_cast<VSSFakeElementSelector *>(Selector)->FakeElementTag == L"groove")
		{
			VSSElementSelector *GrooverSelector = new VSSElementSelector;
			GrooverSelector->ElementTag			= L"scroller-button";
			GrooverSelector->SelectorProperty	= Selector->SelectorProperty;

			VSSScrollerButtonBuilder VerticalGrooveBuilder(
				TargetControl->GetVerticalScrollerInstance()->GetScrollerButtonInstance(), {GrooverSelector}, nullptr);
			VSSScrollerButtonBuilder HorizontalGrooveBuilder(
				TargetControl->GetHorizontalScrollerInstance()->GetScrollerButtonInstance(), {GrooverSelector},
				nullptr);

			delete GrooverSelector;
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementWithClassSelector &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ElementTag == L"viewlabel" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->FakeElementTag == L"groove" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ClassTag == L"hover")
		{
			VSSClassSelector *GrooverSelector = new VSSClassSelector;
			GrooverSelector->TargetElement	  = L"scroller-button";
			GrooverSelector->ClassTag		  = L"hover";
			GrooverSelector->SelectorProperty = Selector->SelectorProperty;

			VSSScrollerButtonBuilder VerticalGrooveBuilder(
				TargetControl->GetVerticalScrollerInstance()->GetScrollerButtonInstance(), {GrooverSelector}, nullptr);
			VSSScrollerButtonBuilder HorizontalGrooveBuilder(
				TargetControl->GetHorizontalScrollerInstance()->GetScrollerButtonInstance(), {GrooverSelector},
				nullptr);

			delete GrooverSelector;
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementWithClassSelector &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ElementTag == L"viewlabel" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->FakeElementTag == L"groove" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ClassTag == L"active")
		{
			VSSClassSelector *GrooverSelector = new VSSClassSelector;
			GrooverSelector->TargetElement	  = L"scroller-button";
			GrooverSelector->ClassTag		  = L"active";
			GrooverSelector->SelectorProperty = Selector->SelectorProperty;

			VSSScrollerButtonBuilder VerticalGrooveBuilder(
				TargetControl->GetVerticalScrollerInstance()->GetScrollerButtonInstance(), {GrooverSelector}, nullptr);
			VSSScrollerButtonBuilder HorizontalGrooveBuilder(
				TargetControl->GetHorizontalScrollerInstance()->GetScrollerButtonInstance(), {GrooverSelector},
				nullptr);

			delete GrooverSelector;
		}

		if (Selector->GetType() == VSSSelectorType::FakeElementSelector &&
			static_cast<VSSFakeElementSelector *>(Selector)->ElementTag == L"viewlabel" &&
			static_cast<VSSFakeElementSelector *>(Selector)->FakeElementTag == L"scroller")
		{
			VSSElementSelector *GrooverSelector = new VSSElementSelector;
			GrooverSelector->ElementTag			= L"scroller-button";
			GrooverSelector->SelectorProperty	= Selector->SelectorProperty;

			VSSScrollerButtonBuilder VerticalGrooveBuilder(
				TargetControl->GetVerticalScrollerInstance()->GetScrollerButtonInstance(), {GrooverSelector}, nullptr);
			VSSScrollerButtonBuilder HorizontalGrooveBuilder(
				TargetControl->GetVerticalScrollerInstance()->GetScrollerButtonInstance(), {GrooverSelector}, nullptr);

			delete GrooverSelector;
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementWithClassSelector &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ElementTag == L"viewlabel" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->FakeElementTag == L"scroller" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ClassTag == L"hover")
		{
			VSSClassSelector *GrooverSelector = new VSSClassSelector;
			GrooverSelector->TargetElement	  = L"scroller-button";
			GrooverSelector->ClassTag		  = L"hover";
			GrooverSelector->SelectorProperty = Selector->SelectorProperty;

			VSSVerticalScrollerBuilder	 VerticalGrooveBuilder(TargetControl->GetVerticalScrollerInstance(),
															   {GrooverSelector}, nullptr);
			VSSHorizontalScrollerBuilder HorizontalGrooveBuilder(TargetControl->GetHorizontalScrollerInstance(),
																 {GrooverSelector}, nullptr);

			delete GrooverSelector;
		}
		if (Selector->GetType() == VSSSelectorType::FakeElementWithClassSelector &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ElementTag == L"viewlabel" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->FakeElementTag == L"scroller" &&
			static_cast<VSSFakeElementWithClassSelector *>(Selector)->ClassTag == L"active")
		{
			VSSClassSelector *GrooverSelector = new VSSClassSelector;
			GrooverSelector->TargetElement	  = L"scroller-button";
			GrooverSelector->ClassTag		  = L"active";
			GrooverSelector->SelectorProperty = Selector->SelectorProperty;

			VSSVerticalScrollerBuilder	 VerticalGrooveBuilder(TargetControl->GetVerticalScrollerInstance(),
															   {GrooverSelector}, nullptr);
			VSSHorizontalScrollerBuilder HorizontalGrooveBuilder(TargetControl->GetHorizontalScrollerInstance(),
																 {GrooverSelector}, nullptr);

			delete GrooverSelector;
		}
	}

	if (TargetControl != nullptr)
	{
		TargetControl->Update(TargetControl->GetRegion());
	}
}
VSSViewLabelBuilder::VSSViewLabelBuilder(Core::VViewLabel *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
										 Core::VViewLabelTheme *ViewLabelTheme)
{
	BuildVSSObject(TargetControl, SelectorSet, ViewLabelTheme);
}

void VSSCircleViewBuilder::BuildVSSObject(Core::VCircleView *TargetControl, std::vector<VSSBasicSelector *> SelectorSet)
{
	if (TargetControl == nullptr)
	{
		return;
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"circle-view")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				// Common Property ( Not fade switch supported )
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}
			}
		}
	}

	if (TargetControl != nullptr)
	{
		TargetControl->Update(TargetControl->GetRegion());
	}
}

VSSCircleViewBuilder::VSSCircleViewBuilder(Core::VCircleView			  *TargetControl,
										   std::vector<VSSBasicSelector *> SelectorSet)
{
	BuildVSSObject(TargetControl, SelectorSet);
}

void VSSPolygonViewBuilder::BuildVSSObject(Core::VPolygonView			  *TargetControl,
										   std::vector<VSSBasicSelector *> SelectorSet)
{
	if (TargetControl == nullptr)
	{
		return;
	}

	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"polygon-view")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				// Common Property ( Not fade switch supported )
				if (Property.first == L"box-shadow")
				{
					TargetControl->SetShadowStats(true);

					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetShadowRadius((double)PropertyValue.PropertyAsInt / 10 * 8);
						}
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetShadowColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}
			}
		}
	}

	if (TargetControl != nullptr)
	{
		TargetControl->Update(TargetControl->GetRegion());
	}
}
VSSPolygonViewBuilder::VSSPolygonViewBuilder(Core::VPolygonView				*TargetControl,
											 std::vector<VSSBasicSelector *> SelectorSet)
{
	BuildVSSObject(TargetControl, SelectorSet);
}

void VSSVWidgetBuilder::BuildVSSObject(Core::VWidget *TargetControl, std::vector<VSSBasicSelector *> SelectorSet)
{
	for (auto &Selector : SelectorSet)
	{
		if (Selector->GetType() == VSSSelectorType::ElementSelector &&
			static_cast<VSSElementSelector *>(Selector)->ElementTag == L"widget")
		{
			for (auto &Property : Selector->SelectorProperty)
			{
				if (Property.first == L"background-color")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::ColorValue)
						{
							TargetControl->SetBackgroundColor(PropertyValue.PropertyAsColorValue);
						}
					}
				}
				if (Property.first == L"border-radius")
				{
					for (auto &PropertyValue : Property.second.ValueList)
					{
						if (PropertyValue.Type == VSSPropertyType::IntValue)
						{
							TargetControl->SetRadius({PropertyValue.PropertyAsInt, PropertyValue.PropertyAsInt});
						}
					}
				}
			}
		}
	}
}
VSSVWidgetBuilder::VSSVWidgetBuilder(Core::VWidget *TargetControl, std::vector<VSSBasicSelector *> SelectorSet)
{
	BuildVSSObject(TargetControl, SelectorSet);
}
} // namespace VSS

VLIB_END_NAMESPACE