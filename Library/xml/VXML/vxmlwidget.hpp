/*
 * VVMLWidget.hpp
 *	@description : The VML Widget of VLib
 *	@birth		 : 2022/6.4
*/

#pragma once

#include "../../control/basic/VBasicControl/vwidget.hpp"
#include "../../vss/VSS/vssparser.hpp"
#include "../../vss/VSS/vssbuilder.hpp"

#include "vvmlparser.hpp"
#include "vxmlbuilder.hpp"

#include <algorithm>

VLIB_BEGIN_NAMESPACE

enum class VVMLObjectType {
	PushButton, Layout, ImageLabel, TextLabel, BlurLabel, IconButton, LineEditor,
	PositionAnimation, GeomteryAnimation, AlphaAnimation, MainWindow, ViewLabel
};

struct VVMLObject {
	VUIObject* UIObject = nullptr;

	std::wstring   VMLID;
	std::wstring   VMLClass;
	std::wstring   VMLDOMID;

	VVMLObjectType VMLObjectType = VVMLObjectType::PushButton;

	std::vector<VVMLObject*> ChildrenObjects;

	VVMLObjectType String2ObjectType(std::wstring String) {
		VVMLObjectType Type = VVMLObjectType::PushButton;

		if (String == L"vpushbutton") {
			return VVMLObjectType::PushButton;
		}
		if (String == L"vtextlabel") {
			return VVMLObjectType::TextLabel;
		}
		if (String == L"vblurlabel") {
			return VVMLObjectType::BlurLabel;
		}
		if (String == L"vlineeditor") {
			return VVMLObjectType::LineEditor;
		}
		if (String == L"vviewlabel") {
			return VVMLObjectType::ViewLabel;
		}
		if (String == L"viconbutton") {
			return VVMLObjectType::IconButton;
		}
		if (String == L"vimagelabel") {
			return VVMLObjectType::ImageLabel;
		}
		if (String == L"vmainwindow") {
			return VVMLObjectType::MainWindow;
		}

		return Type;
	}
	std::wstring   ObjectType2String(VVMLObjectType Type) {
		switch (VMLObjectType) {
		case VVMLObjectType::PushButton: {
			return L"vpushbutton";
		}
		case VVMLObjectType::TextLabel: {
			return L"vtextlabel";
		}
		case VVMLObjectType::BlurLabel: {
			return L"vblurlabel";
		}
		case VVMLObjectType::LineEditor: {
			return L"vlineeditor";
		}
		case VVMLObjectType::ViewLabel: {
			return L"vviewlabel";
		}
		case VVMLObjectType::IconButton: {
			return L"vviconbutton";
		}
		case VVMLObjectType::ImageLabel: {
			return L"vimagelabel";
		}
		}

		return L"";
	}

