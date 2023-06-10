#include "../../../include/core/uibasic/vuitheme.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

VLabelStatusTheme::VLabelStatusTheme(const VLabelStatusTheme &Theme)
{
	BackgroundColor			  = Theme.BackgroundColor;
	BorderColor				  = Theme.BorderColor;
	TextColor				  = Theme.TextColor;
	BorderThickness			  = Theme.BorderThickness;
	Radius					  = Theme.Radius;
	AnimationInterpolatorType = Theme.AnimationInterpolatorType;
}
VLabelCommonTheme::VLabelCommonTheme(const VLabelCommonTheme &LabelObject)
{
	StaticTheme	 = LabelObject.StaticTheme;
	OnHoverTheme = LabelObject.OnHoverTheme;
	ActiveTheme	 = LabelObject.ActiveTheme;
}
VLabelCommonTheme::VLabelCommonTheme()
{
	LCID	LocaleID = GetThreadLocale();
	wchar_t LocaleName[LOCALE_NAME_MAX_LENGTH];
	if (!LCIDToLocaleName(LocaleID, LocaleName, LOCALE_NAME_MAX_LENGTH, 0))
	{
		VLIB_REPORT_ERROR(L"Failed to get the system locale");
	}

	LabelFont = new VFont(L"Microsoft YaHei UI", VFont::WEIGHT_NORMAL, VFont::STYLE_NORMAL, VFont::STRETCH_NORMAL, 12.f,
						  LocaleName);
}
VLabelCommonTheme::~VLabelCommonTheme()
{
	delete LabelFont;
}
VPushButtonTheme::VPushButtonTheme(const VPushButtonTheme &Theme)
{
	StaticTheme	 = Theme.StaticTheme;
	OnHoverTheme = Theme.OnHoverTheme;
	ActiveTheme	 = Theme.ActiveTheme;
	LabelFont	 = new VFont(*Theme.LabelFont);
	PlainText	 = Theme.PlainText;
	LocalTheme	 = Theme.StaticTheme;
}
VPushButtonTheme::VPushButtonTheme() : VLabelCommonTheme()
{
	StaticTheme.BackgroundColor			  = VColor::FromBYTERGBA(41, 41, 41, 255);
	StaticTheme.BorderColor				  = VColor::FromBYTERGBA(102, 102, 102, 255);
	StaticTheme.BorderThickness			  = 1.3f;
	StaticTheme.Radius					  = {5, 5};
	StaticTheme.TextColor				  = VColor::FromBYTERGBA(255, 255, 255, 255);
	StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	OnHoverTheme.BackgroundColor		   = VColor::FromBYTERGBA(61, 61, 61, 255);
	OnHoverTheme.BorderColor			   = VColor::FromBYTERGBA(102, 102, 102, 255);
	OnHoverTheme.TextColor				   = VColor::FromBYTERGBA(255, 255, 255, 255);
	OnHoverTheme.Radius					   = {5, 5};
	OnHoverTheme.BorderThickness		   = 1.3f;
	OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	ActiveTheme.BackgroundColor			  = VColor::FromBYTERGBA(31, 31, 31, 255);
	ActiveTheme.BorderColor				  = VColor::FromBYTERGBA(102, 102, 102, 255);
	ActiveTheme.TextColor				  = VColor::FromBYTERGBA(255, 255, 255, 255);
	ActiveTheme.Radius					  = {5, 5};
	ActiveTheme.BorderThickness			  = 1.3f;
	ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	LabelFont->SetParagraphAlignment(VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	LabelFont->SetLineAlignment(VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER);
	LabelFont->SetTextSize(16);
}

VTextLabelTheme::VTextLabelTheme() : VLabelCommonTheme()
{
	StaticTheme.BackgroundColor			  = VColor::FromBYTERGBA(56, 56, 56, 0);
	StaticTheme.BorderColor				  = VColor::FromBYTERGBA(53, 53, 53, 0);
	StaticTheme.BorderThickness			  = 1.3f;
	StaticTheme.Radius					  = {5, 5};
	StaticTheme.TextColor				  = VColor::FromBYTERGBA(255, 255, 255, 255);
	StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	OnHoverTheme.BackgroundColor		   = VColor::FromBYTERGBA(56, 56, 56, 0);
	OnHoverTheme.BorderColor			   = VColor::FromBYTERGBA(56, 56, 56, 0);
	OnHoverTheme.TextColor				   = VColor::FromBYTERGBA(255, 255, 255, 255);
	OnHoverTheme.Radius					   = {5, 5};
	OnHoverTheme.BorderThickness		   = 1.3f;
	OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	ActiveTheme.BackgroundColor			  = VColor::FromBYTERGBA(98, 100, 167, 0);
	ActiveTheme.BorderColor				  = VColor::FromBYTERGBA(98, 100, 167, 0);
	ActiveTheme.TextColor				  = VColor::FromBYTERGBA(255, 255, 255, 255);
	ActiveTheme.Radius					  = {5, 5};
	ActiveTheme.BorderThickness			  = 1.3f;
	ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	LocalTheme = StaticTheme;
}
VTextLabelTheme::VTextLabelTheme(const VTextLabelTheme &Theme)
{
	StaticTheme	 = Theme.StaticTheme;
	OnHoverTheme = Theme.OnHoverTheme;
	ActiveTheme	 = Theme.ActiveTheme;
	LabelFont	 = new VFont(*Theme.LabelFont);
	PlainText	 = Theme.PlainText;
	LocalTheme	 = Theme.StaticTheme;
}

VRadioButtonTheme::VRadioButtonTheme()
{
	StaticTheme.BackgroundColor			  = VColor::FromBYTERGBA(41, 41, 41, 255);
	StaticTheme.BorderColor				  = VColor::FromBYTERGBA(173, 173, 173, 255);
	StaticTheme.BorderThickness			  = 1.3f;
	StaticTheme.Radius					  = {5, 5};
	StaticTheme.TextColor				  = VColor::FromBYTERGBA(36, 36, 36, 0);
	StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	OnHoverTheme.BackgroundColor		   = VColor::FromBYTERGBA(41, 41, 41, 255);
	OnHoverTheme.BorderColor			   = VColor::FromBYTERGBA(189, 189, 189, 255);
	OnHoverTheme.TextColor				   = VColor::FromBYTERGBA(36, 36, 36, 0);
	OnHoverTheme.Radius					   = {5, 5};
	OnHoverTheme.BorderThickness		   = 1.3f;
	OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	ActiveTheme.BackgroundColor			  = VColor::FromBYTERGBA(71, 158, 245, 255);
	ActiveTheme.BorderColor				  = VColor::FromBYTERGBA(71, 158, 245, 255);
	ActiveTheme.TextColor				  = VColor::FromBYTERGBA(36, 36, 36, 255);
	ActiveTheme.Radius					  = {5, 5};
	ActiveTheme.BorderThickness			  = 1.3f;
	ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	IconThickness = 2.5f;

	LocalTheme = StaticTheme;
}
VRadioButtonTheme::VRadioButtonTheme(const VRadioButtonTheme &Theme)
{
	StaticTheme	  = Theme.StaticTheme;
	OnHoverTheme  = Theme.OnHoverTheme;
	ActiveTheme	  = Theme.ActiveTheme;
	LocalTheme	  = Theme.StaticTheme;
	SwitchStatus  = Theme.SwitchStatus;
	IconThickness = Theme.IconThickness;
}
VCircleScrollBarTheme::VCircleScrollBarTheme()
{
	StaticTheme.BackgroundColor			  = VColor::FromBYTERGBA(71, 158, 245, 255);
	StaticTheme.BorderColor				  = VColor::FromBYTERGBA(41, 41, 41, 255);
	StaticTheme.BorderThickness			  = 2.3f;
	StaticTheme.Radius					  = {5, 5};
	StaticTheme.TextColor				  = VColor::FromBYTERGBA(126, 126, 126, 255);
	StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	OnHoverTheme.BackgroundColor		   = VColor::FromBYTERGBA(98, 171, 245, 255);
	OnHoverTheme.BorderColor			   = VColor::FromBYTERGBA(41, 41, 41, 255);
	OnHoverTheme.TextColor				   = VColor::FromBYTERGBA(126, 126, 126, 255);
	OnHoverTheme.Radius					   = {5, 5};
	OnHoverTheme.BorderThickness		   = 1.3f;
	OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	ActiveTheme.BackgroundColor			  = VColor::FromBYTERGBA(40, 134, 222, 255);
	ActiveTheme.BorderColor				  = VColor::FromBYTERGBA(41, 41, 41, 255);
	ActiveTheme.TextColor				  = VColor::FromBYTERGBA(255, 255, 255, 255);
	ActiveTheme.Radius					  = {5, 5};
	ActiveTheme.BorderThickness			  = 0.f;
	ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	LocalTheme = StaticTheme;
}
VCircleScrollBarTheme::VCircleScrollBarTheme(const VCircleScrollBarTheme &Theme)
{
	StaticTheme	 = Theme.StaticTheme;
	OnHoverTheme = Theme.OnHoverTheme;
	ActiveTheme	 = Theme.ActiveTheme;
	LocalTheme	 = Theme.StaticTheme;
}

VSliderStatusTheme::VSliderStatusTheme(const VSliderStatusTheme &Theme)
{
	BackgroundColor = Theme.BackgroundColor;
	BorderColor		= Theme.BorderColor;
}

VSliderAreaTheme::VSliderAreaTheme(const VSliderAreaTheme &Theme)
{
	LocalTheme = Theme.LocalTheme;
}

VSliderTheme::VSliderTheme(const VSliderTheme &ThemeObject)
{
	UnselectedArea = ThemeObject.UnselectedArea;
	SelectedArea   = ThemeObject.SelectedArea;
}
VSliderTheme::VSliderTheme()
{
	UnselectedArea.LocalTheme.BorderColor	  = VColor::FromBYTERGBA(173, 173, 173, 255);
	UnselectedArea.LocalTheme.BackgroundColor = VColor::FromBYTERGBA(173, 173, 173, 255);
	UnselectedArea.LocalTheme.BorderThickness = 0.f;

	SelectedArea.LocalTheme.BorderColor		= VColor::FromBYTERGBA(71, 158, 245, 255);
	SelectedArea.LocalTheme.BackgroundColor = VColor::FromBYTERGBA(71, 158, 245, 255);
	SelectedArea.LocalTheme.BorderThickness = 0.f;
}

VBlurLabelTheme::VBlurLabelTheme(const VBlurLabelTheme &ThemeObject)
{
	MixedColor	 = ThemeObject.MixedColor;
	BlurRadius	 = ThemeObject.BlurRadius;
	BorderRadius = ThemeObject.BorderRadius;
}
VBlurLabelTheme::VBlurLabelTheme()
{
	MixedColor	 = VColor::FromBYTERGBA(56, 56, 56, 219);
	BlurRadius	 = 23;
	BorderRadius = {0, 0};
}

VIconButtonStatus::VIconButtonStatus()
{
	Image	= nullptr;
	Opacity = 1.f;
}
VIconButtonStatus::VIconButtonStatus(const VIconButtonStatus &Status)
{
	Image	= Status.Image;
	Opacity = Status.Opacity;
}

VIconButtonTheme::VIconButtonTheme(const VIconButtonTheme &Theme)
{
	StaticTheme	 = Theme.StaticTheme;
	OnHoverTheme = Theme.OnHoverTheme;
	ActiveTheme	 = Theme.ActiveTheme;
	LocalTheme	 = Theme.StaticTheme;

	if (Theme.Image != nullptr)
	{
		Image = new VImage(*Theme.Image);
	}
}
VIconButtonTheme::VIconButtonTheme() : VLabelCommonTheme()
{
	StaticTheme.BackgroundColor			  = VColor::FromBYTERGBA(56, 56, 56, 0);
	StaticTheme.BorderColor				  = VColor::FromBYTERGBA(53, 53, 53, 0);
	StaticTheme.BorderThickness			  = 1.3f;
	StaticTheme.Radius					  = {5, 5};
	StaticTheme.TextColor				  = VColor::FromBYTERGBA(126, 126, 126, 255);
	StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	OnHoverTheme.BackgroundColor		   = VColor::FromBYTERGBA(255, 255, 255, 25);
	OnHoverTheme.BorderColor			   = VColor::FromBYTERGBA(255, 255, 255, 25);
	OnHoverTheme.Radius					   = {5, 5};
	OnHoverTheme.BorderThickness		   = 1.3f;
	OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	ActiveTheme.BackgroundColor			  = VColor::FromBYTERGBA(255, 255, 255, 81);
	ActiveTheme.BorderColor				  = VColor::FromBYTERGBA(255, 255, 255, 81);
	ActiveTheme.Radius					  = {5, 5};
	ActiveTheme.BorderThickness			  = 1.3f;
	ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	LocalTheme = StaticTheme;
	Image	   = nullptr;
}

VTextEditorTheme::VTextEditorTheme(const VTextEditorTheme &Theme)
{
	StaticTheme	 = Theme.StaticTheme;
	OnHoverTheme = Theme.OnHoverTheme;
	ActiveTheme	 = Theme.ActiveTheme;
	LabelFont	 = new VFont(*Theme.LabelFont);
	PlainText	 = Theme.PlainText;
	LocalTheme	 = Theme.StaticTheme;
	CursorColor	 = Theme.CursorColor;

	OnSelectedColor			  = Theme.OnSelectedColor;
	OnSelectedBackgroundColor = Theme.OnSelectedBackgroundColor;
	PlaceHolderColor		  = Theme.PlaceHolderColor;
}
VTextEditorTheme::VTextEditorTheme()
{
	StaticTheme.BackgroundColor			  = VColor::FromBYTERGBA(41, 41, 41, 255);
	StaticTheme.BorderColor				  = VColor::FromBYTERGBA(102, 102, 102, 255);
	StaticTheme.BorderThickness			  = 1.3f;
	StaticTheme.Radius					  = {5, 5};
	StaticTheme.TextColor				  = VColor::FromBYTERGBA(153, 153, 153, 255);
	StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	OnHoverTheme.BackgroundColor		   = VColor::FromBYTERGBA(41, 41, 41, 255);
	OnHoverTheme.BorderColor			   = VColor::FromBYTERGBA(102, 102, 102, 255);
	OnHoverTheme.TextColor				   = VColor::FromBYTERGBA(255, 255, 255, 150);
	OnHoverTheme.Radius					   = {5, 5};
	OnHoverTheme.BorderThickness		   = 1.3f;
	OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	ActiveTheme.BackgroundColor			  = VColor::FromBYTERGBA(41, 41, 41, 255);
	ActiveTheme.BorderColor				  = VColor::FromBYTERGBA(71, 158, 245, 255);
	ActiveTheme.TextColor				  = VColor::FromBYTERGBA(255, 255, 255, 255);
	ActiveTheme.Radius					  = {5, 5};
	ActiveTheme.BorderThickness			  = 1.3f;
	ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	CursorColor = VColor::FromBYTERGBA(255, 255, 255);

	LocalTheme = StaticTheme;

	LabelFont->SetParagraphAlignment(VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	LabelFont->SetLineAlignment(VFontAlignment::DWRITE_TEXT_ALIGNMENT_LEADING);

	OnSelectedColor			  = VColor::FromBYTERGBA(255, 255, 255, 255);
	OnSelectedBackgroundColor = VColor::FromBYTERGBA(255, 255, 255, 40);

	PlaceHolderColor = VColor::FromBYTERGBA(255, 255, 255, 150);
}

VViewScrollerButtonTheme::VViewScrollerButtonTheme()
{
	StaticTheme.BackgroundColor			  = VColor::FromBYTERGBA(80, 80, 80, 99);
	StaticTheme.BorderColor				  = VColor::FromBYTERGBA(80, 80, 80, 99);
	StaticTheme.BorderThickness			  = 1.3f;
	StaticTheme.Radius					  = {5, 5};
	StaticTheme.TextColor				  = VColor::FromBYTERGBA(126, 126, 126, 255);
	StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	OnHoverTheme.BackgroundColor		   = VColor::FromBYTERGBA(80, 80, 80, 255);
	OnHoverTheme.BorderColor			   = VColor::FromBYTERGBA(80, 80, 80, 255);
	OnHoverTheme.Radius					   = {5, 5};
	OnHoverTheme.BorderThickness		   = 1.3f;
	OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	ActiveTheme.BackgroundColor			  = VColor::FromBYTERGBA(98, 100, 167, 255);
	ActiveTheme.BorderColor				  = VColor::FromBYTERGBA(98, 100, 167, 255);
	ActiveTheme.Radius					  = {5, 5};
	ActiveTheme.BorderThickness			  = 1.3f;
	ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	LocalTheme = StaticTheme;
}
VViewScrollerButtonTheme::VViewScrollerButtonTheme(const VViewScrollerButtonTheme &Theme)
{
	StaticTheme	 = Theme.StaticTheme;
	OnHoverTheme = Theme.OnHoverTheme;
	ActiveTheme	 = Theme.ActiveTheme;

	LocalTheme = StaticTheme;
}

VViewScrollerTheme::VViewScrollerTheme()
{
	StaticTheme.BackgroundColor			  = VColor::FromBYTERGBA(54, 54, 54, 81);
	StaticTheme.BorderColor				  = VColor::FromBYTERGBA(54, 54, 54, 81);
	StaticTheme.BorderThickness			  = 1.3f;
	StaticTheme.Radius					  = {5, 5};
	StaticTheme.TextColor				  = VColor::FromBYTERGBA(126, 126, 126, 255);
	StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	OnHoverTheme.BackgroundColor		   = VColor::FromBYTERGBA(54, 54, 54, 81);
	OnHoverTheme.BorderColor			   = VColor::FromBYTERGBA(54, 54, 54, 81);
	OnHoverTheme.Radius					   = {5, 5};
	OnHoverTheme.BorderThickness		   = 1.3f;
	OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	ActiveTheme.BackgroundColor			  = VColor::FromBYTERGBA(54, 54, 54, 255);
	ActiveTheme.BorderColor				  = VColor::FromBYTERGBA(54, 54, 54, 255);
	ActiveTheme.Radius					  = {5, 5};
	ActiveTheme.BorderThickness			  = 1.3f;
	ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	LocalTheme = StaticTheme;
}
VViewScrollerTheme::VViewScrollerTheme(const VViewScrollerTheme &Theme)
{
	StaticTheme	 = Theme.StaticTheme;
	OnHoverTheme = Theme.OnHoverTheme;
	ActiveTheme	 = Theme.ActiveTheme;

	LocalTheme = StaticTheme;
}

VViewLabelTheme::VViewLabelTheme()
{
	BackgroundColor = VColor(0.f, 0.f, 0.f, 0.f);
	BorderColor		= VColor(0.f, 0.f, 0.f, 0.f);

	Radius = {8, 8};
}

VDropDownContextTheme::VDropDownContextTheme()
{
	LCID	LocaleID = GetThreadLocale();
	wchar_t LocaleName[LOCALE_NAME_MAX_LENGTH];
	if (!LCIDToLocaleName(LocaleID, LocaleName, LOCALE_NAME_MAX_LENGTH, 0))
	{
		VLIB_REPORT_ERROR(L"Failed to get the system locale");
	}

	Radius	   = {10, 10};
	TitleColor = VColor::FromBYTERGBA(173, 173, 173);
	ItemColor  = VColor::FromBYTERGBA(214, 214, 214);
	GroupFont = new VFont(L"Microsoft YaHei UI", VFont::WEIGHT_MEDIUM, VFont::STYLE_NORMAL, VFont::STRETCH_NORMAL, 12.f,
						  LocaleName);
	ItemFont  = new VFont(L"Microsoft YaHei UI", VFont::WEIGHT_NORMAL, VFont::STYLE_NORMAL, VFont::STRETCH_NORMAL, 16.f,
						  LocaleName);
	BackgroundColor = VColor::FromBYTERGBA(41, 41, 41);
	BorderColor		= VColor::FromBYTERGBA(41, 41, 41);
	ShadowColor		= VColor::FromBYTERGBA(0, 0, 0, 70);
	ShadowPixel		= 10;
	BorderThickness = 0;

	GroupFont->SetTextSize(16);
	ItemFont->SetParagraphAlignment(VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}
VDropDownContextTheme::VDropDownContextTheme(const VDropDownContextTheme &Theme)
{
	Radius			= Theme.Radius;
	TitleColor		= Theme.TitleColor;
	ItemColor		= Theme.ItemColor;
	BackgroundColor = Theme.BackgroundColor;
	BorderColor		= Theme.BorderColor;
	ShadowColor		= Theme.ShadowColor;
	ShadowPixel		= Theme.ShadowPixel;
	GroupFont		= new VFont(*(Theme.GroupFont));
	ItemFont		= new VFont(*(Theme.ItemFont));
	BorderThickness = Theme.BorderThickness;
}
VDropDownTheme::VDropDownTheme(const VDropDownTheme &Theme)
{
	StaticTheme	  = Theme.StaticTheme;
	OnHoverTheme  = Theme.OnHoverTheme;
	ActiveTheme	  = Theme.ActiveTheme;
	LabelFont	  = new VFont(*Theme.LabelFont);
	PlainText	  = Theme.PlainText;
	LocalTheme	  = Theme.StaticTheme;
	IconColor	  = Theme.IconColor;
	IconThickness = Theme.IconThickness;
}
VDropDownTheme::VDropDownTheme() : VLabelCommonTheme()
{
	StaticTheme.BackgroundColor			  = VColor::FromBYTERGBA(41, 41, 41, 255);
	StaticTheme.BorderColor				  = VColor::FromBYTERGBA(102, 102, 102, 255);
	StaticTheme.BorderThickness			  = 1.3f;
	StaticTheme.Radius					  = {5, 5};
	StaticTheme.TextColor				  = VColor::FromBYTERGBA(153, 153, 153, 255);
	StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	OnHoverTheme.BackgroundColor		   = VColor::FromBYTERGBA(41, 41, 41, 255);
	OnHoverTheme.BorderColor			   = VColor::FromBYTERGBA(102, 102, 102, 255);
	OnHoverTheme.TextColor				   = VColor::FromBYTERGBA(153, 153, 153, 255);
	OnHoverTheme.Radius					   = {5, 5};
	OnHoverTheme.BorderThickness		   = 1.3f;
	OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	ActiveTheme.BackgroundColor			  = VColor::FromBYTERGBA(41, 41, 41, 255);
	ActiveTheme.BorderColor				  = VColor::FromBYTERGBA(67, 150, 232, 255);
	ActiveTheme.TextColor				  = VColor::FromBYTERGBA(153, 153, 153, 255);
	ActiveTheme.Radius					  = {5, 5};
	ActiveTheme.BorderThickness			  = 1.3f;
	ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	IconColor	  = VColor::FromBYTERGBA(173, 173, 173, 255);
	IconThickness = 1;

	LabelFont->SetParagraphAlignment(VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	LabelFont->SetTextSize(16);
}
VViewLabelTheme::VViewLabelTheme(const VViewLabelTheme &Theme)
{
	BackgroundColor = Theme.BackgroundColor;
	BorderColor		= Theme.BorderColor;
	Radius			= Theme.Radius;
}
VMainWindowTheme *VElementUITheme::GetMainWindowTheme()
{
	VMainWindowTheme *Theme = new VMainWindowTheme;
	Theme->BackgroundColor	= VColor::FromBYTERGBA(255, 255, 255);
	Theme->WindowSizble		= true;

	return Theme;
}
VTextLabelTheme *VElementUITheme::GetTextLabelTheme()
{
	VTextLabelTheme *Theme = new VTextLabelTheme;

	Theme->StaticTheme.BackgroundColor			 = VColor::FromBYTERGBA(96, 98, 102, 0);
	Theme->StaticTheme.BorderColor				 = VColor::FromBYTERGBA(53, 53, 53, 0);
	Theme->StaticTheme.BorderThickness			 = 1.3f;
	Theme->StaticTheme.Radius					 = {5, 5};
	Theme->StaticTheme.TextColor				 = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->OnHoverTheme.BackgroundColor			  = VColor::FromBYTERGBA(56, 56, 56, 0);
	Theme->OnHoverTheme.BorderColor				  = VColor::FromBYTERGBA(56, 56, 56, 0);
	Theme->OnHoverTheme.TextColor				  = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->OnHoverTheme.Radius					  = {5, 5};
	Theme->OnHoverTheme.BorderThickness			  = 1.3f;
	Theme->OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->ActiveTheme.BackgroundColor			 = VColor::FromBYTERGBA(98, 100, 167, 0);
	Theme->ActiveTheme.BorderColor				 = VColor::FromBYTERGBA(98, 100, 167, 0);
	Theme->ActiveTheme.TextColor				 = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->ActiveTheme.Radius					 = {5, 5};
	Theme->ActiveTheme.BorderThickness			 = 1.3f;
	Theme->ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->LabelFont->SetTextSize(18);

	Theme->LocalTheme = Theme->StaticTheme;

	return Theme;
}
VPushButtonTheme *VElementUITheme::GetPushButtonTheme()
{
	VPushButtonTheme *Theme = new VPushButtonTheme;

	Theme->StaticTheme.BackgroundColor			 = VColor::FromBYTERGBA(64, 158, 255, 0);
	Theme->StaticTheme.BorderColor				 = VColor::FromBYTERGBA(220, 223, 230, 255);
	Theme->StaticTheme.BorderThickness			 = 2.f;
	Theme->StaticTheme.Radius					 = {4, 4};
	Theme->StaticTheme.TextColor				 = VColor::FromBYTERGBA(20, 20, 20, 255);
	Theme->StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->OnHoverTheme.BackgroundColor			  = VColor::FromBYTERGBA(64, 158, 255, 28);
	Theme->OnHoverTheme.BorderColor				  = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->OnHoverTheme.TextColor				  = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->OnHoverTheme.Radius					  = {4, 4};
	Theme->OnHoverTheme.BorderThickness			  = 2.f;
	Theme->OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->ActiveTheme.BackgroundColor			 = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->ActiveTheme.BorderColor				 = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->ActiveTheme.TextColor				 = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->ActiveTheme.Radius					 = {4, 4};
	Theme->ActiveTheme.BorderThickness			 = 2.f;
	Theme->ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->LocalTheme = Theme->StaticTheme;

	Theme->LabelFont->SetTextSize(16);
	Theme->LabelFont->SetParagraphAlignment(VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	Theme->LabelFont->SetLineAlignment(VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER);

	return Theme;
}
VTextEditorTheme *VElementUITheme::GetTextEditorTheme()
{
	VTextEditorTheme *Theme = new VTextEditorTheme;

	Theme->StaticTheme.BackgroundColor			 = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->StaticTheme.BorderColor				 = VColor::FromBYTERGBA(220, 223, 230, 255);
	Theme->StaticTheme.BorderThickness			 = 1.3f;
	Theme->StaticTheme.Radius					 = {5, 5};
	Theme->StaticTheme.TextColor				 = VColor::FromBYTERGBA(96, 98, 102, 255);
	Theme->StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->OnHoverTheme.BackgroundColor			  = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->OnHoverTheme.BorderColor				  = VColor::FromBYTERGBA(220, 223, 230, 255);
	Theme->OnHoverTheme.TextColor				  = VColor::FromBYTERGBA(96, 98, 102, 255);
	Theme->OnHoverTheme.Radius					  = {5, 5};
	Theme->OnHoverTheme.BorderThickness			  = 1.3f;
	Theme->OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->ActiveTheme.BackgroundColor			 = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->ActiveTheme.BorderColor				 = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->ActiveTheme.TextColor				 = VColor::FromBYTERGBA(96, 98, 102, 255);
	Theme->ActiveTheme.Radius					 = {5, 5};
	Theme->ActiveTheme.BorderThickness			 = 1.3f;
	Theme->ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->CursorColor = VColor::FromBYTERGBA(96, 98, 102);

	Theme->LocalTheme = Theme->StaticTheme;

	Theme->LabelFont->SetParagraphAlignment(VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	Theme->LabelFont->SetLineAlignment(VFontAlignment::DWRITE_TEXT_ALIGNMENT_LEADING);

	Theme->OnSelectedColor			 = VColor::FromBYTERGBA(96, 98, 102, 255);
	Theme->OnSelectedBackgroundColor = VColor::FromBYTERGBA(40, 40, 40, 40);

	Theme->LabelFont->SetTextSize(16);

	Theme->PlaceHolderColor = VColor::FromBYTERGBA(96, 98, 102, 255);

	return Theme;
}
VIconButtonTheme *VElementUITheme::GetIconButtonTheme()
{
	VIconButtonTheme *Theme = new VIconButtonTheme;

	Theme->StaticTheme.BackgroundColor			 = VColor::FromBYTERGBA(64, 158, 255, 0);
	Theme->StaticTheme.BorderColor				 = VColor::FromBYTERGBA(220, 223, 230, 255);
	Theme->StaticTheme.BorderThickness			 = 2.f;
	Theme->StaticTheme.Radius					 = {4, 4};
	Theme->StaticTheme.TextColor				 = VColor::FromBYTERGBA(20, 20, 20, 255);
	Theme->StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->OnHoverTheme.BackgroundColor			  = VColor::FromBYTERGBA(64, 158, 255, 28);
	Theme->OnHoverTheme.BorderColor				  = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->OnHoverTheme.TextColor				  = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->OnHoverTheme.Radius					  = {4, 4};
	Theme->OnHoverTheme.BorderThickness			  = 2.f;
	Theme->OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->ActiveTheme.BackgroundColor			 = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->ActiveTheme.BorderColor				 = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->ActiveTheme.TextColor				 = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->ActiveTheme.Radius					 = {4, 4};
	Theme->ActiveTheme.BorderThickness			 = 2.f;
	Theme->ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->LocalTheme = Theme->StaticTheme;

	Theme->Image = nullptr;

	return Theme;
}
VSliderTheme *VElementUITheme::GetSilderTheme()
{
	VSliderTheme *Theme = new VSliderTheme;

	Theme->UnselectedArea.LocalTheme.BorderColor	 = VColor::FromBYTERGBA(220, 223, 255, 255);
	Theme->UnselectedArea.LocalTheme.BackgroundColor = VColor::FromBYTERGBA(220, 223, 255, 255);
	Theme->UnselectedArea.LocalTheme.BorderThickness = 0.f;

	Theme->SelectedArea.LocalTheme.BorderColor	   = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->SelectedArea.LocalTheme.BackgroundColor = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->SelectedArea.LocalTheme.BorderThickness = 0.f;

	return Theme;
}
VCircleScrollBarTheme *VElementUITheme::GetScrollBarTheme()
{
	VCircleScrollBarTheme *Theme = new VCircleScrollBarTheme;

	Theme->StaticTheme.BackgroundColor			 = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->StaticTheme.BorderColor				 = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->StaticTheme.BorderThickness			 = 2.f;
	Theme->StaticTheme.Radius					 = {5, 5};
	Theme->StaticTheme.TextColor				 = VColor::FromBYTERGBA(126, 126, 126, 255);
	Theme->StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->OnHoverTheme.BackgroundColor			  = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->OnHoverTheme.BorderColor				  = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->OnHoverTheme.TextColor				  = VColor::FromBYTERGBA(126, 126, 126, 255);
	Theme->OnHoverTheme.Radius					  = {5, 5};
	Theme->OnHoverTheme.BorderThickness			  = 2.f;
	Theme->OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->ActiveTheme.BackgroundColor			 = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->ActiveTheme.BorderColor				 = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->ActiveTheme.TextColor				 = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->ActiveTheme.Radius					 = {5, 5};
	Theme->ActiveTheme.BorderThickness			 = 2.f;
	Theme->ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->LocalTheme = Theme->StaticTheme;

	return Theme;
}
VRadioButtonTheme *VElementUITheme::GetRadioTheme()
{
	VRadioButtonTheme *Theme = new VRadioButtonTheme;

	Theme->StaticTheme.BackgroundColor			 = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->StaticTheme.BorderColor				 = VColor::FromBYTERGBA(200, 223, 230, 255);
	Theme->StaticTheme.BorderThickness			 = 1.3f;
	Theme->StaticTheme.Radius					 = {5, 5};
	Theme->StaticTheme.TextColor				 = VColor::FromBYTERGBA(126, 126, 126, 119);
	Theme->StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->OnHoverTheme.BackgroundColor			  = VColor::FromBYTERGBA(255, 255, 255, 25);
	Theme->OnHoverTheme.BorderColor				  = VColor::FromBYTERGBA(200, 223, 230, 255);
	Theme->OnHoverTheme.TextColor				  = VColor::FromBYTERGBA(126, 126, 126, 119);
	Theme->OnHoverTheme.Radius					  = {5, 5};
	Theme->OnHoverTheme.BorderThickness			  = 1.3f;
	Theme->OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->ActiveTheme.BackgroundColor			 = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->ActiveTheme.BorderColor				 = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->ActiveTheme.TextColor				 = VColor::FromBYTERGBA(255, 255, 255, 119);
	Theme->ActiveTheme.Radius					 = {5, 5};
	Theme->ActiveTheme.BorderThickness			 = 1.3f;
	Theme->ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->LocalTheme = Theme->StaticTheme;

	return Theme;
}
VViewScrollerButtonTheme *VElementUITheme::GetViewScrollerButtonTheme()
{
	VViewScrollerButtonTheme *Theme = new VViewScrollerButtonTheme;

	Theme->StaticTheme.BackgroundColor			 = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->StaticTheme.BorderColor				 = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->StaticTheme.BorderThickness			 = 2.f;
	Theme->StaticTheme.Radius					 = {5, 5};
	Theme->StaticTheme.TextColor				 = VColor::FromBYTERGBA(126, 126, 126, 255);
	Theme->StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->OnHoverTheme.BackgroundColor			  = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->OnHoverTheme.BorderColor				  = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->OnHoverTheme.TextColor				  = VColor::FromBYTERGBA(126, 126, 126, 255);
	Theme->OnHoverTheme.Radius					  = {5, 5};
	Theme->OnHoverTheme.BorderThickness			  = 2.f;
	Theme->OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->ActiveTheme.BackgroundColor			 = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->ActiveTheme.BorderColor				 = VColor::FromBYTERGBA(64, 158, 255, 255);
	Theme->ActiveTheme.TextColor				 = VColor::FromBYTERGBA(255, 255, 255, 255);
	Theme->ActiveTheme.Radius					 = {5, 5};
	Theme->ActiveTheme.BorderThickness			 = 2.f;
	Theme->ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

	Theme->LocalTheme = Theme->StaticTheme;

	return Theme;
}
VViewLabelTheme *VElementUITheme::GetViewLabelTheme()
{
	VViewLabelTheme *Theme = new VViewLabelTheme;
	Theme->BorderColor	   = VColor::FromBYTERGBA(220, 223, 230, 255);
	Theme->BackgroundColor = VColor::FromBYTERGBA(255, 255, 255, 255);

	return Theme;
}

} // namespace Core

VLIB_END_NAMESPACE