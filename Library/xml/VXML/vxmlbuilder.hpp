/*
 * VVMLBuilder.hpp
 *	@description : The VML Object Builder in VLib
 *	@birth		 : 2022/6.4
*/

#pragma once

#include "../../VBasicControl"

#include "vvmlparser.hpp"

VLIB_BEGIN_NAMESPACE

enum class VVMLControlBuildResultStatus {
	Ok, Failed
};

struct VVMLContronBuildStatus {
	VVMLControlBuildResultStatus BuildStatusCode = VVMLControlBuildResultStatus::Ok;
	std::wstring				 FailedReason;
};

class VVMLCommonBuilder {
protected:
	void Builder(VUIObject* Object, int X, int Y, int Width, int Height, int Transparency) {
		if (Object->IsWidget() == false) {
			Object->Move(X, Y);
		}

		Object->Resize(Width, Height);
		Object->SetTransparency(Transparency);
	}

	virtual void AnalyzeProperty(VUIObject* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus) {
		int X = 0;
		int Y = 0;
		int Height = 0;
		int Width = 0;
		int Transparency = 255;

		BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Ok;

		for (auto& ElementProperty : PropertyValueList) {
			if (ElementProperty.first == L"X") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"X\" Property Must Match the Type \"Int\"";

					return;
				}

				X = ElementProperty.second.PropertyAsInt;
			}
			if (ElementProperty.first == L"Y") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Y\" Property Must Match the Type \"Int\"";

					return;
				}

				Y = ElementProperty.second.PropertyAsInt;
			}
			if (ElementProperty.first == L"Width") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Width\" Property Must Match the Type \"Int\"";

					return;
				}

				Width = ElementProperty.second.PropertyAsInt;
			}
			if (ElementProperty.first == L"Height") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Height\" Property Must Match the Type \"Int\"";

					return;
				}

				Height = ElementProperty.second.PropertyAsInt;
			}
			if (ElementProperty.first == L"Transparency") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Transparency\" Property Must Match the Type \"Int\"";

					return;
				}

				Transparency = ElementProperty.second.PropertyAsInt;
			}
		}

		VVMLCommonBuilder::Builder(Object, X, Y, Width, Height, Transparency);
	}

public:
	VVMLCommonBuilder(VUIObject* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus) {
		AnalyzeProperty(Object, PropertyValueList, BuildStatus);
	}
};
class VVMLVPushButtonBuilder : public VVMLCommonBuilder {
protected:
	void Builder(VPushButton* PushButton, std::wstring PlaneText, int TextSize = 0) {
		PushButton->SetPlaneText(PlaneText);

		if (TextSize != 0) {
			PushButton->SetTextSize(TextSize);
		}
	}

	void AnalyzeProperty(VPushButton* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus) {
		VVMLCommonBuilder::AnalyzeProperty(Object, PropertyValueList, BuildStatus);

		int			 TextSize = 0;
		std::wstring Text;

		for (auto& ElementProperty : PropertyValueList) {
			if (ElementProperty.first == L"TextSize") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"TextSize\" Property Must Match the Type \"Int\"";

					return;
				}

				TextSize = ElementProperty.second.PropertyAsInt;
			}
			if (ElementProperty.first == L"Text") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::StringValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Text\" Property Must Match the Type \"String\"";

					return;
				}

				Text = ElementProperty.second.PropertyAsString;
			}
		}

		Builder(Object, Text, TextSize);
	}

public:
	VVMLVPushButtonBuilder(VPushButton* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus) 
		: VVMLCommonBuilder(Object, PropertyValueList, BuildStatus) {
		AnalyzeProperty(Object, PropertyValueList, BuildStatus);
	}
};
class VVMLImageLabelBuilder : public VVMLCommonBuilder {
protected:
	void Builder(VImageLabel* ImageLabel, VImage* Image) {
		ImageLabel->SetImage(Image);
	}

