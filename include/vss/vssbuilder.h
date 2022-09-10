#pragma once

#include "vssparser.h"

#include "../core/control/vlayout.h"
#include "../core/control/vtextlabel.h"
#include "../core/control/vimagelabel.h"
#include "../core/control/vpushbutton.h"
#include "../core/control/vscrollbar.h"
#include "../core/control/vradiobutton.h"
#include "../core/control/vblurlabel.h"

#include "../core/widget/vwidget.h"

VLIB_BEGIN_NAMESPACE

namespace VSS {
    class VSSParserHelper {
    public:
        static Core::VFontAlignment StringToAlignment(const std::wstring& String);
        static Core::VFontParagraphAlignment StringToParagraphAlignment(const std::wstring& String);
    };

    class VSSVPushButtonBuilder {
    private:
        void BuildVSSObject(Core::VPushButton *TargetControl,
                            std::vector<VSSBasicSelector *> SelectorSet,
                            Core::VPushButtonTheme *PushButtonTheme);

    public:
        VSSVPushButtonBuilder(Core::VPushButton *TargetControl,
                              std::vector<VSSBasicSelector *> SelectorSet,
                              Core::VPushButtonTheme *PushButtonTheme);
    };

    class VSSVRadioButtonBuilder {
    private:
        void BuildVSSObject(Core::VRadioButton *TargetControl,
                            std::vector<VSSBasicSelector *> SelectorSet,
                            Core::VRadioButtonTheme *RadioButtonTheme);

    public:
        VSSVRadioButtonBuilder(Core::VRadioButton *TargetControl,
                               std::vector<VSSBasicSelector *> SelectorSet,
                               Core::VRadioButtonTheme *RadioButtonTheme);
    };

    class VSSVTextLabelBuilder {
    private:
        void BuildVSSObject(Core::VTextLabel *TargetControl,
                            std::vector<VSSBasicSelector *> SelectorSet,
                            Core::VTextLabelTheme *TextLabelTheme);

    public:
        VSSVTextLabelBuilder(Core::VTextLabel *TargetControl,
                             std::vector<VSSBasicSelector *> SelectorSet,
                             Core::VTextLabelTheme *Theme);
    };

    class VSSVImageLabelBuilder {
    private:
        void BuildVSSObject(Core::VImageLabel *TargetControl,
                            std::vector<VSSBasicSelector *> SelectorSet,
                            Core::VImageLabelTheme *ImageLabelTheme);

    public:
        VSSVImageLabelBuilder(Core::VImageLabel *TargetControl,
                              std::vector<VSSBasicSelector *> SelectorSet,
                              Core::VImageLabelTheme *ImageLabelTheme);
    };

    class VSSVMainWindowBuilder {
    private:
        void BuildVSSObject(Core::VMainWindow *TargetControl,
                            std::vector<VSSBasicSelector *> SelectorSet);

    public:
        VSSVMainWindowBuilder(Core::VMainWindow *TargetControl,
                              std::vector<VSSBasicSelector *> SelectorSet);
    };

    class VSSVCircleButtonBuilder {
    private:
        void BuildVSSObject(Core::VCircleScrollBarButton *TargetControl,
                            std::vector<VSSBasicSelector *> SelectorSet,
                            Core::VCircleScrollBarTheme *CircleButtonTheme);

    public:
        VSSVCircleButtonBuilder(Core::VCircleScrollBarButton *TargetControl,
                                std::vector<VSSBasicSelector *> SelectorSet,
                                Core::VCircleScrollBarTheme *CircleButtonTheme);
    };

    class VSSVSliderHorizontalBuilder {
    private:
        void BuildVSSObject(Core::VSliderHorizontal *TargetControl,
                            std::vector<VSSBasicSelector *> SelectorSet,
                            Core::VSliderTheme *Theme);

    public:
        VSSVSliderHorizontalBuilder(Core::VSliderHorizontal *TargetControl,
                                    std::vector<VSSBasicSelector *> SelectorSet,
                                    Core::VSliderTheme *Theme);
    };

    class VSSVSliderVerticalBuilder {
    private:
        void BuildVSSObject(Core::VSliderVertical *TargetControl,
                            std::vector<VSSBasicSelector *> SelectorSet,
                            Core::VSliderTheme *Theme);

    public:
        VSSVSliderVerticalBuilder(Core::VSliderVertical *TargetControl,
                                  std::vector<VSSBasicSelector *> SelectorSet,
                                  Core::VSliderTheme *Theme);
    };

    class VSSVBlurLabelBuilder {
    private:
        void BuildVSSObject(Core::VBlurLabel *TargetControl,
            std::vector<VSSBasicSelector *> SelectorSet,
            Core::VBlurLabelTheme *Theme);

    public:
        VSSVBlurLabelBuilder(Core::VBlurLabel *TargetControl,
            std::vector<VSSBasicSelector *> SelectorSet,
            Core::VBlurLabelTheme *Theme);
    };
}

VLIB_END_NAMESPACE