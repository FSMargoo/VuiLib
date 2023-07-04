#pragma once

#include "vml/vmlwidget.h"

class BannerButton : public Core::VUIObject
{
public:
	BannerButton(Core::VUIObject *Parent, const VString &IconText, const VString &Text);

private:
	Core::VTextLabel *Title;
	Core::VTextLabel *IconText;
	Core::VLayout	 *TitleLayout;
	Core::VLayout	 *IconLayout;
};