	void AnalyzeProperty(VImageLabel* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus) {
		VVMLCommonBuilder::AnalyzeProperty(Object, PropertyValueList, BuildStatus);

		VImage* Image = nullptr;

		for (auto& ElementProperty : PropertyValueList) {
			if (ElementProperty.first == L"ImagePath") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::StringValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"ImagePath\" Property Must Match the Type \"String\"";

					return;
				}

				Image = new VImage(ElementProperty.second.PropertyAsString);
			}
		}

		Builder(Object, Image);
	}

public:
	VVMLImageLabelBuilder(VImageLabel* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus)
		: VVMLCommonBuilder(Object, PropertyValueList, BuildStatus) {
		AnalyzeProperty(Object, PropertyValueList, BuildStatus);
	}
};
class VVMLTextLabelBuilder : public VVMLCommonBuilder {
protected:
	void Builder(VTextLabel* TextLabel, std::wstring PlaneText, int TextSize) {
		TextLabel->SetPlaneText(PlaneText);

		if (TextSize != 0) {
			TextLabel->SetTextSize(TextSize);
		}
	}

	void AnalyzeProperty(VTextLabel* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus) {
		VVMLCommonBuilder::AnalyzeProperty(Object, PropertyValueList, BuildStatus);

		std::wstring Text;
		int          TextSize = 0;

		for (auto& ElementProperty : PropertyValueList) {
			if (ElementProperty.first == L"Text") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::StringValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Text\" Property Must Match the Type \"String\"";

					return;
				}

				Text = ElementProperty.second.PropertyAsString;
			}
			if (ElementProperty.first == L"TextSize") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"TextSize\" Property Must Match the Type \"Int\"";

					return;
				}

				TextSize = ElementProperty.second.PropertyAsInt;
			}
		}

		Builder(Object, Text, TextSize);
	}

public:
	VVMLTextLabelBuilder(VTextLabel* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus)
		: VVMLCommonBuilder(Object, PropertyValueList, BuildStatus) {
		AnalyzeProperty(Object, PropertyValueList, BuildStatus);
	}
};
class VVMLBlurLabelBuilder : public VVMLCommonBuilder {
protected:
	void Builder(VBlurLabel* BlurLabel, int BlurRadius) {
		BlurLabel->SetBlurRadius(BlurRadius);
	}

	void AnalyzeProperty(VBlurLabel* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus) {
		VVMLCommonBuilder::AnalyzeProperty(Object, PropertyValueList, BuildStatus);

		int          BlurRadius = 0;

		for (auto& ElementProperty : PropertyValueList) {
			if (ElementProperty.first == L"BlurRadius") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"BlurRadius\" Property Must Match the Type \"Int\"";

					return;
				}

				BlurRadius = ElementProperty.second.PropertyAsInt;
			}
		}

		Builder(Object, BlurRadius);
	}

public:
	VVMLBlurLabelBuilder(VBlurLabel* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus)
		: VVMLCommonBuilder(Object, PropertyValueList, BuildStatus) {
		AnalyzeProperty(Object, PropertyValueList, BuildStatus);
	}
};
class VVMLIconButtonBuilder : public VVMLCommonBuilder {
protected:
	void Builder(VIconButton* IconButton, VImage* Icon) {
		if (Icon != nullptr) {
			IconButton->SetIcon(Icon);
		}
	}

	void AnalyzeProperty(VIconButton* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus) {
		VVMLCommonBuilder::AnalyzeProperty(Object, PropertyValueList, BuildStatus);

		VImage* Icon = nullptr;

		for (auto& ElementProperty : PropertyValueList) {
			if (ElementProperty.first == L"IconPath") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::StringValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"IconPath\" Property Must Match the Type \"String\"";

					return;
				}

				Icon = new VImage(ElementProperty.second.PropertyAsString);
			}
		}

		Builder(Object, Icon);
	}

