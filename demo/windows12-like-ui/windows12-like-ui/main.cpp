#include "../../../include/vml/vmlwidget.h"

#include "status-bar.h"

int main()
{
	Core::VApplication App;
	Core::VSmartTimer  Timer(&App);

	VML::VMLMainWindow Widget(640, 480, &App);

	Widget.PushUserDefineFunction(StatusBarBuilderFunction);
	Widget.LoadVML(L"./ui.xml", VML::VMLParserParseMode::FromFile);

	// Loading ui
	StatusPercentBarAnimation Animation(
		Widget[L"main-widget"][L"logo-ui"][L"status-percent-bar"].Get<StatusPercentBar>());

	Timer.Start(8000);
	Timer.OnTime.Connect(Widget[L"main-widget"][L"logo-ui"][L"transparent-animation"].Get<Core::VOpacityAnimation>(),
						 &Core::VOpacityAnimation::Start);
	Widget[L"main-widget"][L"logo-ui"][L"transparent-animation"].Get<Core::VOpacityAnimation>()->AnimationEnd.Connect(
		Widget[L"main-widget"][L"intrdouct-ui"][L"transparent-animation"].Get<Core::VOpacityAnimation>(),
		&Core::VOpacityAnimation::Start);
	Timer.OnTime.Connect(&Animation, &StatusPercentBarAnimation::Stop);

	Animation.Play();
	Widget.Show();

	return App.Exec();
}