#include "../../include/vml/vmlbuilder.h"

VLIB_BEGIN_NAMESPACE

namespace VML
{
bool VMLCommonBuilder::CheckNativeCallParameter(std::vector<VMLPropertyValue> Properties,
												std::vector<VMLPropertyType>  PropertiesType)
{
	if (Properties.size() != PropertiesType.size())
	{
		return false;
	}

	for (int PropertiesCount = 0; PropertiesCount != Properties.size(); ++PropertiesCount)
	{
		if (Properties[PropertiesCount].PropertyType != PropertiesType[PropertiesCount])
		{
			return false;
		}
	}

	return true;
}
void VMLCommonBuilder::Builder(Core::VUIObject *Object, const int &X, const int &Y, const int &Width, const int &Height,
							   const int &Transparency, const bool &Visble, const Core::VLayoutMode &VerticalLayoutMode,
							   const Core::VLayoutMode &HorizontalLayoutMode, const double &VerticalLayoutPercent,
							   const double &HorizontalLayoutPercent, const int &RelativeX, const int &RelativeY,
							   const int &XMiddleOffset, const int &YMiddleOffset, const double &WidthRatio,
							   const double &HeightRatio, const bool &ViewOnly, const VMLFinder &ObjectFinder)
{
	Core::VLayout *_NativeLayout = new Core::VLayout(Object, Object->GetParent());
	_NativeLayout->SetHorizontalLayoutMode(HorizontalLayoutMode);
	_NativeLayout->SetVerticalLayoutMode(VerticalLayoutMode);
	_NativeLayout->SetVerticalLayoutPercent(VerticalLayoutPercent);
	_NativeLayout->SetHorizontalLayoutPercent(HorizontalLayoutPercent);
	_NativeLayout->SetRelativeX(RelativeX);
	_NativeLayout->SetRelativeY(RelativeY);
	_NativeLayout->SetXMiddleOffset(XMiddleOffset);
	_NativeLayout->SetYMiddleOffset(YMiddleOffset);
	Core::VScaleLayout *_NativeScaleLayout = new Core::VScaleLayout(Object, Object->GetParent());
	_NativeScaleLayout->SetWidthScalePercent(WidthRatio);
	_NativeScaleLayout->SetHeightScalePercent(HeightRatio);
	_NativeScaleLayout->SetHeightScalePercent(HeightRatio);

	if (!Object->IsWidget())
	{
		Object->Move(X, Y);
	}

	Object->Resize(Width, Height);
	Object->SetTransparency(Transparency);

	if (!Visble)
	{
		Object->Hide();
	}

	Object->SetViewOnly(ViewOnly);
}

void VMLCommonBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VUIObject *Object,
									   std::map<VString, VMLPropertyValue> &PropertyValueList,
									   VMLControlBuildStatus			   *BuildStatus)
{
	int	  X			   = 0;
	int	  Y			   = 0;
	int	  Height	   = 0;
	int	  Width		   = 0;
	float Transparency = 1.f;
	bool  Visble	   = true;

	Core::VLayoutMode VerticalLayoutMode   = Core::VLayoutMode::LayoutModeDontUse;
	Core::VLayoutMode HorizontalLayoutMode = Core::VLayoutMode::LayoutModeDontUse;

	Core::VUIObject *LayoutParent = nullptr;

	double VerticalLayoutPercent   = 0.f;
	double HorizontalLayoutPercent = 0.f;

	int RelativeX = 0;
	int RelativeY = 0;

	int XMiddleOffset = 0;
	int YMiddleOffset = 0;

	double WidthRatio  = 0.f;
	double HeightRatio = 0.f;

	bool ViewOnly = false;

	BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Ok;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"view-only")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"view-only\" Property must match the type \"bolean\"";

				return;
			}

			ViewOnly = ElementProperty.second.PropertyAsBool;
		}
		if (ElementProperty.first == L"x")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue &&
				ElementProperty.second.PropertyType != VMLPropertyType::NativeCall &&
				ElementProperty.second.PropertyType != VMLPropertyType::VariableCall &&
				ElementProperty.second.PropertyType != VMLPropertyType::VariableDefine)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"x\" Property must match the type \"int\" or \"native call "
											L"functional\"";

				return;
			}

			if (ElementProperty.second.PropertyType == VMLPropertyType::IntValue)
			{
				X = ElementProperty.second.PropertyAsInt;
			}
			else if (ElementProperty.second.PropertyType == VMLPropertyType::NativeCall)
			{
				if (ElementProperty.second.NativeCallMethodName == L"center")
				{
					HorizontalLayoutMode = Core::VLayoutMode::LayoutModeCenter;
				}
				if (ElementProperty.second.NativeCallMethodName == L"centeroffset")
				{
					HorizontalLayoutMode = Core::VLayoutMode::LayoutModeMiddleOffset;

					if (ElementProperty.second.NativeCallParameter.size() == 1)
					{
						if (CheckNativeCallParameter(ElementProperty.second.NativeCallParameter,
													 {VMLPropertyType::IntValue}))
						{
							XMiddleOffset = ElementProperty.second.NativeCallParameter[0].PropertyAsInt;
						}
						if (ElementProperty.second.PropertyType == VMLPropertyType::VariableDefine)
						{
							if (ElementProperty.second.VariableType == VMLVariableType::IntType)
							{
								if (!ElementProperty.second.VariableInitValue.empty())
								{
									auto VariablePointer = DefineVariable<int, VMLIntVariable>(
										Object, ElementProperty.second.VariableInitValue[0].PropertyAsInt, RootFinder,
										ElementProperty.second.NativeCallMethodName);
									XMiddleOffset = ElementProperty.second.VariableInitValue[0].PropertyAsInt;
								}
								else
								{
									DefineVariable<int, VMLIntVariable>(Object, 0, RootFinder,
																		ElementProperty.second.NativeCallMethodName);
									XMiddleOffset = 0;
								}
							}
						}
					}
				}
				if (ElementProperty.second.NativeCallMethodName == L"relative")
				{
					HorizontalLayoutMode = Core::VLayoutMode::LayoutModeRelative;

					if (ElementProperty.second.NativeCallParameter.size() == 1 &&
						CheckNativeCallParameter(ElementProperty.second.NativeCallParameter,
												 {VMLPropertyType::IntValue}))
					{
						RelativeX = ElementProperty.second.NativeCallParameter[0].PropertyAsInt;
					}
					if (ElementProperty.second.PropertyType == VMLPropertyType::VariableDefine)
					{
						if (ElementProperty.second.VariableType == VMLVariableType::IntType)
						{
							if (!ElementProperty.second.VariableInitValue.empty())
							{
								DefineVariable<int, VMLIntVariable>(
									Object, ElementProperty.second.VariableInitValue[0].PropertyAsInt, RootFinder,
									ElementProperty.second.NativeCallMethodName);
								RelativeX = ElementProperty.second.VariableInitValue[0].PropertyAsInt;
							}
							else
							{
								DefineVariable<int, VMLIntVariable>(Object, 0, RootFinder,
																	ElementProperty.second.NativeCallMethodName);
								RelativeX = 0;
							}
						}
					}
				}
				if (ElementProperty.second.NativeCallMethodName == L"ratio")
				{
					HorizontalLayoutMode = Core::VLayoutMode::LayoutModePercent;

					if (ElementProperty.second.NativeCallParameter.size() == 1 &&
						CheckNativeCallParameter(ElementProperty.second.NativeCallParameter,
												 {VMLPropertyType::DoubleValue}))
					{
						VerticalLayoutPercent = ElementProperty.second.NativeCallParameter[0].PropertyAsDouble;
					}
					if (ElementProperty.second.PropertyType == VMLPropertyType::VariableDefine)
					{
						if (ElementProperty.second.VariableType == VMLVariableType::DoubleType)
						{
							if (!ElementProperty.second.VariableInitValue.empty())
							{
								DefineVariable<double, VMLDoubleVariable>(
									Object, ElementProperty.second.VariableInitValue[0].PropertyAsDouble, RootFinder,
									ElementProperty.second.NativeCallMethodName);
								VerticalLayoutPercent = ElementProperty.second.VariableInitValue[0].PropertyAsDouble;
							}
							else
							{
								DefineVariable<double, VMLDoubleVariable>(Object, 0.f, RootFinder,
																		  ElementProperty.second.NativeCallMethodName);
								VerticalLayoutPercent = 0.f;
							}
						}
					}
				}
			}
			else if (ElementProperty.second.PropertyType == VMLPropertyType::VariableDefine)
			{
				if (ElementProperty.second.VariableType == VMLVariableType::IntType)
				{
					if (!ElementProperty.second.VariableInitValue.empty())
					{
						DefineVariable<int, VMLIntVariable>(Object,
															ElementProperty.second.VariableInitValue[0].PropertyAsInt,
															RootFinder, ElementProperty.second.NativeCallMethodName);
						X = ElementProperty.second.VariableInitValue[0].PropertyAsInt;
					}
					else
					{
						DefineVariable<int, VMLIntVariable>(Object, 0, RootFinder,
															ElementProperty.second.NativeCallMethodName);
						X = 0;
					}
				}
			}
		}
		if (ElementProperty.first == L"y")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue &&
				ElementProperty.second.PropertyType != VMLPropertyType::NativeCall)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"y\" Property must match the type \"int\"  or \"native call "
											L"functional\"";

				return;
			}

			if (ElementProperty.second.PropertyType == VMLPropertyType::IntValue)
			{
				Y = ElementProperty.second.PropertyAsInt;
			}
			else
			{
				if (ElementProperty.second.NativeCallMethodName == L"center")
				{
					VerticalLayoutMode = Core::VLayoutMode::LayoutModeCenter;
				}
				if (ElementProperty.second.NativeCallMethodName == L"centeroffset")
				{
					VerticalLayoutMode = Core::VLayoutMode::LayoutModeMiddleOffset;

					if (ElementProperty.second.NativeCallParameter.size() == 1 &&
						CheckNativeCallParameter(ElementProperty.second.NativeCallParameter,
												 {VMLPropertyType::IntValue}))
					{
						YMiddleOffset = ElementProperty.second.NativeCallParameter[0].PropertyAsInt;
					}
					if (ElementProperty.second.PropertyType == VMLPropertyType::VariableDefine)
					{
						if (ElementProperty.second.VariableType == VMLVariableType::IntType)
						{
							if (!ElementProperty.second.VariableInitValue.empty())
							{
								DefineVariable<int, VMLIntVariable>(
									Object, ElementProperty.second.VariableInitValue[0].PropertyAsInt, RootFinder,
									ElementProperty.second.NativeCallMethodName);
								YMiddleOffset = ElementProperty.second.VariableInitValue[0].PropertyAsInt;
							}
							else
							{
								DefineVariable<int, VMLIntVariable>(Object, 0, RootFinder,
																	ElementProperty.second.NativeCallMethodName);
								YMiddleOffset = 0;
							}
						}
					}
				}
				if (ElementProperty.second.NativeCallMethodName == L"relative")
				{
					VerticalLayoutMode = Core::VLayoutMode::LayoutModeRelative;

					if (ElementProperty.second.NativeCallParameter.size() == 1 &&
						CheckNativeCallParameter(ElementProperty.second.NativeCallParameter,
												 {VMLPropertyType::IntValue}))
					{
						RelativeY = ElementProperty.second.NativeCallParameter[0].PropertyAsInt;
					}
					if (ElementProperty.second.PropertyType == VMLPropertyType::VariableDefine)
					{
						if (ElementProperty.second.VariableType == VMLVariableType::IntType)
						{
							if (!ElementProperty.second.VariableInitValue.empty())
							{
								DefineVariable<int, VMLIntVariable>(
									Object, ElementProperty.second.VariableInitValue[0].PropertyAsInt, RootFinder,
									ElementProperty.second.NativeCallMethodName);
								RelativeY = ElementProperty.second.VariableInitValue[0].PropertyAsInt;
							}
							else
							{
								DefineVariable<int, VMLIntVariable>(Object, 0, RootFinder,
																	ElementProperty.second.NativeCallMethodName);
								RelativeY = 0;
							}
						}
					}
				}
				if (ElementProperty.second.NativeCallMethodName == L"ratio")
				{
					VerticalLayoutMode = Core::VLayoutMode::LayoutModePercent;

					if (ElementProperty.second.NativeCallParameter.size() == 1 &&
						CheckNativeCallParameter(ElementProperty.second.NativeCallParameter,
												 {VMLPropertyType::DoubleValue}))
					{
						HorizontalLayoutPercent = ElementProperty.second.NativeCallParameter[0].PropertyAsDouble;
					}
					if (ElementProperty.second.PropertyType == VMLPropertyType::VariableDefine)
					{
						if (ElementProperty.second.VariableType == VMLVariableType::DoubleType)
						{
							if (!ElementProperty.second.VariableInitValue.empty())
							{
								DefineVariable<double, VMLDoubleVariable>(
									Object, ElementProperty.second.VariableInitValue[0].PropertyAsDouble, RootFinder,
									ElementProperty.second.NativeCallMethodName);
								HorizontalLayoutPercent = ElementProperty.second.VariableInitValue[0].PropertyAsDouble;
							}
							else
							{
								DefineVariable<double, VMLDoubleVariable>(Object, 0.f, RootFinder,
																		  ElementProperty.second.NativeCallMethodName);
								HorizontalLayoutPercent = 0.f;
							}
						}
					}
				}
			}
		}
		if (ElementProperty.first == L"width")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue &&
				ElementProperty.second.PropertyType != VMLPropertyType::NativeCall)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"width\" Property must match the type \"int\"";

				return;
			}

			if (ElementProperty.second.PropertyType == VMLPropertyType::IntValue)
			{
				Width = ElementProperty.second.PropertyAsInt;
			}
			else if (ElementProperty.second.PropertyType == VMLPropertyType::NativeCall)
			{
				if (ElementProperty.second.NativeCallMethodName == L"fill")
				{
					WidthRatio = 1.f;
				}
				if (ElementProperty.second.NativeCallMethodName == L"scale")
				{
					for (auto &Property : ElementProperty.second.NativeCallParameter)
					{
						if (Property.PropertyType == VMLPropertyType::DoubleValue)
						{
							WidthRatio = Property.PropertyAsDouble;
						}
						else if (Property.PropertyType == VMLPropertyType::VariableDefine)
						{
							if (Property.VariableType == VMLVariableType::DoubleType)
							{
								if (!ElementProperty.second.VariableInitValue.empty())
								{
									DefineVariable<double, VMLDoubleVariable>(
										Object, Property.VariableInitValue[0].PropertyAsDouble, RootFinder,
										ElementProperty.second.NativeCallMethodName);
									WidthRatio = ElementProperty.second.VariableInitValue[0].PropertyAsDouble;
								}
								else
								{
									DefineVariable<double, VMLDoubleVariable>(Object, 0.f, RootFinder,
																			  Property.NativeCallMethodName);
									WidthRatio = 0;
								}
							}
						}
					}
				}
			}
			else if (ElementProperty.second.PropertyType == VMLPropertyType::VariableDefine)
			{
				if (ElementProperty.second.VariableType == VMLVariableType::DoubleType)
				{
					if (!ElementProperty.second.VariableInitValue.empty())
					{
						DefineVariable<int, VMLIntVariable>(Object,
															ElementProperty.second.VariableInitValue[0].PropertyAsInt,
															RootFinder, ElementProperty.second.NativeCallMethodName);
						Width = ElementProperty.second.VariableInitValue[0].PropertyAsInt;
					}
					else
					{
						DefineVariable<int, VMLIntVariable>(Object, 0, RootFinder,
															ElementProperty.second.NativeCallMethodName);
						Width = 0;
					}
				}
			}
		}
		if (ElementProperty.first == L"height")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue &&
				ElementProperty.second.PropertyType != VMLPropertyType::NativeCall)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"height\" Property must match the type \"int\" or \"native call "
											L"functional\"";

				return;
			}

			if (ElementProperty.second.PropertyType == VMLPropertyType::IntValue)
			{
				Height = ElementProperty.second.PropertyAsInt;
			}
			else if (ElementProperty.second.PropertyType == VMLPropertyType::NativeCall)
			{
				if (ElementProperty.second.NativeCallMethodName == L"fill")
				{
					HeightRatio = 1.f;
				}
				if (ElementProperty.second.NativeCallMethodName == L"scale")
				{
					for (auto &Property : ElementProperty.second.NativeCallParameter)
					{
						if (Property.PropertyType == VMLPropertyType::DoubleValue)
						{
							HeightRatio = Property.PropertyAsDouble;
						}
						else if (Property.PropertyType == VMLPropertyType::VariableDefine)
						{
							if (Property.VariableType == VMLVariableType::DoubleType)
							{
								if (!ElementProperty.second.VariableInitValue.empty())
								{
									DefineVariable<double, VMLDoubleVariable>(
										Object, Property.VariableInitValue[0].PropertyAsDouble, RootFinder,
										ElementProperty.second.NativeCallMethodName);
									HeightRatio = ElementProperty.second.VariableInitValue[0].PropertyAsDouble;
								}
								else
								{
									DefineVariable<double, VMLDoubleVariable>(Object, 0.f, RootFinder,
																			  Property.NativeCallMethodName);
									HeightRatio = 0;
								}
							}
						}
					}
				}
			}
			else if (ElementProperty.second.PropertyType == VMLPropertyType::VariableDefine)
			{
				if (ElementProperty.second.VariableType == VMLVariableType::DoubleType)
				{
					if (!ElementProperty.second.VariableInitValue.empty())
					{
						DefineVariable<int, VMLIntVariable>(Object,
															ElementProperty.second.VariableInitValue[0].PropertyAsInt,
															RootFinder, ElementProperty.second.NativeCallMethodName);
						Height = ElementProperty.second.VariableInitValue[0].PropertyAsInt;
					}
					else
					{
						DefineVariable<int, VMLIntVariable>(Object, 0, RootFinder,
															ElementProperty.second.NativeCallMethodName);
						Height = 0;
					}
				}
			}
		}
		if (ElementProperty.first == L"transparency")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"transparency\" Property must match the type \"int\"";

				return;
			}

			Transparency = ElementProperty.second.PropertyAsInt / 255.f;
		}
		if (ElementProperty.first == L"visible")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"visible\" Property Must Match the Type \"boolean\"";

				return;
			}

			Visble = ElementProperty.second.PropertyAsBool;
		}
	}

	VMLCommonBuilder::Builder(Object, X, Y, Width, Height, Transparency, Visble, VerticalLayoutMode,
							  HorizontalLayoutMode, VerticalLayoutPercent, HorizontalLayoutPercent, RelativeX,
							  RelativeY, XMiddleOffset, YMiddleOffset, WidthRatio, HeightRatio, ViewOnly, RootFinder);
}