public:
	VVMLIconButtonBuilder(VIconButton* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus)
		: VVMLCommonBuilder(Object, PropertyValueList, BuildStatus) {
		AnalyzeProperty(Object, PropertyValueList, BuildStatus);
	}
};
class VVMLLineEditorBuilder : public VVMLCommonBuilder {
protected:
	void AnalyzeProperty(VLineEditor* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus) {
		VVMLCommonBuilder::AnalyzeProperty(Object, PropertyValueList, BuildStatus);
	}

public:
	VVMLLineEditorBuilder(VLineEditor* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus)
		: VVMLCommonBuilder(Object, PropertyValueList, BuildStatus) {
		AnalyzeProperty(Object, PropertyValueList, BuildStatus);
	}
};
class VVMLLayoutBuilder : public VVMLCommonBuilder {
protected:
	void Builder(VLayout* Layout, VLayoutMode VerticalLayoutMode, VLayoutMode HorizontalLayoutMode,
		double VerticalLayoutPercent, double HorziontalLayoutPercent,
		int RelativeX, int RelativeY, int XMiddleOffset, int YMiddleOffset) {
		Layout->SetVerticalLayoutMode(VerticalLayoutMode);
		Layout->SetHorizontalLayoutMode(HorizontalLayoutMode);
		Layout->SetVerticalLayoutPercent(VerticalLayoutPercent);
		Layout->SetHorziontalLayoutPercent(HorziontalLayoutPercent);
		Layout->SetRelativeX(RelativeX);
		Layout->SetRelativeY(RelativeY);
		Layout->SetXMiddleOffset(XMiddleOffset);
		Layout->SetXMiddleOffset(YMiddleOffset);
	}

	void AnalyzeProperty(VLayout* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus) {
		VVMLCommonBuilder::AnalyzeProperty(Object, PropertyValueList, BuildStatus);

		VLayoutMode  VerticalLayoutMode   = VLayoutMode::LayoutModeCenter;
		VLayoutMode  HorizontalLayoutMode = VLayoutMode::LayoutModeCenter;

		double       VerticalLayoutPercent = 0.f;
		double       HorziontalLayoutPercent = 0.f;

		int          RelativeX = 0;
		int          RelativeY = 0;

		int          XMiddleOffset = 0;
		int          YMiddleOffset = 0;

		for (auto& ElementProperty : PropertyValueList) {
			if (ElementProperty.first == L"VerticalPercent") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"VerticalPercent\" Property Must Match the Type \"Int\"";

					return;
				}

				VerticalLayoutPercent = ElementProperty.second.PropertyAsDouble;
			}
			if (ElementProperty.first == L"HorziontalPercent") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"HorziontalPercent\" Property Must Match the Type \"Int\"";

