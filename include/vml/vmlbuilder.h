#pragma once

#include "../core/vbasiccore.h"

#include "../core/control/vlayout.h"
#include "../core/control/vtextlabel.h"
#include "../core/control/vradiobutton.h"
#include "../core/control/vimagelabel.h"
#include "../core/control/vpushbutton.h"

#include "../core/widget/vwidget.h"

#include "vmlparser.h"

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
                     const double& WidthRatio, const double& HeightRatio);

        virtual void AnalyzeProperty(Core::VUIObject* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                                     VMLControlBuildStatus* BuildStatus);

    public:
        VMLCommonBuilder(Core::VUIObject* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                          VMLControlBuildStatus* BuildStatus) {
            AnalyzeProperty(Object, PropertyValueList, BuildStatus);
        }
    };
    class VMLPushButtonBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VPushButton* PushButton, const std::wstring& PlaneText, const int& TextSize = 0);

        void AnalyzeProperty(Core::VPushButton* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLPushButtonBuilder(Core::VPushButton* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                               VMLControlBuildStatus* BuildStatus);
    };
    class VMLImageLabelBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VImageLabel* ImageLabel, Core::VImage* Image);

        void AnalyzeProperty(Core::VImageLabel* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLImageLabelBuilder(Core::VImageLabel* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                              VMLControlBuildStatus* BuildStatus);
    };
    class VMLTextLabelBuilder : public VMLCommonBuilder {
    private:
        Core::VFontAlignment ConvertAlignment(std::wstring AlignmentString);
        Core::VFontParagraphAlignment ConvertParagraphAlignment(std::wstring ParagraphString);

    protected:
        void Builder(Core::VTextLabel* TextLabel, const std::wstring& PlaneText, const int& TextSize, const Core::VFontAlignment& Alignment, const Core::VFontParagraphAlignment& LineAlignment);

        void AnalyzeProperty(Core::VTextLabel* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLTextLabelBuilder(Core::VTextLabel* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);
    };
    class VMLLayoutBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VLayout* Layout, Core::VLayoutMode VerticalLayoutMode, Core::VLayoutMode HorizontalLayoutMode,
                     double VerticalLayoutPercent, double HorziontalLayoutPercent,
                     int RelativeX, int RelativeY, int XMiddleOffset, int YMiddleOffset);

        void AnalyzeProperty(Core::VLayout* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLLayoutBuilder(Core::VLayout* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                          VMLControlBuildStatus* BuildStatus);
    };
    class VMLScaleLayoutBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VScaleLayout* Layout, const double& ScaleWidthPercent, const double& ScaleHeightPercent);

        void AnalyzeProperty(Core::VScaleLayout* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLScaleLayoutBuilder(Core::VScaleLayout* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                VMLControlBuildStatus* BuildStatus);
    };
    class VMLRadioButtonBuilder : public VMLCommonBuilder {
    protected:
        void Builder(Core::VRadioButton* RadioButton, const bool& Status);

        void AnalyzeProperty(Core::VRadioButton* RadioButton, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLRadioButtonBuilder(Core::VRadioButton* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                              VMLControlBuildStatus* BuildStatus);
    };

    class VMLMainWindowBuilder {
    protected:
        void Builder(Core::VMainWindow* MainWindow, const int& Width, const int& Height, const std::wstring& Title, const bool& Sizble);

        void AnalyzeProperty(Core::VMainWindow* Object, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                             VMLControlBuildStatus* BuildStatus);

    public:
        VMLMainWindowBuilder(Core::VMainWindow* MainWindow, std::map<std::wstring, VMLPropertyValue>& PropertyValueList,
                              VMLControlBuildStatus* BuildStatus);
    };
    
}

VLIB_END_NAMESPACE