void VMLPushButtonBuilder::Builder(Core::VPushButton *PushButton, const VString &PlainText, const int &TextSize)
{
	PushButton->SetPlainText(PlainText);

	if (TextSize != 0)
	{
		PushButton->SetTextSize(TextSize);
	}
}

void VMLPushButtonBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VPushButton *Object,
										   std::map<VString, VMLPropertyValue> &PropertyValueList,
										   VMLControlBuildStatus			   *BuildStatus)
{
	int		TextSize = 0;
	VString Text;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"font-size")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"font-size\" Property Must Match the Type \"int\"";

				return;
			}

			TextSize = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"text")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"text\" Property Must Match the Type \"string\"";

				return;
			}

			Text = ElementProperty.second.PropertyAsString;
		}
	}

	Builder(Object, Text, TextSize);
}

VMLPushButtonBuilder::VMLPushButtonBuilder(const VMLFinder &RootFinder, Core::VPushButton *Object,
										   std::map<VString, VMLPropertyValue> &PropertyValueList,
										   VMLControlBuildStatus			   *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

void VMLImageLabelBuilder::Builder(Core::VImageLabel *ImageLabel, Core::VImage *Image, bool AutoSize, bool LockHeight)
{
	ImageLabel->SetImage(Image);
	ImageLabel->SetLockStatus(LockHeight);

	if (AutoSize)
	{
		ImageLabel->ResizeByImage();
	}
}

void VMLImageLabelBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VImageLabel *Object,
										   std::map<VString, VMLPropertyValue> &PropertyValueList,
										   VMLControlBuildStatus			   *BuildStatus)
{
	Core::VImage *Image		 = nullptr;
	bool		  AutoSize	 = false;
	bool		  LockHeight = false;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"src")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"src\" Property Must Match the Type \"string\"";

				return;
			}

			Image = new Core::VImage(ElementProperty.second.PropertyAsString, Object->CallWidgetGetDCRenderTarget());
		}
		if (ElementProperty.first == L"auto-size")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"auto-size\" Property Must Match the Type \"boolean\"";

				return;
			}

			AutoSize = ElementProperty.second.PropertyAsBool;
		}
		if (ElementProperty.first == L"lock-height")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"lock-height\" Property Must Match the Type \"boolean\"";

				return;
			}

			LockHeight = ElementProperty.second.PropertyAsBool;
		}
	}

	Builder(Object, Image, AutoSize, LockHeight);
}