	void SetNativeStyleSheet(VVSSBasicSelector* Selector) {
		switch (VMLObjectType) {
		case VVMLObjectType::PushButton: {
			VVSSVPushButtonBuilder Builder(static_cast<VPushButton*>(UIObject), std::vector<VVSSBasicSelector*>{Selector}, nullptr);

			break;
		}
		case VVMLObjectType::TextLabel: {
			VVSSVTextLabelBuilder Builder(static_cast<VTextLabel*>(UIObject), std::vector<VVSSBasicSelector*>{Selector}, nullptr);

			break;
		}
		case VVMLObjectType::BlurLabel: {
			VVSSVBlurLabelBuilder Builder(static_cast<VBlurLabel*>(UIObject), std::vector<VVSSBasicSelector*>{Selector}, nullptr);

			break;
		}
		case VVMLObjectType::LineEditor: {
			VVSSVLineEditorBuilder Builder(static_cast<VLineEditor*>(UIObject), std::vector<VVSSBasicSelector*>{Selector}, nullptr);

			break;
		}
		case VVMLObjectType::ViewLabel: {
			VVSSVViewLabelBuilder Builder(static_cast<VViewLabel*>(UIObject), std::vector<VVSSBasicSelector*>{Selector}, nullptr);

			break;
		}
		case VVMLObjectType::IconButton: {
			VVSSVIconButtonBuilder Builder(static_cast<VIconButton*>(UIObject), std::vector<VVSSBasicSelector*>{Selector}, nullptr);

			break;
		}
		case VVMLObjectType::ImageLabel: {
			VVSSVImageLabelBuilder Builder(static_cast<VImageLabel*>(UIObject), std::vector<VVSSBasicSelector*>{Selector}, nullptr);

			break;
		}
		case VVMLObjectType::MainWindow: {
			VVSSVMainWindowBuilder Builder(static_cast<VMainWindow*>(UIObject), std::vector<VVSSBasicSelector*>{Selector});

			break;
		}
		}
	}
	void SetStyleSheet(VVSSBasicSelector* Selector) {
		switch (Selector->GetType()) {
		case VVSSSelectorType::ClassSelector: {
			if (static_cast<VVSSClassSelector*>(Selector)->ClassTag == VMLClass) {
				VVSSElementSelector* ElementSelector = new VVSSElementSelector;

				ElementSelector->ElementTag = ObjectType2String(VMLObjectType);
				ElementSelector->SelectorProperty = Selector->SelectorProperty;

				SetNativeStyleSheet(ElementSelector);

				delete ElementSelector;
			}

			break;
		}
		case VVSSSelectorType::ElementSelector: {
			if (String2ObjectType(static_cast<VVSSElementSelector*>(Selector)->ElementTag) == VMLObjectType &&
				VMLClass.empty() == true) {
				SetNativeStyleSheet(Selector);
			}

			break;
		}
		case VVSSSelectorType::ClassWithFakeClassSelector: {
			if (static_cast<VVSSClassWithFakeClassSelector*>(Selector)->ClassTag == VMLClass) {
				VVSSFakeClassSelector* FakeClassSelector = new VVSSFakeClassSelector;

				FakeClassSelector->ElementTag = ObjectType2String(VMLObjectType);
				FakeClassSelector->SelectorProperty = Selector->SelectorProperty;
				FakeClassSelector->ClassTag = static_cast<VVSSClassWithFakeClassSelector*>(Selector)->FakeClassTag;

				SetNativeStyleSheet(FakeClassSelector);

				delete FakeClassSelector;
			}

			break;
		}
		case VVSSSelectorType::GenericSelector: {
			SetNativeStyleSheet(Selector);

			break;
		}
		case VVSSSelectorType::IDSelector: {
			if (static_cast<VVSSIDSelector*>(Selector)->IDTag == VMLDOMID) {
				VVSSElementSelector* ElementSelector = new VVSSElementSelector;

				ElementSelector->ElementTag = ObjectType2String(VMLObjectType);
				ElementSelector->SelectorProperty = Selector->SelectorProperty;

				SetNativeStyleSheet(ElementSelector);

				delete ElementSelector;
			}

			break;
		}
		case VVSSSelectorType::FakeClassSelector: {
			if (String2ObjectType(static_cast<VVSSFakeClassSelector*>(Selector)->ElementTag) == VMLObjectType &&
				VMLClass.empty() == true) {
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
};

enum class VVMLWidgetVMLLoadStats {
	Ok, Failed, InvalidAstTree
};

struct VVMLWidgetLoadResult {
	VVMLWidgetVMLLoadStats Status = VVMLWidgetVMLLoadStats::Ok;

	std::wstring           FailedMessage;
};

struct VVMLWidgetVMLObjectList {
	std::vector<VVMLObject*> Objects;
};

class VVMLFinder {
private:
	std::vector<VVMLObject*> ObjectList;
	VVMLObject* OriginObject;

public:
	VVMLFinder(VVMLObject* Object, std::vector<VVMLObject*> List) {
		ObjectList = List;
		OriginObject = Object;
	}

	VVMLFinder operator[](std::wstring ChildrenId) {
		for (auto& Object : ObjectList) {
			if (Object->VMLID == ChildrenId) {
				return VVMLFinder(Object, Object->ChildrenObjects);
			}
		}

		return VVMLFinder(nullptr, std::vector<VVMLObject*>());
	}

	VVMLFinder Get(std::wstring ChildrenId) {
		for (auto& Object : ObjectList) {
			if (Object->VMLID == ChildrenId) {
				return VVMLFinder(Object, Object->ChildrenObjects);
			}
		}

		return VVMLFinder(nullptr, std::vector<VVMLObject*>());
	}

	bool       IsValid() {
		return OriginObject != nullptr;
	}

	template<class Type = VUIObject>
	Type* Get() {
		return static_cast<Type*>(OriginObject->UIObject);
	}

	operator VUIObject* () {
		return OriginObject->UIObject;
	}

};

class VVMLWidget : public VMainWindow {
protected:
	std::vector<VVMLObject*> ObjectList;

private:
	void SortVMLAstNode(std::vector<VVMLNode>& Nodes) {
		std::sort(Nodes.begin(), Nodes.end(), [](VVMLNode Left, VVMLNode Right) -> bool {
			return Left.ChildrenSequence < Right.ChildrenSequence;
			});
	}

public:
	VVMLWidget(int Width, int Height, VApplication* Parent, bool Sizble = true)
		: VMainWindow(Width, Height, Parent, Sizble) {    }

public:
	VVMLWidgetLoadResult LoadVML(VVMLParserResult VMLAstTree, VUIObject* UIParent = nullptr) {
		if (VMLAstTree.ParserStatus == VVMLParserStatus::Error) {
			std::wstring ASTError;

			for (auto& ErrorString : VMLAstTree.ErrorInfo) {
				ASTError.append(ErrorString.ErrorString + L"At Line (" + std::to_wstring(ErrorString.Line) + L")\n");
			}

			return { VVMLWidgetVMLLoadStats::InvalidAstTree, L"Received an Invalid Ast Tree\nAST Error : \n" + ASTError };
		}
		if (VMLAstTree.ParserStatus == VVMLParserStatus::Failed) {
			return { VVMLWidgetVMLLoadStats::InvalidAstTree, L"Received an Invalid Ast Tree\nAST Error : " + VMLAstTree.ErrorInfo[0].ErrorString };
		}


		auto LoadResult = LoadVML(VMLAstTree.Nodes, nullptr, UIParent);

		Update({ 0, 0, Surface()->Rect.GetWidth(), Surface()->Rect.GetHeight() });

		return LoadResult;
	}
	VVMLWidgetLoadResult LoadVML(std::map<std::wstring, VVMLNode> VMLAstTree, VVMLWidgetVMLObjectList* ObjectCacheList, VUIObject* UIParent = nullptr) {
		VVMLWidgetLoadResult Result;
		Result.Status = VVMLWidgetVMLLoadStats::Ok;

		if (UIParent == nullptr) {
			UIParent = this;
		}

		std::vector<VVMLNode> VMLAstOrderlyNodes;
		for (auto& Element : VMLAstTree) {
			VMLAstOrderlyNodes.push_back(Element.second);
		}

		std::sort(VMLAstOrderlyNodes.begin(), VMLAstOrderlyNodes.end(), [](VVMLNode Left, VVMLNode Right) -> bool {
			return Left.ChildrenSequence < Right.ChildrenSequence;
			});

		for (auto& Element : VMLAstOrderlyNodes) {
			std::wstring StyleSheetString;
			VVMLObject* VMLObject = new VVMLObject;
			VMLObject->VMLID = Element.NodeTag;

			if (Element.PropertyExsit(L"Type")) {
				VVMLPropertyValue ElementProperty = Element.GetProperty(L"Type");

				if (ElementProperty.PropertyType == VVMLPropertyType::StringValue) {
					if (ElementProperty.PropertyAsString == L"VPushButton") {
						VPushButton* PushButton = new VPushButton(UIParent);
						VMLObject->UIObject = PushButton;
						VMLObject->VMLObjectType = VVMLObjectType::PushButton;

						VVMLContronBuildStatus BuildStatus;
						VVMLVPushButtonBuilder Builder(PushButton, Element.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else if (ElementProperty.PropertyAsString == L"VImageLabel") {
						VImageLabel* ImageLabel = new VImageLabel(nullptr, UIParent);
						VMLObject->UIObject = ImageLabel;
						VMLObject->VMLObjectType = VVMLObjectType::ImageLabel;

						VVMLContronBuildStatus  BuildStatus;
						VVMLImageLabelBuilder   Builder(ImageLabel, Element.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else if (ElementProperty.PropertyAsString == L"VTextLabel") {
						VTextLabel* TextLabel = new VTextLabel(UIParent, L"");
						VMLObject->UIObject = TextLabel;
						VMLObject->VMLObjectType = VVMLObjectType::TextLabel;

						VVMLContronBuildStatus BuildStatus;
						VVMLTextLabelBuilder   Builder(TextLabel, Element.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else if (ElementProperty.PropertyAsString == L"VBlurLabel") {
						VBlurLabel* BlurLabel = new VBlurLabel(UIParent);
						VMLObject->UIObject = BlurLabel;
						VMLObject->VMLObjectType = VVMLObjectType::BlurLabel;

						VVMLContronBuildStatus BuildStatus;
						VVMLBlurLabelBuilder   Builder(BlurLabel, Element.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else if (ElementProperty.PropertyAsString == L"VIconButton") {
						VIconButton* IconButton = new VIconButton(UIParent);
						VMLObject->UIObject = IconButton;
						VMLObject->VMLObjectType = VVMLObjectType::IconButton;

						VVMLContronBuildStatus BuildStatus;
						VVMLIconButtonBuilder  Builder(IconButton, Element.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else if (ElementProperty.PropertyAsString == L"VLineEditor") {
						VLineEditor* LineEditor = new VLineEditor(UIParent);
						VMLObject->UIObject = LineEditor;
						VMLObject->VMLObjectType = VVMLObjectType::LineEditor;

						VVMLContronBuildStatus BuildStatus;
						VVMLLineEditorBuilder  Builder(LineEditor, Element.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else if (ElementProperty.PropertyAsString == L"VLayout") {
						VLayout* Layout;

						if (UIParent->GetParent()->IsWidget() == false) {
							Layout = new VLayout(UIParent, UIParent->GetParent());
						}
						else {
							Layout = new VLayout(UIParent, this);
						}

						VMLObject->UIObject = Layout;
						VMLObject->VMLObjectType = VVMLObjectType::Layout;

						VVMLContronBuildStatus BuildStatus;
						VVMLLayoutBuilder      Builder(Layout, Element.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else if (ElementProperty.PropertyAsString == L"VPositionAnimation") {
						VPositionAnimation* Animation = new VPositionAnimation(UIParent, 0, VInterpolatorType::AccelerateDecelerateInterpolator);
						VMLObject->UIObject = Animation;
						VMLObject->VMLObjectType = VVMLObjectType::PositionAnimation;

						VVMLContronBuildStatus       BuildStatus;
						VVMLPositionAnimationBuilder Builder(Animation, Element.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else if (ElementProperty.PropertyAsString == L"VGeomteryAnimation") {
						VGeomteryAnimation* Animation = new VGeomteryAnimation(UIParent, 0, VInterpolatorType::AccelerateDecelerateInterpolator);
						VMLObject->UIObject = Animation;
						VMLObject->VMLObjectType = VVMLObjectType::GeomteryAnimation;

						VVMLContronBuildStatus       BuildStatus;
						VVMLGeomteryAnimationBuilder Builder(Animation, Element.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else if (ElementProperty.PropertyAsString == L"VAlphaAnimation") {
						VAlphaAnimation* Animation = new VAlphaAnimation(UIParent, 0, VInterpolatorType::AccelerateDecelerateInterpolator);
						VMLObject->UIObject = Animation;
						VMLObject->VMLObjectType = VVMLObjectType::AlphaAnimation;

						VVMLContronBuildStatus       BuildStatus;
						VVMLAlphaAnimationBuilder Builder(Animation, Element.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else if (ElementProperty.PropertyAsString == L"VMainWindow") {
						VMLObject->UIObject = this;
						VMLObject->VMLObjectType = VVMLObjectType::MainWindow;

						VVMLContronBuildStatus BuildStatus;
						VVMLMainWindowBuilder  Builder(this, Element.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else if (ElementProperty.PropertyAsString == L"VViewLabel") {
						VViewLabel* ViewLabel = new VViewLabel(0, 0, UIParent);
						VMLObject->UIObject = ViewLabel;
						VMLObject->VMLObjectType = VVMLObjectType::ViewLabel;

						VVMLContronBuildStatus BuildStatus;
						VVMLViewLabelBuilder   Builder(ViewLabel, Element.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else {
						delete VMLObject;

						return { VVMLWidgetVMLLoadStats::Failed, L"Element \"" + Element.NodeTag + L"\" Don't Owns an Valid Type" };
					}
				}
				else {
					delete VMLObject;

					return { VVMLWidgetVMLLoadStats::Failed, L"Element Type Must Use String Value" };
				}

				VVMLWidgetVMLObjectList* ChildList = new VVMLWidgetVMLObjectList;
				LoadVML(Element.ChildrenNodes, ChildList, VMLObject->UIObject);

				for (auto& ChildObject : ChildList->Objects) {
					VMLObject->ChildrenObjects.push_back(ChildObject);
				}
			}
			if (Element.PropertyExsit(L"Id")) {
				VVMLPropertyValue ElementId = Element.GetProperty(L"Id");

				if (ElementId.PropertyType == VVMLPropertyType::StringValue) {
					VMLObject->VMLDOMID = ElementId.PropertyAsString;
				}
				else {
					delete VMLObject;

					return { VVMLWidgetVMLLoadStats::Failed, L"Id Must Use String Value" };
				}
			}
			if (Element.PropertyExsit(L"Class")) {
				VVMLPropertyValue ElementId = Element.GetProperty(L"Class");

				if (ElementId.PropertyType == VVMLPropertyType::StringValue) {
					VMLObject->VMLClass = ElementId.PropertyAsString;
				}
				else {
					delete VMLObject;

					return { VVMLWidgetVMLLoadStats::Failed, L"Class Must Use String Value" };
				}
			}
			if (Element.PropertyExsit(L"StyleSheet")) {
				VVMLPropertyValue ElementStyleSheet = Element.GetProperty(L"StyleSheet");

				if (ElementStyleSheet.PropertyType == VVMLPropertyType::StringValue) {
					StyleSheetString.append(ElementStyleSheet.PropertyAsString + L"\n");
				}
				else {
					delete VMLObject;

					return { VVMLWidgetVMLLoadStats::Failed, L"Class Must Use String Value" };
				}
			}
			if (Element.PropertyExsit(L"StyleSheetFile")) {
				VVMLPropertyValue ElementStyleSheet = Element.GetProperty(L"StyleSheetFile");

				if (ElementStyleSheet.PropertyType == VVMLPropertyType::StringValue &&
					VVMLParserHelper::FileExist(ElementStyleSheet.PropertyAsString) == true) {
					StyleSheetString.append(VVMLParserHelper::ReadFromFile(ElementStyleSheet.PropertyAsString) + L"\n");
				}
				else {
					delete VMLObject;

					return { VVMLWidgetVMLLoadStats::Failed, L"Class Must Use String Value" };
				}
			}

			if (ObjectCacheList != nullptr) {
				ObjectCacheList->Objects.push_back(VMLObject);

				if (StyleSheetString.empty() == false) {
					VSSParser Parser(StyleSheetString);
					SetStyleSheet(Parser.ParseVSS(), ObjectCacheList->Objects);
				}
			}
			else {
				ObjectList.push_back(VMLObject);

				if (StyleSheetString.empty() == false) {
					VSSParser Parser(StyleSheetString);
					SetStyleSheet(Parser.ParseVSS(), ObjectList);
				}
			}
		}

		return Result;
	}

public:
	void SetStyleSheet(VSSParserResult VSSParserResult, std::vector<VVMLObject*> List) {
		for (auto& Selector : VSSParserResult.SelectorSet) {
			if (Selector->GetType() == VVSSSelectorType::ElementSelector ||
				Selector->GetType() == VVSSSelectorType::FakeClassSelector) {
				std::wstring ElementTag = static_cast<VVSSElementSelector*>(Selector)->ElementTag;

				if (ElementTag == L"vpushbutton") {
					VPushButtonTheme* PushButtonTheme = new VPushButtonTheme(*(static_cast<VPushButtonTheme*>(SearchThemeFromParent(VPUSHBUTTON_THEME))));

					VVSSVPushButtonBuilder Builder(nullptr, std::vector<VVSSBasicSelector*>{Selector}, PushButtonTheme);

					static_cast<VApplication*>(Parent())->SetTheme(PushButtonTheme);
				}
				if (ElementTag == L"viconbutton") {
					VIconButtonTheme* IconButtonTheme = new VIconButtonTheme(*(static_cast<VIconButtonTheme*>(SearchThemeFromParent(VICONBUTTON_THEME))));

					VVSSVIconButtonBuilder Builder(nullptr, std::vector<VVSSBasicSelector*>{Selector}, IconButtonTheme);

					static_cast<VApplication*>(Parent())->SetTheme(IconButtonTheme);
				}
				if (ElementTag == L"vimagelabel") {
					VImageLabelTheme* ImageLabelTheme = new VImageLabelTheme(*(static_cast<VImageLabelTheme*>(SearchThemeFromParent(VIMAGELABEL_THEME))));

					VVSSVImageLabelBuilder Builder(nullptr, std::vector<VVSSBasicSelector*>{Selector}, ImageLabelTheme);

					static_cast<VApplication*>(Parent())->SetTheme(ImageLabelTheme);
				}
				if (ElementTag == L"vlineeditor") {
					VLineEditorTheme* LineEditorTheme = new VLineEditorTheme(*(static_cast<VLineEditorTheme*>(SearchThemeFromParent(VLINEEDITOR_THEME))));

					VVSSVLineEditorBuilder Builder(nullptr, std::vector<VVSSBasicSelector*>{Selector}, LineEditorTheme);

					static_cast<VApplication*>(Parent())->SetTheme(LineEditorTheme);
				}
				if (ElementTag == L"vtextlabel") {
					VTextLabelTheme* TextLabelTheme = new VTextLabelTheme(*(static_cast<VTextLabelTheme*>(SearchThemeFromParent(VTEXTLABEL_THEME))));

					VVSSVTextLabelBuilder Builder(nullptr, std::vector<VVSSBasicSelector*>{Selector}, TextLabelTheme);

					static_cast<VApplication*>(Parent())->SetTheme(TextLabelTheme);
				}
				if (ElementTag == L"vviewlabel") {
					VViewLabelTheme* ViewLabelTheme = new VViewLabelTheme(*(static_cast<VViewLabelTheme*>(SearchThemeFromParent(VVIEWLABEL_THEME))));

					VVSSVViewLabelBuilder Builder(nullptr, std::vector<VVSSBasicSelector*>{Selector}, ViewLabelTheme);

					static_cast<VApplication*>(Parent())->SetTheme(ViewLabelTheme);
				}
			}

			for (auto& Object : List) {
				Object->SetStyleSheet(Selector);
			}
		}
	}
	void SetStyleSheet(VSSParserResult VSSParserResult) {
		SetStyleSheet(VSSParserResult, ObjectList);

		for (int Count = 0; Count < VSSParserResult.SelectorSet.size(); ++Count) {
			delete VSSParserResult.SelectorSet.at(Count);
		}

		VSSParserResult.SelectorSet.clear();
	}

public:
	VVMLFinder Get(std::wstring ChildrenId) {
		for (auto& Object : ObjectList) {
			if (Object->VMLID == ChildrenId) {
				return VVMLFinder(Object, Object->ChildrenObjects);
			}
		}

		return VVMLFinder(nullptr, std::vector<VVMLObject*>());
	}

	VVMLFinder operator[](std::wstring ChildrenId) {
		for (auto& Object : ObjectList) {
			if (Object->VMLID == ChildrenId) {
				return VVMLFinder(Object, Object->ChildrenObjects);
			}
		}

		return VVMLFinder(nullptr, std::vector<VVMLObject*>());
	}
};

VLIB_END_NAMESPACE