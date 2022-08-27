#include "../../../include/core/uibasic/vuitheme.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VLabelStatusTheme::VLabelStatusTheme(const VLabelStatusTheme& Theme) {
    BackgroundColor = Theme.BackgroundColor;
    BorderColor = Theme.BorderColor;
    TextColor = Theme.TextColor;
    BorderThickness = Theme.BorderThickness;
    Radius = Theme.Radius;
    AnimationInterpolatorType = Theme.AnimationInterpolatorType;
}
VLabelCommonTheme::VLabelCommonTheme(const VLabelCommonTheme& LabelObject) {
	StaticTheme  = LabelObject.StaticTheme;
	OnHoverTheme = LabelObject.OnHoverTheme;
	ActiveTheme  = LabelObject.ActiveTheme;
}
VLabelCommonTheme::VLabelCommonTheme() {
    LCID    LocaleID = GetThreadLocale();
    wchar_t LocaleName[LOCALE_NAME_MAX_LENGTH];
    if(!LCIDToLocaleName(LocaleID, LocaleName, LOCALE_NAME_MAX_LENGTH, 0)){
        VLIB_REPORT_ERROR(L"Failed to get the system locale");
    }

    LabelFont = new VFont(L"Microsoft YaHei UI", VFont::WEIGHT_NORMAL, VFont::STYLE_NORMAL, VFont::STRETCH_NORMAL, 12.f, LocaleName);
}
VPushButtonTheme::VPushButtonTheme(const VPushButtonTheme& Theme) {
    StaticTheme  = Theme.StaticTheme;
    OnHoverTheme = Theme.OnHoverTheme;
    ActiveTheme  = Theme.ActiveTheme;
    LabelFont    = new VFont(*Theme.LabelFont);
    PlaneText    = Theme.PlaneText;
    LocalTheme   = Theme.StaticTheme;
}
VPushButtonTheme::VPushButtonTheme() : VLabelCommonTheme() {
	StaticTheme.BackgroundColor = VColor::FromBYTERGBA(56, 56, 56, 0);
	StaticTheme.BorderColor = VColor::FromBYTERGBA(53, 53, 53, 255);
    StaticTheme.BorderThickness = 1.3f;
    StaticTheme.Radius = { 5, 5 };
	StaticTheme.TextColor = VColor::FromBYTERGBA(126, 126, 126, 255);
	StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	OnHoverTheme.BackgroundColor = VColor::FromBYTERGBA(56, 56, 56, 255);
	OnHoverTheme.BorderColor = VColor::FromBYTERGBA(56, 56, 56, 255);
	OnHoverTheme.TextColor = VColor::FromBYTERGBA(126, 126, 126, 255);
    OnHoverTheme.Radius = { 5, 5 };
    OnHoverTheme.BorderThickness = 1.3f;
	OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

    ActiveTheme.BackgroundColor = VColor::FromBYTERGBA(98, 100, 167, 255);
    ActiveTheme.BorderColor = VColor::FromBYTERGBA(98, 100, 167, 255);
    ActiveTheme.TextColor = VColor::FromBYTERGBA(255, 255, 255, 255);
    ActiveTheme.Radius = { 5, 5 };
    ActiveTheme.BorderThickness = 1.3f;
    ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	LocalTheme = StaticTheme;

    LabelFont->SetParagraphAlignment(VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    LabelFont->SetLineAlignment(VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER);
}

VTextLabelTheme::VTextLabelTheme() : VLabelCommonTheme() {
    StaticTheme.BackgroundColor = VColor::FromBYTERGBA(56, 56, 56, 0);
    StaticTheme.BorderColor = VColor::FromBYTERGBA(53, 53, 53, 0);
    StaticTheme.BorderThickness = 1.3f;
    StaticTheme.Radius = { 5, 5 };
    StaticTheme.TextColor = VColor::FromBYTERGBA(126, 126, 126, 119);
    StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

    OnHoverTheme.BackgroundColor = VColor::FromBYTERGBA(56, 56, 56, 0);
    OnHoverTheme.BorderColor = VColor::FromBYTERGBA(56, 56, 56, 0);
    OnHoverTheme.TextColor = VColor::FromBYTERGBA(126, 126, 126, 119);
    OnHoverTheme.Radius = { 5, 5 };
    OnHoverTheme.BorderThickness = 1.3f;
    OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

    ActiveTheme.BackgroundColor = VColor::FromBYTERGBA(98, 100, 167, 0);
    ActiveTheme.BorderColor = VColor::FromBYTERGBA(98, 100, 167, 0);
    ActiveTheme.TextColor = VColor::FromBYTERGBA(255, 255, 255, 119);
    ActiveTheme.Radius = { 5, 5 };
    ActiveTheme.BorderThickness = 1.3f;
    ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

    LocalTheme = StaticTheme;
}
VTextLabelTheme::VTextLabelTheme(const VTextLabelTheme& Theme) {
    StaticTheme  = Theme.StaticTheme;
    OnHoverTheme = Theme.OnHoverTheme;
    ActiveTheme  = Theme.ActiveTheme;
    LabelFont    = new VFont(*Theme.LabelFont);
    PlaneText    = Theme.PlaneText;
    LocalTheme   = Theme.StaticTheme;
}

VRadioButtonTheme::VRadioButtonTheme() {
    StaticTheme.BackgroundColor = VColor::FromBYTERGBA(75, 75, 75, 0);
    StaticTheme.BorderColor = VColor::FromBYTERGBA(75, 75, 75, 255);
    StaticTheme.BorderThickness = 1.3f;
    StaticTheme.Radius = { 5, 5 };
    StaticTheme.TextColor = VColor::FromBYTERGBA(126, 126, 126, 119);
    StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

    OnHoverTheme.BackgroundColor = VColor::FromBYTERGBA(75, 75, 75, 255);
    OnHoverTheme.BorderColor = VColor::FromBYTERGBA(75, 75, 75, 255);
    OnHoverTheme.TextColor = VColor::FromBYTERGBA(126, 126, 126, 119);
    OnHoverTheme.Radius = { 5, 5 };
    OnHoverTheme.BorderThickness = 1.3f;
    OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

    ActiveTheme.BackgroundColor = VColor::FromBYTERGBA(98, 100, 167, 255);
    ActiveTheme.BorderColor = VColor::FromBYTERGBA(98, 100, 167, 255);
    ActiveTheme.TextColor = VColor::FromBYTERGBA(255, 255, 255, 119);
    ActiveTheme.Radius = { 5, 5 };
    ActiveTheme.BorderThickness = 1.3f;
    ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

    LocalTheme = StaticTheme;
}
VRadioButtonTheme::VRadioButtonTheme(const VRadioButtonTheme& Theme) {
    StaticTheme  = Theme.StaticTheme;
    OnHoverTheme = Theme.OnHoverTheme;
    ActiveTheme  = Theme.ActiveTheme;
    LocalTheme   = Theme.StaticTheme;
    SwitchStatus = Theme.SwitchStatus;
}
VCircleScrollBarTheme::VCircleScrollBarTheme() {
    StaticTheme.BackgroundColor = VColor::FromBYTERGBA(87, 87, 87, 255);
    StaticTheme.BorderColor     = VColor::FromBYTERGBA(87, 87, 87, 255);
    StaticTheme.BorderThickness = 1.3f;
    StaticTheme.Radius          = { 5, 5 };
    StaticTheme.TextColor       = VColor::FromBYTERGBA(126, 126, 126, 255);
    StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

    OnHoverTheme.BackgroundColor = VColor::FromBYTERGBA(87, 87, 87, 255);
    OnHoverTheme.BorderColor = VColor::FromBYTERGBA(87, 87, 87, 255);
    OnHoverTheme.TextColor = VColor::FromBYTERGBA(126, 126, 126, 255);
    OnHoverTheme.Radius = { 5, 5 };
    OnHoverTheme.BorderThickness = 1.3f;
    OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

    ActiveTheme.BackgroundColor = VColor::FromBYTERGBA(98, 100, 167, 255);
    ActiveTheme.BorderColor = VColor::FromBYTERGBA(98, 100, 167, 255);
    ActiveTheme.TextColor = VColor::FromBYTERGBA(255, 255, 255, 255);
    ActiveTheme.Radius = { 5, 5 };
    ActiveTheme.BorderThickness = 1.3f;
    ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

    LocalTheme = StaticTheme;
}
VCircleScrollBarTheme::VCircleScrollBarTheme(const VCircleScrollBarTheme& Theme) {
    StaticTheme  = Theme.StaticTheme;
    OnHoverTheme = Theme.OnHoverTheme;
    ActiveTheme  = Theme.ActiveTheme;
    LocalTheme   = Theme.StaticTheme;
}

VSliderStatusTheme::VSliderStatusTheme(const VSliderStatusTheme& Theme) {
    BackgroundColor = Theme.BackgroundColor;
    BorderColor     = Theme.BorderColor;
}

VSliderAreaTheme::VSliderAreaTheme(const VSliderAreaTheme& Theme) {
    LocalTheme = Theme.LocalTheme;
}

VSliderTheme::VSliderTheme(const VSliderTheme& ThemeObject) {
    UnselectedArea = ThemeObject.UnselectedArea;
    SelectedArea   = ThemeObject.SelectedArea;
}
VSliderTheme::VSliderTheme() {
    UnselectedArea.LocalTheme.BorderColor = VColor::FromBYTERGBA(58, 58, 58, 255);
    UnselectedArea.LocalTheme.BackgroundColor = VColor::FromBYTERGBA(58, 58, 58, 255);
    UnselectedArea.LocalTheme.BorderThickness = 0.f;

    SelectedArea.LocalTheme.BorderColor = VColor::FromBYTERGBA(98, 100, 167, 255);
    SelectedArea.LocalTheme.BackgroundColor = VColor::FromBYTERGBA(98, 100, 167, 255);
    SelectedArea.LocalTheme.BorderThickness = 0.f;
}

}