VMLImageLabelBuilder::VMLImageLabelBuilder(const VMLFinder &RootFinder, Core::VImageLabel *Object,
										   std::map<VString, VMLPropertyValue> &PropertyValueList,
										   VMLControlBuildStatus			   *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

Core::VFontAlignment VMLTextLabelBuilder::ConvertAlignment(VString AlignmentString)
{
	if (AlignmentString == L"alignment-near")
	{
		return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_LEADING;
	}
	if (AlignmentString == L"alignment-center")
	{
		return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER;
	}
	if (AlignmentString == L"alignment-far")
	{
		return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_TRAILING;
	}

	return Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_LEADING;
}

Core::VFontParagraphAlignment VMLTextLabelBuilder::ConvertParagraphAlignment(VString ParagraphString)
{
	if (ParagraphString == L"alignment-near")
	{
		return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
	}
	if (ParagraphString == L"alignment-center")
	{
		return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
	}
	if (ParagraphString == L"alignment-far")
	{
		return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_FAR;
	}
	return Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
}

void VMLTextLabelBuilder::Builder(Core::VTextLabel *TextLabel, const VString &PlainText, const int &TextSize,
								  const Core::VFontAlignment		  &Alignment,
								  const Core::VFontParagraphAlignment &LineAlignment, const bool &AutoSize)
{
	TextLabel->SetPlainText(PlainText);

	if (TextSize != 0)
	{
		TextLabel->SetTextSize(TextSize);
	}

	TextLabel->SetLineAlignment(Alignment);
	TextLabel->SetParagraphAlignment(LineAlignment);

	TextLabel->SetAutoSize(AutoSize);
}

void VMLTextLabelBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VTextLabel *Object,
										  std::map<VString, VMLPropertyValue> &PropertyValueList,
										  VMLControlBuildStatus				  *BuildStatus)
{
	VString Text;
	int		TextSize = 0;
	bool	AutoSize = false;

	Core::VFontAlignment		  Alignment		= Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_LEADING;
	Core::VFontParagraphAlignment LineAlignment = Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_NEAR;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"text")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"text\" Property Must Match the Type \"string\"";

				return;
			}

			Text = ElementProperty.second.PropertyAsString;
		}
		if (ElementProperty.first == L"font-size")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"font-size\" Property Must Match the Type \"int\"";

				return;
			}

			TextSize = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"alignment")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"alignment\" Property Must Match the Type \"alignment-string\"";

				return;
			}

			Alignment = ConvertAlignment(ElementProperty.second.PropertyAsString);
		}
		if (ElementProperty.first == L"line-alignment")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"line-alignment\" Property Must Match the Type "
											L"\"alignment-string\"";

				return;
			}

			LineAlignment = ConvertParagraphAlignment(ElementProperty.second.PropertyAsString);
		}
		if (ElementProperty.first == L"auto-size")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"auto-size\" Property Must Match the Type \"boolean\"";

				return;
			}

			AutoSize = ElementProperty.second.PropertyAsBool;
		}
		if (ElementProperty.first == L"size-layout")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::NativeCall)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"size-layout\" Property must match the type \"size-layout "
											L"native function\"";

				return;
			}

			if (ElementProperty.second.PropertyType == VMLPropertyType::NativeCall &&
				ElementProperty.second.NativeCallMethodName == L"scale" &&
				CheckNativeCallParameter(ElementProperty.second.NativeCallParameter, {VMLPropertyType::DoubleValue}))
			{
				Core::VTextSizeLayout *Layout = new Core::VTextSizeLayout(Object, Object->GetParent());
				Layout->SetScalePercent(ElementProperty.second.NativeCallParameter[0].PropertyAsDouble);
			}
		}
	}

	Builder(Object, Text, TextSize, Alignment, LineAlignment, AutoSize);
}

