#pragma once

#include "../core/animation/vanimation.h"
#include "../core/control/vblurlabel.h"
#include "../core/control/vcanvas.h"
#include "../core/control/vdropdown.h"
#include "../core/control/veditor.h"
#include "../core/control/viconbutton.h"
#include "../core/control/vimagelabel.h"
#include "../core/control/vlayout.h"
#include "../core/control/vlineeditor.h"
#include "../core/control/vpushbutton.h"
#include "../core/control/vradiobutton.h"
#include "../core/control/vscrollbar.h"
#include "../core/control/vscroller.h"
#include "../core/control/vtextlabel.h"
#include "../core/control/vviewlabel.h"
#include "../core/vbasiccore.h"
#include "../core/visual/vvisual.h"
#include "../core/widget/vwidget.h"
#include "vml.h"
#include "vmlparser.h"
#include <map>

VLIB_BEGIN_NAMESPACE

namespace VML
{
enum class VMLControlBuildResultStatus
{
	Ok,
	Failed
};

struct VMLControlBuildStatus
{
	VMLControlBuildResultStatus BuildStatusCode = VMLControlBuildResultStatus::Ok;
	VString						FailedReason;
};

class VMLCommonBuilder
{
protected:
	template <class DatatType, class ClassType>
	ClassType *DefineVariable(Core::VUIObject *ObjectInstance, const DatatType &InitValue, const VMLFinder &Finder,
							  const VString &VariableName)
	{
		ClassType *Variable = new ClassType(ObjectInstance, InitValue);

		return Variable;
	}

protected:
	bool CheckNativeCallParameter(std::vector<VMLPropertyValue> Properties,
								  std::vector<VMLPropertyType>	PropertiesType);

protected:
	const VMLFinder &VMLObjectFinder;

protected:
	void Builder(Core::VUIObject *Object, const int &X, const int &Y, const int &Width, const int &Height,
				 const int &Transparency, const bool &Visble, const Core::VLayoutMode &VerticalLayoutMode,
				 const Core::VLayoutMode &HorizontalLayoutMode, const double &VerticalLayoutPercent,
				 const double &HorizontalLayoutPercent, const int &RelativeX, const int &RelativeY,
				 const int &XMiddleOffset, const int &YMiddleOffset, const double &WidthRatio,
				 const double &HeightRatio, const bool &ViewOnly, const VMLFinder &RootFinder);

