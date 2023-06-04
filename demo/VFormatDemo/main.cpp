#include "vml/vmlwidget.h"

#pragma comment(lib, "vuilib.lib")

int main()
{
	auto Application = Core::VApplication();
	auto MainWindow	 = Core::VMainWindow(640, 800, &Application);
	auto TextLabel	 = Core::VTextLabel(640, 480, &MainWindow);

	TextLabel.SetTextColor(Core::VColor::White);

	// Call VFmt to print string on control
	Core::VFmt::PrintDevice(&TextLabel,
							L"This is a string from {}\n"
							"This string is powered by {}\n"
							"TextLabel's region: {}",
							"VFormator!", "VFMT", TextLabel.GetChildrenVisualRegion());

	// Call VFmt to print stringon console
	Core::VFmt::Print("Hello {}{}", "VUILIB", 2);
	Core::VFileStream File("./vuilib.txt");
	Core::VFmt::PrintDevice(&File, "VFmt can also be printed in file, like this {}",
							TextLabel.GetChildrenVisualRegion());

	MainWindow.Show();

	return Application.Exec();
}