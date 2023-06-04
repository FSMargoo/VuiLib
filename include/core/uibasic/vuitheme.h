// License(MIT)
// Athuor: Margoo
// This is the native theme in vuilib
#pragma once
#pragma warning(disable : 26495)

#include "../render/vpainter.h"
#include "../vbasiccore.h"
#include "vuiinterpolator.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

enum class VUIThemeType
{
	VUnknown,
	VMainWindow,
	VPushButton,
	VTextLabel,
	VImageLabel,
	VRadioButton,
	VCircleBarButton,
	VSlider,
	VBlurLabel,
	VIconButton,
	VTextEditor,
	VViewScroller,
	VViewScrollerPushButton,
	VViewLabel
};

struct VBasicUITheme
{
public:
	VBasicUITheme() = default;

public:
	virtual VUIThemeType GetThemeType()
	{
		return VUIThemeType::VUnknown;
	}
};

class VMainWindowTheme : public VBasicUITheme
{
public:
	VColor	BackgroundColor;
	bool	WindowSizble;
	VString WindowPlainText;

	VMainWindowTheme()
	{
		BackgroundColor = VColor::FromBYTERGBA(41, 42, 43, 255);
		WindowSizble	= true;
	}

	VUIThemeType GetThemeType() override
	{
		return VUIThemeType::VMainWindow;
	}
};

struct VLabelStatusTheme
{
	VColor BackgroundColor;
	VColor BorderColor;
	VColor TextColor;

	float BorderThickness;

	VPoint Radius;

	VInterpolatorType AnimationInterpolatorType;

	VLabelStatusTheme() = default;
	VLabelStatusTheme(const VLabelStatusTheme &Theme);
};

struct VLabelCommonTheme : public VBasicUITheme
{
public:
	VLabelStatusTheme StaticTheme;
	VLabelStatusTheme OnHoverTheme;
	VLabelStatusTheme ActiveTheme;
	VLabelStatusTheme LocalTheme;
	VFont			 *LabelFont;

public:
	VLabelCommonTheme(const VLabelCommonTheme &LabelObject);
	VLabelCommonTheme();

	~VLabelCommonTheme();
};

struct VPushButtonTheme : public VLabelCommonTheme
{
public:
	VString PlainText;

public:
	VPushButtonTheme();
	VPushButtonTheme(const VPushButtonTheme &Theme);

public:
	VUIThemeType GetThemeType() override
	{
		return VUIThemeType::VPushButton;
	}
};

struct VRadioButtonTheme : public VLabelCommonTheme
{
public:
	bool	SwitchStatus = false;
	VString PlainText;

public:
	VRadioButtonTheme();
	VRadioButtonTheme(const VRadioButtonTheme &Theme);

public:
	VUIThemeType GetThemeType() override
	{
		return VUIThemeType::VRadioButton;
	}
};

struct VTextLabelTheme : public VLabelCommonTheme
{
public:
	VString PlainText;

public:
	VTextLabelTheme();
	VTextLabelTheme(const VTextLabelTheme &Theme);

public:
	VUIThemeType GetThemeType() override
	{
		return VUIThemeType::VTextLabel;
	}
};

struct VImageLabelTheme : public VBasicUITheme
{
	VImage *Image;
	VPoint	BorderRadius;

	VImageLabelTheme()
	{
		Image		 = nullptr;
		BorderRadius = {0, 0};
	}
	~VImageLabelTheme()
	{
		delete Image;
	}

	VUIThemeType GetThemeType() override
	{
		return VUIThemeType::VImageLabel;
	}
};

class VCircleScrollBarTheme : public VPushButtonTheme
{
public:
	VCircleScrollBarTheme();
	VCircleScrollBarTheme(const VCircleScrollBarTheme &Theme);

public:
	VUIThemeType GetThemeType() override
	{
		return VUIThemeType::VCircleBarButton;
	}
};

struct VSliderStatusTheme
{
	VColor BackgroundColor;
	VColor BorderColor;

	float BorderThickness;

	VInterpolatorType AnimationInterpolatorType;

	VSliderStatusTheme() = default;
	VSliderStatusTheme(const VSliderStatusTheme &Theme);
};

struct VSliderAreaTheme
{
	VSliderStatusTheme LocalTheme;

	VSliderAreaTheme() = default;
	VSliderAreaTheme(const VSliderAreaTheme &Theme);
};

class VSliderTheme : public VBasicUITheme
{
public:
	VSliderAreaTheme UnselectedArea;
	VSliderAreaTheme SelectedArea;

public:
	VSliderTheme(const VSliderTheme &ThemeObject);
	VSliderTheme();

public:
	VUIThemeType GetThemeType() override
	{
		return VUIThemeType::VSlider;
	}
};

class VBlurLabelTheme : public VBasicUITheme
{
public:
	VColor MixedColor;
	int	   BlurRadius;

	VPoint BorderRadius;

public:
	VBlurLabelTheme(const VBlurLabelTheme &ThemeObject);
	VBlurLabelTheme();

public:
	VUIThemeType GetThemeType() override
	{
		return VUIThemeType::VBlurLabel;
	}
};

struct VIconButtonStatus
{
	VImage *Image;
	float	Opacity;

	VIconButtonStatus();
	VIconButtonStatus(const VIconButtonStatus &Status);
};

class VIconButtonTheme : public VLabelCommonTheme
{
public:
	VImage *Image = nullptr;

public:
	VIconButtonTheme();
	VIconButtonTheme(const VIconButtonTheme &Theme);

public:
	VUIThemeType GetThemeType() override
	{
		return VUIThemeType::VIconButton;
	}
};

