#pragma once

#include "../core/vbasiccore.h"

#include "../core/control/vlayout.h"
#include "../core/control/vtextlabel.h"
#include "../core/control/vradiobutton.h"
#include "../core/control/vimagelabel.h"
#include "../core/control/vpushbutton.h"
#include "../core/control/vscrollbar.h"
#include "../core/control/vblurlabel.h"
#include "../core/control/viconbutton.h"
#include "../core/control/vlineeditor.h"
#include "../core/control/vscroller.h"
#include "../core/control/vviewlabel.h"

#include "../core/animation/vanimation.h"

#include "../core/widget/vwidget.h"

#include "vmlparser.h"
#include "vml.h"

#include <map>

VLIB_BEGIN_NAMESPACE

namespace VML {
    enum class VMLControlBuildResultStatus {
        Ok, Failed
    };

    struct VMLControlBuildStatus {
        VMLControlBuildResultStatus BuildStatusCode = VMLControlBuildResultStatus::Ok;
        std::wstring				 FailedReason;
    };

    class VMLCommonBuilder {
    protected:
        bool CheckNativeCallParameter(std::vector<VMLPropertyValue> Properties,
                                      std::vector<VMLPropertyType>  PropertiesType);

    protected:
        const VMLFinder& VMLObjectFinder;

    protected:
        void Builder(Core::VUIObject* Object,
                     const int& X, const int& Y,
                     const int& Width,
                     const int& Height,
                     const int& Transparency,
                     const bool& Visble,
                     const Core::VLayoutMode& VerticalLayoutMode,
                     const Core::VLayoutMode& HorizontalLayoutMode,
                     const double& VerticalLayoutPercent, const double& HorizontalLayoutPercent,
                     const int& RelativeX, const int& RelativeY,
                     const int& XMiddleOffset, const int& YMiddleOffset,
                     const double& WidthRatio, const double& HeightRatio,
                     const VMLFinder& RootFinder);

        virtual void AnalyzeProperty(const VMLFinder& RootFinder, Core::VUIObject* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                     VMLControlBuildStatus* BuildStatus);

