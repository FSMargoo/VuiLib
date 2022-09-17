// License(MIT)
// Athuor: Margoo
// This is the native theme in vuilib
#pragma once

#include "../vbasiccore.h"
#include "vuiinterpolator.h"
#include "../render/vpainter.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

enum class VUIThemeType {
	VUnknown, VMainWindow, VPushButton, VTextLabel, VImageLabel, VRadioButton, VCircleBarButton, VSlider, VBlurLabel,
    VIconButton
};

struct VBasicUITheme {
 public:
	 VBasicUITheme() = default;

 public:
	 virtual VUIThemeType GetThemeType() {
		 return VUIThemeType::VUnknown;
	 }
};

class VMainWindowTheme : public VBasicUITheme {
 public:
     VColor       BackgroundColor;
     bool         WindowSizble;
     std::wstring WindowPlaneText;

     VMainWindowTheme() {
         BackgroundColor = VColor::FromBYTERGBA(41, 42, 43, 255);
         WindowSizble    = true;
     }

     VUIThemeType GetThemeType() override {
         return VUIThemeType::VMainWindow;
     }
};

struct VLabelStatusTheme {
	VColor  BackgroundColor;
	VColor  BorderColor;
	VColor  TextColor;

	float   BorderThickness;

    VPoint  Radius;

	VInterpolatorType
		AnimationInterpolatorType;

    VLabelStatusTheme() = default;
    VLabelStatusTheme(const VLabelStatusTheme& Theme);
};

struct VLabelCommonTheme : public VBasicUITheme {
 public:
	 VLabelStatusTheme StaticTheme;
	 VLabelStatusTheme OnHoverTheme;
	 VLabelStatusTheme ActiveTheme;
	 VLabelStatusTheme LocalTheme;
     VFont*            LabelFont;

 public:
	 VLabelCommonTheme(const VLabelCommonTheme& LabelObject);
	 VLabelCommonTheme();
};

struct VPushButtonTheme : public VLabelCommonTheme {
 public:
     std::wstring PlaneText;

 public:
	 VPushButtonTheme();
     VPushButtonTheme(const VPushButtonTheme& Theme);

 public:
	 VUIThemeType GetThemeType() override {
		 return VUIThemeType::VPushButton;
	 }
};

struct VRadioButtonTheme : public VLabelCommonTheme {
 public:
     bool SwitchStatus = false;

 public:
     VRadioButtonTheme();
     VRadioButtonTheme(const VRadioButtonTheme& Theme);

 public:
     VUIThemeType GetThemeType() override {
         return VUIThemeType::VRadioButton;
     }
};

struct VTextLabelTheme : public VLabelCommonTheme {
 public:
    std::wstring PlaneText;

 public:
     VTextLabelTheme();
     VTextLabelTheme(const VTextLabelTheme& Theme);

 public:
     VUIThemeType GetThemeType() override {
        return VUIThemeType::VTextLabel;
     }
};

struct VImageLabelTheme : public VBasicUITheme {
    VImage* Image;

    VImageLabelTheme() {

    }
    ~VImageLabelTheme() {
        delete Image;
    }

    VUIThemeType GetThemeType() override {
        return VUIThemeType::VImageLabel;
    }
};

class VCircleScrollBarTheme : public VPushButtonTheme {
public:
    VCircleScrollBarTheme();
    VCircleScrollBarTheme(const VCircleScrollBarTheme& Theme);

public:
    VUIThemeType GetThemeType() override {
        return VUIThemeType::VCircleBarButton;
    }
};

struct VSliderStatusTheme {
    VColor  BackgroundColor;
    VColor  BorderColor;

    float   BorderThickness;

    VInterpolatorType
    AnimationInterpolatorType;

    VSliderStatusTheme() = default;
    VSliderStatusTheme(const VSliderStatusTheme& Theme);
};

struct VSliderAreaTheme {
    VSliderStatusTheme LocalTheme;

    VSliderAreaTheme() = default;
    VSliderAreaTheme(const VSliderAreaTheme& Theme);
};

class VSliderTheme : public VBasicUITheme {
public:
    VSliderAreaTheme UnselectedArea;
    VSliderAreaTheme SelectedArea;

public:
    VSliderTheme(const VSliderTheme& ThemeObject);
    VSliderTheme();

public:
    VUIThemeType GetThemeType() override {
        return VUIThemeType::VSlider;
    }
};

class VBlurLabelTheme : public VBasicUITheme {
public:
    VColor MixedColor;
    int    BlurRadius;

    VPoint BorderRadius;

public:
    VBlurLabelTheme(const VBlurLabelTheme& ThemeObject);
    VBlurLabelTheme();

public:
    VUIThemeType GetThemeType() override {
        return VUIThemeType::VBlurLabel;
    }
};

struct VIconButtonStatus {
    VImage* Image;
    float   Opacity;

    VIconButtonStatus();
    VIconButtonStatus(const VIconButtonStatus& Status);
};

class VIconButtonTheme  : public VLabelCommonTheme {
public:
    VImage* Image = nullptr;

public:
    VIconButtonTheme();
    VIconButtonTheme(const VIconButtonTheme& Theme);

public:
    VUIThemeType GetThemeType() override {
        return VUIThemeType::VIconButton;
    }
};

}

VLIB_END_NAMESPACE