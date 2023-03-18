#pragma once

#include "../core/control/vblurlabel.h"
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
#include "../core/visual/vvisual.h"
#include "../core/widget/vwidget.h"
#include "vssparser.h"

VLIB_BEGIN_NAMESPACE

namespace VSS
{
class VSSParserHelper
{
public:
	static Core::VFontAlignment			 StringToAlignment(const VString &String);
	static Core::VFontParagraphAlignment StringToParagraphAlignment(const VString &String);
};

class VSSVPushButtonBuilder
{
private:
	void BuildVSSObject(Core::VPushButton *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VPushButtonTheme *PushButtonTheme);

public:
	VSSVPushButtonBuilder(Core::VPushButton *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						  Core::VPushButtonTheme *PushButtonTheme);
};

class VSSVRadioButtonBuilder
{
private:
	void BuildVSSObject(Core::VRadioButton *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VRadioButtonTheme *RadioButtonTheme);

public:
	VSSVRadioButtonBuilder(Core::VRadioButton *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						   Core::VRadioButtonTheme *RadioButtonTheme);
};

class VSSVIconButtonBuilder
{
private:
	void BuildVSSObject(Core::VIconButton *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VIconButtonTheme *IconButtonTheme);

public:
	VSSVIconButtonBuilder(Core::VIconButton *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						  Core::VIconButtonTheme *IconButtonTheme);
};

class VSSVTextLabelBuilder
{
private:
	void BuildVSSObject(Core::VTextLabel *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VTextLabelTheme *TextLabelTheme);

public:
	VSSVTextLabelBuilder(Core::VTextLabel *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						 Core::VTextLabelTheme *Theme);
};

class VSSVImageLabelBuilder
{
private:
	void BuildVSSObject(Core::VImageLabel *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VImageLabelTheme *ImageLabelTheme);

public:
	VSSVImageLabelBuilder(Core::VImageLabel *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						  Core::VImageLabelTheme *ImageLabelTheme);
};

class VSSVMainWindowBuilder
{
private:
	void BuildVSSObject(Core::VMainWindow *TargetControl, std::vector<VSSBasicSelector *> SelectorSet);

public:
	VSSVMainWindowBuilder(Core::VMainWindow *TargetControl, std::vector<VSSBasicSelector *> SelectorSet);
};

class VSSVCircleButtonBuilder
{
private:
	void BuildVSSObject(Core::VCircleScrollBarButton *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VCircleScrollBarTheme *CircleButtonTheme);

public:
	VSSVCircleButtonBuilder(Core::VCircleScrollBarButton *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
							Core::VCircleScrollBarTheme *CircleButtonTheme);
};

class VSSVSliderHorizontalBuilder
{
private:
	void BuildVSSObject(Core::VSliderHorizontal *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VSliderTheme *Theme);

public:
	VSSVSliderHorizontalBuilder(Core::VSliderHorizontal *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
								Core::VSliderTheme *Theme);
};

class VSSVSliderVerticalBuilder
{
private:
	void BuildVSSObject(Core::VSliderVertical *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VSliderTheme *Theme);

public:
	VSSVSliderVerticalBuilder(Core::VSliderVertical *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
							  Core::VSliderTheme *Theme);
};

class VSSVBlurLabelBuilder
{
private:
	void BuildVSSObject(Core::VBlurLabel *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VBlurLabelTheme *Theme);

public:
	VSSVBlurLabelBuilder(Core::VBlurLabel *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						 Core::VBlurLabelTheme *Theme);
};

class VSSVLineEditorBuilder
{
private:
	void BuildVSSObject(Core::VLineEditor *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VTextEditorTheme *TextEditorTheme);

public:
	VSSVLineEditorBuilder(Core::VLineEditor *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						  Core::VTextEditorTheme *TextEditorTheme);
};

class VSSVEditorBuilder
{
private:
	void BuildVSSObject(Core::VEditor *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VTextEditorTheme *TextEditorTheme);

public:
	VSSVEditorBuilder(Core::VEditor *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
					  Core::VTextEditorTheme *TextEditorTheme);
};

class VSSScrollerButtonBuilder
{
private:
	void BuildVSSObject(Core::VScrollerPushButton *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VViewScrollerButtonTheme *ScrollerTheme);

public:
	VSSScrollerButtonBuilder(Core::VScrollerPushButton *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
							 Core::VViewScrollerButtonTheme *ScrollerTheme);
};

class VSSVerticalScrollerBuilder
{
private:
	void BuildVSSObject(Core::VScrollerVertical *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VViewScrollerTheme *ScrollerTheme);

public:
	VSSVerticalScrollerBuilder(Core::VScrollerVertical *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
							   Core::VViewScrollerTheme *ScrollerTheme);
};
class VSSHorizontalScrollerBuilder
{
private:
	void BuildVSSObject(Core::VScrollerHorizontal *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VViewScrollerTheme *ScrollerTheme);

public:
	VSSHorizontalScrollerBuilder(Core::VScrollerHorizontal *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
								 Core::VViewScrollerTheme *ScrollerTheme);
};

class VSSViewLabelBuilder
{
private:
	void BuildVSSObject(Core::VViewLabel *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VViewLabelTheme *ViewLabelTheme);

public:
	VSSViewLabelBuilder(Core::VViewLabel *TargetControl, std::vector<VSSBasicSelector *> SelectorSet,
						Core::VViewLabelTheme *ViewLabelTheme);
};

class VSSCircleViewBuilder
{
private:
	void BuildVSSObject(Core::VCircleView *TargetControl, std::vector<VSSBasicSelector *> SelectorSet);

public:
	VSSCircleViewBuilder(Core::VCircleView *TargetControl, std::vector<VSSBasicSelector *> SelectorSet);
};

class VSSPolygonViewBuilder
{
private:
	void BuildVSSObject(Core::VPolygonView *TargetControl, std::vector<VSSBasicSelector *> SelectorSet);

public:
	VSSPolygonViewBuilder(Core::VPolygonView *TargetControl, std::vector<VSSBasicSelector *> SelectorSet);
};

class VSSVWidgetBuilder
{
private:
	void BuildVSSObject(Core::VWidget *TargetControl, std::vector<VSSBasicSelector *> SelectorSet);

public:
	VSSVWidgetBuilder(Core::VWidget *TargetControl, std::vector<VSSBasicSelector *> SelectorSet);
};
} // namespace VSS

VLIB_END_NAMESPACE