    public:
        VMLCommonBuilder(const VMLFinder& RootFinder, Core::VUIObject* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                          VMLControlBuildStatus* BuildStatus) : VMLObjectFinder(RootFinder) {
            AnalyzeProperty(RootFinder, Object, PropertyValueList, BuildStatus);
        }
    };
    class VMLPushButtonBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VPushButton* PushButton, const std::wstring& PlaneText, const int& TextSize = 0);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VPushButton* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLPushButtonBuilder(const VMLFinder& RootFinder, Core::VPushButton* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                               VMLControlBuildStatus* BuildStatus);
    };
    class VMLImageLabelBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VImageLabel* ImageLabel, Core::VImage* Image);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VImageLabel* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLImageLabelBuilder(const VMLFinder& RootFinder, Core::VImageLabel* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                              VMLControlBuildStatus* BuildStatus);
    };
    class VMLTextLabelBuilder : public VMLCommonBuilder {
    private:
        Core::VFontAlignment ConvertAlignment(std::wstring AlignmentString);
        Core::VFontParagraphAlignment ConvertParagraphAlignment(std::wstring ParagraphString);

    protected:
        void Builder(Core::VTextLabel* TextLabel, const std::wstring& PlaneText, const int& TextSize, const Core::VFontAlignment& Alignment, const Core::VFontParagraphAlignment& LineAlignment, const bool& AutoSize);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VTextLabel* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLTextLabelBuilder(const VMLFinder& RootFinder, Core::VTextLabel* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);
    };
    class VMLLayoutBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VLayout* Layout, Core::VLayoutMode VerticalLayoutMode, Core::VLayoutMode HorizontalLayoutMode,
                     double VerticalLayoutPercent, double HorziontalLayoutPercent,
                     int RelativeX, int RelativeY, int XMiddleOffset, int YMiddleOffset);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VLayout* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLLayoutBuilder(const VMLFinder& RootFinder, Core::VLayout* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                          VMLControlBuildStatus* BuildStatus);
    };
    class VMLScaleLayoutBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VScaleLayout* Layout, const double& ScaleWidthPercent, const double& ScaleHeightPercent);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VScaleLayout* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLScaleLayoutBuilder(const VMLFinder& RootFinder, Core::VScaleLayout* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                VMLControlBuildStatus* BuildStatus);
    };
    class VMLRadioButtonBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VRadioButton* RadioButton, const bool& Status);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VRadioButton* RadioButton, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLRadioButtonBuilder(const VMLFinder& RootFinder, Core::VRadioButton* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                              VMLControlBuildStatus* BuildStatus);
    };
    class VMLSliderHorizontalBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VSliderHorizontal* SliderHorizontal, const double& Value);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VSliderHorizontal* Horizontal, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLSliderHorizontalBuilder(const VMLFinder& RootFinder, Core::VSliderHorizontal* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                              VMLControlBuildStatus* BuildStatus);
    };
    class VMLSliderVerticalBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VSliderVertical* SliderHorizontal, const double& Value);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VSliderVertical* Vertical, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLSliderVerticalBuilder(const VMLFinder& RootFinder, Core::VSliderVertical* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                   VMLControlBuildStatus* BuildStatus);
    };
    class VMLIconButtonBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VIconButton* IconButton, Core::VImage* Image);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VIconButton* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLIconButtonBuilder(const VMLFinder& RootFinder, Core::VIconButton* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                 VMLControlBuildStatus* BuildStatus);
    };
    class VMLLineEditorBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VLineEditor* LineEditor, std::wstring PlaneText);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VLineEditor* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLLineEditorBuilder(const VMLFinder& RootFinder, Core::VLineEditor* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);
    };
    class VMLVerticalScrollerBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VScrollerVertical* Scroller, const int& Value, const int& ViewValue);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VScrollerVertical* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLVerticalScrollerBuilder(const VMLFinder& RootFinder, Core::VScrollerVertical* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);
    };
    class VMLHorizontalScrollerBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VScrollerHorizontal* Scroller, const int& Value, const int& ViewValue);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VScrollerHorizontal* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLHorizontalScrollerBuilder(const VMLFinder& RootFinder, Core::VScrollerHorizontal* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                   VMLControlBuildStatus* BuildStatus);
    };

    class VMLViewLabelBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VViewLabel* ViewLabel, const int& ViewWidth, const int& ViewHeight,
                     const Core::VViewLabelVerticalAlign& VerticalAlign, const Core::VViewLabelHorizontalAlign& HorizontalAlign,
                     const bool& TieWheel);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VViewLabel* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLViewLabelBuilder(const VMLFinder& RootFinder, Core::VViewLabel* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                     VMLControlBuildStatus* BuildStatus);
    };

    class VMLGeometryAnimationBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VGeometryAnimation* Animation, const Core::VPoint& TargetPoint, const Core::VAnimationCurveFlag& Flag, const int& Duration);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VGeometryAnimation* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLGeometryAnimationBuilder(const VMLFinder& RootFinder, Core::VGeometryAnimation* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                            VMLControlBuildStatus* BuildStatus);
    };
    class VMLPositionAnimationBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VPositionAnimation* Animation, const Core::VPoint& TargetPoint, const Core::VAnimationCurveFlag& Flag, const int& Duration);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VPositionAnimation* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLPositionAnimationBuilder(const VMLFinder& RootFinder, Core::VPositionAnimation* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                    VMLControlBuildStatus* BuildStatus);
    };
    class VMLOpacityAnimationBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VOpacityAnimation* Animation, const int& TargetValue, const Core::VAnimationCurveFlag& Flag, const int& Duration);

        void AnalyzeProperty(const VMLFinder& RootFinder, Core::VOpacityAnimation* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLOpacityAnimationBuilder(const VMLFinder& RootFinder, Core::VOpacityAnimation* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                    VMLControlBuildStatus* BuildStatus);
    };

    class VMLMainWindowBuilder {
    protected:
        void Builder(Core::VMainWindow *MainWindow, const int &Width, const int &Height,
                     const std::wstring &Title, const bool &Sizble, const bool &FramelessStatus);

        void AnalyzeProperty(Core::VMainWindow* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLMainWindowBuilder(Core::VMainWindow* MainWindow, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                              VMLControlBuildStatus* BuildStatus);
    };
    
}

VLIB_END_NAMESPACE