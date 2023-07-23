#include "../../../include/vml/vmlwidget.h"

#include "status-bar.h"

Core::VTextLabel  *DateText;
Core::VSmartTimer *MainUITimer;
Core::VSmartTimer *DateTimer;

void StartMainUITimer()
{
	MainUITimer->Start(4000);
}

void UpdateTime()
{
	time_t LocalTime = time(0);

	tm TimeStucture;
	gmtime_s(&TimeStucture, &LocalTime);
	wchar_t Format[1024];
	_wasctime_s(Format, 1024, &TimeStucture);

	if (Format)
	{
		DateText->SetPlainText(Format);
	}

	DateTimer->Start(500);
}

int main()
{
	Core::VApplication App;
	Core::VSmartTimer  Timer(&App);

	VML::VMLMainWindow Widget(640, 480, &App);

	MainUITimer = new Core::VSmartTimer(&App);
	DateTimer	= new Core::VSmartTimer(&App);

	Widget.PushUserDefineFunction(StatusBarBuilderFunction);
	Widget.LoadVML(L"./ui.xml", VML::VMLParserParseMode::FromFile);

	// Loading ui
	StatusPercentBarAnimation Animation(
		Widget[L"main-widget"][L"logo-ui"][L"status-percent-bar"].Get<StatusPercentBar>());

	Timer.Start(6000);
	Timer.OnTime.Connect(Widget[L"main-widget"][L"logo-ui"][L"transparent-animation"].Get<Core::VOpacityAnimation>(),
						 &Core::VOpacityAnimation::Start);
	Widget[L"main-widget"][L"logo-ui"][L"transparent-animation"].Get<Core::VOpacityAnimation>()->AnimationEnd.Connect(
		Widget[L"main-widget"][L"intrdouct-ui"][L"transparent-animation"].Get<Core::VOpacityAnimation>(),
		&Core::VOpacityAnimation::Start);
	Timer.OnTime.Connect(&Animation, &StatusPercentBarAnimation::Stop);
	Timer.OnTime.Connect(StartMainUITimer);

	MainUITimer->OnTime.Connect(
		Widget[L"main-widget"][L"intrdouct-ui"][L"fade-out-animation"].Get<Core::VOpacityAnimation>(),
		&Core::VOpacityAnimation::Start);
	Widget[L"main-widget"][L"intrdouct-ui"][L"fade-out-animation"].Get<Core::VOpacityAnimation>()->AnimationEnd.Connect(
		Widget[L"main-widget"][L"main-ui"][L"transparent-animation"].Get<Core::VOpacityAnimation>(),
		&Core::VOpacityAnimation::Start);
	Widget[L"main-widget"][L"intrdouct-ui"][L"fade-out-animation"].Get<Core::VOpacityAnimation>()->AnimationEnd.Connect(
		Widget[L"main-widget"][L"main-ui"][L"bar"][L"icons-contain"][L"transparent-animation"]
			.Get<Core::VOpacityAnimation>(),
		&Core::VOpacityAnimation::Start);
	Widget[L"main-widget"][L"intrdouct-ui"][L"fade-out-animation"].Get<Core::VOpacityAnimation>()->AnimationEnd.Connect(
		Widget[L"main-widget"][L"main-ui"][L"bar"][L"icons-contain"][L"move-in-animation"]
			.Get<Core::VPositionAnimation>(),
		&Core::VPositionAnimation::Start);

	DateText = Widget[L"main-widget"][L"main-ui"][L"bar"][L"time-text"].Get<Core::VTextLabel>();

	DateTimer->OnTime.Connect(UpdateTime);
	DateTimer->Start(500);

	Animation.Play();
	Widget.Show();

	return App.Exec();
}