					return;
				}

				HorziontalLayoutPercent = ElementProperty.second.PropertyAsDouble;
			}
			if (ElementProperty.first == L"RelativeX") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"RelativeX\" Property Must Match the Type \"Int\"";

					return;
				}

				RelativeX = ElementProperty.second.PropertyAsDouble;
			}
			if (ElementProperty.first == L"RelativeY") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"RelativeY\" Property Must Match the Type \"Int\"";

					return;
				}

				RelativeY = ElementProperty.second.PropertyAsDouble;
			}
			if (ElementProperty.first == L"XMiddleOffset") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"XMiddleOffset\" Property Must Match the Type \"Int\"";

					return;
				}

				XMiddleOffset = ElementProperty.second.PropertyAsDouble;
			}
			if (ElementProperty.first == L"YMiddleOffset") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"YMiddleOffset\" Property Must Match the Type \"Int\"";

					return;
				}

				YMiddleOffset = ElementProperty.second.PropertyAsDouble;
			}
			if (ElementProperty.first == L"VerticalLayoutMode") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::StringValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"VerticalLayoutMode\" Property Must Match the Type \"String\"";

					return;
				}

				if (ElementProperty.second.PropertyAsString == L"Center") {
					VerticalLayoutMode = VLayoutMode::LayoutModeCenter;
				}
				if (ElementProperty.second.PropertyAsString == L"Far") {
					VerticalLayoutMode = VLayoutMode::LayoutModeFar;
				}
				if (ElementProperty.second.PropertyAsString == L"Percent") {
					VerticalLayoutMode = VLayoutMode::LayoutModePercent;
				}
				if (ElementProperty.second.PropertyAsString == L"Relative") {
					VerticalLayoutMode = VLayoutMode::LayoutModeRelative;
				}
				if (ElementProperty.second.PropertyAsString == L"RelativeBottom") {
					VerticalLayoutMode = VLayoutMode::LayoutModeRelativeBottom;
				}
				if (ElementProperty.second.PropertyAsString == L"RelativeTop") {
					VerticalLayoutMode = VLayoutMode::LayoutModeRelativeTop;
				}
				if (ElementProperty.second.PropertyAsString == L"MiddleOffset") {
					VerticalLayoutMode = VLayoutMode::LayoutModeMiddleOffset;
				}
			}
			if (ElementProperty.first == L"HorizontalLayoutMode") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::StringValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"HorizontalLayoutMode\" Property Must Match the Type \"String\"";

					return;
				}

				if (ElementProperty.second.PropertyAsString == L"Center") {
					HorizontalLayoutMode = VLayoutMode::LayoutModeCenter;
				}
				if (ElementProperty.second.PropertyAsString == L"Far") {
					HorizontalLayoutMode = VLayoutMode::LayoutModeFar;
				}
				if (ElementProperty.second.PropertyAsString == L"Percent") {
					HorizontalLayoutMode = VLayoutMode::LayoutModePercent;
				}
				if (ElementProperty.second.PropertyAsString == L"Relative") {
					HorizontalLayoutMode = VLayoutMode::LayoutModeRelative;
				}
				if (ElementProperty.second.PropertyAsString == L"RelativeBottom") {
					HorizontalLayoutMode = VLayoutMode::LayoutModeRelativeBottom;
				}
				if (ElementProperty.second.PropertyAsString == L"RelativeTop") {
					HorizontalLayoutMode = VLayoutMode::LayoutModeRelativeTop;
				}
				if (ElementProperty.second.PropertyAsString == L"MiddleOffset") {
					HorizontalLayoutMode = VLayoutMode::LayoutModeMiddleOffset;
				}
			}
		}

		Builder(Object, VerticalLayoutMode, HorizontalLayoutMode,
			VerticalLayoutPercent, HorziontalLayoutPercent,
			RelativeX, RelativeY, XMiddleOffset, YMiddleOffset);
	}

public:
	VVMLLayoutBuilder(VLayout* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus)
		: VVMLCommonBuilder(Object, PropertyValueList, BuildStatus) {
		AnalyzeProperty(Object, PropertyValueList, BuildStatus);
	}
};
class VVMLMainWindowBuilder {
protected:
	void Builder(VMainWindow* MainWindow, int Width, int Height, std::wstring Title, bool Sizble) {
		MainWindow->Resize(Width, Height);

		if (Title.empty() == false) {
			MainWindow->SetTitle(Title);
		}

		MainWindow->SetSizble(Sizble);
	}

	void AnalyzeProperty(VMainWindow* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus) {
		int          Width  = 0;
		int          Height = 0;
		
		std::wstring Title;

		bool         Sizble = true;

		for (auto& ElementProperty : PropertyValueList) {
			if (ElementProperty.first == L"Width") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Width\" Property Must Match the Type \"Int\"";

					return;
				}

				Width = ElementProperty.second.PropertyAsInt;
			}
			if (ElementProperty.first == L"Height") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Height\" Property Must Match the Type \"Int\"";

					return;
				}

				Height = ElementProperty.second.PropertyAsInt;
			}
			if (ElementProperty.first == L"Title") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::StringValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Title\" Property Must Match the Type \"String\"";

					return;
				}

				Title = ElementProperty.second.PropertyAsString;
			}
			if (ElementProperty.first == L"Sizble") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Sizble\" Property Must Match the Type \"Boolean\"";

					return;
				}

				Sizble = ElementProperty.second.PropertyAsBool;
			}
		}

		Builder(Object, Width, Height, Title, Sizble);
	}

