#include "../../../include/vml/vmlwidget.h"

#include "status-bar.h"

int main()
{
	Core::VApplication App;
	VML::VMLMainWindow Widget(640, 480, &App);

	Widget.PushUserDefineFunction(StatusBarBuilderFunction);
	Widget.LoadVML(L"./ui.xml", VML::VMLParserParseMode::FromFile);

	/* StatusPercentBar	  PercentBar(&Widget, 240, 4); */
	StatusPercentBarAnimation Animation(
		Widget[L"main-widget"][L"logo-ui"][L"status-percent-bar"].Get<StatusPercentBar>());

	Animation.Play();

	Widget.Show();

	return App.Exec();
}