struct VTextEditorTheme : public VLabelCommonTheme
{
public:
	VString PlainText;

	VColor CursorColor;
	VColor OnSelectedColor;
	VColor OnSelectedBackgroundColor;

	VColor PlaceHolderColor;

public:
	VTextEditorTheme();
	VTextEditorTheme(const VTextEditorTheme &Theme);

public:
	VUIThemeType GetThemeType() override
	{
		return VUIThemeType::VTextEditor;
	}
};

struct VViewScrollerButtonTheme : public VLabelCommonTheme
{
public:
	VViewScrollerButtonTheme();
	VViewScrollerButtonTheme(const VViewScrollerButtonTheme &Theme);

public:
	VUIThemeType GetThemeType() override
	{
		return VUIThemeType::VViewScrollerPushButton;
	}
};

struct VViewScrollerTheme : public VLabelCommonTheme
{
public:
	VViewScrollerTheme();
	VViewScrollerTheme(const VViewScrollerTheme &Theme);

public:
	VUIThemeType GetThemeType() override
	{
		return VUIThemeType::VViewScroller;
	}
};

class VViewLabelTheme : public VBasicUITheme
{
public:
	VColor BackgroundColor;
	VColor BorderColor;

	VPoint Radius;

public:
	VViewLabelTheme();
	VViewLabelTheme(const VViewLabelTheme &Theme);

public:
	VUIThemeType GetThemeType() override
	{
		return VUIThemeType::VViewLabel;
	}
};

class VThemeAbstract
{
public:
	virtual VMainWindowTheme		 *GetMainWindowTheme()		   = 0;
	virtual VPushButtonTheme		 *GetPushButtonTheme()		   = 0;
	virtual VTextLabelTheme			 *GetTextLabelTheme()		   = 0;
	virtual VTextEditorTheme		 *GetTextEditorTheme()		   = 0;
	virtual VIconButtonTheme		 *GetIconButtonTheme()		   = 0;
	virtual VSliderTheme			 *GetSilderTheme()			   = 0;
	virtual VCircleScrollBarTheme	 *GetScrollBarTheme()		   = 0;
	virtual VRadioButtonTheme		 *GetRadioTheme()			   = 0;
	virtual VViewScrollerButtonTheme *GetViewScrollerButtonTheme() = 0;
	virtual VViewLabelTheme			 *GetViewLabelTheme()		   = 0;
};

class VElementUITheme : public VThemeAbstract
{
public:
	VMainWindowTheme *GetMainWindowTheme() override
	{
		VMainWindowTheme *Theme = new VMainWindowTheme;
		Theme->BackgroundColor	= VColor::FromBYTERGBA(255, 255, 255);
		Theme->WindowSizble		= true;

		return Theme;
	}
	VTextLabelTheme *GetTextLabelTheme() override
	{
		VTextLabelTheme *Theme = new VTextLabelTheme;

		Theme->StaticTheme.BackgroundColor			 = VColor::FromBYTERGBA(96, 98, 102, 0);
		Theme->StaticTheme.BorderColor				 = VColor::FromBYTERGBA(53, 53, 53, 0);
		Theme->StaticTheme.BorderThickness			 = 1.3f;
		Theme->StaticTheme.Radius					 = {5, 5};
		Theme->StaticTheme.TextColor				 = VColor::FromBYTERGBA(96, 98, 102, 255);
		Theme->StaticTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

		Theme->OnHoverTheme.BackgroundColor			  = VColor::FromBYTERGBA(56, 56, 56, 0);
		Theme->OnHoverTheme.BorderColor				  = VColor::FromBYTERGBA(56, 56, 56, 0);
		Theme->OnHoverTheme.TextColor				  = VColor::FromBYTERGBA(96, 98, 102, 255);
		Theme->OnHoverTheme.Radius					  = {5, 5};
		Theme->OnHoverTheme.BorderThickness			  = 1.3f;
		Theme->OnHoverTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

		Theme->ActiveTheme.BackgroundColor			 = VColor::FromBYTERGBA(98, 100, 167, 0);
		Theme->ActiveTheme.BorderColor				 = VColor::FromBYTERGBA(98, 100, 167, 0);
		Theme->ActiveTheme.TextColor				 = VColor::FromBYTERGBA(96, 98, 102, 255);
		Theme->ActiveTheme.Radius					 = {5, 5};
		Theme->ActiveTheme.BorderThickness			 = 1.3f;
		Theme->ActiveTheme.AnimationInterpolatorType = VInterpolatorType::AccelerateInterpolator;

		Theme->LabelFont->SetTextSize(16);

		Theme->LocalTheme = Theme->StaticTheme;

		return Theme;
	}
	VPushButtonTheme *GetPushButtonTheme() override
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
	VTextEditorTheme *GetTextEditorTheme() override
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
	VIconButtonTheme *GetIconButtonTheme() override
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
	VSliderTheme *GetSilderTheme() override
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
	VCircleScrollBarTheme *GetScrollBarTheme() override
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
	VRadioButtonTheme *GetRadioTheme() override
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
	VViewScrollerButtonTheme *GetViewScrollerButtonTheme() override
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
	VViewLabelTheme *GetViewLabelTheme() override
	{
		VViewLabelTheme *Theme = new VViewLabelTheme;
		Theme->BorderColor	   = VColor::FromBYTERGBA(220, 223, 230, 255);
		Theme->BackgroundColor = VColor::FromBYTERGBA(255, 255, 255, 255);

		return Theme;
	}
};
} // namespace Core

VLIB_END_NAMESPACE