public:
	VVMLMainWindowBuilder(VMainWindow* MainWindow, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus) {
		AnalyzeProperty(MainWindow, PropertyValueList, BuildStatus);
	}
};
class VVMLAnimationCommonBuilderAPI {
public:
	static VInterpolatorType ConvertInterpolator(std::wstring InterpolatorName) {
		if (InterpolatorName == L"AnticipateInterpolator") {
			return VInterpolatorType::AnticipateInterpolator;
		}
		else if (InterpolatorName == L"AccelerateDecelerateInterpolator") {
			return VInterpolatorType::AccelerateDecelerateInterpolator;
		}
		else if (InterpolatorName == L"AccelerateInterpolator") {
			return VInterpolatorType::AccelerateInterpolator;
		}
		else if (InterpolatorName == L"AnticipateOvershootInterpolator") {
			return VInterpolatorType::AnticipateOvershootInterpolator;
		}
		else if (InterpolatorName == L"DecelerateInterpolator") {
			return VInterpolatorType::DecelerateInterpolator;
		}
		else if (InterpolatorName == L"LinearInterpolator") {
			return VInterpolatorType::LinearInterpolator;
		}
		else if (InterpolatorName == L"OvershootInterpolator") {
			return VInterpolatorType::OvershootInterpolator;
		}
		else if (InterpolatorName == L"CycleInterpolator") {
			return VInterpolatorType::CycleInterpolator;
		}

		return VInterpolatorType::AccelerateDecelerateInterpolator;
	}
};
class VVMLPositionAnimationBuilder : public VVMLCommonBuilder {
protected:
	void Builder(
		VPositionAnimation* Animation, 
		int                 TargetX,
		int                 TargetY,
		int                 Duraction,
		VInterpolatorType   Interpolator) {
		Animation->SetTargetPoint({ TargetX, TargetY });
		Animation->SetDuraction(Duraction);
		Animation->SetInterpolator(Interpolator);
	}

	void AnalyzeProperty(VPositionAnimation* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus) {
		VVMLCommonBuilder::AnalyzeProperty(Object, PropertyValueList, BuildStatus);

		int               TargetX      = 0;
		int               TargetY      = 0;
		int               Duraction    = 0;
		VInterpolatorType Interpolator = VInterpolatorType::AccelerateInterpolator;

		for (auto& ElementProperty : PropertyValueList) {
			if (ElementProperty.first == L"TargetX") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"TargetX\" Property Must Match the Type \"Int\"";

					return;
				}

				TargetX = ElementProperty.second.PropertyAsInt;
			}
			if (ElementProperty.first == L"TargetY") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"TargetY\" Property Must Match the Type \"Int\"";

					return;
				}

				TargetY = ElementProperty.second.PropertyAsInt;
			}

			if (ElementProperty.first == L"Duraction") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Duraction\" Property Must Match the Type \"Int\"";

					return;
				}

				Duraction = ElementProperty.second.PropertyAsInt;
			}
			if (ElementProperty.first == L"Interpolator") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::StringValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Interpolator\" Property Must Match the Type \"String\"";

					return;
				}

				Interpolator = Interpolator = VVMLAnimationCommonBuilderAPI::ConvertInterpolator(ElementProperty.second.PropertyAsString);
			}
		}

		Builder(Object, TargetX, TargetY, Duraction, Interpolator);
	}

public:
	VVMLPositionAnimationBuilder(VPositionAnimation* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus)
		: VVMLCommonBuilder(Object, PropertyValueList, BuildStatus) {
		AnalyzeProperty(Object, PropertyValueList, BuildStatus);
	}
};
class VVMLAlphaAnimationBuilder : public VVMLCommonBuilder {
protected:
	void Builder(
		VAlphaAnimation*  Animation,
		int               Alpha,
		int               Duraction,
		VInterpolatorType Interpolator) {
		Animation->SetTargetAlpha(Alpha);
		Animation->SetDuraction(Duraction);
		Animation->SetInterpolator(Interpolator);
	}

