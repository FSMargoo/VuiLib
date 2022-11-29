#include "../../include/core/widget/vwidget.h"
#include "../../include/vml/vmlwidget.h"

int main() {
	Core::VApplication VMLDemo;
	Core::VMainWindow MainWindow(1000, 400, &VMLDemo);
	Core::VPushButton Button(100, 400, L"Hello World!", &MainWindow);

	return VMLDemo.Exec();
}