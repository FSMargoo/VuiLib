#include "vml/vmlwidget.h"

#pragma comment(lib, "vuilib.lib")

int main()
{
	Core::VElementUITheme ElementStyle;

	auto Application = Core::VApplication();

	// Set the element UI style
	Application.SetTheme(&ElementStyle);

	auto MainWindow			= Core::VMainWindow(1000, 800, &Application);
	auto TextLabel			= Core::VTextLabel(640, 480, &MainWindow);
	auto Editor				= Core::VEditor(400, 100, &MainWindow);
	auto Scroller			= Core::VSliderVertical(&MainWindow);
	auto ScrollerHorziontal = Core::VSliderHorizontal(&MainWindow);
	auto Radio				= Core::VRadioButton(&MainWindow);

	Scroller.Resize(8, 400);
	ScrollerHorziontal.Resize(400, 8);
	Radio.Resize(20, 20);

	Core::VFmt::PrintDevice(&TextLabel, "这是 VUILib 的 Element UI 主题");

	auto Button = Core::VPushButton(185, 50, L"Push Me", &MainWindow);
	Button.Move(80, 80);
	Editor.Move(80, 160);
	Scroller.Move(500, 80);
	Radio.Move(300, 80);
	ScrollerHorziontal.Move(520, 80);

	MainWindow.Show();

	return Application.Exec();
}