VMLTextLabelBuilder::VMLTextLabelBuilder(const VMLFinder &RootFinder, Core::VTextLabel *Object,
										 std::map<VString, VMLPropertyValue> &PropertyValueList,
										 VMLControlBuildStatus				 *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

void VMLMainWindowBuilder::Builder(Core::VMainWindow *MainWindow, const int &Width, const int &Height,
								   const VString &Title, const bool &Sizble, const bool &FramelessStatus,
								   const bool &BorderlessStatus, const bool &AllowFileDrag)
{
	MainWindow->Resize(Width, Height);

	if (!Title.empty())
	{
		MainWindow->SetTitle(Title);
	}

	MainWindow->SetSizable(Sizble);
	MainWindow->SetFrameless(FramelessStatus);
	MainWindow->SetBorderLess(BorderlessStatus);
	MainWindow->SetFileDragStatus(AllowFileDrag);
}

void VMLMainWindowBuilder::AnalyzeProperty(Core::VMainWindow				   *Object,
										   std::map<VString, VMLPropertyValue> &PropertyValueList,
										   VMLControlBuildStatus			   *BuildStatus)
{
	int Width  = 0;
	int Height = 0;

	VString Title;

	bool Sizble			  = true;
	bool FramelessStatus  = false;
	bool BorderlessStatus = false;
	bool AllowFileDrag	  = false;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"width")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"width\" Property Must Match the Type \"int\"";

				return;
			}

			Width = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"height")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"height\" Property Must Match the Type \"int\"";

				return;
			}

			Height = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"title")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"title\" Property Must Match the Type \"string\"";

				return;
			}

			Title = ElementProperty.second.PropertyAsString;
		}
		if (ElementProperty.first == L"allow-file-drag")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"allow-file-drag\" Property Must Match the Type \"boolean\"";

				return;
			}

			AllowFileDrag = ElementProperty.second.PropertyAsBool;
		}
		if (ElementProperty.first == L"sizable")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"sizable\" Property Must Match the Type \"boolean\"";

				return;
			}

			Sizble = ElementProperty.second.PropertyAsBool;
		}
		if (ElementProperty.first == L"frameless")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"frameless\" Property Must Match the Type \"boolean\"";

				return;
			}

			FramelessStatus = ElementProperty.second.PropertyAsBool;
		}
		if (ElementProperty.first == L"borderless")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"borderless\" Property Must Match the Type \"boolean\"";

				return;
			}

			BorderlessStatus = ElementProperty.second.PropertyAsBool;
		}
	}

	Builder(Object, Width, Height, Title, Sizble, FramelessStatus, BorderlessStatus, AllowFileDrag);
}

VMLMainWindowBuilder::VMLMainWindowBuilder(Core::VMainWindow				   *MainWindow,
										   std::map<VString, VMLPropertyValue> &PropertyValueList,
										   VMLControlBuildStatus			   *BuildStatus)
{
	AnalyzeProperty(MainWindow, PropertyValueList, BuildStatus);
}

void VMLLayoutBuilder::Builder(Core::VLayout *Layout, Core::VLayoutMode VerticalLayoutMode,
							   Core::VLayoutMode HorizontalLayoutMode, double VerticalLayoutPercent,
							   double HorziontalLayoutPercent, int RelativeX, int RelativeY, int XMiddleOffset,
							   int YMiddleOffset)
{
	Layout->SetVerticalLayoutMode(VerticalLayoutMode);
	Layout->SetHorizontalLayoutMode(HorizontalLayoutMode);
	Layout->SetVerticalLayoutPercent(VerticalLayoutPercent);
	Layout->SetHorizontalLayoutPercent(HorziontalLayoutPercent);
	Layout->SetRelativeX(RelativeX);
	Layout->SetRelativeY(RelativeY);
	Layout->SetXMiddleOffset(XMiddleOffset);
	Layout->SetXMiddleOffset(YMiddleOffset);
}

void VMLLayoutBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VLayout *Object,
									   std::map<VString, VMLPropertyValue> &PropertyValueList,
									   VMLControlBuildStatus			   *BuildStatus)
{
	Core::VLayoutMode VerticalLayoutMode   = Core::VLayoutMode::LayoutModeCenter;
	Core::VLayoutMode HorizontalLayoutMode = Core::VLayoutMode::LayoutModeCenter;

	double VerticalLayoutPercent   = 0.f;
	double HorziontalLayoutPercent = 0.f;

	int RelativeX = 0;
	int RelativeY = 0;

	int XMiddleOffset = 0;
	int YMiddleOffset = 0;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"vertical-percent")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::DoubleValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"vertical-percent\" Property Must Match the Type \"int\"";

				return;
			}

			VerticalLayoutPercent = ElementProperty.second.PropertyAsDouble;
		}
		if (ElementProperty.first == L"horziontal-percent")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::DoubleValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"horziontal-percent\" Property Must Match the Type \"int\"";

				return;
			}

			HorziontalLayoutPercent = ElementProperty.second.PropertyAsDouble;
		}
		if (ElementProperty.first == L"relative-x")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"relative-x\" Property Must Match the Type \"int\"";

				return;
			}

			RelativeX = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"relative-y")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"relative-y\" Property Must Match the Type \"int\"";

				return;
			}

			RelativeY = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"x-middle-offset")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"x-middle-offset\" Property Must Match the Type \"int\"";

				return;
			}

			XMiddleOffset = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"y-middle-offset")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"y-middle-offset\" Property Must Match the Type \"int\"";

				return;
			}

			YMiddleOffset = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"vertical-layout-mode")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"vertical-layout-mode\" Property Must Match the Type \"string\"";

				return;
			}

			if (ElementProperty.second.PropertyAsString == L"center")
			{
				VerticalLayoutMode = Core::VLayoutMode::LayoutModeCenter;
			}
			if (ElementProperty.second.PropertyAsString == L"far")
			{
				VerticalLayoutMode = Core::VLayoutMode::LayoutModeFar;
			}
			if (ElementProperty.second.PropertyAsString == L"percent")
			{
				VerticalLayoutMode = Core::VLayoutMode::LayoutModePercent;
			}
			if (ElementProperty.second.PropertyAsString == L"relative")
			{
				VerticalLayoutMode = Core::VLayoutMode::LayoutModeRelative;
			}
			if (ElementProperty.second.PropertyAsString == L"relative-bottom")
			{
				VerticalLayoutMode = Core::VLayoutMode::LayoutModeRelativeBottom;
			}
			if (ElementProperty.second.PropertyAsString == L"relative-top")
			{
				VerticalLayoutMode = Core::VLayoutMode::LayoutModeRelativeTop;
			}
			if (ElementProperty.second.PropertyAsString == L"middle-offset")
			{
				VerticalLayoutMode = Core::VLayoutMode::LayoutModeMiddleOffset;
			}
		}
		if (ElementProperty.first == L"horizontal-layout")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"horizontal-layout\" Property Must Match the Type \"string\"";

				return;
			}

			if (ElementProperty.second.PropertyAsString == L"center")
			{
				HorizontalLayoutMode = Core::VLayoutMode::LayoutModeCenter;
			}
			if (ElementProperty.second.PropertyAsString == L"far")
			{
				HorizontalLayoutMode = Core::VLayoutMode::LayoutModeFar;
			}
			if (ElementProperty.second.PropertyAsString == L"percent")
			{
				HorizontalLayoutMode = Core::VLayoutMode::LayoutModePercent;
			}
			if (ElementProperty.second.PropertyAsString == L"relative")
			{
				HorizontalLayoutMode = Core::VLayoutMode::LayoutModeRelative;
			}
			if (ElementProperty.second.PropertyAsString == L"relative-bottom")
			{
				HorizontalLayoutMode = Core::VLayoutMode::LayoutModeRelativeBottom;
			}
			if (ElementProperty.second.PropertyAsString == L"relative-top")
			{
				HorizontalLayoutMode = Core::VLayoutMode::LayoutModeRelativeTop;
			}
			if (ElementProperty.second.PropertyAsString == L"middle-offset")
			{
				HorizontalLayoutMode = Core::VLayoutMode::LayoutModeMiddleOffset;
			}
		}
	}

	Builder(Object, VerticalLayoutMode, HorizontalLayoutMode, VerticalLayoutPercent, HorziontalLayoutPercent, RelativeX,
			RelativeY, XMiddleOffset, YMiddleOffset);
}
VMLLayoutBuilder::VMLLayoutBuilder(const VMLFinder &RootFinder, Core::VLayout *Object,
								   std::map<VString, VMLPropertyValue> &PropertyValueList,
								   VMLControlBuildStatus			   *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

void VMLRadioButtonBuilder::Builder(Core::VRadioButton *RadioButton, const bool &Status, const VString &PlainText,
									const bool &LockBack)
{
	RadioButton->SetSwitchStatus(Status);
	RadioButton->SetPlainText(PlainText);
	RadioButton->SetLockBackStatus(LockBack);
}
void VMLRadioButtonBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VRadioButton *RadioButton,
											std::map<VString, VMLPropertyValue> &PropertyValueList,
											VML::VMLControlBuildStatus			*BuildStatus)
{
	VMLCommonBuilder::AnalyzeProperty(RootFinder, RadioButton, PropertyValueList, BuildStatus);

	bool	SwitchStatus   = false;
	bool	LockBackStatus = false;
	VString PlainText;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"switch-status")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"switch-status\" Property Must Match the Type \"bool\"";

				return;
			}

			SwitchStatus = ElementProperty.second.PropertyAsBool;
		}
		if (ElementProperty.first == L"lock-back")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"lock-back\" Property Must Match the Type \"bool\"";

				return;
			}

			LockBackStatus = ElementProperty.second.PropertyAsBool;
		}
		if (ElementProperty.first == L"text")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"text\" Property Must Match the Type \"string\"";

				return;
			}

			PlainText = ElementProperty.second.PropertyAsString;
		}
	}

	Builder(RadioButton, SwitchStatus, PlainText, LockBackStatus);
}
VMLRadioButtonBuilder::VMLRadioButtonBuilder(const VMLFinder &RootFinder, Core::VRadioButton *Object,
											 std::map<VString, VMLPropertyValue> &PropertyValueList,
											 VMLControlBuildStatus				 *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}
