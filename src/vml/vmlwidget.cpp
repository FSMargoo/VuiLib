#include "../../include/vml/vmlwidget.h"

#include <direct.h>

VLIB_BEGIN_NAMESPACE

namespace VML
{
void VMLMainWindow::SortVMLAstNode(std::vector<VMLNode> &Nodes)
{
	std::sort(Nodes.begin(), Nodes.end(),
			  [](VMLNode Left, VMLNode Right) -> bool { return Left.ChildrenSequence < Right.ChildrenSequence; });
}
VMLMainWindow::VMLMainWindow(Core::VApplication *Parent, const bool &Sizble) : VMainWindow(0, 0, Parent, Sizble)
{
}
VMLMainWindow::VMLMainWindow(const int &Width, const int &Height, Core::VApplication *Parent, const bool &Sizble)
	: VMainWindow(Width, Height, Parent, Sizble)
{
}
VMLMainWindow::~VMLMainWindow()
{
	for (auto &MetaFunction : MetaFunctionList)
	{
		delete MetaFunction.second;
	}

	MetaFunctionList.clear();

	VUIObject::~VUIObject();
}
VMLWidgetLoadResult VMLMainWindow::LoadVML(const VString VML, VMLParserParseMode StringMode)
{
	VMLParser Parser(VML, StringMode);

	return LoadVML(Parser.ParseVML());
}

VMLFinder VMLMainWindow::GetRootFinder()
{
	return VMLFinder(nullptr, ObjectList, &VariableList);
}

VMLWidgetLoadResult VMLMainWindow::LoadVML(VMLParserResult VMLAstTree, VMLObject *UIParent)
{
	if (VMLAstTree.ParserStatus == VMLParserStatus::Error)
	{
		VString ASTError;

		for (auto &ErrorString : VMLAstTree.ErrorInfo)
		{
			ASTError.append(ErrorString.ErrorString + L"At Line (" + std::to_wstring(ErrorString.Line) + L")\n");
		}

		return {VMLWidgetVMLLoadStats::InvalidAstTree, L"Received an Invalid Ast Tree\nAST Error : \n" + ASTError};
	}
	if (VMLAstTree.ParserStatus == VMLParserStatus::Failed)
	{
		return {VMLWidgetVMLLoadStats::InvalidAstTree,
				L"Received an Invalid Ast Tree\nAST Error : " + VMLAstTree.ErrorInfo[0].ErrorString};
	}

	auto LoadResult = LoadVML(VMLAstTree.Nodes, nullptr, UIParent);

	Update({0, 0, GetWidth(), GetHeight()});

	return LoadResult;
}
VMLWidgetLoadResult VMLMainWindow::LoadVML(std::map<VString, VMLNode> VMLAstTree,
										   VMLWidgetVMLObjectList *ObjectCacheList, VMLObject *UIParent)
{
	VMLWidgetLoadResult Result;
	Result.Status				 = VMLWidgetVMLLoadStats::Ok;
	bool AlreadyCreateMainWindow = false;

	if (UIParent == nullptr)
	{
		UIParent = new VMLObject;

		UIParent->UIObject = this;
	}

	std::vector<VMLNode> VMLAstOrderlyNodes;
	for (auto &Element : VMLAstTree)
	{
		VMLAstOrderlyNodes.push_back(Element.second);
	}

	std::sort(VMLAstOrderlyNodes.begin(), VMLAstOrderlyNodes.end(),
			  [](VMLNode Left, VMLNode Right) -> bool { return Left.ChildrenSequence < Right.ChildrenSequence; });

	for (auto &Element : VMLAstOrderlyNodes)
	{
		VString	   StyleSheetString;
		VMLObject *VMLObject = new struct VMLObject;
		VMLObject->VMLID	 = Element.NodeTag;

		if (Element.PropertyExsit(L"type"))
		{
			VMLPropertyValue ElementProperty = Element.GetProperty(L"type");

			if (ElementProperty.PropertyType == VMLPropertyType::StringValue)
			{
				if (ElementProperty.PropertyAsString == L"pushbutton")
				{
					Core::VPushButton *PushButton = new Core::VPushButton(UIParent->UIObject);
					VMLObject->UIObject			  = PushButton;
					VMLObject->VMLType			  = VMLObjectType::PushButton;

					VMLControlBuildStatus BuildStatus;
					VMLPushButtonBuilder  Builder(GetRootFinder(), PushButton, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"on-clicked"))
					{
						if (Element.NodeValue[L"on-clicked"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"on-clicked"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								PushButton->ButtonPushed.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
																 &Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								PushButton->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								PushButton->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								PushButton->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															&Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"lost-focus"))
					{
						if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								PushButton->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															  &Core::VSignal<>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"imagelabel")
				{
					Core::VImageLabel *ImageLabel = new Core::VImageLabel(0, 0, nullptr, UIParent->UIObject);
					VMLObject->UIObject			  = ImageLabel;
					VMLObject->VMLType			  = VMLObjectType::ImageLabel;

					VMLControlBuildStatus BuildStatus;
					VMLImageLabelBuilder  Builder(GetRootFinder(), ImageLabel, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								ImageLabel->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								ImageLabel->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								ImageLabel->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															&Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"lost-focus"))
					{
						if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								ImageLabel->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															  &Core::VSignal<>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"textlabel")
				{
					Core::VTextLabel *TextLabel = new Core::VTextLabel(UIParent->UIObject);
					VMLObject->UIObject			= TextLabel;
					VMLObject->VMLType			= VMLObjectType::TextLabel;

					VMLControlBuildStatus BuildStatus;
					VMLTextLabelBuilder	  Builder(GetRootFinder(), TextLabel, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								TextLabel->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								TextLabel->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								TextLabel->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
														   &Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"lost-focus"))
					{
						if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								TextLabel->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															 &Core::VSignal<>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"fake-caption")
				{
					Core::VFakeCaption *FakeCaption = new Core::VFakeCaption(UIParent->UIObject);
					VMLObject->UIObject				= FakeCaption;
					VMLObject->VMLType				= VMLObjectType::FakeCaption;

					VMLControlBuildStatus BuildStatus;
					VMLCommonBuilder	  Builder(GetRootFinder(), FakeCaption, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								FakeCaption->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								FakeCaption->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								FakeCaption->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															 &Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"lost-focus"))
					{
						if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								FakeCaption->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															   &Core::VSignal<>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"layout")
				{
					if (UIParent->UIObject->GetParent()->IsApplication() == true)
					{
						continue;
					}

					Core::VLayout *Layout = new Core::VLayout(UIParent->UIObject, UIParent->UIObject->GetParent());
					VMLObject->UIObject	  = Layout;
					VMLObject->VMLType	  = VMLObjectType::Layout;

					VMLControlBuildStatus BuildStatus;
					VMLLayoutBuilder	  Builder(GetRootFinder(), Layout, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Layout->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Layout->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Layout->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
														&Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"lost-focus"))
					{
						if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Layout->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
														  &Core::VSignal<>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"scalelayout")
				{
					if (UIParent->UIObject->GetParent()->IsApplication() == true)
					{
						continue;
					}

					Core::VScaleLayout *Layout =
						new Core::VScaleLayout(UIParent->UIObject, UIParent->UIObject->GetParent());
					VMLObject->UIObject = Layout;
					VMLObject->VMLType	= VMLObjectType::Layout;

					VMLControlBuildStatus BuildStatus;
					VMLScaleLayoutBuilder Builder(GetRootFinder(), Layout, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Layout->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Layout->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Layout->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
														&Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"lost-focus"))
					{
						if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Layout->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
														  &Core::VSignal<>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"text-size-layout")
				{
					if (UIParent->UIObject->GetParent()->IsApplication() == true)
					{
						continue;
					}

					if (UIParent->VMLType == VMLObjectType::TextLabel)
					{
						Core::VTextSizeLayout *Layout = new Core::VTextSizeLayout(
							static_cast<Core::VTextLabel *>(UIParent->UIObject), UIParent->UIObject->GetParent());
						VMLObject->UIObject = Layout;
						VMLObject->VMLType	= VMLObjectType::Layout;

						VMLControlBuildStatus	 BuildStatus;
						VMLTextSizeLayoutBuilder Builder(GetRootFinder(), Layout, Element.NodeValue, &BuildStatus);

						if (Element.PropertyExsit(L"rectangle-changed"))
						{
							if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									Layout->Resized.Connect(
										(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
										&Core::VSignal<const int &, const int &>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"position-changed"))
						{
							if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									Layout->Moved.Connect(
										(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
										&Core::VSignal<const int &, const int &>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"got-focus"))
						{
							if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									Layout->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															&Core::VSignal<>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"lost-focus"))
						{
							if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									Layout->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															  &Core::VSignal<>::Emit);
								}
							}
						}

						if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
						{
							Result.Status		 = VMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
												   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"\'text-size-layout\' "
											   L"should match parent \'text-label\'\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"horizontal-slider")
				{
					Core::VSliderHorizontal *SliderHorizontal = new Core::VSliderHorizontal(UIParent->UIObject);
					VMLObject->UIObject						  = SliderHorizontal;
					VMLObject->VMLType						  = VMLObjectType::HorizontalSlider;

					VMLControlBuildStatus	   BuildStatus;
					VMLSliderHorizontalBuilder Builder(GetRootFinder(), SliderHorizontal, Element.NodeValue,
													   &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								SliderHorizontal->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								SliderHorizontal->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								SliderHorizontal->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
																  &Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"lost-focus"))
					{
						if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								SliderHorizontal->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
																	&Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"value-changed"))
					{
						if (Element.NodeValue[L"value-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"value-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								SliderHorizontal->ValueChanged.Connect(
									(Core::VSignal<const double &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const double &>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"vertical-slider")
				{
					Core::VSliderVertical *SliderHorizontal = new Core::VSliderVertical(UIParent->UIObject);
					VMLObject->UIObject						= SliderHorizontal;
					VMLObject->VMLType						= VMLObjectType::VerticalSlider;

					VMLControlBuildStatus	 BuildStatus;
					VMLSliderVerticalBuilder Builder(GetRootFinder(), SliderHorizontal, Element.NodeValue,
													 &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								SliderHorizontal->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								SliderHorizontal->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								SliderHorizontal->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
																  &Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"lost-focus"))
					{
						if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								SliderHorizontal->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
																	&Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"value-changed"))
					{
						if (Element.NodeValue[L"value-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"value-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								SliderHorizontal->ValueChanged.Connect(
									(Core::VSignal<const double &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const double &>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"mainwindow")
				{
					this->Show();
					auto ScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
					auto ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

					VMLObject->UIObject = this;
					VMLObject->VMLType	= VMLObjectType::MainWindow;

					AlreadyCreateMainWindow = true;

					VMLControlBuildStatus BuildStatus;
					VMLMainWindowBuilder  Builder(this, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								this->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"file-drag"))
					{
						if (Element.NodeValue[L"file-drag"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"file-drag"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								this->FileOnDrag.Connect(
									(Core::VSignal<std::vector<VString>> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<std::vector<VString>>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								this->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								this->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
													  &Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"lost-focus"))
					{
						if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								this->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
														&Core::VSignal<>::Emit);
							}
						}
					}

					MoveWindow(this->CallWidgetGetHWND(), ScreenWidth / 2 - GetWidth() / 2,
							   ScreenHeight / 2 - GetHeight() / 2, GetWidth(), GetHeight(), FALSE);

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"radiobutton")
				{
					if (UIParent->VMLType != VMLObjectType::SwitchGroup)
					{
						Core::VRadioButton *RadioButton = new Core::VRadioButton(UIParent->UIObject);
						VMLObject->UIObject				= RadioButton;
						VMLObject->VMLType				= VMLObjectType::RadioButton;

						VMLControlBuildStatus BuildStatus;
						VMLRadioButtonBuilder Builder(GetRootFinder(), RadioButton, Element.NodeValue, &BuildStatus);

						if (Element.PropertyExsit(L"rectangle-changed"))
						{
							if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									RadioButton->Resized.Connect(
										(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
										&Core::VSignal<const int &, const int &>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"position-changed"))
						{
							if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									RadioButton->Moved.Connect(
										(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
										&Core::VSignal<const int &, const int &>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"got-focus"))
						{
							if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									RadioButton->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
																 &Core::VSignal<>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"lost-focus"))
						{
							if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									RadioButton->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
																   &Core::VSignal<>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"on-clicked"))
						{
							if (Element.NodeValue[L"on-clicked"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"on-clicked"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									RadioButton->ButtonPushed.Connect(
										(Core::VSignal<> *)MetaFunctionList[NativeCallName], &Core::VSignal<>::Emit);
								}
							}
						}

						if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
						{
							Result.Status		 = VMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
												   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else
					{
						Core::VRadioButton *RadioButton = new Core::VRadioButton(UIParent->UIObject->GetParent());
						VMLObject->UIObject				= RadioButton;
						VMLObject->VMLType				= VMLObjectType::RadioButton;

						((Core::VSwitchGroup *)UIParent->UIObject)->AddObject(RadioButton);

						VMLControlBuildStatus BuildStatus;
						VMLRadioButtonBuilder Builder(GetRootFinder(), RadioButton, Element.NodeValue, &BuildStatus);

						if (Element.PropertyExsit(L"rectangle-changed"))
						{
							if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									RadioButton->Resized.Connect(
										(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
										&Core::VSignal<const int &, const int &>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"position-changed"))
						{
							if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									RadioButton->Moved.Connect(
										(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
										&Core::VSignal<const int &, const int &>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"got-focus"))
						{
							if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									RadioButton->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
																 &Core::VSignal<>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"lost-focus"))
						{
							if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									RadioButton->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
																   &Core::VSignal<>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"on-clicked"))
						{
							if (Element.NodeValue[L"on-clicked"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"on-clicked"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									RadioButton->ButtonPushed.Connect(
										(Core::VSignal<> *)MetaFunctionList[NativeCallName], &Core::VSignal<>::Emit);
								}
							}
						}

						if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
						{
							Result.Status		 = VMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
												   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
				}
				else if (ElementProperty.PropertyAsString == L"blurlabel")
				{
					Core::VBlurLabel *BlurLabel = new Core::VBlurLabel(UIParent->UIObject);

					VMLObject->UIObject = BlurLabel;
					VMLObject->VMLType	= VMLObjectType::BlurLabel;

					VMLControlBuildStatus BuildStatus;
					VMLCommonBuilder	  Builder(GetRootFinder(), BlurLabel, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								BlurLabel->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								BlurLabel->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								BlurLabel->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
														   &Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"lost-focus"))
					{
						if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								BlurLabel->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															 &Core::VSignal<>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"iconbutton")
				{
					Core::VIconButton *IconButton = new Core::VIconButton(UIParent->UIObject);

					VMLObject->UIObject = IconButton;
					VMLObject->VMLType	= VMLObjectType::IconButton;

					VMLControlBuildStatus BuildStatus;
					VMLIconButtonBuilder  Builder(GetRootFinder(), IconButton, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								IconButton->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								IconButton->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								IconButton->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															&Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"lost-focus"))
					{
						if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								IconButton->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															  &Core::VSignal<>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"lineeditor")
				{
					Core::VLineEditor *LineEditor = new Core::VLineEditor(UIParent->UIObject);

					VMLObject->UIObject = LineEditor;
					VMLObject->VMLType	= VMLObjectType::LineEditor;

					VMLControlBuildStatus BuildStatus;
					VMLLineEditorBuilder  Builder(GetRootFinder(), LineEditor, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								LineEditor->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								LineEditor->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								LineEditor->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															&Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"lost-focus"))
					{
						if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								LineEditor->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															  &Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"inputed"))
					{
						if (Element.NodeValue[L"inputed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"inputed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								LineEditor->TextOnChange.Connect(
									(Core::VSignal<const VString &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const VString &>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"lineeditor")
				{
					Core::VLineEditor *LineEditor = new Core::VLineEditor(UIParent->UIObject);

					VMLObject->UIObject = LineEditor;
					VMLObject->VMLType	= VMLObjectType::LineEditor;

					VMLControlBuildStatus BuildStatus;
					VMLLineEditorBuilder  Builder(GetRootFinder(), LineEditor, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								LineEditor->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								LineEditor->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								LineEditor->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															&Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"lost-focus"))
					{
						if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								LineEditor->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															  &Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"inputed"))
					{
						if (Element.NodeValue[L"inputed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"inputed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								LineEditor->TextOnChange.Connect(
									(Core::VSignal<const VString &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const VString &>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"lineeditor")
				{
					Core::VLineEditor *LineEditor = new Core::VLineEditor(UIParent->UIObject);

					VMLObject->UIObject = LineEditor;
					VMLObject->VMLType	= VMLObjectType::LineEditor;

					VMLControlBuildStatus BuildStatus;
					VMLLineEditorBuilder  Builder(GetRootFinder(), LineEditor, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								LineEditor->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								LineEditor->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								LineEditor->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															&Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"lost-focus"))
					{
						if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								LineEditor->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															  &Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"inputed"))
					{
						if (Element.NodeValue[L"inputed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"inputed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								LineEditor->TextOnChange.Connect(
									(Core::VSignal<const VString &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const VString &>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"editor")
				{
					Core::VEditor *Editor = new Core::VEditor(UIParent->UIObject);

					VMLObject->UIObject = Editor;
					VMLObject->VMLType	= VMLObjectType::Editor;

					VMLControlBuildStatus BuildStatus;
					VMLEditorBuilder	  Builder(GetRootFinder(), Editor, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Editor->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Editor->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Editor->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
														&Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"lost-focus"))
					{
						if (Element.NodeValue[L"lost-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"lost-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Editor->LostFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
														  &Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"inputed"))
					{
						if (Element.NodeValue[L"inputed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"inputed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Editor->TextOnChange.Connect(
									(Core::VSignal<const VString &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const VString &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"check-input"))
					{
						if (Element.NodeValue[L"check-input"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"check-input"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Editor->CheckInput.Connect(
									(Core::VSignal<const wchar_t &, bool *> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const wchar_t &, bool *>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"before-input"))
					{
						if (Element.NodeValue[L"check-input"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"before-input"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Editor->TextBeforeChange.Connect(
									(Core::VSignal<const VString &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const VString &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"new-char"))
					{
						if (Element.NodeValue[L"new-char"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"new-char"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Editor->PushNewCharacter.Connect(
									(Core::VSignal<const wchar_t &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const wchar_t &>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"switchgroup")
				{
					Core::VSwitchGroup *SwitchGroup = new Core::VSwitchGroup(UIParent->UIObject);

					VMLObject->UIObject = SwitchGroup;
					VMLObject->VMLType	= VMLObjectType::SwitchGroup;
				}
				else if (ElementProperty.PropertyAsString == L"vertical-scroller")
				{
					Core::VScrollerVertical *Widget = new Core::VScrollerVertical(UIParent->UIObject);

					VMLObject->UIObject = Widget;
					VMLObject->VMLType	= VMLObjectType::Widget;

					VMLControlBuildStatus	   BuildStatus;
					VMLVerticalScrollerBuilder Builder(GetRootFinder(), Widget, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Widget->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Widget->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Widget->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
														&Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"value-changed"))
					{
						if (Element.NodeValue[L"value-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"value-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Widget->ValueOnChange.Connect(
									(Core::VSignal<const double &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const double &>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"horizontal-scroller")
				{
					Core::VScrollerHorizontal *Widget = new Core::VScrollerHorizontal(UIParent->UIObject);

					VMLObject->UIObject = Widget;
					VMLObject->VMLType	= VMLObjectType::Widget;

					VMLControlBuildStatus		 BuildStatus;
					VMLHorizontalScrollerBuilder Builder(GetRootFinder(), Widget, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Widget->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Widget->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Widget->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
														&Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"value-changed"))
					{
						if (Element.NodeValue[L"value-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"value-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Widget->ValueOnChange.Connect(
									(Core::VSignal<const double &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const double &>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"viewlabel")
				{
					Core::VViewLabel *ViewLabel = new Core::VViewLabel(UIParent->UIObject);

					VMLObject->UIObject = ViewLabel;
					VMLObject->VMLType	= VMLObjectType::ViewLabel;

					VMLControlBuildStatus BuildStatus;
					VMLViewLabelBuilder	  Builder(GetRootFinder(), ViewLabel, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								ViewLabel->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								ViewLabel->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								ViewLabel->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
														   &Core::VSignal<>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"canvas")
				{
					Core::VCanvas *Canvas = new Core::VCanvas(UIParent->UIObject);

					VMLObject->UIObject = Canvas;
					VMLObject->VMLType	= VMLObjectType::Canvas;

					VMLControlBuildStatus BuildStatus;
					VMLCanvasBuilder	  Builder(GetRootFinder(), Canvas, Element.NodeValue, &BuildStatus);

					if (Element.PropertyExsit(L"rectangle-changed"))
					{
						if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Canvas->Resized.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"position-changed"))
					{
						if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Canvas->Moved.Connect(
									(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
									&Core::VSignal<const int &, const int &>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"got-focus"))
					{
						if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Canvas->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
														&Core::VSignal<>::Emit);
							}
						}
					}
					if (Element.PropertyExsit(L"on-paint"))
					{
						if (Element.NodeValue[L"on-paint"].PropertyType == VMLPropertyType::NativeCall)
						{
							auto NativeCallName = Element.NodeValue[L"on-paint"].NativeCallMethodName;

							if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
							{
								Canvas->OnPaintSignal.Connect(
									(Core::VSignal<Core::VCanvasPainter *, const Core::VRenderHandle &> *)
										MetaFunctionList[NativeCallName],
									&Core::VSignal<Core::VCanvasPainter *, const Core::VRenderHandle &>::Emit);
							}
						}
					}

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"geometry-animation")
				{
					Core::VGeometryAnimation *Animation =
						new Core::VGeometryAnimation(UIParent->UIObject, nullptr, 0.f);

					VMLObject->UIObject = Animation;
					VMLObject->VMLType	= VMLObjectType::GeometryAnimation;

					VMLControlBuildStatus		BuildStatus;
					VMLGeometryAnimationBuilder Builder(GetRootFinder(), Animation, Element.NodeValue, &BuildStatus);

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"position-animation")
				{
					Core::VPositionAnimation *Animation =
						new Core::VPositionAnimation(UIParent->UIObject, nullptr, 0.f);

					VMLObject->UIObject = Animation;
					VMLObject->VMLType	= VMLObjectType::PositionAnimation;

					VMLControlBuildStatus		BuildStatus;
					VMLPositionAnimationBuilder Builder(GetRootFinder(), Animation, Element.NodeValue, &BuildStatus);

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"opacity-animation")
				{
					Core::VOpacityAnimation *Animation = new Core::VOpacityAnimation(UIParent->UIObject, nullptr, 0.f);

					VMLObject->UIObject = Animation;
					VMLObject->VMLType	= VMLObjectType::OpacityAnimation;

					VMLControlBuildStatus	   BuildStatus;
					VMLOpacityAnimationBuilder Builder(GetRootFinder(), Animation, Element.NodeValue, &BuildStatus);

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"circle-view")
				{
					Core::VCircleView *CircleView = new Core::VCircleView(UIParent->UIObject);

					VMLObject->UIObject = CircleView;
					VMLObject->VMLType	= VMLObjectType::CircleView;

					VMLControlBuildStatus BuildStatus;
					VMLCommonBuilder	  Builder(GetRootFinder(), CircleView, Element.NodeValue, &BuildStatus);

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"polygon-view")
				{
					Core::VPolygonView *PolygonView = new Core::VPolygonView(UIParent->UIObject);

					VMLObject->UIObject = PolygonView;
					VMLObject->VMLType	= VMLObjectType::PolygonView;

					VMLControlBuildStatus BuildStatus;
					VMLPolygonViewBuilder Builder(GetRootFinder(), PolygonView, Element.NodeValue, &BuildStatus);

					if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
					{
						Result.Status		 = VMLWidgetVMLLoadStats::Failed;
						Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
											   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

						return Result;
					}
				}
				else if (ElementProperty.PropertyAsString == L"widget")
				{
					if (AlreadyCreateMainWindow)
					{
						Core::VWidget *Widget =
							new Core::VWidget(0, 0, static_cast<Core::VApplication *>(GetParent()), L"WidgetFromVML");

						VMLObject->UIObject = Widget;
						VMLObject->VMLType	= VMLObjectType::Widget;

						VMLControlBuildStatus BuildStatus;
						VMLWidgetBuilder	  Builder(Widget, Element.NodeValue, &BuildStatus);

						if (Element.PropertyExsit(L"rectangle-changed"))
						{
							if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									Widget->Resized.Connect(
										(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
										&Core::VSignal<const int &, const int &>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"file-drag"))
						{
							if (Element.NodeValue[L"file-drag"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"file-drag"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									Widget->FileOnDrag.Connect(
										(Core::VSignal<std::vector<VString>> *)MetaFunctionList[NativeCallName],
										&Core::VSignal<std::vector<VString>>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"position-changed"))
						{
							if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									Widget->Moved.Connect(
										(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
										&Core::VSignal<const int &, const int &>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"got-focus"))
						{
							if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									Widget->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															&Core::VSignal<>::Emit);
								}
							}
						}

						if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
						{
							Result.Status		 = VMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
												   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else if (UIParent->VMLType == VMLObjectType::MainWindow)
					{
						Core::VWidget *Widget = new Core::VWidget(
							0, 0, static_cast<Core::VMainWindow *>(UIParent->UIObject), L"WidgetFromVML");

						VMLObject->UIObject = Widget;
						VMLObject->VMLType	= VMLObjectType::Widget;

						VMLControlBuildStatus BuildStatus;
						VMLWidgetBuilder	  Builder(Widget, Element.NodeValue, &BuildStatus);

						if (Element.PropertyExsit(L"rectangle-changed"))
						{
							if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									Widget->Resized.Connect(
										(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
										&Core::VSignal<const int &, const int &>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"position-changed"))
						{
							if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									Widget->Moved.Connect(
										(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
										&Core::VSignal<const int &, const int &>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"got-focus"))
						{
							if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									Widget->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															&Core::VSignal<>::Emit);
								}
							}
						}

						if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
						{
							Result.Status		 = VMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
												   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
					else if (UIParent->VMLType == VMLObjectType::Widget)
					{
						Core::VWidget *Widget =
							new Core::VWidget(0, 0, static_cast<Core::VWidget *>(GetParent()), L"WidgetFromVML");

						VMLObject->UIObject = Widget;
						VMLObject->VMLType	= VMLObjectType::Widget;

						VMLControlBuildStatus BuildStatus;
						VMLWidgetBuilder	  Builder(Widget, Element.NodeValue, &BuildStatus);

						if (Element.PropertyExsit(L"rectangle-changed"))
						{
							if (Element.NodeValue[L"rectangle-changed"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"rectangle-changed"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									Widget->Resized.Connect(
										(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
										&Core::VSignal<const int &, const int &>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"position-changed"))
						{
							if (Element.NodeValue[L"position-changed"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"position-changed"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									Widget->Moved.Connect(
										(Core::VSignal<const int &, const int &> *)MetaFunctionList[NativeCallName],
										&Core::VSignal<const int &, const int &>::Emit);
								}
							}
						}
						if (Element.PropertyExsit(L"got-focus"))
						{
							if (Element.NodeValue[L"got-focus"].PropertyType == VMLPropertyType::NativeCall)
							{
								auto NativeCallName = Element.NodeValue[L"got-focus"].NativeCallMethodName;

								if (MetaFunctionList.find(NativeCallName) != MetaFunctionList.end())
								{
									Widget->InFocus.Connect((Core::VSignal<> *)MetaFunctionList[NativeCallName],
															&Core::VSignal<>::Emit);
								}
							}
						}

						if (BuildStatus.BuildStatusCode != VMLControlBuildResultStatus::Ok)
						{
							Result.Status		 = VMLWidgetVMLLoadStats::Failed;
							Result.FailedMessage = L"In Control VMLID[" + VMLObject->VMLID +
												   L"] Build Failed, Reason : \"" + BuildStatus.FailedReason + L"\"";

							return Result;
						}
					}
				}
				else
				{
					VUIObject			 *Object = nullptr;
					VMLControlBuildStatus BuildStatus;

					bool Flag = false;

					for (auto &UserFunction : UserDefineList)
					{
						if (UserFunction(ElementProperty.PropertyAsString, GetRootFinder(), &Object, UIParent->UIObject,
										 Element.NodeValue, &BuildStatus))
						{
							Flag = true;

							VMLObject->UIObject = Object;
							VMLObject->VMLType	= VMLObjectType::UserDefine;

							break;
						}
					}

					if (!Flag)
					{
						delete VMLObject;

						return {VMLWidgetVMLLoadStats::Failed,
								L"Element \"" + Element.NodeTag + L"\" Don't Owns an Valid Type"};
					}
				}
			}
			else
			{
				delete VMLObject;

				return {VMLWidgetVMLLoadStats::Failed, L"Element Type Must Use String Value"};
			}

			VMLWidgetVMLObjectList *ChildList = new VMLWidgetVMLObjectList;
			LoadVML(Element.ChildrenNodes, ChildList, VMLObject);

			for (auto &ChildObject : ChildList->Objects)
			{
				VMLObject->ChildrenObjects.push_back(ChildObject);
			}
		}
		if (Element.PropertyExsit(L"id"))
		{
			VMLPropertyValue ElementId = Element.GetProperty(L"id");

			if (ElementId.PropertyType == VMLPropertyType::StringValue)
			{
				VMLObject->VMLDOMID = ElementId.PropertyAsString;
			}
			else
			{
				delete VMLObject;

				return {VMLWidgetVMLLoadStats::Failed, L"id must use string value"};
			}
		}
		if (Element.PropertyExsit(L"class"))
		{
			VMLPropertyValue ElementId = Element.GetProperty(L"class");

			if (ElementId.PropertyType == VMLPropertyType::StringValue)
			{
				VMLObject->VMLClass = ElementId.PropertyAsString;
			}
			else
			{
				delete VMLObject;

				return {VMLWidgetVMLLoadStats::Failed, L"class must use string value"};
			}
		}
		if (Element.PropertyExsit(L"style-sheet"))
		{
			VMLPropertyValue ElementStyleSheet = Element.GetProperty(L"style-sheet");

			if (ElementStyleSheet.PropertyType == VMLPropertyType::StringValue)
			{
				StyleSheetString.append(ElementStyleSheet.PropertyAsString + L"\n");
			}
			else
			{
				delete VMLObject;

				return {VMLWidgetVMLLoadStats::Failed, L"style-sheet must use string value"};
			}
		}
		if (Element.PropertyExsit(L"style-sheet-src"))
		{
			VMLPropertyValue ElementStyleSheet = Element.GetProperty(L"style-sheet-src");

			if (ElementStyleSheet.PropertyType == VMLPropertyType::StringValue &&
				VKits::VParserHelper::FileExist(ElementStyleSheet.PropertyAsString))
			{
				StyleSheetString.append(VKits::VParserHelper::ReadFromFile(ElementStyleSheet.PropertyAsString) + L"\n");
			}
			else
			{
				delete VMLObject;

				return {VMLWidgetVMLLoadStats::Failed,
						L"style-sheet-src must use string value (or value dosen't exsits)"};
			}
		}

		if (ObjectCacheList != nullptr)
		{
			ObjectCacheList->Objects.push_back(VMLObject);

			if (!StyleSheetString.empty())
			{
				VSS::VSSParser Parser(StyleSheetString);
				SetStyleSheet(Parser.ParseVSS(), ObjectCacheList->Objects);
			}
		}
		else
		{
			ObjectList.push_back(VMLObject);

			if (!StyleSheetString.empty())
			{
				VSS::VSSParser Parser(StyleSheetString);
				SetStyleSheet(Parser.ParseVSS(), ObjectList);
			}
		}

		if (VMLObject->VMLType != VMLObjectType::Widget && VMLObject->VMLType != VMLObjectType::MainWindow &&
			VMLObject->VMLType != VMLObjectType::SwitchGroup)
		{
			VMLObject->VMLNativeLaytout		 = (Core::VLayout *)VMLObject->UIObject->GetChildObjectByPosition(0);
			VMLObject->VMLNativeScaleLaytout = (Core::VScaleLayout *)VMLObject->UIObject->GetChildObjectByPosition(1);
		}
	}

	return Result;
}
void VMLMainWindow::SetStyleSheet(VSS::VSSParserResult VSSParserResult, std::vector<VMLObject *> List)
{
	for (auto &Selector : VSSParserResult.SelectorSet)
	{
		if (Selector->GetType() == VSS::VSSSelectorType::ElementSelector ||
			Selector->GetType() == VSS::VSSSelectorType::FakeClassSelector)
		{
			VString ElementTag = static_cast<VSS::VSSElementSelector *>(Selector)->ElementTag;

			if (ElementTag == L"pushbutton")
			{
				Core::VPushButtonTheme *PushButtonTheme = new Core::VPushButtonTheme(
					*(static_cast<Core::VPushButtonTheme *>(GetTargetTheme(Core::VUIThemeType::VPushButton))));

				VSS::VSSVPushButtonBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector *>{Selector},
												   PushButtonTheme);

				static_cast<Core::VApplication *>(GetParent())->SetTheme(PushButtonTheme);
			}
			if (ElementTag == L"imagelabel")
			{
				Core::VImageLabelTheme *ImageLabelTheme = new Core::VImageLabelTheme(
					*(static_cast<Core::VImageLabelTheme *>(GetTargetTheme(Core::VUIThemeType::VImageLabel))));

				VSS::VSSVImageLabelBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector *>{Selector},
												   ImageLabelTheme);

				static_cast<Core::VApplication *>(GetParent())->SetTheme(ImageLabelTheme);
			}
			if (ElementTag == L"textlabel")
			{
				Core::VTextLabelTheme *TextLabelTheme = new Core::VTextLabelTheme(
					*(static_cast<Core::VTextLabelTheme *>(GetTargetTheme(Core::VUIThemeType::VTextLabel))));

				VSS::VSSVTextLabelBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector *>{Selector},
												  TextLabelTheme);

				static_cast<Core::VApplication *>(GetParent())->SetTheme(TextLabelTheme);
			}
			if (ElementTag == L"radiobutton")
			{
				Core::VRadioButtonTheme *TextLabelTheme = new Core::VRadioButtonTheme(
					*(static_cast<Core::VRadioButtonTheme *>(GetTargetTheme(Core::VUIThemeType::VRadioButton))));

				VSS::VSSVRadioButtonBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector *>{Selector},
													TextLabelTheme);

				static_cast<Core::VApplication *>(GetParent())->SetTheme(TextLabelTheme);
			}
			if (ElementTag == L"horizontal-slider")
			{
				Core::VSliderTheme *SliderTheme = new Core::VSliderTheme(
					*(static_cast<Core::VSliderTheme *>(GetTargetTheme(Core::VUIThemeType::VSlider))));

				VSS::VSSVSliderHorizontalBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector *>{Selector},
														 SliderTheme);

				static_cast<Core::VApplication *>(GetParent())->SetTheme(SliderTheme);
			}
			if (ElementTag == L"vertical-slider")
			{
				Core::VSliderTheme *SliderTheme = new Core::VSliderTheme(
					*(static_cast<Core::VSliderTheme *>(GetTargetTheme(Core::VUIThemeType::VSlider))));

				VSS::VSSVSliderHorizontalBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector *>{Selector},
														 SliderTheme);

				static_cast<Core::VApplication *>(GetParent())->SetTheme(SliderTheme);
			}
			if (ElementTag == L"horizontal-scroller")
			{
				Core::VViewScrollerTheme *SliderTheme = new Core::VViewScrollerTheme(
					*(static_cast<Core::VViewScrollerTheme *>(GetTargetTheme(Core::VUIThemeType::VViewLabel))));

				VSS::VSSHorizontalScrollerBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector *>{Selector},
														  SliderTheme);

				static_cast<Core::VApplication *>(GetParent())->SetTheme(SliderTheme);
			}
			if (ElementTag == L"vertical-scroller")
			{
				Core::VViewScrollerTheme *SliderTheme = new Core::VViewScrollerTheme(
					*(static_cast<Core::VViewScrollerTheme *>(GetTargetTheme(Core::VUIThemeType::VViewLabel))));

				VSS::VSSHorizontalScrollerBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector *>{Selector},
														  SliderTheme);

				static_cast<Core::VApplication *>(GetParent())->SetTheme(SliderTheme);
			}
			if (ElementTag == L"view-label")
			{
				Core::VViewLabelTheme *SliderTheme = new Core::VViewLabelTheme(
					*(static_cast<Core::VViewLabelTheme *>(GetTargetTheme(Core::VUIThemeType::VViewLabel))));

				VSS::VSSViewLabelBuilder Builder(nullptr, std::vector<VSS::VSSBasicSelector *>{Selector}, SliderTheme);

				static_cast<Core::VApplication *>(GetParent())->SetTheme(SliderTheme);
			}
		}

		for (auto &Object : List)
		{
			Object->SetStyleSheet(Selector);
		}
	}
}
void VMLMainWindow::SetStyleSheet(VSS::VSSParserResult VSSParserResult)
{
	SetStyleSheet(VSSParserResult, ObjectList);

	for (int Count = 0; Count < VSSParserResult.SelectorSet.size(); ++Count)
	{
		delete VSSParserResult.SelectorSet.at(Count);
	}

	VSSParserResult.SelectorSet.clear();
}
VMLFinder VMLMainWindow::Get(const VString &ChildrenId)
{
	for (auto &Object : ObjectList)
	{
		if (Object->VMLID == ChildrenId)
		{
			return VMLFinder(Object, Object->ChildrenObjects, &VariableList);
		}
	}

	return VMLFinder(nullptr, std::vector<VMLObject *>(), &VariableList);
}
VMLFinder VMLMainWindow::operator[](const VString &ChildrenId)
{
	for (auto &Object : ObjectList)
	{
		if (Object->VMLID == ChildrenId)
		{
			return VMLFinder(Object, Object->ChildrenObjects, &VariableList);
		}
	}

	return VMLFinder(nullptr, std::vector<VMLObject *>(), &VariableList);
}

HWND VMLMainWindow::GetLocalWinId()
{
	return Core::VMainWindow::CallWidgetGetHWND();
}

void VMLMainWindow::PushUserDefineFunction(
	std::function<bool(const VString &, const VMLFinder &RootFinder, Core::VUIObject **, Core::VUIObject *,
					   std::map<VString, VMLPropertyValue> &, VMLControlBuildStatus *)>
		FunctionObject)
{
	UserDefineList.push_back(FunctionObject);
}
} // namespace VML

VLIB_END_NAMESPACE