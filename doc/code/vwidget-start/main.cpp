#include "../../../include/core/widget/vwidget.h"
#include "../../../include/core/control/vpushbutton.h"
#include "../../../include/vml/vmlwidget.h"

int main() {
	Core::VApplication MyDemo;
	Core::VMainWindow  MyWindow(640, 480, &MyDemo);

	Core::VPushButton MyPushButton(100, 40, L"Push me!", &MyWindow);

	MyPushButton.ButtonPushed.Connect([]()->void { MessageBox(GetHWnd(), L"You pushed me uwu!", L"uwu", MB_OK); });
	MyWindow.SetTitle(L"Hello VuiLib!");

	return MyDemo.Exec();
}