void VMLScaleLayoutBuilder::Builder(Core::VScaleLayout *Layout, const double &ScaleWidthPercent,
									const double &ScaleHeightPercent)
{
	Layout->SetWidthScalePercent(ScaleWidthPercent);
	Layout->SetHeightScalePercent(ScaleHeightPercent);
}
void VMLScaleLayoutBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VScaleLayout *Object,
											std::map<VString, VMLPropertyValue> &PropertyValueList,
											VMLControlBuildStatus				*BuildStatus)
{
	double WidthScale  = 1.f;
	double HeightScale = 1.f;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"width-scale")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::DoubleValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"width-scale\" Property Must Match the Type \"double\"";

				return;
			}

			WidthScale = ElementProperty.second.PropertyAsDouble;
		}
		if (ElementProperty.first == L"height-scale")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::DoubleValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"height-scale\" Property Must Match the Type \"double\"";

				return;
			}

			HeightScale = ElementProperty.second.PropertyAsDouble;
		}
	}

	Builder(Object, WidthScale, HeightScale);
}
VMLScaleLayoutBuilder::VMLScaleLayoutBuilder(const VMLFinder &RootFinder, Core::VScaleLayout *Object,
											 std::map<VString, VMLPropertyValue> &PropertyValueList,
											 VMLControlBuildStatus				 *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}
void VMLTextSizeLayoutBuilder::Builder(Core::VTextSizeLayout *Layout, const double &Scale)
{
	Layout->SetScalePercent(Scale);
}
void VMLTextSizeLayoutBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VTextSizeLayout *Object,
											   std::map<VString, VMLPropertyValue> &PropertyValueList,
											   VMLControlBuildStatus			   *BuildStatus)
{
	double Scale = 1.f;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"scale")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::DoubleValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"scale-scale\" Property Must Match the Type \"double\"";

				return;
			}

			Scale = ElementProperty.second.PropertyAsDouble;
		}
	}

	Builder(Object, Scale);
}
VMLTextSizeLayoutBuilder::VMLTextSizeLayoutBuilder(const VMLFinder &RootFinder, Core::VTextSizeLayout *Object,
												   std::map<VString, VMLPropertyValue> &PropertyValueList,
												   VMLControlBuildStatus			   *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

void VMLSliderHorizontalBuilder::Builder(Core::VSliderHorizontal *SliderHorizontal, const double &Value,
										 const bool &Draggable)
{
	SliderHorizontal->SetValue(Value);
	SliderHorizontal->SetDraggable(Draggable);
}
void VMLSliderHorizontalBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VSliderHorizontal *Horizontal,
												 std::map<VString, VMLPropertyValue> &PropertyValueList,
												 VMLControlBuildStatus				 *BuildStatus)
{
	VMLCommonBuilder::AnalyzeProperty(RootFinder, Horizontal, PropertyValueList, BuildStatus);

	double Value	 = 0.f;
	bool   Draggable = true;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"value")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::DoubleValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"value\" Property Must Match the Type \"double\"";

				return;
			}

			Value = ElementProperty.second.PropertyAsDouble;
		}
		if (ElementProperty.first == L"draggable")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"draggable\" Property Must Match the Type \"boolean\"";

				return;
			}

			Draggable = ElementProperty.second.PropertyAsBool;
		}
	}

	Builder(Horizontal, Value, Draggable);
}
VMLSliderHorizontalBuilder::VMLSliderHorizontalBuilder(const VMLFinder &RootFinder, Core::VSliderHorizontal *Object,
													   std::map<VString, VMLPropertyValue> &PropertyValueList,
													   VMLControlBuildStatus			   *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

void VMLSliderVerticalBuilder::Builder(Core::VSliderVertical *SliderHorizontal, const double &Value,
									   const bool &Draggable)
{
	SliderHorizontal->SetValue(Value);
	SliderHorizontal->SetDraggable(Draggable);
}
void VMLSliderVerticalBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VSliderVertical *Vertical,
											   std::map<VString, VMLPropertyValue> &PropertyValueList,
											   VMLControlBuildStatus			   *BuildStatus)
{
	VMLCommonBuilder::AnalyzeProperty(RootFinder, Vertical, PropertyValueList, BuildStatus);

	double Value	 = 0.f;
	bool   Draggable = true;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"value")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::DoubleValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"value\" Property Must Match the Type \"double\"";

				return;
			}

			Value = ElementProperty.second.PropertyAsDouble;
		}
		if (ElementProperty.first == L"draggable")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"draggable\" Property Must Match the Type \"boolean\"";

				return;
			}

			Draggable = ElementProperty.second.PropertyAsBool;
		}
	}

	Builder(Vertical, Value, Draggable);
}
VMLSliderVerticalBuilder::VMLSliderVerticalBuilder(const VMLFinder &RootFinder, Core::VSliderVertical *Object,
												   std::map<VString, VMLPropertyValue> &PropertyValueList,
												   VMLControlBuildStatus			   *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

void VMLIconButtonBuilder::Builder(Core::VIconButton *IconButton, Core::VImage *Image)
{
	IconButton->SetIconImage(Image);
}

void VMLIconButtonBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VIconButton *Object,
										   std::map<VString, VMLPropertyValue> &PropertyValueList,
										   VMLControlBuildStatus			   *BuildStatus)
{
	Core::VImage *Image = nullptr;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"src")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"src\" Property Must Match the Type \"string\"";

				return;
			}

			Image = new Core::VImage(ElementProperty.second.PropertyAsString, Object->CallWidgetGetDCRenderTarget());
		}
	}

	Builder(Object, Image);
}