	void AnalyzeProperty(VAlphaAnimation* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus) {
		VVMLCommonBuilder::AnalyzeProperty(Object, PropertyValueList, BuildStatus);

		int               TargetAlpha = 0;
		int               Duraction = 0;
		VInterpolatorType Interpolator = VInterpolatorType::AccelerateInterpolator;

		for (auto& ElementProperty : PropertyValueList) {
			if (ElementProperty.first == L"TargetAlpha") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"TargetAlpha\" Property Must Match the Type \"Int\"";

					return;
				}

				TargetAlpha = ElementProperty.second.PropertyAsInt;
			}

			if (ElementProperty.first == L"Duraction") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Duraction\" Property Must Match the Type \"Int\"";

					return;
				}

				Duraction = ElementProperty.second.PropertyAsInt;
			}
			if (ElementProperty.first == L"Interpolator") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::StringValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Interpolator\" Property Must Match the Type \"String\"";

					return;
				}

				Interpolator = Interpolator = VVMLAnimationCommonBuilderAPI::ConvertInterpolator(ElementProperty.second.PropertyAsString);
			}
		}

		Builder(Object, TargetAlpha, Duraction, Interpolator);
	}

public:
	VVMLAlphaAnimationBuilder(VAlphaAnimation* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus)
		: VVMLCommonBuilder(Object, PropertyValueList, BuildStatus) {
		AnalyzeProperty(Object, PropertyValueList, BuildStatus);
	}
};
class VVMLGeomteryAnimationBuilder : public VVMLCommonBuilder {
protected:
	void Builder(
		VGeomteryAnimation* Animation,
		int                 TargetWidth,
		int                 TargetHeight,
		int                 Duraction,
		VInterpolatorType   Interpolator) {
		Animation->SetTargetSize({ TargetWidth, TargetHeight });
		Animation->SetDuraction(Duraction);
		Animation->SetInterpolator(Interpolator);
	}

	void AnalyzeProperty(VGeomteryAnimation* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus) {
		VVMLCommonBuilder::AnalyzeProperty(Object, PropertyValueList, BuildStatus);

		int               TargetWidth  = 0;
		int               TargetHeight = 0;
		int               Duraction    = 0;
		VInterpolatorType Interpolator = VInterpolatorType::AccelerateInterpolator;

		for (auto& ElementProperty : PropertyValueList) {
			if (ElementProperty.first == L"TargetWidth") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"TargetWidth\" Property Must Match the Type \"Int\"";

					return;
				}

				TargetWidth = ElementProperty.second.PropertyAsInt;
			}
			if (ElementProperty.first == L"TargetHeight") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"TargetHeight\" Property Must Match the Type \"Int\"";

					return;
				}

				TargetHeight = ElementProperty.second.PropertyAsInt;
			}

			if (ElementProperty.first == L"Duraction") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::IntValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Duraction\" Property Must Match the Type \"Int\"";

					return;
				}

				Duraction = ElementProperty.second.PropertyAsInt;
			}
			if (ElementProperty.first == L"Interpolator") {
				if (ElementProperty.second.PropertyType != VVMLPropertyType::StringValue) {
					BuildStatus->BuildStatusCode = VVMLControlBuildResultStatus::Failed;
					BuildStatus->FailedReason = L"\"Interpolator\" Property Must Match the Type \"String\"";

					return;
				}

				Interpolator = Interpolator = VVMLAnimationCommonBuilderAPI::ConvertInterpolator(ElementProperty.second.PropertyAsString);
			}
		}

		Builder(Object, TargetWidth, TargetHeight, Duraction, Interpolator);
	}

public:
	VVMLGeomteryAnimationBuilder(VGeomteryAnimation* Object, std::map<std::wstring, VVMLPropertyValue> PropertyValueList,
		VVMLContronBuildStatus* BuildStatus)
		: VVMLCommonBuilder(Object, PropertyValueList, BuildStatus) {
		AnalyzeProperty(Object, PropertyValueList, BuildStatus);
	}
};

VLIB_END_NAMESPACE