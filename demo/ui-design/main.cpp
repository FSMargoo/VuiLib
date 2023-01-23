#include "../../include/vml/vmlwidget.h"

Core::VTextLabel *CurrentLabel;

Core::VTextLabel *Page1Label;
Core::VTextLabel *Page2Label;
Core::VTextLabel *Page3Label;

void SwitchToPage1()
{
	if (CurrentLabel != Page1Label)
	{
		Page1Label->Move(0, 40);

		auto FadeOutOpacityInAnimation =
			static_cast<Core::VOpacityAnimation *>(CurrentLabel->GetChildObjectByPosition(13));
		auto FadeOutAnimation = static_cast<Core::VPositionAnimation *>(CurrentLabel->GetChildObjectByPosition(11));

		auto FadeInOpacityInAnimation =
			static_cast<Core::VOpacityAnimation *>(Page1Label->GetChildObjectByPosition(14));
		auto FadeInAnimation = static_cast<Core::VPositionAnimation *>(Page1Label->GetChildObjectByPosition(12));

		FadeOutOpacityInAnimation->Start();
		FadeOutAnimation->Start();
		FadeInAnimation->Start();
		FadeInOpacityInAnimation->Start();

		CurrentLabel = Page1Label;
	}
}
void SwitchToPage2()
{
	if (CurrentLabel != Page2Label)
	{
		Page2Label->Move(0, 40);

		auto FadeOutOpacityInAnimation =
			static_cast<Core::VOpacityAnimation *>(CurrentLabel->GetChildObjectByPosition(13));
		auto FadeOutAnimation = static_cast<Core::VPositionAnimation *>(CurrentLabel->GetChildObjectByPosition(11));

		auto FadeInOpacityInAnimation =
			static_cast<Core::VOpacityAnimation *>(Page2Label->GetChildObjectByPosition(14));
		auto FadeInAnimation = static_cast<Core::VPositionAnimation *>(Page2Label->GetChildObjectByPosition(12));

		FadeOutOpacityInAnimation->Start();
		FadeOutAnimation->Start();
		FadeInAnimation->Start();
		FadeInOpacityInAnimation->Start();

		CurrentLabel = Page2Label;
	}
}
void SwitchToPage3()
{
	if (CurrentLabel != Page3Label)
	{
		Page3Label->Move(0, 40);

		auto FadeOutOpacityInAnimation =
			static_cast<Core::VOpacityAnimation *>(CurrentLabel->GetChildObjectByPosition(13));
		auto FadeOutAnimation = static_cast<Core::VPositionAnimation *>(CurrentLabel->GetChildObjectByPosition(11));

		auto FadeInOpacityInAnimation =
			static_cast<Core::VOpacityAnimation *>(Page3Label->GetChildObjectByPosition(14));
		auto FadeInAnimation = static_cast<Core::VPositionAnimation *>(Page3Label->GetChildObjectByPosition(12));

		FadeOutOpacityInAnimation->Start();
		FadeOutAnimation->Start();
		FadeInAnimation->Start();
		FadeInOpacityInAnimation->Start();

		CurrentLabel = Page3Label;
	}
}

int main()
{
	Core::VApplication App;
	VML::VMLMainWindow VMLWindow(&App);

	VMLWindow.RegisterMetaFunction(VML_META_FUNCTION(SwitchToPage1));
	VMLWindow.RegisterMetaFunction(VML_META_FUNCTION(SwitchToPage2));
	VMLWindow.RegisterMetaFunction(VML_META_FUNCTION(SwitchToPage3));

	VMLWindow.LoadVML(L"./main-ui.xml", VML::VMLParserParseMode::FromFile);

	Page1Label = VMLWindow[L"main-window"][L"purple-block"].Get<Core::VTextLabel>();
	Page2Label = VMLWindow[L"main-window"][L"orange-block"].Get<Core::VTextLabel>();
	Page3Label = VMLWindow[L"main-window"][L"blue-block"].Get<Core::VTextLabel>();

	CurrentLabel = Page1Label;

	VMLWindow.Show();

	return App.Exec();
}