VMLIconButtonBuilder::VMLIconButtonBuilder(const VMLFinder &RootFinder, Core::VIconButton *Object,
										   std::map<VString, VMLPropertyValue> &PropertyValueList,
										   VMLControlBuildStatus			   *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

void VMLLineEditorBuilder::Builder(Core::VLineEditor *LineEditor, const VString &PlainText, const VString &LeadText)
{
	LineEditor->SetPlainText(PlainText);
	LineEditor->SetLeadText(LeadText);
}
void VMLLineEditorBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VLineEditor *Object,
										   std::map<VString, VMLPropertyValue> &PropertyValueList,
										   VMLControlBuildStatus			   *BuildStatus)
{
	VString PlainText;
	VString LeadText;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"plane-text")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"plane-text\" Property Must Match the Type \"string\"";

				return;
			}

			PlainText = ElementProperty.second.PropertyAsString;
		}
		if (ElementProperty.first == L"placeholder")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"placeholder\" Property Must Match the Type \"string\"";

				return;
			}

			LeadText = ElementProperty.second.PropertyAsString;
		}
	}

	Builder(Object, PlainText, LeadText);
}
VMLLineEditorBuilder::VMLLineEditorBuilder(const VMLFinder &RootFinder, Core::VLineEditor *Object,
										   std::map<VString, VMLPropertyValue> &PropertyValueList,
										   VMLControlBuildStatus			   *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

void VMLEditorBuilder::Builder(Core::VEditor *Editor, const VString &PlainText, const VString &LeadingText,
							   const int &DeltaY, const bool &AllowEdit, const bool &AllowDragFontSizeChange,
							   const bool &AllowOperationBack, const int &MaxOperationCache, const int &LeftMargin,
							   const int &TopMargin)
{
	Editor->SetPlainText(PlainText);
	Editor->SetLeadingText(LeadingText);
	Editor->SetDeltaY(DeltaY);
	Editor->SetAllowEditStatus(AllowEdit);
	Editor->SetAllowFontSizeDragStatus(AllowDragFontSizeChange);
	Editor->SetOperationBackStatus(AllowOperationBack);
	Editor->SetMaxOperationCache(MaxOperationCache);
	Editor->SetLeftMargin(LeftMargin);
	Editor->SetTopMargin(TopMargin);
}
void VMLEditorBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VEditor *Object,
									   std::map<VString, VMLPropertyValue> &PropertyValueList,
									   VMLControlBuildStatus			   *BuildStatus)
{
	VString PlainText;
	VString LeadingText;
	int		DeltaY			  = 25;
	int		MaxOperationCache = 20;
	int		LeftMargin		  = 6;
	int		TopMargin		  = 0;
	bool	AllowBack		  = true;
	bool	EditStatus		  = true;
	bool	FontDragChange	  = false;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"left-margin")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"left-margin\" Property Must Match the Type \"int\"";

				return;
			}

			LeftMargin = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"top-margin")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"top-margin\" Property Must Match the Type \"int\"";

				return;
			}

			TopMargin = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"text")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"text\" Property Must Match the Type \"string\"";

				return;
			}

			PlainText = ElementProperty.second.PropertyAsString;
		}
		if (ElementProperty.first == L"placeholder")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"placeholder\" Property Must Match the Type \"string\"";

				return;
			}

			LeadingText = ElementProperty.second.PropertyAsString;
		}
		if (ElementProperty.first == L"delta-y")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"delta-y\" Property Must Match the Type \"int\"";

				return;
			}

			DeltaY = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"allow-edit")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"allow-edit\" Property Must Match the Type \"boolean\"";

				return;
			}

			EditStatus = ElementProperty.second.PropertyAsBool;
		}
		if (ElementProperty.first == L"allow-font-size-drag")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"allow-font-size-drag\" Property Must Match the Type "
											L"\"boolean\"";

				return;
			}

			FontDragChange = ElementProperty.second.PropertyAsBool;
		}
		if (ElementProperty.first == L"allow-operation-back")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"allow-operation-back\" Property Must Match the Type "
											L"\"boolean\"";

				return;
			}

			AllowBack = ElementProperty.second.PropertyAsBool;
		}
		if (ElementProperty.first == L"max-operation-cache")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"max-operation-cache\" Property Must Match the Type \"int\"";

				return;
			}

			MaxOperationCache = ElementProperty.second.PropertyAsInt;
		}
	}

	Builder(Object, PlainText, LeadingText, DeltaY, EditStatus, FontDragChange, AllowBack, MaxOperationCache,
			LeftMargin, TopMargin);
}
VMLEditorBuilder::VMLEditorBuilder(const VMLFinder &RootFinder, Core::VEditor *Object,
								   std::map<VString, VMLPropertyValue> &PropertyValueList,
								   VMLControlBuildStatus			   *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

void VMLVerticalScrollerBuilder::Builder(Core::VScrollerVertical *Scroller, const int &Value, const int &ViewValue)
{
	Scroller->SetViewPoint(ViewValue);
	Scroller->SetViewHeight(Value);
}
void VMLVerticalScrollerBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VScrollerVertical *Object,
												 std::map<VString, VMLPropertyValue> &PropertyValueList,
												 VMLControlBuildStatus				 *BuildStatus)
{
	int ViewHeight = 0;
	int ViewPoint  = 0;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"view-height")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"view-height\" Property Must Match the Type \"int\"";

				return;
			}

			ViewHeight = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"view-point")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue &&
				ElementProperty.second.PropertyType != VMLPropertyType::NativeCall)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"view-point\" Property must match the type \"int\" or \"native "
											L"call functional\"";

				return;
			}

			if (ElementProperty.second.PropertyType == VMLPropertyType::IntValue)
			{
				ViewPoint = ElementProperty.second.PropertyAsInt;
			}
			else
			{
				if (ElementProperty.second.NativeCallMethodName == L"ratio")
				{
					if (ElementProperty.second.NativeCallParameter.size() == 1 &&
						CheckNativeCallParameter(ElementProperty.second.NativeCallParameter,
												 {VMLPropertyType::DoubleValue}))
					{
						ViewPoint =
							ElementProperty.second.NativeCallParameter[0].PropertyAsDouble * Object->GetHeight();
					}
				}
			}
		}
	}

	Builder(Object, ViewHeight, ViewPoint);
}
VMLVerticalScrollerBuilder::VMLVerticalScrollerBuilder(const VMLFinder &RootFinder, Core::VScrollerVertical *Object,
													   std::map<VString, VMLPropertyValue> &PropertyValueList,
													   VMLControlBuildStatus			   *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

void VMLHorizontalScrollerBuilder::Builder(Core::VScrollerHorizontal *Scroller, const int &Value, const int &ViewValue)
{
	Scroller->SetViewPoint(ViewValue);
	Scroller->SetViewWidth(Value);
}
void VMLHorizontalScrollerBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VScrollerHorizontal *Object,
												   std::map<VString, VMLPropertyValue> &PropertyValueList,
												   VMLControlBuildStatus			   *BuildStatus)
{
	int ViewWidth = 0;
	int ViewPoint = 0;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"view-width")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"view-width\" Property Must Match the Type \"int\"";

				return;
			}

			ViewWidth = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"view-point")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue &&
				ElementProperty.second.PropertyType != VMLPropertyType::NativeCall)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"view-point\" Property must match the type \"int\" or \"native "
											L"call functional\"";

				return;
			}

			if (ElementProperty.second.PropertyType == VMLPropertyType::IntValue)
			{
				ViewPoint = ElementProperty.second.PropertyAsInt;
			}
			else
			{
				if (ElementProperty.second.NativeCallMethodName == L"ratio")
				{
					if (ElementProperty.second.NativeCallParameter.size() == 1 &&
						CheckNativeCallParameter(ElementProperty.second.NativeCallParameter,
												 {VMLPropertyType::DoubleValue}))
					{
						ViewPoint = ElementProperty.second.NativeCallParameter[0].PropertyAsDouble * Object->GetWidth();
					}
				}
			}
		}
	}

	Builder(Object, ViewWidth, ViewPoint);
}
VMLHorizontalScrollerBuilder::VMLHorizontalScrollerBuilder(const VMLFinder					   &RootFinder,
														   Core::VScrollerHorizontal		   *Object,
														   std::map<VString, VMLPropertyValue> &PropertyValueList,
														   VMLControlBuildStatus			   *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

void VMLViewLabelBuilder::Builder(Core::VViewLabel *ViewLabel, const int &ViewWidth, const int &ViewHeight,
								  const Core::VViewLabelVerticalAlign	&VerticalAlign,
								  const Core::VViewLabelHorizontalAlign &HorizontalAlign, const bool &TieWheel)
{
	ViewLabel->SetViewRegion({ViewWidth, ViewHeight});
	ViewLabel->SetVerticalAlign(VerticalAlign);
	ViewLabel->SetHorizontalAlign(HorizontalAlign);
	ViewLabel->SetTieWithWheel(TieWheel);
}
void VMLViewLabelBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VViewLabel *Object,
										  std::map<VString, VMLPropertyValue> &PropertyValueList,
										  VMLControlBuildStatus				  *BuildStatus)
{
	int ViewWidth  = 0;
	int ViewHeight = 0;

	Core::VViewLabelVerticalAlign	VerticalAlign	= Core::VViewLabelVerticalAlign::Right;
	Core::VViewLabelHorizontalAlign HorizontalAlign = Core::VViewLabelHorizontalAlign::Bottom;

	bool TieWheel = false;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"view-width")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"view-width\" Property Must Match the Type \"int\"";

				return;
			}

			ViewWidth = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"view-height")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"view-height\" Property Must Match the Type \"int\"";

				return;
			}

			ViewHeight = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"vertical-align")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"vertical-align\" Property Must Match the Type "
											L"\"vertical-align-string\"";

				return;
			}

			if (ElementProperty.second.PropertyAsString == L"left")
			{
				VerticalAlign = Core::VViewLabelVerticalAlign::Left;
			}
			else if (ElementProperty.second.PropertyAsString == L"right")
			{
				VerticalAlign = Core::VViewLabelVerticalAlign::Right;
			}
			else
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"vertical-align\" Property Must Match the Type "
											L"\"vertical-align-string\"";

				return;
			}
		}
		if (ElementProperty.first == L"horizontal-align")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"horizontal-align\" Property Must Match the Type "
											L"\"horizontal-align-string\"";

				return;
			}

			if (ElementProperty.second.PropertyAsString == L"top")
			{
				HorizontalAlign = Core::VViewLabelHorizontalAlign::Top;
			}
			else if (ElementProperty.second.PropertyAsString == L"bottom")
			{
				HorizontalAlign = Core::VViewLabelHorizontalAlign::Bottom;
			}
			else
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"horizontal-align\" Property Must Match the Type "
											L"\"horizontal-align-string\"";

				return;
			}
		}
		if (ElementProperty.first == L"tie-wheel")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"view-wheel\" Property Must Match the Type \"boolean\"";

				return;
			}

			TieWheel = ElementProperty.second.PropertyAsBool;
		}
	}

	Builder(Object, ViewWidth, ViewHeight, VerticalAlign, HorizontalAlign, TieWheel);
}
VMLViewLabelBuilder::VMLViewLabelBuilder(const VMLFinder &RootFinder, Core::VViewLabel *Object,
										 std::map<VString, VMLPropertyValue> &PropertyValueList,
										 VMLControlBuildStatus				 *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

void VMLGeometryAnimationBuilder::Builder(Core::VGeometryAnimation *Animation, const Core::VPoint &TargetPoint,
										  const Core::VAnimationCurveFlag &Flag, const int &Duration)
{
	Animation->SetTargetSize(TargetPoint);
	Animation->SetCurve(Core::VAnimationCurveFactory::GetCurve(Flag));
	Animation->SetDuration(Duration);
}
void VMLGeometryAnimationBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VGeometryAnimation *Object,
												  std::map<VString, VMLPropertyValue> &PropertyValueList,
												  VMLControlBuildStatus				  *BuildStatus)
{
	Core::VPoint			  TargetPoint;
	Core::VAnimationCurveFlag AnimationCurve = Core::VAnimationCurveFlag::EaseLinerCurve;

	int Duration = 0;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"target-point")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::NativeCall)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"target-point\" Property Must Match the Type \"point-native\"";

				return;
			}

			if (ElementProperty.second.NativeCallMethodName == L"point")
			{
				if (ElementProperty.second.NativeCallParameter.size() == 2 &&
					CheckNativeCallParameter(ElementProperty.second.NativeCallParameter, {VMLPropertyType::IntValue}))
				{
					TargetPoint = {ElementProperty.second.NativeCallParameter[0].PropertyAsInt,
								   ElementProperty.second.NativeCallParameter[0].PropertyAsInt};
				}
			}
		}

		if (ElementProperty.first == L"animation-curve")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"animation-curve\" Property Must Match the Type "
											L"\"animation-curve-string\"";

				return;
			}

			if (ElementProperty.second.PropertyAsString == L"ease-liner-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseLinerCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-sine-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInSineCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-out-sine-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseOutSineCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-out-sine-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInOutSineCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-quad-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInQuadCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-out-quad-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseOutQuadCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-out-quad-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInOutQuadCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-cubic-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInCubicCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-out-cubic-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseOutCubicCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-out-cubic-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInOutCubicCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-quart-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInQuartCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-out-quart-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseOutQuartCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-out-quart-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInOutQuartCurve;
			}
			else
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"animation-curve\" Property Must Match the Type "
											L"\"animation-curve-string\"";

				return;
			}
		}

		if (ElementProperty.first == L"duration")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"duration\" Property Must Match the Type \"int\"";

				return;
			}

			Duration = ElementProperty.second.PropertyAsInt;
		}
	}

	Builder(Object, TargetPoint, AnimationCurve, Duration);
}
VMLGeometryAnimationBuilder::VMLGeometryAnimationBuilder(const VMLFinder &RootFinder, Core::VGeometryAnimation *Object,
														 std::map<VString, VMLPropertyValue> &PropertyValueList,
														 VMLControlBuildStatus				 *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

void VMLPositionAnimationBuilder::Builder(Core::VPositionAnimation *Animation, const Core::VPoint &TargetPoint,
										  const Core::VAnimationCurveFlag &Flag, const int &Duration)
{
	Animation->SetTargetPosition(TargetPoint);
	Animation->SetDuration(Duration);
	Animation->SetCurve(Core::VAnimationCurveFactory::GetCurve(Flag));
}
void VMLPositionAnimationBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VPositionAnimation *Object,
												  std::map<VString, VMLPropertyValue> &PropertyValueList,
												  VMLControlBuildStatus				  *BuildStatus)
{
	Core::VPoint			  TargetPoint;
	Core::VAnimationCurveFlag AnimationCurve = Core::VAnimationCurveFlag::EaseLinerCurve;

	int Duration = 0;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"target-position")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::NativeCall)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"target-position\" Property Must Match the Type \"size-native\"";

				return;
			}

			if (ElementProperty.second.NativeCallMethodName == L"point")
			{
				if (ElementProperty.second.NativeCallParameter.size() == 2 &&
					CheckNativeCallParameter(ElementProperty.second.NativeCallParameter,
											 {VMLPropertyType::IntValue, VMLPropertyType::IntValue}))
				{
					TargetPoint = {ElementProperty.second.NativeCallParameter[0].PropertyAsInt,
								   ElementProperty.second.NativeCallParameter[1].PropertyAsInt};
				}
			}
		}

		if (ElementProperty.first == L"animation-curve")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"animation-curve\" Property Must Match the Type "
											L"\"animation-curve-string\"";

				return;
			}

			if (ElementProperty.second.PropertyAsString == L"ease-liner-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseLinerCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-sine-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInSineCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-out-sine-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseOutSineCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-out-sine-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInOutSineCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-quad-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInQuadCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-out-quad-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseOutQuadCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-out-quad-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInOutQuadCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-cubic-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInCubicCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-out-cubic-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseOutCubicCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-out-cubic-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInOutCubicCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-quart-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInQuartCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-out-quart-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseOutQuartCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-out-quart-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInOutQuartCurve;
			}
			else
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"animation-curve\" Property Must Match the Type "
											L"\"animation-curve-string\"";

				return;
			}
		}

		if (ElementProperty.first == L"duration")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"duration\" Property Must Match the Type \"int\"";

				return;
			}

			Duration = ElementProperty.second.PropertyAsInt;
		}
	}

	Builder(Object, TargetPoint, AnimationCurve, Duration);
}
VMLPositionAnimationBuilder::VMLPositionAnimationBuilder(const VMLFinder &RootFinder, Core::VPositionAnimation *Object,
														 std::map<VString, VMLPropertyValue> &PropertyValueList,
														 VMLControlBuildStatus				 *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

void VMLOpacityAnimationBuilder::Builder(Core::VOpacityAnimation *Animation, const int &TargetValue,
										 const Core::VAnimationCurveFlag &Flag, const int &Duration)
{
	Animation->SetTargetValue(TargetValue);
	Animation->SetDuration(Duration);
	Animation->SetCurve(Core::VAnimationCurveFactory::GetCurve(Flag));
}
void VMLOpacityAnimationBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VOpacityAnimation *Object,
												 std::map<VString, VMLPropertyValue> &PropertyValueList,
												 VMLControlBuildStatus				 *BuildStatus)
{
	int						  TargetValue	 = 255;
	int						  Duration		 = 0;
	Core::VAnimationCurveFlag AnimationCurve = Core::VAnimationCurveFlag::EaseLinerCurve;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"target-opacity")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"target-opacity\" Property Must Match the Type \"int\"";

				return;
			}

			TargetValue = ElementProperty.second.PropertyAsInt;
		}

		if (ElementProperty.first == L"animation-curve")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"animation-curve\" Property Must Match the Type "
											L"\"animation-curve-string\"";

				return;
			}

			if (ElementProperty.second.PropertyAsString == L"ease-liner-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseLinerCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-sine-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInSineCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-out-sine-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseOutSineCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-out-sine-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInOutSineCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-quad-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInQuadCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-out-quad-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseOutQuadCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-out-quad-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInOutQuadCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-cubic-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInCubicCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-out-cubic-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseOutCubicCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-out-cubic-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInOutCubicCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-quart-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInQuartCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-out-quart-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseOutQuartCurve;
			}
			else if (ElementProperty.second.PropertyAsString == L"ease-in-out-quart-curve")
			{
				AnimationCurve = Core::VAnimationCurveFlag::EaseInOutQuartCurve;
			}
			else
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"animation-curve\" Property Must Match the Type "
											L"\"animation-curve-string\"";

				return;
			}
		}

		if (ElementProperty.first == L"duration")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"duration\" Property Must Match the Type \"int\"";

				return;
			}

			Duration = ElementProperty.second.PropertyAsInt;
		}
	}

	Builder(Object, TargetValue, AnimationCurve, Duration);
}
VMLOpacityAnimationBuilder::VMLOpacityAnimationBuilder(const VMLFinder &RootFinder, Core::VOpacityAnimation *Object,
													   std::map<VString, VMLPropertyValue> &PropertyValueList,
													   VMLControlBuildStatus			   *BuildStatus)
	: VMLCommonBuilder(RootFinder, Object, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
}

