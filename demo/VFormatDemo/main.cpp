#include "vml/vmlwidget.h"

#pragma comment(lib, "vuilib.lib")

int main()
{
	auto Application = Core::VApplication();

	auto MainWindow = Core::VMainWindow(1000, 800, &Application);
	auto TextLabel	= Core::VTextLabel(640, 480, &MainWindow);

	// Call VFmt to print string on control
	Core::VFmt::PrintDevice(&TextLabel, "This is {}, 1 + 1 = {}", 1, 1 + 1);

	// Call VFmt to print string on console
	Core::VFmt::Print("Hello {}{}", "VUILIB", 2);
	Core::VFileStream File("./vuilib.txt");
	Core::VFmt::PrintDevice(&File, "VFmt can also be printed in file, like this {}",
							TextLabel.GetChildrenVisualRegion());

	auto Button = Core::VPushButton(185, 50, L"Push Me", &MainWindow);

	MainWindow.Show();

	return Application.Exec();
}