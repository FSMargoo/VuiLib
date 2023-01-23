#include "../../include/core/widget/vwidget.h"
#include "../../include/vml/vmlwidget.h"

int main()
{
	Core::VApplication VMLDemo;
	VML::VMLMainWindow MainWindow(1000, 400, &VMLDemo);

	auto Result = MainWindow.LoadVML(L"./mainui.xml", VML::VMLParserParseMode::FromFile);

	return VMLDemo.Exec();
}