void VMLPolygonViewBuilder::Builder(Core::VPolygonView *PolygonView, const std::vector<Core::VPointF> &Points)
{
	for (auto &Point : Points)
	{
		PolygonView->AddPoint(Point);
	}
}
void VMLPolygonViewBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VPolygonView *PolygonView,
											std::map<VString, VMLPropertyValue> &PropertyValueList,
											VMLControlBuildStatus				*BuildStatus)
{
	std::vector<Core::VPointF> PointsContainer;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"points")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::NativeCall)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"points\" Property Must Match the Type "
											L"\"points-native-function\"";

				return;
			}

			for (auto &Element : ElementProperty.second.NativeCallParameter)
			{
				if (Element.PropertyType != VMLPropertyType::DoubleValue &&
					Element.NativeCallMethodName != L"parallelogram")
				{
					BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason	 = L"Unexpected parameters type, is should be "
												L"\"point-native-function\" "
												L"in points-native-function or parallelogram-native-function";

					return;
				}

				if (ElementProperty.second.NativeCallMethodName == L"parallelogram" &&
					CheckNativeCallParameter(ElementProperty.second.NativeCallParameter,
											 {VMLPropertyType::DoubleValue}))
				{
					PointsContainer.push_back(
						Core::VPointF{ElementProperty.second.NativeCallParameter[0].PropertyAsDouble, 0});
					PointsContainer.push_back(Core::VPointF{0, 1.f});
					PointsContainer.push_back(
						Core::VPointF{1.f - ElementProperty.second.NativeCallParameter[0].PropertyAsDouble, 1.f});
					PointsContainer.push_back(Core::VPointF{1.f, 0});

					break;
				}
				if (ElementProperty.second.NativeCallMethodName == L"parallelogram" &&
					CheckNativeCallParameter(ElementProperty.second.NativeCallParameter,
											 {VMLPropertyType::DoubleValue, VMLPropertyType::DoubleValue,
											  VMLPropertyType::DoubleValue, VMLPropertyType::DoubleValue}))
				{
					PointsContainer.push_back(
						Core::VPointF{ElementProperty.second.NativeCallParameter[0].PropertyAsDouble, 0});
					PointsContainer.push_back(
						Core::VPointF{ElementProperty.second.NativeCallParameter[1].PropertyAsDouble, 1.f});
					PointsContainer.push_back(
						Core::VPointF{1.f - ElementProperty.second.NativeCallParameter[2].PropertyAsDouble, 1.f});
					PointsContainer.push_back(
						Core::VPointF{1.f - ElementProperty.second.NativeCallParameter[3].PropertyAsDouble, 0});

					break;
				}
			}
		}
	}

	Builder(PolygonView, PointsContainer);
}
VMLPolygonViewBuilder::VMLPolygonViewBuilder(const VMLFinder &RootFinder, Core::VPolygonView *PolygonView,
											 std::map<VString, VMLPropertyValue> &PropertyValueList,
											 VMLControlBuildStatus				 *BuildStatus)
	: VMLCommonBuilder(RootFinder, PolygonView, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, PolygonView, PropertyValueList, BuildStatus);
}
VMLDropDownBuilder::VMLDropDownBuilder(const VMLFinder &RootFinder, Core::VDropDown *DropDown,
									   std::map<VString, VMLPropertyValue> &PropertyValueList,
									   VMLControlBuildStatus			   *BuildStatus)
	: VMLCommonBuilder(RootFinder, DropDown, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, DropDown, PropertyValueList, BuildStatus);
}

