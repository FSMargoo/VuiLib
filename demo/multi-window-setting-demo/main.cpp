#include "../../include/vml/vmlwidget.h"

Core::VWidget *SettingWidget;

void SettingBtnOnClick()
{
	SettingWidget->Show();
}

int main()
{
	Core::VApplication Applicaiton;
	VML::VMLMainWindow VMLMainWindow(0, 0, &Applicaiton);

	VMLMainWindow.LoadVML(L"./mainui.xml", VML::VMLParserParseMode::FromFile);
	VMLMainWindow.Show();

	VMLMainWindow[L"main-ui"][L"setting-button"].Get<Core::VPushButton>()->ButtonPushed.Connect(SettingBtnOnClick);

	SettingWidget = VMLMainWindow[L"main-ui"][L"setting-widget"].Get<Core::VWidget>();

	SettingWidget->SetQuitEvent([]() -> bool {
		SettingWidget->Hide();
		return true;
	});
	SettingWidget->Hide();

	return Applicaiton.Exec();
}