	virtual void AnalyzeProperty(const VMLFinder &RootFinder, Core::VUIObject *Object,
								 std::map<VString, VMLPropertyValue> &PropertyValueList,
								 VMLControlBuildStatus				 *BuildStatus);

public:
	VMLCommonBuilder(const VMLFinder &RootFinder, Core::VUIObject *Object,
					 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus)
		: VMLObjectFinder(RootFinder)
	{
		AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
	}
};
class VMLPushButtonBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VPushButton *PushButton, const VString &PlainText, const int &TextSize = 0);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VPushButton *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLPushButtonBuilder(const VMLFinder &RootFinder, Core::VPushButton *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);
};
class VMLImageLabelBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VImageLabel *ImageLabel, Core::VImage *Image, bool AutoSize, bool LockHeight);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VImageLabel *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLImageLabelBuilder(const VMLFinder &RootFinder, Core::VImageLabel *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);
};
class VMLTextLabelBuilder : public VMLCommonBuilder
{
private:
	Core::VFontAlignment		  ConvertAlignment(VString AlignmentString);
	Core::VFontParagraphAlignment ConvertParagraphAlignment(VString ParagraphString);

protected:
	void Builder(Core::VTextLabel *TextLabel, const VString &PlainText, const int &TextSize,
				 const Core::VFontAlignment &Alignment, const Core::VFontParagraphAlignment &LineAlignment,
				 const bool &AutoSize);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VTextLabel *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLTextLabelBuilder(const VMLFinder &RootFinder, Core::VTextLabel *Object,
						std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);
};
class VMLLayoutBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VLayout *Layout, Core::VLayoutMode VerticalLayoutMode, Core::VLayoutMode HorizontalLayoutMode,
				 double VerticalLayoutPercent, double HorziontalLayoutPercent, int RelativeX, int RelativeY,
				 int XMiddleOffset, int YMiddleOffset);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VLayout *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLLayoutBuilder(const VMLFinder &RootFinder, Core::VLayout *Object,
					 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);
};
class VMLScaleLayoutBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VScaleLayout *Layout, const double &ScaleWidthPercent, const double &ScaleHeightPercent);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VScaleLayout *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLScaleLayoutBuilder(const VMLFinder &RootFinder, Core::VScaleLayout *Object,
						  std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);
};
class VMLTextSizeLayoutBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VTextSizeLayout *Layout, const double &Scale);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VTextSizeLayout *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLTextSizeLayoutBuilder(const VMLFinder &RootFinder, Core::VTextSizeLayout *Object,
							 std::map<VString, VMLPropertyValue> &PropertyValueList,
							 VMLControlBuildStatus				 *BuildStatus);
};
class VMLRadioButtonBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VRadioButton *RadioButton, const bool &Status, const VString &PlainText, const bool &LockBack);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VRadioButton *RadioButton,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLRadioButtonBuilder(const VMLFinder &RootFinder, Core::VRadioButton *Object,
						  std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);
};
class VMLSliderHorizontalBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VSliderHorizontal *SliderHorizontal, const double &Value, const bool &Draggable);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VSliderHorizontal *Horizontal,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLSliderHorizontalBuilder(const VMLFinder &RootFinder, Core::VSliderHorizontal *Object,
							   std::map<VString, VMLPropertyValue> &PropertyValueList,
							   VMLControlBuildStatus			   *BuildStatus);
};
class VMLSliderVerticalBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VSliderVertical *SliderHorizontal, const double &Value, const bool &Draggable);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VSliderVertical *Vertical,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLSliderVerticalBuilder(const VMLFinder &RootFinder, Core::VSliderVertical *Object,
							 std::map<VString, VMLPropertyValue> &PropertyValueList,
							 VMLControlBuildStatus				 *BuildStatus);
};
class VMLIconButtonBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VIconButton *IconButton, Core::VImage *Image);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VIconButton *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLIconButtonBuilder(const VMLFinder &RootFinder, Core::VIconButton *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);
};
class VMLLineEditorBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VLineEditor *LineEditor, const VString &PlainText, const VString &LeadText);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VLineEditor *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLLineEditorBuilder(const VMLFinder &RootFinder, Core::VLineEditor *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);
};
class VMLEditorBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VEditor *Editor, const VString &PlainText, const VString &LeadingText, const int &DeltaY,
				 const bool &AllowEdit, const bool &AllowDragFontSizeChange, const bool &AllowOperationBack,
				 const int &MaxOperationCache, const int &LeftMargin, const int &TopMargin);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VEditor *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLEditorBuilder(const VMLFinder &RootFinder, Core::VEditor *Object,
					 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);
};
class VMLVerticalScrollerBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VScrollerVertical *Scroller, const int &Value, const int &ViewValue);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VScrollerVertical *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLVerticalScrollerBuilder(const VMLFinder &RootFinder, Core::VScrollerVertical *Object,
							   std::map<VString, VMLPropertyValue> &PropertyValueList,
							   VMLControlBuildStatus			   *BuildStatus);
};
class VMLHorizontalScrollerBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VScrollerHorizontal *Scroller, const int &Value, const int &ViewValue);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VScrollerHorizontal *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLHorizontalScrollerBuilder(const VMLFinder &RootFinder, Core::VScrollerHorizontal *Object,
								 std::map<VString, VMLPropertyValue> &PropertyValueList,
								 VMLControlBuildStatus				 *BuildStatus);
};

class VMLViewLabelBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VViewLabel *ViewLabel, const int &ViewWidth, const int &ViewHeight,
				 const Core::VViewLabelVerticalAlign   &VerticalAlign,
				 const Core::VViewLabelHorizontalAlign &HorizontalAlign, const bool &TieWheel);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VViewLabel *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLViewLabelBuilder(const VMLFinder &RootFinder, Core::VViewLabel *Object,
						std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);
};

class VMLGeometryAnimationBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VGeometryAnimation *Animation, const Core::VPoint &TargetPoint,
				 const Core::VAnimationCurveFlag &Flag, const int &Duration);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VGeometryAnimation *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLGeometryAnimationBuilder(const VMLFinder &RootFinder, Core::VGeometryAnimation *Object,
								std::map<VString, VMLPropertyValue> &PropertyValueList,
								VMLControlBuildStatus				*BuildStatus);
};
class VMLPositionAnimationBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VPositionAnimation *Animation, const Core::VPoint &TargetPoint,
				 const Core::VAnimationCurveFlag &Flag, const int &Duration);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VPositionAnimation *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLPositionAnimationBuilder(const VMLFinder &RootFinder, Core::VPositionAnimation *Object,
								std::map<VString, VMLPropertyValue> &PropertyValueList,
								VMLControlBuildStatus				*BuildStatus);
};

class VMLOpacityAnimationBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VOpacityAnimation *Animation, const int &TargetValue, const Core::VAnimationCurveFlag &Flag,
				 const int &Duration);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VOpacityAnimation *Object,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLOpacityAnimationBuilder(const VMLFinder &RootFinder, Core::VOpacityAnimation *Object,
							   std::map<VString, VMLPropertyValue> &PropertyValueList,
							   VMLControlBuildStatus			   *BuildStatus);
};

class VMLPolygonViewBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VPolygonView *PolygonView, const std::vector<Core::VPointF> &Points);

	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VPolygonView *PolygonView,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLPolygonViewBuilder(const VMLFinder &RootFinder, Core::VPolygonView *PolygonView,
						  std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);
};

class VMLDropDownBuilder : public VMLCommonBuilder
{
public:
	VMLDropDownBuilder(const VMLFinder &RootFinder, Core::VDropDown *DropDown,
					   std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);
};

class VMLMainWindowBuilder
{
protected:
	void Builder(Core::VMainWindow *MainWindow, const int &Width, const int &Height, const VString &Title,
				 const bool &Sizable, const bool &FramelessStatus, const bool &BorderlessStatus,
				 const bool &AllowFileDrag);

	void AnalyzeProperty(Core::VMainWindow *Object, std::map<VString, VMLPropertyValue> &PropertyValueList,
						 VMLControlBuildStatus *BuildStatus);

public:
	VMLMainWindowBuilder(Core::VMainWindow *MainWindow, std::map<VString, VMLPropertyValue> &PropertyValueList,
						 VMLControlBuildStatus *BuildStatus);
};

class VMLWidgetBuilder
{
protected:
	void Builder(Core::VWidget *Widget, const int &Width, const int &Height, const VString &Title, const bool &Sizable,
				 const bool &FramelessStatus, const bool &Visible);
	void AnalyzeProperty(Core::VWidget *Widget, std::map<VString, VMLPropertyValue> &PropertyValueList,
						 VMLControlBuildStatus *BuildStatus);

public:
	VMLWidgetBuilder(Core::VWidget *Widget, std::map<VString, VMLPropertyValue> &PropertyValueList,
					 VMLControlBuildStatus *BuildStatus);
};
class VMLCanvasBuilder : public VMLCommonBuilder
{
protected:
	void Builder(Core::VCanvas *Canvas, const int &Fps);
	void AnalyzeProperty(const VMLFinder &RootFinder, Core::VCanvas *Canvas,
						 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);

public:
	VMLCanvasBuilder(const VMLFinder &RootFinder, Core::VCanvas *Canvas,
					 std::map<VString, VMLPropertyValue> &PropertyValueList, VMLControlBuildStatus *BuildStatus);
};
} // namespace VML

VLIB_END_NAMESPACE