void VMLWidgetBuilder::Builder(Core::VWidget *Widget, const int &Width, const int &Height, const VString &Title,
							   const bool &Sizable, const bool &FramelessStatus, const bool &Visible)
{
	Widget->Resize(Width, Height);
	Widget->SetTitle(Title);
	Widget->SetSizable(Sizable);
	Widget->SetFrameless(FramelessStatus);

	if (Visible)
	{
		Widget->Show();
	}
}
void VMLWidgetBuilder::AnalyzeProperty(Core::VWidget *Widget, std::map<VString, VMLPropertyValue> &PropertyValueList,
									   VML::VMLControlBuildStatus *BuildStatus)
{
	int	 Width			 = 0;
	int	 Height			 = 0;
	bool Sizble			 = true;
	bool FramelessStatus = false;
	bool Visible		 = false;

	VString Title;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"width")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"width\" Property Must Match the Type \"int\"";

				return;
			}

			Width = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"height")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"height\" Property Must Match the Type \"int\"";

				return;
			}

			Height = ElementProperty.second.PropertyAsInt;
		}
		if (ElementProperty.first == L"title")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::StringValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"title\" Property Must Match the Type \"string\"";

				return;
			}

			Title = ElementProperty.second.PropertyAsString;
		}
		if (ElementProperty.first == L"sizable")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"sizable\" Property Must Match the Type \"boolean\"";

				return;
			}

			Sizble = ElementProperty.second.PropertyAsBool;
		}
		if (ElementProperty.first == L"frameless")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"frameless\" Property Must Match the Type \"boolean\"";

				return;
			}

			FramelessStatus = ElementProperty.second.PropertyAsBool;
		}
		if (ElementProperty.first == L"visible")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::BooleanValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"visible\" Property Must Match the Type \"boolean\"";

				return;
			}

			Visible = ElementProperty.second.PropertyAsBool;
		}
	}

	Builder(Widget, Width, Height, Title, Sizble, FramelessStatus, Visible);
}

VMLWidgetBuilder::VMLWidgetBuilder(Core::VWidget *Widget, std::map<VString, VMLPropertyValue> &PropertyValueList,
								   VMLControlBuildStatus *BuildStatus)
{
	AnalyzeProperty(Widget, PropertyValueList, BuildStatus);
}
void VMLCanvasBuilder::Builder(Core::VCanvas *Canvas, const int &Fps)
{
	Canvas->SetFps(Fps);
}
void VMLCanvasBuilder::AnalyzeProperty(const VMLFinder &RootFinder, Core::VCanvas *Canvas,
									   std::map<VString, VMLPropertyValue> &PropertyValueList,
									   VMLControlBuildStatus			   *BuildStatus)
{
	int Fps = 0;

	for (auto &ElementProperty : PropertyValueList)
	{
		if (ElementProperty.first == L"fps")
		{
			if (ElementProperty.second.PropertyType != VMLPropertyType::IntValue)
			{
				BuildStatus->BuildStatusCode = VMLControlBuildResultStatus::Failed;
				BuildStatus->FailedReason	 = L"\"fps\" Property Must Match the Type \"int\"";

				return;
			}

			Fps = ElementProperty.second.PropertyAsInt;
		}
	}

	Builder(Canvas, Fps);
}

VMLCanvasBuilder::VMLCanvasBuilder(const VMLFinder &RootFinder, Core::VCanvas *Canvas,
								   std::map<VString, VMLPropertyValue> &PropertyValueList,
								   VMLControlBuildStatus			   *BuildStatus)
	: VMLCommonBuilder(RootFinder, Canvas, PropertyValueList, BuildStatus)
{
	AnalyzeProperty(RootFinder, Canvas, PropertyValueList, BuildStatus);
}
} // namespace VML

VLIB_END_NAMESPACE