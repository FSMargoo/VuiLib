#include "../../include/vml/vmlwidget.h"

Core::VTextLabel* CurrentLabel;

Core::VTextLabel* Page1Label;
Core::VTextLabel* Page2Label;
Core::VTextLabel* Page3Label;

Core::VRadioButton* SwitchButton1;
Core::VRadioButton* SwitchButton2;
Core::VRadioButton* SwitchButton3;

void SwitchToPage1() {
	if (CurrentLabel != Page1Label) {
		Page1Label->Move(0, 40);

		auto FadeOutOpacityInAnimation = static_cast<Core::VOpacityAnimation*>(CurrentLabel->GetChildObjectByPosition(13));
		auto FadeOutAnimation = static_cast<Core::VPositionAnimation*>(CurrentLabel->GetChildObjectByPosition(11));

		auto FadeInOpacityInAnimation = static_cast<Core::VOpacityAnimation*>(Page1Label->GetChildObjectByPosition(14));
		auto FadeInAnimation		= static_cast<Core::VPositionAnimation*>(Page1Label->GetChildObjectByPosition(12));

		FadeOutOpacityInAnimation->Start();
		FadeOutAnimation->Start();
		FadeInAnimation->Start();
		FadeInOpacityInAnimation->Start();

		CurrentLabel = Page1Label;
	}
	else {
		SwitchButton1->SetSwitchStatus(true);
	}
}
void SwitchToPage2() {
	if (CurrentLabel != Page2Label) {
		Page2Label->Move(0, 40);

		auto FadeOutOpacityInAnimation = static_cast<Core::VOpacityAnimation*>(CurrentLabel->GetChildObjectByPosition(13));
		auto FadeOutAnimation = static_cast<Core::VPositionAnimation*>(CurrentLabel->GetChildObjectByPosition(11));

		auto FadeInOpacityInAnimation = static_cast<Core::VOpacityAnimation*>(Page2Label->GetChildObjectByPosition(14));
		auto FadeInAnimation = static_cast<Core::VPositionAnimation*>(Page2Label->GetChildObjectByPosition(12));

		FadeOutOpacityInAnimation->Start();
		FadeOutAnimation->Start();
		FadeInAnimation->Start();
		FadeInOpacityInAnimation->Start();

		CurrentLabel = Page2Label;
	}
	else {
		SwitchButton2->SetSwitchStatus(true);
	}
}
void SwitchToPage3() {
	if (CurrentLabel != Page3Label) {
		Page3Label->Move(0, 40);

		auto FadeOutOpacityInAnimation = static_cast<Core::VOpacityAnimation*>(CurrentLabel->GetChildObjectByPosition(13));
		auto FadeOutAnimation = static_cast<Core::VPositionAnimation*>(CurrentLabel->GetChildObjectByPosition(11));

		auto FadeInOpacityInAnimation = static_cast<Core::VOpacityAnimation*>(Page3Label->GetChildObjectByPosition(14));
		auto FadeInAnimation = static_cast<Core::VPositionAnimation*>(Page3Label->GetChildObjectByPosition(12));

		FadeOutOpacityInAnimation->Start();
		FadeOutAnimation->Start();
		FadeInAnimation->Start();
		FadeInOpacityInAnimation->Start();

		CurrentLabel = Page3Label;
	}
	else {
		SwitchButton3->SetSwitchStatus(true);
	}
}

int main() {
	Core::VApplication App;
	VML::VMLMainWindow VMLWindow(&App);

	VMLWindow.RegisterMetaFunction(VML_META_FUNCTION(SwitchToPage1));
	VMLWindow.RegisterMetaFunction(VML_META_FUNCTION(SwitchToPage2));
	VMLWindow.RegisterMetaFunction(VML_META_FUNCTION(SwitchToPage3));

	VMLWindow.LoadVML(L"./main-ui.xml", VML::VMLParserParseMode::FromFile);

	Page1Label = VMLWindow[L"main-window"][L"purple-block"].Get<Core::VTextLabel>();
	Page2Label = VMLWindow[L"main-window"][L"orange-block"].Get<Core::VTextLabel>();
	Page3Label = VMLWindow[L"main-window"][L"blue-block"].Get<Core::VTextLabel>();

	SwitchButton1 = VMLWindow[L"main-window"][L"switch-group"][L"switch-button-1"].Get<Core::VRadioButton>();
	SwitchButton2 = VMLWindow[L"main-window"][L"switch-group"][L"switch-button-2"].Get<Core::VRadioButton>();
	SwitchButton3 = VMLWindow[L"main-window"][L"switch-group"][L"switch-button-3"].Get<Core::VRadioButton>();

	CurrentLabel = Page1Label;

	VMLWindow.Show();

	return App.Exec();
}