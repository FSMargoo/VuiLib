/*
 * VVMLWidget.hpp
 *	@description : The VML Widget of VLib
 *	@birth		 : 2022/6.4
*/

#pragma once

#include "../../control/basic/VBasicControl/vwidget.hpp"

#include "vvmlparser.hpp"
#include "vxmlbuilder.hpp"

VLIB_BEGIN_NAMESPACE

struct VVMLObject {
	VUIObject* UIObject = nullptr;
	std::wstring VMLID;

	std::vector<VVMLObject*> ChildrenObjects;
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

	void ReleaseVMLObjectList(std::vector<VVMLObject*> GCList) {
		for (auto& Item : GCList) {
			delete Item;
		}

		GCList.clear();
	}

public:
	VVMLWidget(int Width, int Height, VApplication* Parent, bool Sizble = true)
		: VMainWindow(Width, Height, Parent, Sizble) {    }

public:
	VVMLWidgetLoadResult LoadVML(VVMLParserResult VMLAstTree, VUIObject* UIParent = nullptr) {
		if (VMLAstTree.ParserStatus == VVMLParserStatus::Error) {
			std::wstring ASTError;

			for (auto& ErrorString : VMLAstTree.ErrorInfo) {
				ASTError.append(ErrorString.ErrorString + L"At Line (" + std::to_wstring(ErrorString.Line)) + L")\n";
			}

			return { VVMLWidgetVMLLoadStats::InvalidAstTree, L"Recived an Invalid Ast Tree\nAST Error : \n" + ASTError };
		}
		if (VMLAstTree.ParserStatus == VVMLParserStatus::Failed) {
			return { VVMLWidgetVMLLoadStats::InvalidAstTree, L"Recived an Invalid Ast Tree\nAST Error : " + VMLAstTree.ErrorInfo[0].ErrorString	};
		}


		return LoadVML(VMLAstTree.Nodes, nullptr, UIParent);
	}
	VVMLWidgetLoadResult LoadVML(std::map<std::wstring, VVMLNode> VMLAstTree, VVMLWidgetVMLObjectList* ObjectCacheList, VUIObject* UIParent = nullptr) {
		if (ObjectList.empty() == false) {
			ReleaseVMLObjectList(ObjectList);
		}

		VVMLWidgetLoadResult Result;
		Result.Status = VVMLWidgetVMLLoadStats::Ok;

		if (UIParent == nullptr) {
			UIParent = this;
		}

		for (auto& Element : VMLAstTree) {
			VVMLObject* VMLObject = new VVMLObject;
			VMLObject->VMLID = Element.first;

			if (Element.second.PropertyExsit(L"Type")) {
				VVMLPropertyValue ElementProperty = Element.second.GetProperty(L"Type");

				if (ElementProperty.PropertyType == VVMLPropertyType::StringValue) {
					if (ElementProperty.PropertyAsString == L"VPushButton") {
						VPushButton* PushButton = new VPushButton(UIParent);
						VMLObject->UIObject = PushButton;

						VVMLContronBuildStatus BuildStatus;
						VVMLVPushButtonBuilder Builder(PushButton, Element.second.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Builed Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					if (ElementProperty.PropertyAsString == L"VImageLabel") {
						VImageLabel* ImageLabel = new VImageLabel(nullptr, UIParent);
						VMLObject->UIObject = ImageLabel;

						VVMLContronBuildStatus  BuildStatus;
						VVMLImageLabelBuilder   Builder(ImageLabel, Element.second.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Builed Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					if (ElementProperty.PropertyAsString == L"VTextLabel") {
						VTextLabel* TextLabel = new VTextLabel(UIParent, L"");
						VMLObject->UIObject = TextLabel;

						VVMLContronBuildStatus BuildStatus;
						VVMLTextLabelBuilder   Builder(TextLabel, Element.second.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Builed Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					if (ElementProperty.PropertyAsString == L"VBlurLabel") {
						VBlurLabel* BlurLabel = new VBlurLabel(UIParent);
						VMLObject->UIObject = BlurLabel;

						VVMLContronBuildStatus BuildStatus;
						VVMLBlurLabelBuilder   Builder(BlurLabel, Element.second.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Builed Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					if (ElementProperty.PropertyAsString == L"VIconButton") {
						VIconButton* IconButton = new VIconButton(UIParent);
						VMLObject->UIObject = IconButton;

						VVMLContronBuildStatus BuildStatus;
						VVMLIconButtonBuilder  Builder(IconButton, Element.second.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Builed Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					if (ElementProperty.PropertyAsString == L"VLineEditor") {
						VLineEditor* LineEditor = new VLineEditor(UIParent);
						VMLObject->UIObject = LineEditor;

						VVMLContronBuildStatus BuildStatus;
						VVMLLineEditorBuilder  Builder(LineEditor, Element.second.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Builed Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					if (ElementProperty.PropertyAsString == L"VLayout") {
						VLayout* Layout = new VLayout(UIParent, this);
						VMLObject->UIObject = Layout;

						VVMLContronBuildStatus BuildStatus;
						VVMLLayoutBuilder      Builder(Layout, Element.second.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Builed Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					if (ElementProperty.PropertyAsString == L"VMainWindow") {
						VMLObject->UIObject = this;

						VVMLContronBuildStatus BuildStatus;
						VVMLMainWindowBuilder  Builder(this, Element.second.NodeValue, &BuildStatus);

						if (BuildStatus.BuildStatusCode != VVMLControlBuildResultStatus::Ok) {
							Result.Status = VVMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID + L"] Builed Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
				}
				else {
					delete VMLObject;

					return { VVMLWidgetVMLLoadStats::Failed, L"Element Type Must Use String Value" };
				}

				VVMLWidgetVMLObjectList* ChildList = new VVMLWidgetVMLObjectList;
				LoadVML(Element.second.ChildrenNodes, ChildList, VMLObject->UIObject);

				for (auto& ChildObject : ChildList->Objects) {
					VMLObject->ChildrenObjects.push_back(ChildObject);
				}
				if (ObjectCacheList != nullptr) {
					ObjectCacheList->Objects.push_back(VMLObject);
				}

				else {
					ObjectList.push_back(VMLObject);
				}

			}
			else {
				delete VMLObject;

				return { VVMLWidgetVMLLoadStats::Failed, L"Element \"" + Element.first + L"\" Dont Owns an Valid Type" };
			}
		}

		return Result;
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