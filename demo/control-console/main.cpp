#include "vml/vmlwidget.h"

#pragma comment(lib, "vuilib.lib")

int main()
{
	Core::VApplication App;
	VML::VMLMainWindow Window(&App);

	Window.LoadVML(VStr("./ui.xml"), VML::VMLParserParseMode::FromFile);
	Window.Show();
	Window.SetMiniSize({1344, 756});
	Window.SetMaxSize({1920, 1080});

	return App.Exec();
}